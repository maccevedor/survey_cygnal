
jQuery(function ($){
	
	/*Coimportamientos especiales Drupal*/
	Drupal.behaviors.setNumberQuestion = {
	    attach: function(context, settings) {
	      resizeTextarea();
	      proccesNumberQuestion();
	      $(".button-remove-question").hide();
	      $(".instructions").hide();
	      //numberLineTextarea();
	      $('.group-options  textarea').numberedtextarea();	 

	      var $valid = $("#survey-node-form").valid();
  			if(!$valid) {
  				$validator.focusInvalid();
  				return false;
  			}
	    }
	}

	Drupal.behaviors.setNumberDemographics = {
	    attach: function(context, settings) {
	      proccesNumberDemographics();
	      $(".button-remove-question-control").hide();
	      $(".instructions").hide();
	      //numberLineTextarea();	 
	    }
	}

	/*Variables*/
	var num_question=0;
	var num_demographics=0;
	var roleId = Drupal.settings.RoleId; //assign the variable
	var records_consumer = Drupal.settings.records_consumer;
	var records_consultant = Drupal.settings.records_consultant;
	

	/*Funciones */

		var allowNumber=function(e){
			if (!(!e.shiftKey //Disallow: any Shift+digit combination
					&& !(e.keyCode < 48 || e.keyCode > 57) //Disallow: everything but digits
					|| !(e.keyCode < 96 || e.keyCode > 105) //Allow: numeric pad digits
					|| e.keyCode == 46 // Allow: delete
					|| e.keyCode == 8  // Allow: backspace
					|| e.keyCode == 9  // Allow: tab
					|| e.keyCode == 27 // Allow: escape
					|| (e.keyCode == 65 && (e.ctrlKey === true || e.metaKey === true)) // Allow: Ctrl+A
					|| (e.keyCode == 67 && (e.ctrlKey === true || e.metaKey === true)) // Allow: Ctrl+C
					 || (e.keyCode == 86 && (e.ctrlKey === true || e.metaKey === true)) // Allow: Ctrl+V
					//Uncommenting the next line allows Ctrl+V usage, but requires additional code from you to disallow pasting non-numeric symbols
					//|| (event.keyCode == 86 && (event.ctrlKey === true || event.metaKey === true)) // Allow: Ctrl+Vpasting 
					|| (e.keyCode >= 35 && e.keyCode <= 39) // Allow: Home, End
					)) {
				e.preventDefault();
			}
		
		};

		var resizeTextarea = function(){

			$(".field-name-field-cll-survey-question-script input").each(function (){
				var style = $(this).attr('style'),
			    textbox = $(document.createElement('textarea'));
			    textbox.attr('style','white-space:normal; word-wrap: break-word!important;');
			    textbox.prop('id',$(this).prop('id'));
			    textbox.prop('name',$(this).prop('name'));
			    textbox.prop('class',$(this).prop('class'));
			    textbox.val($(this).val());
			    textbox.prop('cols',"50");
			    textbox.prop('rows',"1");
			    $(this).replaceWith(textbox);		    
			});

			//autosize($('.field-name-field-cll-survey-question-script textarea'));

			$('.field-name-field-cll-survey-question-script textarea').each(function (){				
				var ta = this;				
				ta.addEventListener('focus', function(){
				  autosize(ta);
				});				
			});
		};

		var proccesNumberQuestion=function(){
			initQuestion();
			$("#edit-field-survey-questions table.field-multiple-table tbody tr .question-num").each(function(event){
	   			num=getNumberQuestion();	  
	   			//Asigna el numero de la pregunta
	   			$(this).html("Q"+num+"-");
	   			//Asigna id a los botones de question Bank y Instructions para recuperar los datos
	   			//$(this).parents(".panel-body").find(".group-tooltip #instruction-text button").attr("id","Q"+(parseInt(num)));
	   			//$(this).parents(".panel-body").find(".group-tooltip #question_bank button").attr("id","Q"+(parseInt(num)));	

	   			$(this).parent().parent().parent().parent().parent().find(".group-tooltip #instruction-text button").attr("id","Q"+(parseInt(num)));
	   			$(this).parent().parent().parent().parent().parent().find(".group-tooltip #question_bank button").attr("id","Q"+(parseInt(num)));	

	   			//Asigna id al contendor de la informacion de la pregunta, para poder asignarlo desde la modal
	   			$(this).parent().parent().parent().parent().parent().find(".group-question-right").attr("id","Q"+(parseInt(num)));

	   			
	   			//Incrementa el contador de preguntas
	   			
	   			
	   			addNumberQuestion();	
	   		});   					
		};

		var proccesNumberDemographics=function(){
			initDemographics();
			$("#edit-field-survey-demographics table.field-multiple-table tbody tr .demographics-num").each(function(event){
	   			num=getNumberDemographics();	  
	   			//Asigna el numero de la pregunta
	   			$(this).html("D"+num+"-");
	   			
	   			//Asigna id a los botones de demographics Bank para recuperar los datos
	   			//$(this).parents(".panel-body").find(".group-tooltip-demo #demographics_bank button").attr("id","D"+(parseInt(num)));
	   			$(this).parent().parent().parent().parent().parent().find(".group-tooltip-demo #demographics_bank button").attr("id","D"+(parseInt(num)));	
	   			
	   			//Incrementa el contador de preguntas
	   			//Asigna id al contendor de la informacion de la demographics, para poder asignarlo desde la modal
	   			//$(this).parents(".panel-body").find(".group-demographics-right").attr("id","D"+(parseInt(num)));
	   			$(this).parent().parent().parent().parent().parent().find(".group-demographics-right").attr("id","D"+(parseInt(num)));

	   			
	   			addNumberDemographics();	
	   		});   					
		};

		var initQuestion=function (){
			num_question=1;
		};

		var initDemographics=function (){
			num_demographics=1;
		};

		var addNumberQuestion=function(){
			num_question=parseInt(num_question)+1;		
		};

		var addNumberDemographics=function(){
			num_demographics=parseInt(num_demographics)+1;		
		};

		var getNumberQuestion=function(){
			return num_question;	
		};

		var getNumberDemographics=function(){
			return num_demographics;	
		};

		//Convierte el textarea en un control con filas numeradas
		var numberLineTextarea=function(){
			$('.modal-body  textarea').numberedtextarea();
			$('.group-config  textarea').numberedtextarea();
			$('.group-labels  textarea').numberedtextarea();
			$('.group-options  textarea').numberedtextarea();

			$('.field-name-field-survey-demo-instructions textarea').numberedtextarea();			
			//$('.field-name-field-cll-survey-quest-opt-inst  textarea').numberedtextarea();
			//$('.field-name-field-cll-survey-quest-opt-txt textarea').numberedtextarea();
		};

		var loadModalQuestionBank=function(){
			$('#modal-question-bank').modal({
				backdrop: 'static', keyboard: true
			});
		};

		var loadModalDemographicsBank=function(){
			$('#modal-demographics-bank').modal({
				backdrop: 'static', keyboard: true
			});
		};

		var loadModalInstruction=function(){
			$('#modal-instruction').modal({
				backdrop: 'static', keyboard: true
			});
		};

		var loadOptions=function(data,obj,validas){
				//Se hace el cargue de las opciones del primer nivel
				var html="<option value='_none'>- None -</option>";
				data.forEach( function(value, index){	
					if(validas.length>0){
						validas.forEach( function(value2, index2){
							if(value.id===value2){
								html +="<option value='"+value.id+"'>"+value.text+"</option>";
							}
						});
					}else{
						html +="<option value='"+value.id+"'>"+value.text+"</option>";
					}

				});			
				$(obj).html(html);
		};

		var getTitles=function(){
			var nid=$("#nid").val();
			var title=$("#edit-field-survey-project-id-und-0-value").val()+"-"+$("#edit-field-survey-district-und-0-value").val();			
			$.ajax({
			  url: Drupal.settings.base_url + '/survey/title/validate',
			  type: 'post',
			  data: {
				nid: nid,
				title: title,					
			  },
			  dataType: 'json',
			  success: function (response) {
				if (response!== undefined) {				  
					if(response[0].num>0){
						$("#modal-message .modal-body").html("<div class='alert alert-warning' role='alert' id='alert-message'>Change the Project ID, there is already a survey with this number</div>");
						$('#modal-message').modal({
							backdrop: 'static', keyboard: false
						});
						$("#edit-field-survey-project-id-und-0-value").val("");						
						$(".button-previous").click();
					}				
				}
			  }
			});
		};

		var getListQuestionBank=function(){
			
			$.ajax({
			  url: Drupal.settings.base_url + '/survey/question-bank/list',
			  type: 'post',			  
			  dataType: 'json',
			  success: function (response) {
				if (response!== undefined) {				  
				 $(".modal-body #list-question-bank").html(response);
				}
			  }
			});
		};

		var getListDemographicsBank=function(){
			
			$.ajax({
			  url: Drupal.settings.base_url + '/survey/demographics-bank/list',
			  type: 'post',			 
			  dataType: 'json',
			  success: function (response) {
				if (response!== undefined) {				  
				 $(".modal-body #list-demographics-bank").html(response);
				}
			  }
			});
		};

		var getDataQuestionBank=function(nid,id){
			
			$.ajax({
			  url: Drupal.settings.base_url + '/survey/question-bank/data',
			  type: 'post',
			  data: {
				id: id,
				nid: nid,					
			  },
			  dataType: 'json',
			  success: function (response) {
				if (response!== undefined) {				  
				 setDataQuestion(response,id);
				}
			  }
			});
		};

		var getDataDemographicsBank=function(nid,id){
			
			$.ajax({
			  url: Drupal.settings.base_url + '/survey/demographics-bank/data',
			  type: 'post',
			  data: {
				id: id,
				nid: nid,					
			  },
			  dataType: 'json',
			  success: function (response) {
				if (response!== undefined) {				  
				 setDataDemographics(response,id);
				}
			  }
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

		var getDataConsumer=function(id){
			
			$.ajax({
			  url: Drupal.settings.base_url + '/survey/consumer/data',
			  type: 'post',
			  data: {
				id: id,									
			  },
			  dataType: 'json',
			  success: function (response) {
				if (response!== undefined) {				  
				 setDataConsumer(response,id);
				}
			  }
			});
		};		

		var setDataQuestion=function(response,id){
					
			$(".group-question-right#"+id+" .field-name-field-cll-survey-question-script textarea").val(response.question_script);
			$(".group-question-right#"+id+" .field-name-field-cll-survey-question-xtab input").val(response.question_xtab);
			$(".group-question-right#"+id+" .field-name-field-cll-survey-question-type select").val(response.question_type);
			$(".group-question-right#"+id+" .field-name-field-cll-survey-question-txt-he textarea").val(response.instruction_he);
			$(".group-question-right#"+id+" .field-name-field-cll-survey-question-txt-fo textarea").val(response.instruction_fo);
			$(".group-question-right#"+id+" .field-name-field-cll-survey-question-txt-op textarea").val(response.instruction_op);
			$(".group-question-right#"+id+" .field-name-field-cll-survey-quest-opt-txt textarea").val(response.response_script);
			$(".group-question-right#"+id+" .field-name-field-cll-survey-quest-opt-xtab textarea").val(response.response_xtab);
			$(".group-question-right#"+id+" .field-name-field-cll-survey-quest-opt-inst textarea").val(response.response_instruction);
			
			$(".group-options  textarea").each(function() {
				$(this).keyup();
			});	

			$('#modal-question-bank').modal('hide');
		};

		var setDataDemographics=function(response,id){
					
			$(".group-demographics-right#"+id+" .field-name-field-cll-survey-quest-label input").val(response.label_script);
			$(".group-demographics-right#"+id+" .field-name-field-cll-survey-quest-xtab input").val(response.label_xtab);
			$(".group-demographics-right#"+id+" .field-name-field-cll-survey-quest-opt-scr textarea").val(response.option_script);
			$(".group-demographics-right#"+id+" .field-name-field-cll-survey-quest-option-xt textarea").val(response.option_xtab);
			$(".group-demographics-right#"+id+" .field-name-field-cll-survey-quest-option-in textarea").val(response.option_instruction);
			
			$(".group-options  textarea").each(function() {
				$(this).keyup();
			});	

			$('#modal-demographics-bank').modal('hide');
		};

		var setDataResponse=function(response,id){
			
			$(".group-question-right#"+id+" .field-name-field-cll-survey-quest-opt-txt textarea").val(response.response_script);
			$(".group-question-right#"+id+" .field-name-field-cll-survey-quest-opt-xtab textarea").val(response.response_xtab);
			$(".group-question-right#"+id+" .field-name-field-cll-survey-quest-opt-inst textarea").val(response.response_instruction);
			
			$(".group-options  textarea").each(function() {
				$(this).keyup();
			});			
		};

		var setDataConsumer=function(response,id){
			loadOptions(response,'#edit-field-survey-consumer-und','');
		};

		var setTxtInstruction=function(id){
			var txtHe=$(".group-question-right#"+id+" .field-name-field-cll-survey-question-txt-he textarea").val();
			$("#instruction-fielding-instructions").val(txtHe);

			var txtFo=$(".group-question-right#"+id+" .field-name-field-cll-survey-question-txt-fo textarea").val();
			$("#instruction-post-answer-instructions").val(txtFo);

			var txtOp=$(".group-question-right#"+id+" .field-name-field-cll-survey-question-txt-op textarea").val();
			$("#instruction-response-instructions").val(txtOp);
		};

		var saveTxtInstruction=function(id){
			var txtHe=$("#instruction-fielding-instructions").val();			
			$(".group-question-right#"+id+" .field-name-field-cll-survey-question-txt-he textarea").val(txtHe);

			var txtFo=$("#instruction-post-answer-instructions").val();
			$(".group-question-right#"+id+" .field-name-field-cll-survey-question-txt-fo textarea").val(txtFo);

			var txtOp=$("#instruction-response-instructions").val();
			$(".group-question-right#"+id+" .field-name-field-cll-survey-question-txt-op textarea").val(txtOp);
		};	

		var preload=function(){
			var consumer=$("#edit-field-survey-consumer-und").val();
			var consultant=$("#edit-field-survey-consumer-und").val();

			loadOptions(records_consumer,"#edit-field-survey-consumer-und",'');
			loadOptions(records_consultant,"#edit-field-survey-consultant-und",'');

			$("#edit-field-survey-consumer-und").val(consumer);
			$("#edit-field-survey-consumer-und").val(consultant);

			proccesNumberQuestion();
			proccesNumberDemographics();
			numberLineTextarea();	

			if($("#nid").val()!=0){
				var cancel="<input type='button' class='btn btn-primary button-cancel' name='cancel' value='Cancel' />";
				$(".form-actions").append(cancel);
			}
		};

		var validaFieldQuestion=function(){

			var checked=false;					
			$(".field-name-field-cll-survey-question-script input").each(function() {
				if($.trim($(this).val())!=""){
					checked=true;
				}
			});
			return checked;		
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
	
	
  		var $validator = $("#survey-node-form").validate({
		  rules: {
			//Datos 1
			//"title":"required ",
			"field_survey_district[und][0][value]":"required",
			//"field_survey_population[und][0][value]":"required",
			"field_survey_survey_of[und][0][value]":"required",
			"field_survey_consumer[und]":"required defaultInvalid",	
			"field_survey_script_close[und][0][value]":"required",
			"field_survey_percente_cell[und][0][value]":{required:true, number:true, range:[0,100]},
			"field_survey_sample_size[und][0][value]":{required:true, number:true},	
			"field_survey_methodology[und]":"required defaultInvalid",
				
						
		  },
		  messages:{
			//Datos radicador
			//"title":"Dato obligatorio",
			"field_survey_district[und][0][value]":"This field is required",
			//"field_survey_population[und][0][value]":"This field is required",
			"field_survey_survey_of[und][0][value]":"This field is required",
			"field_survey_consumer[und]":"This field is required",	
			"field_survey_script_close[und][0][value]":"This field is required",	
			"field_survey_percente_cell[und][0][value]":"This field is required, only numbers, range[0,100]",
			"field_survey_sample_size[und][0][value]":"This field is required",	
			"field_survey_methodology[und]":"This field is required",
		  },
		   submitHandler: function(form) {				
				/*$("#edit-submit").append("<button type='button' id='edit-send-message' name='op' value='Guardando...' class='btn btn-success form-submit'><span class='icon glyphicon glyphicon-ok' aria-hidden='true'></span> Guardando ...</button>");*/
				var title=$("#edit-field-survey-project-id-und-0-value").val()+"-"+$("#edit-field-survey-district-und-0-value").val();
				$("#edit-title").val(title);

				$("#edit-submit").prop("disabled",true);	
				$("#edit-submit").html("<span class='icon glyphicon glyphicon-save-file' aria-hidden='true'></span> Saving ...");			
				return true;				
		   }
		  
		});	
	

		$('#rootwizard').bootstrapWizard({
	  		'nextSelector': '.button-next', 
			'previousSelector': '.button-previous',
			'tabClass': 'nav nav-tabs',
	  		'onNext': function(tab, navigation, index) {
	  			var $valid = $("#survey-node-form").valid();
	  			if(!$valid) {
	  				$validator.focusInvalid();
	  				return false;
	  			}
	  			if(index==1){
	  				getTitles();
	  			}
	  			/*console.log(index);
	  			if(index==2){//Esta en la pestaña de Datos de la victima
		  			if(!validaFieldQuestion()){
		  				$("#modal-message .modal-body").html("<div class='alert alert-warning' role='alert' id='alert-message'>The field Question - Script is required</div>");
						$('#modal-message').modal();
						return false;
		  			}
	  			}*/
	  		},
			'onTabShow': function(tab, navigation, index) {
				var $total = navigation.find('li').length;
				var $current = index+1;
				var $percent = ($current/$total) * 100;
				$('#rootwizard .progress-bar').css({width:$percent+'%'});
			},
			'onTabClick': function(tab, navigation, index) {
				IdEstado=jQuery("#edit-field-survey-status-und").val();	
			    if($.isNumeric(IdEstado)){//Si existe el estado, es porque se esta editando
					return true;					
				}else{
					$("#modal-message .modal-body").html("<div class='alert alert-warning' role='alert' id='alert-message'>You must use the next and previous buttons</div>");
					$('#modal-message').modal({
						backdrop: 'static', keyboard: false
					});
					return false;
				}	
			}
	  	});

		$('#edit-field-survey-start-date-und-0-value').datetimepicker({               												
			//locale: 'es',				
			format: 'DD/MM/YYYY', //formato de fecha y hora
			minDate: moment(),
			//maxDate: moment().add(1, 'day'),
			//daysOfWeekDisabled: DSA.dias_semana_no_habiles,//[0, 6]Bloquea el dia domingo, y el sabado como dias no habiles,
			//disabledDates:DSA.dias_no_habiles,											
			//enabledHours: DSA.horas_habiles,//[0, 1, 2, 3, 4, 5, 6, 7, 8, 18, 19, 20, 21, 22, 23, 24]
			sideBySide: false,//Muestra el reloj al lado del calendario	
			calendarWeeks:true,//muestra el numero de semana
			//stepping:DSA.tiempo_evento,//intervalo de minutos
			//stepping:15,
			ignoreReadonly:true, // ingnora el comportamiento de readonly en el control, deja funcionar el calendario				
			widgetPositioning:{
				horizontal: 'auto',
				vertical: 'bottom'	
			},										
		});

		$('#edit-field-survey-end-date-und-0-value').datetimepicker({               												
			//locale: 'es',				
			format: 'DD/MM/YYYY', //formato de fecha y hora
			minDate: moment().add(1, 'day'),
			//maxDate: moment()..add(8, 'day'),
			//daysOfWeekDisabled: DSA.dias_semana_no_habiles,//[0, 6]Bloquea el dia domingo, y el sabado como dias no habiles,
			//disabledDates:DSA.dias_no_habiles,											
			//enabledHours: DSA.horas_habiles,//[0, 1, 2, 3, 4, 5, 6, 7, 8, 18, 19, 20, 21, 22, 23, 24]
			sideBySide: false,//Muestra el reloj al lado del calendario	
			calendarWeeks:true,//muestra el numero de semana
			//stepping:DSA.tiempo_evento,//intervalo de minutos
			//stepping:15,
			ignoreReadonly:true, // ingnora el comportamiento de readonly en el control, deja funcionar el calendario				
			widgetPositioning:{
				horizontal: 'auto',
				vertical: 'bottom'	
			},										
		});
		
		$('#edit-field-survey-start-date-und-0-value').prop("readonly",true);	
		$('#edit-field-survey-end-date-und-0-value').prop("readonly",true);	

		/*Eventos*/			

		$(document).on({
			mouseenter: function(){
				 $(this).find(".instructions").show();
				 $(this).find(".button-remove-question").show();
				 
			},
			mouseleave: function(){
					$(this).find(".instructions").hide();
					$(this).find(".button-remove-question").hide();
			}
		}, '#edit-field-survey-questions table.field-multiple-table tbody tr');

		$(document).on({
			mouseenter: function(){
				 $(this).find(".instructions").show();
				 $(this).find(".button-remove-question-control").show();
				 
			},
			mouseleave: function(){
					$(this).find(".instructions").hide();
					$(this).find(".button-remove-question-control").hide();
			}
		}, '#edit-field-survey-demographics table.field-multiple-table tbody tr');
		

		$(document).on('click','.field-add-more-submit',function(e){

			var $valid = $("#survey-node-form").valid();
	  			if(!$valid) {
	  				$validator.focusInvalid();
	  				return false;
	  			}

		});
		
		$(document).on('click','#question_bank button',function(e) {
			event.preventDefault();
			var id=$(this).prop("id");
			$("#id-question-bank").val(id);

			$(".modal-body #list-question-bank").html("");	
			loadModalQuestionBank();
			getListQuestionBank();
		});

		$(document).on('click','#demographics_bank button',function(e) {
			event.preventDefault();
			var id=$(this).prop("id");
			$("#id-demographics-bank").val(id);

			$(".modal-body #list-demographics-bank").html("");	
			loadModalDemographicsBank();
			getListDemographicsBank();
		});


		

		$(document).on('click','#selection-question-bank a',function(e) {
			event.preventDefault();
			var nid=$(this).prop("id");
			var id=$("#id-question-bank").val();
			getDataQuestionBank(nid,id);
		});

		$(document).on('click','#selection-demographics-bank a',function(e) {
			event.preventDefault();
			var nid=$(this).prop("id");
			var id=$("#id-demographics-bank").val();
			getDataDemographicsBank(nid,id);
		});

		

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
			//var id=$(this).parents(".panel-body").find(".group-question-right").attr("id");
			var id=$(this).parent().parent().parent().parent().parent().parent().parent().parent().parent().find(".group-question-right").attr("id");
			var tid=$(this).val();
			getDataResponseBank(tid,id);
		});

		$(document).on('click','#edit-submit-question-bank',function(e) {
			$("#edit-submit-question-bank").submit();			
		});

		$(document).on('change','#edit-field-survey-consultant-und',function(e) {
			var id=$(this).val();
			if(id=="_none"){id=0;}//Se envia cero, para que no se tenga en cuenta en el filtro y muestre todos
			getDataConsumer(id);
		});

		$(document).on('click','.button-cancel',function(e) {			
			history.back();
		});	

		$("#edit-field-survey-sample-size-und-0-value,#edit-field-survey-percente-cell-und-0-value").keydown(function(event) {
			allowNumber(event);			
        });	


		//Probar esto, para el banco de preguntas, reconstruyendo el datatable
		//$(".modal-body #list-question-bank table").dataTable();

		/*Iniciales*/
		resizeTextarea();
		preload();

				
});