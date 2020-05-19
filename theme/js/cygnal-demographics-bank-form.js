
jQuery(function ($){
	
	var roleId = Drupal.settings.RoleId; //assign the variable
		

	/*Funciones */

		//Convierte el textarea en un control con filas numeradas
		var numberLineTextarea=function(){
			$('.modal-body  textarea').numberedtextarea();
			$('.group-config  textarea').numberedtextarea();
			$('.field-name-field-cll-survey-quest-opt-scr  textarea').numberedtextarea();
			$('.field-name-field-cll-survey-quest-option-xt  textarea').numberedtextarea();
			$('.field-name-field-cll-survey-quest-option-in textarea').numberedtextarea();
		};

		
		var loadModalInstruction=function(){			
			$('#modal-instruction').modal({
				backdrop: 'static', keyboard: true
			});
		};

		
	//Eventos
		$.validator.addMethod("defaultInvalid", function(value, element) 
	    {
			 switch (element.value){
				  case "_none":
					   if (element.name == "_none"){
						  return false;
					   }
				  break;
				  case "0":
					   if (element.name == "0"){
						  return false;
					   }
				  break;
				  default:
						return true;	  
						break;
			 }
		 
	    });
	
	
  		var $validator = $("#demographics-recode-bank-node-form").validate({
		  rules: {
			//Datos 1
			//"title":"required ",
			"field_demographics_category[und]":"required defaultInvalid",
			"field_survey_question_control[und][0][field_cll_survey_quest_label][und][0][value]":"required",	
			"field_survey_question_control[und][0][field_cll_survey_quest_opt_scr][und][0][value]":"required",		
						
		  },
		  messages:{
			//Datos radicador
			//"title":"Dato obligatorio",
			"field_demographics_category[und]":"This field is required",
			"field_survey_question_control[und][0][field_cll_survey_quest_label][und][0][value]":"This field is required",		
		  	"field_survey_question_control[und][0][field_cll_survey_quest_opt_scr][und][0][value]":"This field is required",
		  },
		   submitHandler: function(form) {				
				/*$("#edit-submit").append("<button type='button' id='edit-send-message' name='op' value='Guardando...' class='btn btn-success form-submit'><span class='icon glyphicon glyphicon-ok' aria-hidden='true'></span> Guardando ...</button>");*/
				$("#edit-submit").prop("disabled",true);	
				$("#edit-submit").html("<span class='icon glyphicon glyphicon-save-file' aria-hidden='true'></span> Saving ...");			
				return true;				
		   }
		  
		});	

		
		
		/*Eventos*/	
		/*$.each($('textarea'), function() {
			    var offset = this.offsetHeight - this.clientHeight;
			 
			    var resizeTextarea = function(el) {
			        $(el).css('height', 'auto').css('height', el.scrollHeight + offset);
			    };
			    resizeTextarea(this);
			    $(this).on('keyup input', function() { resizeTextarea(this); });
		});*/

		
		/*Iniciales*/	
		numberLineTextarea();			
});