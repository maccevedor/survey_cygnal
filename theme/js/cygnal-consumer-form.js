
jQuery(function ($){
	
	var roleId = Drupal.settings.RoleId; //assign the variable
	
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
	
	
  		var $validator = $("#consumer-node-form").validate({
		  rules: {
			//Datos 1
			//"title":"required ",
			"field_consumer_name[und][0][value]":"required",
			"field_consumer_email[und][0][value]":{required: true,email:true},			
						
		  },
		  messages:{
			//Datos radicador
			//"title":"Dato obligatorio",
			"field_consumer_name[und][0][value]":"This field is required",
			"field_consumer_email[und][0][value]":"This field is required, please enter a valid email address",					
		  },
		   submitHandler: function(form) {				
				/*$("#edit-submit").append("<button type='button' id='edit-send-message' name='op' value='Guardando...' class='btn btn-success form-submit'><span class='icon glyphicon glyphicon-ok' aria-hidden='true'></span> Guardando ...</button>");*/
				$("#edit-submit").prop("disabled",true);	
				$("#edit-submit").html("<span class='icon glyphicon glyphicon-save-file' aria-hidden='true'></span> Saving ...");			
				return true;				
		   }
		  
		});	
});