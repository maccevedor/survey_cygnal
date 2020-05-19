
jQuery(function ($){
	
	var roleId = Drupal.settings.RoleId; //assign the variable
		

	/*Funciones */

		//Convierte el textarea en un control con filas numeradas
		var numberLineTextarea=function(){
			$('.modal-body  textarea').numberedtextarea();
			$('.group-config  textarea').numberedtextarea();
			$('.field-name-field-cll-survey-quest-opt-inst  textarea').numberedtextarea();
			$('.field-name-field-cll-survey-quest-opt-xtab  textarea').numberedtextarea();
			$('.field-name-field-cll-survey-quest-opt-txt textarea').numberedtextarea();
		};

		
		var loadModalInstruction=function(){			
			$('#modal-instruction').modal({
				backdrop: 'static', keyboard: true
			});
		};

		var getDataResponseBank=function(tid,id){
			
			$.ajax({
			  url: Drupal.settings.base_url + '/survey/response-bank/data',
			  type: 'post',
			  data: {
				id: id,
				tid: tid,					
			  },
			  dataType: 'json',
			  success: function (response) {
				if (response!== undefined) {				  
				 setDataResponse(response,id);
				}
			  }
			});
		};

		var setDataResponse=function(response,id){					
			$(".field-name-field-cll-survey-quest-opt-txt textarea").val(response.response_script);
			$(".field-name-field-cll-survey-quest-opt-inst textarea").val(response.response_instruction);			
		};

		var setTxtInstruction=function(id){
			var txtHe=$(".field-name-field-cll-survey-question-txt-he textarea").val();
			$("#instruction-fielding-instructions").val(txtHe);

			var txtFo=$(".field-name-field-cll-survey-question-txt-fo textarea").val();
			$("#instruction-post-answer-instructions").val(txtFo);

			var txtOp=$(".field-name-field-cll-survey-question-txt-op textarea").val();
			$("#instruction-response-instructions").val(txtOp);
		};

		var saveTxtInstruction=function(id){
			var txtHe=$("#instruction-fielding-instructions").val();			
			$(".field-name-field-cll-survey-question-txt-he textarea").val(txtHe);

			var txtFo=$("#instruction-post-answer-instructions").val();
			$(".field-name-field-cll-survey-question-txt-fo textarea").val(txtFo);

			var txtOp=$("#instruction-response-instructions").val();
			$(".field-name-field-cll-survey-question-txt-op textarea").val(txtOp);
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
	
	
  		var $validator = $("#question-bank-node-form").validate({
		  rules: {
			//Datos 1
			//"title":"required ",
			"field_question_bank_category[und]":"required defaultInvalid",
			"field_question_bank_company[und]":"required defaultInvalid",
			"field_cll_survey_question[und][0][field_cll_survey_question_script][und][0][value]":"required",
			"field_cll_survey_question[und][0][field_cll_survey_question_type][und]":"required defaultInvalid",
			"field_cll_survey_question[und][0][field_cll_survey_question_option][und][0][field_cll_survey_quest_opt_txt][und][0][value]":"required",			
						
		  },
		  messages:{
			//Datos radicador
			//"title":"Dato obligatorio",
			"field_question_bank_category[und]":"This field is required",
			"field_question_bank_company[und]":"This field is required",
			"field_cll_survey_question[und][0][field_cll_survey_question_script][und][0][value]":"This field is required",		
		  	"field_cll_survey_question[und][0][field_cll_survey_question_type][und]":"This field is required",
		  	"field_cll_survey_question[und][0][field_cll_survey_question_option][und][0][field_cll_survey_quest_opt_txt][und][0][value]":"This field is required",
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

		

		$(document).on('click','#instruction-text button',function(e) {
			event.preventDefault();
			var id=$(this).prop("id");
			$("#id-question-instruction").val(id);
				
			//$(".modal-body #list-instruction").html("");	
			setTxtInstruction(id);
			loadModalInstruction();
			
		});

		$(document).on('click','#save-instruction',function(e) {
			var id=$("#id-question-instruction").val();
			saveTxtInstruction(id);
		});

		$(document).on('change','.group-answer-bank select',function(e) {
			var id=$(this).parents(".panel-body").find(".group-question-right").attr("id");
			var tid=$(this).val();
			getDataResponseBank(tid,id);
		});

		/*Iniciales*/	
		numberLineTextarea();			
});