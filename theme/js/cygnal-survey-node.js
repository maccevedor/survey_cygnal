


jQuery(function ($){
		
		var roleId = Drupal.settings.RoleId; //assign the variable
		console.log(roleId);
		var num_question=0;
		var num_demographics=0;

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

		/*Funciones */
		var validateRol=function(rid){
			 switch (rid){
				  case 4://Consultant
					   hideField();
				  break;
				  case 6://Consumer
					   hideField();
				  break;
				 
				  default:
						showField();	  
						break;
			 }
		};

		var hideField=function(){
			//Question
			$(".field-name-field-cll-survey-question-script .field-label").hide();
			$(".field-name-field-cll-survey-question-xtab").hide();
			$(".field-name-field-cll-survey-question-type").hide();
			//Answer
			$(".field-name-field-cll-survey-quest-opt-txt .field-label").hide();
			$(".field-name-field-cll-survey-quest-opt-xtab").hide();
			$(".field-name-field-cll-survey-quest-opt-inst").hide();	
			
			
		};

		var showField=function(){
			//Question
			$(".field-name-field-cll-survey-question-script .field-label").show();
			$(".field-name-field-cll-survey-question-xtab").show();	
			$(".field-name-field-cll-survey-question-type").show();
			//Answer
			$(".field-name-field-cll-survey-quest-opt-txt .field-label").show();
			$(".field-name-field-cll-survey-quest-opt-xtab").show();
			$(".field-name-field-cll-survey-quest-opt-inst").show();
		};

		var proccesNumberQuestion=function(){
			initQuestion();
			$(".group-question-left-node .question-num").each(function(event){
	   			num=getNumberQuestion();	  
	   			//Asigna el numero de la pregunta
	   			$(this).html("Q"+num+"-");
	   			
	   			
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
	   			$(this).parents(".panel-body").find(".group-tooltip-demo #demographics_bank button").attr("id","D"+(parseInt(num)));	
	   			
	   			//Incrementa el contador de preguntas
	   			//Asigna id al contendor de la informacion de la demographics, para poder asignarlo desde la modal
	   			$(this).parents(".panel-body").find(".group-demographics-right").attr("id","D"+(parseInt(num)));
	   			
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

		var getNumberQuestion=function (){
			return num_question;	
		};

		var getNumberDemographics=function (){
			return num_demographics;	
		};
 
	  	$('#rootwizard').bootstrapWizard({			
			'tabClass': 'nav nav-tabs',
	  		'onNext': function(tab, navigation, index) {
	  			return true;
	  		},
	  	});
		
		$(".comment-add a").addClass("btn btn-success");

		$(document).on('change','select[name="field_survey_status[und]"]',function(e) {			
			$("#modal-message .modal-body").html("<div class='alert alert-success' role='alert' id='alert-message'>The status has been successfully changed</div>");
			$('#modal-message').modal();
		});

		$('.field-name-field-survey-start-date input').datetimepicker({               												
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

		$('.field-name-field-survey-end-date input').datetimepicker({               												
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
		
		$('.field-name-field-survey-start-date input').prop("readonly",true);	
		$('.field-name-field-survey-end-date input').prop("readonly",true);	

		$(".field-name-field-survey-sample-size input,.field-name-field-survey-percente-cell input").keydown(function(event) {
			allowNumber(event);			
        });	

		$(document).on('change','input[name="field_cll_survey_question_appro[und]"]',function(e) {
			var value=$(this).val();
			if(value==1){ //Esta aceptando la pregunta
				$(this).parents(".group-approved").find(".group-comment").hide();
				//if(!confirm("Are you sure?")){
					//$(this).prop("checked",false);
				//}else{
					//$(this).parents(".group-approved").find(".group-comment").hide();
				//}
			}else{
				$(this).parents(".group-approved").find(".group-comment").show();
			}
		});

		$(document).on('change','.approved-all-questions',function(e) {
			if($(this).is(":checked")){
				$('input[name="field_cll_survey_question_appro[und]"]').each(function(event){
					if($(this).val()==1){
						$(this).prop("checked",true);
						$(this).change();
					}
				});
			}else{
				$('input[name="field_cll_survey_question_appro[und]"]').each(function(event){					
						$(this).prop("checked",false);
						$(this).change();				
				});
			}	
		});

		/*Iniciales*/
		proccesNumberQuestion();
		validateRol(roleId);		

});