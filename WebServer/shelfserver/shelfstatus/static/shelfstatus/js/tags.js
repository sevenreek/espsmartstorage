var MODAL_ID = "edit_tag_modal"

function setModal(tag_id, tag, product_id, product_name, container_id, container_name)
{
    console.log("called");
    if(tag_id=="new")
    {
        $('#edit_tag_modal_title').html("Create new tag");
        $('#edit_tag_save_button').html("Create tag");
    }
    else 
    {
        $('#edit_tag_modal_title').html("Edit tag");
        
        $('#edit_tag_save_button').html("Save tag");
        
    }
    $('#edit_tag_tag').val(tag);
    $('#edit_tag_product').val(product_id);
    $('#edit_tag_product').trigger('change');
    $('#edit_tag_container').val(container_id);
    $('#edit_tag_container').trigger('change');
}