from django.contrib import admin
from .models import *
# Register your models here.

class ContainerAdmin(admin.ModelAdmin):
    list_display = ('name', 'weight')

class ShelfAdmin(admin.ModelAdmin):
    list_display = ('name',)

class ProductAdmin(admin.ModelAdmin):
    list_display = ('id','name',)

class ProductTagAdmin(admin.ModelAdmin):
    list_display = ['tag', 'relation', 'get_product_name', 'get_container_name', 'active']
    def get_product_name(self, obj):
        if(obj.relation is not None):
            return obj.relation.product.name
        else:
            return None
    get_product_name.short_description = "Product"
    def get_container_name(self, obj):
        if(obj.relation is not None):
            return obj.relation.container.name
        else:
            return None
    get_container_name.short_description = "Container"
    def get_form(self, request, obj=None, **kwargs):
        form = super(ProductTagAdmin, self).get_form(request, obj, **kwargs)
        form.base_fields['relation'].label_from_instance = lambda obj: "({}) {} : {}".format(obj.id, obj.product.name, obj.container.name)
        return form

class ProductSpotAdmin(admin.ModelAdmin):
    list_display = ('get_shelf_name', 'position', 'get_tag_tag')
    def get_tag_tag(self, obj):
        if(obj.last_tag is not None):
            return obj.last_tag.tag
        else:
            return None
    get_tag_tag.short_description = "Last tag"
    def get_shelf_name(self, obj):
        return obj.shelf.name
    get_shelf_name.short_description = "Shelf"
    def get_form(self, request, obj=None, **kwargs):
        form = super(ProductSpotAdmin, self).get_form(request, obj, **kwargs)
        form.base_fields['last_tag'].label_from_instance = lambda obj: "({}) {}".format(obj.id, obj.tag)
        form.base_fields['shelf'].label_from_instance = lambda obj: "({}) {}".format(obj.id, obj.name)            
        return form

    
class ProductContainerRelationAdmin(admin.ModelAdmin):
    list_display = ('get_container_name', 'get_product_name', 'max_measurement')
    def get_container_name(self, obj):
        return obj.container.name
    get_container_name.short_description = "Container"
    def get_product_name(self, obj):
        return obj.product.name
    get_product_name.short_description = "Product"
    pass
    def get_form(self, request, obj=None, **kwargs):
            form = super(ProductContainerRelationAdmin, self).get_form(request, obj, **kwargs)
            form.base_fields['container'].label_from_instance = lambda obj: "({}) {}".format(obj.id, obj.name)
            form.base_fields['product'].label_from_instance = lambda obj: "({}) {}".format(obj.id, obj.name)            
            return form
    
class ProductSpotStateAdmin(admin.ModelAdmin):
    pass

admin.site.register(Container, ContainerAdmin)
admin.site.register(Shelf, ShelfAdmin)
admin.site.register(Product, ProductAdmin)
admin.site.register(ProductTag, ProductTagAdmin)
admin.site.register(ProductSpot, ProductSpotAdmin)
admin.site.register(ProductContainerRelation, ProductContainerRelationAdmin)
admin.site.register(ProductSpotState, ProductSpotStateAdmin)