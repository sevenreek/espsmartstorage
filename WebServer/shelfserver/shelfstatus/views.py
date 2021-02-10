from django.shortcuts import render
from django.http import HttpResponse, JsonResponse
import random
from django.views.generic.list import ListView
from .models import ProductTag, Shelf, Product, Container, ProductSpot
from django.core import serializers
import json
from django.core.serializers.json import DjangoJSONEncoder
from django.db.models import F

def shelf_view(request, shelf_id):
    shelves = Shelf.objects.all()
    shelf = Shelf.objects.get(id=shelf_id)
    # this gives you a list of dicts
    product_spots = ProductSpot.objects.filter(shelf=shelf_id).values(
        'position',
        #container_id=F('last_tag__relation__container__id'),
        #container_name=F('last_tag__relation__container__name'),
        container_weight=F('last_tag__relation__container__weight'),
        max_measurement=F('last_tag__relation__max_measurement'),
        product_id=F('last_tag__relation__product__id'),
        product_name=F('last_tag__relation__product__name'),
    )

    return render(request, "shelfstatus/shelf.html", {
        "mqtt_config" : {
            "websockets_broker" : "localhost",
            "websockets_port" : 9001,
            "client_name" : random.randrange(0, 999),
        },
        "shelf": shelf,
        "shelves": shelves,
        "activetab": "shelves",
        "product_spots_js" : json.dumps(list(product_spots), cls=DjangoJSONEncoder),
    })

def tags_view(request):
    shelves = Shelf.objects.all()
    page0 = int(request.GET.get('p', 1)) - 1
    tags_per_page = int(request.GET.get('opp', 25))
    tags = ProductTag.objects.all()[page0*tags_per_page:(page0+1)*tags_per_page]
    products = Product.objects.all()
    containers = Container.objects.all()
    return render(request, "shelfstatus/tags.html", {
        "tags" : tags,
        "products" : products,
        "containers" : containers,
        "shelves": shelves,
        "activetab": "tags",
    })




def product_view(request, product_id):
    shelves = Shelf.objects.all()
    return render(request, "shelfstatus/product.html", {
        "shelves": shelves,
        "activetab": "products",
    })

def products_view(request):
    shelves = Shelf.objects.all()
    products = Product.objects.all()
    return render(request, "shelfstatus/product.html", {
        "shelves": shelves,
        "products": products,
        "activetab": "products",
    })


def containers_view(request):
    shelves = Shelf.objects.all()
    return render(request, "shelfstatus/containers.html", {
        "shelves": shelves,
        "activetab": "containers",
    })

def tag_id(request, tag_string):
    returned_tag = ProductTag.objects.filter(tag=tag_string)
    if not returned_tag:
        data = None
    else:
        data = returned_tag.values(
            'tag',
            tag_id=F('id'),
            container_weight=F('relation__container__weight'),
            max_measurement=F('relation__max_measurement'),
            product_id=F('relation__product__id'),
            product_name=F('relation__product__name'),
        )
    return JsonResponse(list(data)[0], safe=False)

def set_last_tag(request, shelf_id):
    if(request.method == 'PUT'):
        data = json.loads(request.body)
        position = data["pos"]
        new_tag_tag = data["tag"]
        try:
            spot = ProductSpot.objects.get(shelf=int(shelf_id), position=position)
            new_tag = ProductTag.objects.get(tag=new_tag_tag)
            spot.last_tag = new_tag
            spot.save()
            tag_info = {
                'tag':new_tag.tag,
                'tag_id':new_tag.id,
                'container_weight': new_tag.relation.container.weight,
                'max_measurement': new_tag.relation.max_measurement,
                'product_id': new_tag.relation.product.id,
                'product_name': new_tag.relation.product.name,
            }
            return JsonResponse(tag_info, safe=False)
        except Exception as e:
            print(e)
            return HttpResponse(status=404)
    else:
        return HttpResponse(status=405)

def free_last_tag(request, shelf_id):
    if(request.method == 'PUT'):
        data = json.loads(request.body)
        position = data["pos"]
        try:
            spot = ProductSpot.objects.get(shelf=int(shelf_id), position=position)
            spot.last_tag = None
            spot.save()
            return HttpResponse(status=202)
        except Exception as e:
            print(e)
            return HttpResponse(status=404)
    else:
        return HttpResponse(status=405)