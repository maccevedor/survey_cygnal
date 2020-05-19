<?php
  //Se obtiene el nid del nodo, en este caso, se obtiene con la ayuda del campo de status, 
  // ya que directamente no lo trae la estructura  
  $nid=$content["field_survey_status"]["#object"]->nid;
  //Obtiene el estado actual del nodo
  $tid_status=$content["field_survey_status"]["#object"]->field_survey_status[LANGUAGE_NONE][0]['tid'];
  $term_status=taxonomy_term_load($tid_status);
  $code_status=strtoupper($term_status->field_tax_survey_status_code[LANGUAGE_NONE][0]["value"]);
  $show_upload=false;
  if($code_status=="7"){//Solo aplica para el estado Completed
    $show_upload=true;
  }

  $not_defined="Not defined yet";

  #Se obtienen valores de campo que pueden ser editables desde el nodo, para replicar estos valores en lugares diferentes
  $start_date=isset($content["field_survey_start_date"]["#object"]->field_survey_start_date[LANGUAGE_NONE][0])?$content["field_survey_start_date"]["#object"]->field_survey_start_date[LANGUAGE_NONE][0]['value']:"";
  $end_date=isset($content["field_survey_end_date"]["#object"]->field_survey_end_date[LANGUAGE_NONE][0])?$content["field_survey_end_date"]["#object"]->field_survey_end_date[LANGUAGE_NONE][0]['value']:"";
  $sample_size=isset($content["field_survey_sample_size"]["#object"]->field_survey_sample_size[LANGUAGE_NONE][0])?$content["field_survey_sample_size"]["#object"]->field_survey_sample_size[LANGUAGE_NONE][0]['value']:"";
  $percent_cell=isset($content["field_survey_percente_cell"]["#object"]->field_survey_percente_cell[LANGUAGE_NONE][0])?$content["field_survey_percente_cell"]["#object"]->field_survey_percente_cell[LANGUAGE_NONE][0]['value']:"";
  $coefficient=isset($content["field_survey_coefficient"]["#object"]->field_survey_coefficient[LANGUAGE_NONE][0])?$content["field_survey_coefficient"]["#object"]->field_survey_coefficient[LANGUAGE_NONE][0]['value']:"";
  $margin_error=isset($content["field_survey_margin_error"]["#object"]->field_survey_margin_error[LANGUAGE_NONE][0])?$content["field_survey_margin_error"]["#object"]->field_survey_margin_error[LANGUAGE_NONE][0]['value']:"";
  if(isset($content["field_survey_methodology"]["#object"]->field_survey_methodology[LANGUAGE_NONE][0])){
    $tid_methodology=$content["field_survey_methodology"]["#object"]->field_survey_methodology[LANGUAGE_NONE][0]['tid'];
    $term_methodology=taxonomy_term_load($tid_methodology);
    $methodology=$term_methodology->name;
  }else{
    $methodology="";
  }

    #Si el rol del usuario es 4 o 6, osea consultan o consumer, desabilitara los tabs no tendra opcion a editar.
    $permite_rol=true;
    foreach($user->roles as $key=>$role){  
      if($key==4 || $key==6){
        $permite_rol=false;
      }
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

<div class="modal fade" id="modal-download-file" tabindex="-1" role="modal">
  <div class="modal-dialog" role="document" >
    <div class="modal-content">
      <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        <h4 class="modal-title">Download files</h4>
      </div>
      <div class="modal-body">
           <div class="row">
              <div class="col-md-12">
                  <div id="list-download-file">
                        <div class="alert alert-info" role="alert"> <i class="fa fa-gear fa-4x fa-spin"></i> Generate links download</div>
                    </div>
                </div>
            </div>
      </div>
      <div class="modal-footer">
        <input type="hidden" name="nid" id="nid" value="">
        <button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
      </div>
    </div><!-- /.modal-content -->
  </div><!-- /.modal-dialog -->
</div><!-- /.modal -->

<div class="modal fade" id="modal-upload-file" tabindex="-1" role="modal">
  <div class="modal-dialog" role="document" >
    <div class="modal-content">
      <div class="modal-header">
        <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
        <h4 class="modal-title">Upload files</h4>
      </div>
      <div class="modal-body">
            <div class="row">
              <div class="col-xs-12 col-sm-12 col-md-12">                  
                    <div class="alert alert-info" role="alert"> <i class="fa fa-upload fa-2x"></i> Upload the SS file that contains the survey responses</div>
              </div>
            </div>

            <div class="row">
              <div class="col-xs-12 col-sm-12 col-md-12">                  
                    <div class="alert alert-warning" role="alert"> <i class="fa fa-info-circle fa-2x"></i> The fields below can be changed to generate the Crosstab and Toplines file with real data, keep in mind that they are automatic fields, they are saved when making any changes, with the exception of the SS field, if you must click on Save</div>
              </div>
            </div>
            <div class="row">
                <div class="col-xs-12 col-sm-4 col-md-4"><?php print drupal_render($content["field_survey_start_date"]);?><div class="oculto-campo"></div></div>
                <div class="col-xs-12 col-sm-4 col-md-4"><?php print drupal_render($content["field_survey_end_date"]);?><div class="oculto-campo"></div></div>
                <div class="col-xs-12 col-sm-4 col-md-4"><?php print drupal_render($content["field_survey_sample_size"]);?><div class="oculto-campo"></div></div>
            </div>           
            <div class="row">    
                <div class="col-xs-12 col-sm-4 col-md-4"><?php print drupal_render($content["field_survey_methodology"]);?><div class="oculto-campo"></div></div>
                <div class="col-xs-12 col-sm-4 col-md-4"><?php print drupal_render($content["field_survey_percente_cell"]);?><div class="oculto-campo"></div></div>
                <div class="col-xs-12 col-sm-4 col-md-4"><?php print drupal_render($content["field_survey_margin_error"]);?><div class="oculto-campo"></div></div>
            </div>

          <div class="row">                          
              <div class="col-xs-12 col-sm-12 col-md-12"> <?php print drupal_render($content["field_survey_ss_file"]);?></div>
          </div>  
               
      </div>
      <div class="modal-footer">        
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
              <li><a href="#tab1" data-toggle="tab" class="tab-md"><i class="fa fa-info fa-2x"></i> Basic Data</a></li>
              <li><a href="#tab1" data-toggle="tab" class="tab-sm"><i class="fa fa-info fa-2x"></i> Basic</a></li>
              <li><a href="#tab1" data-toggle="tab" class="tab-xs"><i class="fa fa-info fa-2x"></i></a></li>
              <li><a href="#tab2" data-toggle="tab" class="tab-md"><i class="fa fa-question fa-2x"></i> Questions</a></li>
              <li><a href="#tab2" data-toggle="tab" class="tab-sm"><i class="fa fa-question fa-2x"></i> Questions</a></li>
              <li><a href="#tab2" data-toggle="tab" class="tab-xs"><i class="fa fa-question fa-2x"></i></a></li>
              <li><a href="#tab3" data-toggle="tab" class="tab-md"><i class="fa fa-users fa-2x"></i> Demographics & Recodes</a></li>
              <li><a href="#tab3" data-toggle="tab" class="tab-sm"><i class="fa fa-users fa-2x"></i> Demo&Reco</a></li>
              <li><a href="#tab3" data-toggle="tab" class="tab-xs"><i class="fa fa-users fa-2x"></i></a></li>
              <li><a href="#" class="download-file tab-md" id="<?=$nid?>"  title="Download"><i class="fa fa-download fa-2x"></i> Download</a></li> 
              <li><a href="#" class="download-file tab-sm" id="<?=$nid?>"  title="Download"><i class="fa fa-download fa-2x"></i> Down</a></li> 
              <li><a href="#" class="download-file tab-xs" id="<?=$nid?>"  title="Download"><i class="fa fa-download fa-2x"></i></a></li>  
              <?php if($show_upload==true && $permite_rol==true){?>
                <li><a href="#" class="upload-file tab-md" id="<?=$nid?>" title="Upload SS File"><i class="fa fa-upload fa-2x"></i> Upload</a></li>
                <li><a href="#" class="upload-file tab-sm" id="<?=$nid?>" title="Upload SS File"><i class="fa fa-upload fa-2x"></i> Up</a></li>
                <li><a href="#" class="upload-file tab-xs" id="<?=$nid?>" title="Upload SS File"><i class="fa fa-upload fa-2x"></i></a></li>             
              <?php }?>
            </ul>
          </div>
        </div>
      </div>         
           
      <div class="tab-content">          
        <!--Inicio Tab-->
        <div class="tab-pane" id="tab1">              
          <div class="col-xs-12">
            <h3> <span class="label label-success">Step 1</span> - Basic data</h3>                          
            <!--Fila1-->

            <div class="row">                          
                <div class="col-xs-12 col-sm-4 col-md-2"><?php print drupal_render($content["field_survey_project_id"]);?><div class="oculto-campo"></div></div>
                <div class="col-xs-12 col-sm-4 col-md-3"><?php print drupal_render($content["field_survey_district"]);?><div class="oculto-campo"></div></div>              
                <div class="col-xs-12 col-sm-4 col-md-2"><div class="field field-name-field-survey-start-date field-type-text field-label-above"><div class="field-label">Start Date:&nbsp;</div><div class="field-items"><div class="field-item even"><?=$start_date?></div></div></div><div class="oculto-campo"></div></div>
                <div class="col-xs-12 col-sm-4 col-md-2"><div class="field field-name-field-survey-start-date field-type-text field-label-above"><div class="field-label">Start Date:&nbsp;</div><div class="field-items"><div class="field-item even"><?=$start_date?></div></div></div><div class="oculto-campo"></div></div>
                <div class="col-xs-12 col-sm-4 col-md-1" title="Qustion number"><?php print drupal_render($content["field_survey_question_number"]);?><div class="oculto-campo"></div></div>
                <div class="col-xs-12 col-sm-4 col-md-2"><?php print drupal_render($content["field_survey_status"]);?><div class="oculto-campo"></div></div>
            </div>
            <!--Fila2-->
            <div class="row">                          
                <div class="col-xs-12 col-sm-4 col-md-6"><?php print drupal_render($content["field_survey_survey_of"]);?><div class="oculto-campo"></div></div>
                <div class="col-xs-12 col-sm-4 col-md-3"><?php print drupal_render($content["field_survey_consumer"]);?><div class="oculto-campo"></div></div>
                <div class="col-xs-12 col-sm-4 col-md-3"><?php print drupal_render($content["field_survey_consultant"]);?><div class="oculto-campo"></div></div>
            </div> 
            
            <div class="row">
                <div class="col-xs-12 col-sm-6 col-md-3"><div class="field field-name-field-survey-sample_size field-type-text field-label-above"><div class="field-label">Sample size:&nbsp;</div><div class="field-items"><div class="field-item even"><? if(trim($sample_size)!=""){echo $sample_size;}else{echo $not_defined;}?></div></div></div><div class="oculto-campo"></div></div>
                <div class="col-xs-12 col-sm-6 col-md-3"><div class="field field-name-field-survey-methodology field-type-text field-label-above"><div class="field-label">Methodology:&nbsp;</div><div class="field-items"><div class="field-item even"><? if(trim($methodology)!=""){echo $methodology;}else{echo $not_defined;}?></div></div></div><div class="oculto-campo"></div></div>
                <div class="col-xs-12 col-sm-6 col-md-3"><div class="field field-name-field-survey-percente_cell field-type-text field-label-above"><div class="field-label">%Cell:&nbsp;</div><div class="field-items"><div class="field-item even"><? if(trim($percent_cell)!=""){echo $percent_cell;}else{echo $not_defined;}?></div></div></div><div class="oculto-campo"></div></div>
                <div class="col-xs-12 col-sm-6 col-md-3"><div class="field field-name-field-survey-margin-error field-type-text field-label-above"><div class="field-label">Margin Of Error:&nbsp;</div><div class="field-items"><div class="field-item even"><? if(trim($margin_error)!=""){echo $margin_error;}else{echo $not_defined;}?></div></div></div><div class="oculto-campo"></div></div>
            </div>                     
            <!--Fila3-->
            <div class="row">                          
                <div class="col-md-12"><?php print drupal_render($content["field_survey_script_intro"]);?><div class="oculto-campo"></div></div>                            
            </div> 
             <div class="row">
                <div class="col-md-12"><?php print drupal_render($content["field_survey_script_close"]);?><div class="oculto-campo"></div></div>            
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
              <div class="col-md-12">
                  <div class="form-group">
                    <div class="form-check">
                      <input class="form-check-input approved-all-questions" type="checkbox" id="gridCheckUP">
                      <label class="form-check-label" for="gridCheckUP">
                        Approved all
                      </label>
                    </div>
                  </div>
              </div>  
            </div> 
            <!--Fila2-->
            <div class="row">                          
              <div class="col-md-12"><?php print drupal_render($content["field_survey_questions"]);?><div class="oculto-campo"></div></div>
            </div> 

             <!--Fila3-->
            <div class="row">                          
              <div class="col-md-12">
                  <div class="form-group">
                    <div class="form-check">
                      <input class="form-check-input approved-all-questions" type="checkbox" id="gridCheckDown">
                      <label class="form-check-label" for="gridCheckDown">
                        Approved all
                      </label>
                    </div>
                  </div>
              </div>  
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
              <div class="col-md-12"><?php print drupal_render($content["field_survey_demo_instructions"]);?><div class="oculto-campo"></div></div>
            </div>
            <!--Fila2-->            
            <div class="row">                          
              <div class="col-md-12"><?php print drupal_render($content["field_survey_demographics"]);?><div class="oculto-campo"></div></div>
            </div>  
           
          </div>            
        </div>
        <!--Fin Tab-->     
            
       
      </div>
      <!--Fin Tabcontent-->  
    </div>   
    <!--Fin rootwizard -->
  </div>
  <!--Fin col -->
</div>
<!--Fin row --> 

<div class="row">
	<div class="col-md-12">
    	 <?php print drupal_render($content);?>
  </div>
</div> 
</div>
</fieldset>  