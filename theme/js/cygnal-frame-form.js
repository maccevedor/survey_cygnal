
jQuery(function ($){
	
	var roleId = Drupal.settings.RoleId; //assign the variable
	
	var loadModalFrame=function(){			
			$('#modal-frame').modal({
				backdrop: 'static', keyboard: true
			});
		};

	var getApplications=function(){
			
		$.ajax({
		  url: 'https://api.votermapping.com/api/v1/customer/applications?id=cygnalapi&apikey=ac3e1c856db9cbc5b52a6eb9700d0518f195a1ef',
		  type: 'get',
		  data: {},
		  dataType: 'json',
		  success: function (response) {
			if (response!== undefined) {				  
			 setApplications(response);
			}
		  }
		});
	};

	var getDatasources=function(app){
			
		$.ajax({
		  url: 'https://api.votermapping.com/api/v1/customer/application/datasources/'+app+'?id=cygnalapi&apikey=ac3e1c856db9cbc5b52a6eb9700d0518f195a1ef',
		  type: 'get',
		  data: {},
		  dataType: 'json',
		  success: function (response) {
			if (response!== undefined) {
			 response.app=app;				  
			 setDatasources(response);
			}
		  }
		});
	};

	var getColumns=function(app,ds){
			
		$.ajax({
		  url: 'https://api.votermapping.com/api/v1/customer/application/datasource/columns/'+app+'/'+ds+'?id=cygnalapi&apikey=ac3e1c856db9cbc5b52a6eb9700d0518f195a1ef',
		  type: 'get',
		  data: {},
		  dataType: 'json',
		  success: function (response) {
			if (response!== undefined) {
			 response.app=app;
			 response.ds=ds;				  
			 setColumns(response);
			}
		  }
		});
	};

	var getValues=function(app,ds,column){
			
		$.ajax({
		  url: 'https://api.votermapping.com/api/v1/customer/application/datasource/column/values/'+app+'/'+ds+'/'+column+'?id=cygnalapi&apikey=ac3e1c856db9cbc5b52a6eb9700d0518f195a1ef',
		  type: 'get',
		  data: {},
		  dataType: 'json',
		  success: function (response) {
			if (response!== undefined) {
			 response.app=app;
			 response.ds=ds;
			 response.column=column;				  
			 setvalues(response);
			}
		  }
		});
	};

		
	var setApplications=function(response){
		loadDataApplication(response.applications,'#application-frame');		
	}

	var setDatasources=function(response){
		loadDataDatasources(response);
	}

	var setColumns=function(response){
		loadDataColumns(response);
	}

	var setvalues=function(response){
		loadDataValues(response);
	}	

	var loadDefault=function(obj){
		var html="<option value='_none'>None</option>";
		$(obj).html(html);	
	};

	var loadDataApplication=function(data,obj){
			//Se hace el cargue de las opciones del primer nivel
			var html="<option value='_none'>Select</option>";
			data.forEach( function(value, index){			
				html +="<option value='"+value.appname+"'>"+value.name+"</option>";				
			});			
			$(obj).html(html);
	};

	var loadDataDatasources=function(data){
		var options="";
		data.datasources.forEach( function(value, index){			
				options +="<li id='"+value.name+"'>"+value.ext_name+"</li>";				
			});
		var html='<div class="row"><div class="col-md-12">'
				+'<ul id="list-applications">'
				+'<li id="'+data.app+'">'
				+'<ul id="list-datasources">'
				+options
				+'</ul>'
				+'</li>'
				+'</ul>'
				+'</div>'
				+'</div>';
		$("#tree-frame").html(html); 
	}

	var loadDataColumns=function(data){
		var options="";
		data.columns.forEach( function(value, index){			
				options +="<li id='"+value.id+"'>"+value.name+"</li>";				
			});
		var html='<ul id="list-columns">'
				+options
				+'</ul>';
		$("#"+data.ds).html(html); 
	}

	var loadDataValues=function(data){
		var options="";
		data.values.forEach( function(value, index){			
				options +="<li id='"+value+"'>"+value+"</li>";				
			});
		var html='<ul id="list-values">'
				+options
				+'</ul>';
		$("#"+data.column).html(html); 
	}

	$(document).on('click','#get-variable-frame button',function(e) {
		loadModalFrame();
		getApplications();
	});

	$(document).on('change','#application-frame',function(e) {
		var app=$(this).val();		
		getDatasources(app);
	});

	$(document).on('click','ul#list-datasources li',function(e) {
		var ds=$(this).prop("id");	
		var app=$(this).parent().parent().attr("id");	
		getColumns(app,ds);
	});

	$(document).on('click','ul#list-columns li',function(e) {
		var column=$(this).prop("id");
		var ds=$(this).parent().parent().prop("id");	
		var app=$(this).parent().parent().parent().parent().prop("id");	
		getValues(app,ds,column);
	});
	
});