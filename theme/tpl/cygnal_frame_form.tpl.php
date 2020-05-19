
<div class="modal fade" id="modal-message" tabindex="-1" role="modal">
  <div class="modal-dialog " role="document" >
    <div class="modal-content">
      <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        <h4 class="modal-title">Message</h4>
      </div>
      <div class="modal-body">
           <!--Aqui va contenido personalizado-->
      </div>
      <div class="modal-footer">
        <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
      </div>
    </div><!-- /.modal-content -->
  </div><!-- /.modal-dialog -->
</div><!-- /.modal -->


<div class="modal fade" id="modal-frame" tabindex="-1" role="modal">
  <div class="modal-dialog modal-lg" role="document" >
    <div class="modal-content">
      <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        <h4 class="modal-title">Question Instruction</h4>
      </div>
      <div class="modal-body">
           <div class="row">
              <div class="col-md-12">
                      <div class="row">
                        <div class="col-md-12">
                            <div class="alert alert-info" role="alert"> <strong>Get Variables</strong> </div>
                          </div>
                      </div>
                      <!--Fila1-->            
                      <div class="row">
                             <div class="col-md-12">
                                <div class="form-wrapper form-group">
                                    <div class="form-group"> 
                                      <label class="control-label" for="application-frame">Application</label>
                                      <select name="application_frame" id="application-frame"></select>
                                    </div>
                                </div>
                             </div>                            
                      </div>  
                      <div id="tree-frame">
                      </div>
                    
                </div>
            </div>
      </div>
      <div class="modal-footer">     
        
        <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
      </div>
    </div><!-- /.modal-content -->
  </div><!-- /.modal-dialog -->
</div><!-- /.modal -->


<!--Fila2-->
<div class="row"> 
  <div class="col-md-12">
     <?php print drupal_render_children($form);?>
  </div>
</div> 

<div class="row">
  <div class="col-md-9">&nbsp;</div>
    <div class="col-md-3">
       <?php print drupal_render($form['actions']['submit']);?>
    </div>     
</div>