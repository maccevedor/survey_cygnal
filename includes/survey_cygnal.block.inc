<?php

function _alfonso_get_ultimos_productos()
{ 
	$path = drupal_get_path('module', 'survey_cygnal');
	//drupal_add_css($path.'/theme/css/agendamiento.css');
	$array = array("nombre"=>"hola");  
	$output = theme('ultimos_productos', $array); 
	return $output;
}