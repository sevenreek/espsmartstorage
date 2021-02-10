from django.db import models
from django.utils.timezone import now
# Create your models here.
class Container(models.Model):
    name = models.CharField(max_length=256)
    weight = models.FloatField()

class Shelf(models.Model):
    name = models.CharField(max_length=256)

class Product(models.Model):
    name = models.CharField(max_length=256)

class ProductContainerRelation(models.Model):
    container = models.ForeignKey(Container, on_delete=models.CASCADE)
    product = models.ForeignKey(Product, on_delete=models.CASCADE)
    max_measurement = models.FloatField()

class ProductTag(models.Model):
    tag = models.CharField(max_length=64, unique=True)
    relation = models.ForeignKey(ProductContainerRelation, default=None, null=True, blank=True, on_delete=models.SET_NULL)
    active = models.BooleanField()
    created_on = models.DateTimeField(auto_now_add=True)
    updated_on = models.DateTimeField(auto_now=True)

class ProductSpot(models.Model):
    last_tag = models.ForeignKey(ProductTag, default=None, null=True, blank=True, on_delete=models.SET_NULL)
    shelf = models.ForeignKey(Shelf, on_delete=models.CASCADE)
    position = models.IntegerField(default=-1)
    position_name = models.CharField(max_length=16, null=True, blank=True, default=None)

class ProductSpotState(models.Model):
    spot = models.ForeignKey(ProductSpot, on_delete=models.CASCADE)
    tag_present = models.ForeignKey(ProductTag, on_delete=models.CASCADE)
    measurement = models.FloatField()
    timestamp = models.DateTimeField(auto_now_add=True)
