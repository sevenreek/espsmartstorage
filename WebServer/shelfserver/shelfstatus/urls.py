from django.urls import path
from . import views

urlpatterns = [
    path('shelves/<int:shelf_id>', views.shelf_view, name='shelves'),
    path('tags', views.tags_view, name='tags'),
    path('product/<int:product_id>', views.product_view, name='product'),
    path('products', views.products_view, name='products'),
    path('containers', views.containers_view, name='containers'),
    path('tag/<str:tag_string>', views.tag_id, name='tag_id'),
    path('shelves/<int:shelf_id>/set_tag', views.set_last_tag, name='set_last_tag'),
    path('shelves/<int:shelf_id>/free_tag', views.free_last_tag, name='free_last_tag'),
]

