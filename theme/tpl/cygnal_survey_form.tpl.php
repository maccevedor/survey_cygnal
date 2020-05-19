<?php
 
 if(is_numeric($form["nid"]["#value"])){
    $nid=$form["nid"]["#value"];
 }else{
    $nid=0;
 }
 
?>

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

<div class="modal fade" id="modal-question-bank" tabindex="-1" role="modal">
  <div class="modal-dialog modal-lg" role="document" >
    <div class="modal-content">
      <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        <h4 class="modal-title">Question Bank</h4>
      </div>
      <div class="modal-body">
           
           <div class="row">
              <div class="col-md-12">
                  <div id="list-question-bank">
                      <!--Aqui sale el listado de nodos de banco de preguntas-->                      
                    </div>
                </div>
            </div>
      </div>
      <div class="modal-footer">
        <input type="hidden" id="id-question-bank" value="">
        <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
      </div>
    </div><!-- /.modal-content -->
  </div><!-- /.modal-dialog -->
</div><!-- /.modal -->

<div class="modal fade" id="modal-demographics-bank" tabindex="-1" role="modal">
  <div class="modal-dialog modal-lg" role="document" >
    <div class="modal-content">
      <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        <h4 class="modal-title">Demographics & Recode Bank</h4>
      </div>
      <div class="modal-body">
           
           <div class="row">
              <div class="col-md-12">
                  <div id="list-demographics-bank">
                      <!--Aqui sale el listado de nodos de banco de preguntas-->                      
                    </div>
                </div>
            </div>
      </div>
      <div class="modal-footer">
        <input type="hidden" id="id-demographics-bank" value="">
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
                            <div class="alert alert-info" role="alert"> <strong>Add the instructions</strong>  that accompany the question, this information will be reflected in the script that is sent to the operator</div>
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

<fieldset class="required-fields field-group-fieldset panel panel-default form-wrapper">
  <legend class="panel-heading">
      <span class="panel-title fieldset-legend">Survey</span>
    </legend>
    <div class="panel-body">

<div class="row">
  <div class="col col-md-12">
    <div id="rootwizard">
      
      <div class="navbar">
        <div class="navbar-inner">
          <div class="container">
            <ul>
              <li><a href="#tab1" data-toggle="tab">
                    <span class="tab-md"><i class="fa fa-info fa-2x"></i> Basic Data<span>
                    <span class="tab-sm"><i class="fa fa-info fa-2x"></i> Basic<span>
                    <span class="tab-xs"><i class="fa fa-info fa-2x"></i><span>  
                  </a>
              </li>
              <li><a href="#tab2" data-toggle="tab" class="tab-md">
                    <span class="tab-md"><i class="fa fa-question fa-2x"></i> Questions<span>
                    <span class="tab-sm"><i class="fa fa-question fa-2x"></i> Questions<span>
                    <span class="tab-xs"><i class="fa fa-question fa-2x"></i><span>
                  </a>
              </li>              
              <li><a href="#tab3" data-toggle="tab" class="tab-md">
                    <span class="tab-md"><i class="fa fa-users fa-2x"></i> Demographics & Recodes<span>
                    <span class="tab-sm"><i class="fa fa-users fa-2x"></i> Demo&Reco<span>
                    <span class="tab-xs"><i class="fa fa-users fa-2x"></i><span>                    
                  </a>
              </li>              
            </ul>
          </div>
        </div>
      </div>
          
      <div id="bar" class="progress">
        <div class="progress-bar" role="progressbar" aria-valuenow="0" aria-valuemin="0" aria-valuemax="100" style="width: 0%;"></div>
      </div>
      
      <div class="tab-content">          
        <!--Inicio Tab-->
        <div class="tab-pane" id="tab1">              
          <div class="col-xs-12">
            <h3> <span class="label label-success">Step 1</span> - Basic data</h3>                          
            <!--Fila0-->
            <div class="row oculto">                          
              <div class="col-md-12"><?php print drupal_render($form["title"]);?><div class="oculto-campo"></div></div>             
            </div>
            <!--Fila1-->
                <div class="row">                          
                    <div class="col-xs-12 col-sm-6 col-md-2"><?php print drupal_render($form["field_survey_project_id"]);?><div class="oculto-campo"></div></div>
                    <div class="col-xs-12 col-sm-6 col-md-4"><?php print drupal_render($form["field_survey_district"]);?><div class="oculto-campo"></div></div>
                    <div class="col-xs-12 col-sm-4 col-md-2"><?php print drupal_render($form["field_survey_start_date"]);?><div class="oculto-campo"></div></div>
                    <div class="col-xs-12 col-sm-4 col-md-2"><?php print drupal_render($form["field_survey_end_date"]);?><div class="oculto-campo"></div></div>           
                    <div class="col-xs-12 col-sm-4 col-md-2"><?php print drupal_render($form["field_survey_status"]);?><div class="oculto-campo"></div></div>
                </div>
            <!--Fila2-->
            <div class="row">                          
              <div class="col-xs-12 col-sm-12 col-md-12"><?php print drupal_render($form["field_survey_survey_of"]);?><div class="oculto-campo"></div></div>
            </div> 
            <!--Fila2-->
            <div class="row">
              <div class="col-xs-12 col-sm-6 col-md-6"><?php print drupal_render($form["field_survey_consultant"]);?><div class="oculto-campo"></div></div>
              <div class="col-xs-12 col-sm-6 col-md-6"><?php print drupal_render($form["field_survey_consumer"]);?><div class="oculto-campo"></div></div>
            </div>

            <!--Fila2-->
            <div class="row">                    
                <div class="col-xs-12 col-sm-6 col-md-3"><?php print drupal_render($form["field_survey_sample_size"]);?><div class="oculto-campo"></div></div>
                <div class="col-xs-12 col-sm-6 col-md-3"><?php print drupal_render($form["field_survey_methodology"]);?><div class="oculto-campo"></div></div>
                <div class="col-xs-12 col-sm-6 col-md-3"><?php print drupal_render($form["field_survey_percente_cell"]);?><div class="oculto-campo"></div></div>
                <div class="col-xs-12 col-sm-6 col-md-3"><?php print drupal_render($form["field_survey_margin_error"]);?><div class="oculto-campo"></div></div>
            </div>                        
            <!--Fila3-->
            <div class="row">                          
              <div class="col-xs-12 col-sm-6 col-md-6"><?php print drupal_render($form["field_survey_script_intro"]);?><div class="oculto-campo"></div></div>
              <div class="col-xs-12 col-sm-6 col-md-6"><?php print drupal_render($form["field_survey_script_close"]);?><div class="oculto-campo"></div></div>            
            </div>        

          </div>            
        </div>
        <!--Fin Tab-->

        <!--Inicio Tab-->
        <div class="tab-pane" id="tab2">              
          <div class="col-xs-12">
            <h3> <span class="label label-success">Step 2</span> - Questions</h3>               
            <!--Fila1-->
            
            <div class="row">                          
              <div class="col-md-12"><?php print drupal_render($form["field_survey_questions"]);?><div class="oculto-campo"></div></div>
            </div>  

          </div>            
        </div>
        <!--Fin Tab-->

        <!--Inicio Tab-->
        <div class="tab-pane" id="tab3">              
          <div class="col-xs-12">
            <h3> <span class="label label-success">Step 3</span> - Demographics & Recodes</h3>               
            
            <!--Fila1-->            
            <div class="row">                          
              <div class="col-md-12"><?php print drupal_render($form["field_survey_demo_instructions"]);?><div class="oculto-campo"></div></div>
            </div>
            <!--Fila2-->            
            <div class="row">                          
              <div class="col-md-12"><?php print drupal_render($form["field_survey_demographics"]);?><div class="oculto-campo"></div></div>
            </div>  
            <!--Fila3-->
            <div class="row"> 
                 <?php print drupal_render_children($form);?>
            </div> 
            
            <div class="row">
              <div class="col-md-9">&nbsp;</div>
                <div class="col-md-3">
                   <?php print drupal_render($form['actions']['submit']);?>
                </div>    
                
            </div> 

          </div>            
        </div>
        <!--Fin Tab-->             
            
        <div style="float:right">
            <input type='button' class='btn btn-primary button-next' name='next' value='Next' />
        </div>
        <div style="float:left">
            <input type='button' class='btn btn-primary button-previous' name='previous' value='Previous' />
        </div>

        <input type="hidden" id="nid" value="<?php echo $nid;?>">
          
      </div>
      <!--Fin Tabcontent-->  
    </div>   
    <!--Fin rootwizard -->
  </div>
  <!--Fin col -->
</div>
<!--Fin row -->  

</div>
</fieldset>      
    	

                                  
                    
              
