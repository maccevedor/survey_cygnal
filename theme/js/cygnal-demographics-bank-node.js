
jQuery(function ($){
	

 
	  	$('#rootwizard').bootstrapWizard({			
			'tabClass': 'nav nav-tabs',
	  		'onNext': function(tab, navigation, index) {
	  			return true;
	  		},
	  	});
		
		$(".comment-add a").addClass("btn btn-success");

});