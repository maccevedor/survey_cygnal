
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


<div class="modal fade" id="modal-instruction" tabindex="-1" role="modal">
  <div class="modal-dialog modal-lg" role="document" >
    <div class="modal-content">
      <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        <h4 class="modal-title">Question Instruction</h4>
      </div>
      <div class="modal-body">
           <div class="row">
              <div class="col-md-12">
                  <div id="list-instruction">
                      <!--Aqui sale los controles de instrucciones-->  
                      <div class="row">
                        <div class="col-md-12">
                            <div class="alert alert-info" role="alert"> <strong>Add the instructions</strong> Add the instructions that accompany the question, this information will be reflected in the script that is sent to the operator</div>
                          </div>
                      </div>
                      <!--Fila1-->            
                      <div class="row">
                             <div class="col-md-12">
                                <div class="form-wrapper form-group">
                                    <div class="form-group"> 
                                      <label class="control-label" for="instruction-fielding-instructions">Fielding Instructions</label>
                                      <textarea class="text-full form-control form-textarea" id="instruction-fielding-instructions" cols="60" rows="5"></textarea>
                                    </div>
                                </div>
                             </div>                            
                      </div>  
                      <!--Fila2-->            
                      <div class="row" style="margin-top:15px;">
                             <div class="col-md-12">
                                <div class="form-wrapper form-group">
                                    <div class="form-group"> 
                                      <label class="control-label" for="instruction-post-answer-instructions">Post-Answer Instruction</label>
                                      <textarea class="text-full form-control form-textarea" id="instruction-post-answer-instructions" cols="60" rows="5"></textarea>
                                    </div>
                                </div>
                             </div>                            
                      </div>

                      <!--Fila3-->            
                      <div class="row" style="margin-top:15px;">
                             <div class="col-md-12">
                                <div class="form-wrapper form-group">
                                    <div class="form-group"> 
                                      <label class="control-label" for="instruction-response-instructions">Response Instructions</label>
                                      <textarea class="text-full form-control form-textarea" id="instruction-response-instructions" cols="60" rows="5"></textarea>
                                    </div>
                                </div>
                             </div>                            
                      </div>


                    </div>
                </div>
            </div>
      </div>
      <div class="modal-footer">
        <input type="hidden" id="id-question-instruction" value="">
        <button type="button" class="btn btn-success" data-dismiss="modal" id="save-instruction"><span class="icon glyphicon glyphicon-ok" aria-hidden="true"></span>Save</button>
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