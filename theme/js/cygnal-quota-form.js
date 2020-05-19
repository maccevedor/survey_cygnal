
jQuery(function ($){
	
	var roleId = Drupal.settings.RoleId; //assign the variable
	
	var ShowTarget=function(id){
		switch(id){
				case "15"://LandLine
						  $(".field-name-field-cll-quotas-values-landline").show();
						  $(".field-name-field-cll-quotas-values-landline").parent().parent().parent().parent().find("thead tr th:nth-child(2)").html("Landline");
				break;
				case "16"://Cell
						  $(".field-name-field-cll-quotas-values-cell").show();
						  $(".field-name-field-cll-quotas-values-cell").parent().parent().parent().parent().find("thead tr th:nth-child(3)").html("Cell");
				break;
				case "17"://Email
						  $(".field-name-field-cll-quotas-values-email").show();
						  $(".field-name-field-cll-quotas-values-email").parent().parent().parent().parent().find("thead tr th:nth-child(4)").html("Email");
				break;
				case "19"://Whatsapp
						  $(".field-name-field-cll-quotas-values-whatsapp").show();
						  $(".field-name-field-cll-quotas-values-whatsapp").parent().parent().parent().parent().find("thead tr th:nth-child(5)").html("Whatsapp");
				break;
				case "20"://Facebook
						  $(".field-name-field-cll-quotas-values-facebook").show();
						  $(".field-name-field-cll-quotas-values-facebook").parent().parent().parent().parent().find("thead tr th:nth-child(6)").html("Facebook");
				break;
				case "21"://Field
						  $(".field-name-field-cllquotas-values-field").show();
						  $(".field-name-field-cllquotas-values-field").parent().parent().parent().parent().find("thead tr th:nth-child(7)").html("Field");
				break;		
			}
	};

	var HideTarget=function(id){
		switch(id){
				case "15"://LandLine
						  $(".field-name-field-cll-quotas-values-landline").hide();
						  $(".field-name-field-cll-quotas-values-landline").parent().parent().parent().parent().find("thead tr th:nth-child(2)").html("");
				break;
				case "16"://Cell
						  $(".field-name-field-cll-quotas-values-cell").hide();
						  $(".field-name-field-cll-quotas-values-cell").parent().parent().parent().parent().find("thead tr th:nth-child(3)").html("");
				break;
				case "17"://Email
						  $(".field-name-field-cll-quotas-values-email").hide();
						  $(".field-name-field-cll-quotas-values-email").parent().parent().parent().parent().find("thead tr th:nth-child(4)").html("");
				break;
				case "19"://Whatsapp
						  $(".field-name-field-cll-quotas-values-whatsapp").hide();
						  $(".field-name-field-cll-quotas-values-whatsapp").parent().parent().parent().parent().find("thead tr th:nth-child(5)").html("");
				break;
				case "20"://Facebook
						  $(".field-name-field-cll-quotas-values-facebook").hide();
						  $(".field-name-field-cll-quotas-values-facebook").parent().parent().parent().parent().find("thead tr th:nth-child(6)").html("");
				break;
				case "21"://Field
						  $(".field-name-field-cllquotas-values-field").hide();
						  $(".field-name-field-cllquotas-values-field").parent().parent().parent().parent().find("thead tr th:nth-child(7)").html("");
				break;
			}
	};

	var loadQuota=function(){
		$('input[type="checkbox"][id^=edit-field-quota-target-und]').each(function() {
				id=$(this).val();
				if(!$(this).is(":checked")){
					HideTarget(id);
				}else{
					ShowTarget(id);
				}
		});
	}

	/*
	  ******************************************************************************************
	  								E V E N T S      C L I C K
	  ******************************************************************************************
	*/
	$(document).on('change', 'input[type="checkbox"][id^=edit-field-quota-target-und]', function(e) {				 							 
		var id=$(this).val();
		var checked=true;	
		if(!$(this).is(":checked")){
			checked=false;
		}
		var target=	new Array();
		target[15]="field-name-field-cll-quotas-values-landline";
		target[16]="field-name-field-cll-quotas-values-cell";

		if(checked===true){
			ShowTarget(id);		
		}else{
			
			HideTarget(id);	
		}
			
	 });

	 loadQuota();	
});