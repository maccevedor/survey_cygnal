<?php

/**
 * @file
 * Archivo para funciones de consultas a la base de datos.
 */
 
 /** 
  * Obtiene el listado de terminos en jeraquia
 */
function _survey_cygnal_db_get_datos(){           
	$result = db_query("SELECT nid, created, changed FROM node");
	$i=0;
	$data=array();
	foreach ($result as $record) {                
		$data[$i]['id']=$record->nid;
		$data[$i]['crea']=$record->created;
		$data[$i]['actualiza']=$record->changed;
		$i++;
	}
	return $data;
}

function _survey_cygnal_db_get_quota_id($data){

	$result=db_query("	SELECT n.nid as nid
						FROM node n
						INNER JOIN field_data_field_quota_survey s ON n.nid=s.entity_id and s.bundle='quota' 
															 AND s.field_quota_survey_nid=".$data["survey_id"]."
						WHERE n.type='quota'
						AND n.status=1
						ORDER BY n.changed desc");
	 $i=0;
	 $data=array();
	  foreach ($result as $record) {		 													
		$data[$i]['nid']=$record->nid;	
		$i++;
	 }
	 if(isset($data[0])){
	 	return $data[0]['nid'];
	 }else{
	 	return 0;
	 }	
}

/** 
  * Obtiene el listado de terminos en jeraquia
 */
function _survey_cygnal_db_get_customer($datos){           
 
	 $join="";
	 if($datos["id_consultant"]!=0){
	 	$join=" INNER JOIN field_data_field_consumer_consultant ct ON n.nid=ct.entity_id 
				  AND ct.bundle='consumer' 
				  AND ct.field_consumer_consultant_nid=".$datos["id_consultant"]." ";
	 }
	 if($datos["id_company"]!=0){
	 	$join.=" INNER JOIN field_data_field_consumer_company cp ON n.nid=cp.entity_id 
	 	          AND cp.bundle='consumer' 
	 	          AND cp.field_consumer_company_nid=".$datos["id_company"]." ";
	 }	

	 $query=" SELECT  n.nid id
					 ,nm.field_consumer_name_value text
			  FROM node n
			  INNER JOIN field_data_field_consumer_name nm ON n.nid=nm.entity_id and nm.bundle='consumer'
			  ".$join."					
			  WHERE n.type='consumer'
						and n.status=1";
	 //echo $query;					
	 $result=db_query($query);
	 $i=0;
	 $data=array();
	  foreach ($result as $record) {		 													
		$data[$i]['id']=$record->id;
		$data[$i]['text']=$record->text;	
		$i++;
	 }
	 return $data;
}

/** 
  * Obtiene el listado de terminos en jeraquia
 */
function _survey_cygnal_db_get_consultant($datos){           
 
	 $join="";	 
	 if($datos["id_company"]!=0){
	 	$join.=" INNER JOIN field_data_field_consultant_company cp ON n.nid=cp.entity_id 
	 	          AND cp.bundle='consultant' 
	 	          AND cp.field_consultant_company_nid=".$datos["id_company"]." ";
	 }	

	 $query=" SELECT  n.nid id
					 ,nm.field_consultant_name_value text
			  FROM node n
			  INNER JOIN field_data_field_consultant_name nm ON n.nid=nm.entity_id and nm.bundle='consultant'
			  ".$join."					
			  WHERE n.type='consultant'
						and n.status=1";
	 //echo $query;					
	 $result=db_query($query);
	 $i=0;
	 $data=array();
	  foreach ($result as $record) {		 													
		$data[$i]['id']=$record->id;
		$data[$i]['text']=$record->text;	
		$i++;
	 }
	 return $data;
}

/** 
  * Obtiene el listado de terminos en jeraquia
 */
function _survey_cygnal_db_validate_survey_title($data){           
	$query="SELECT count(n.nid) num 
		                FROM node n
		                WHERE n.type='survey'
		                AND n.status=1
		                AND n.nid !=".$data["nid"]."
		                AND TRIM(LOWER(n.title)) like '".strtolower($data["title"])."'";

	//echo $query;
	$result = db_query($query);
	$i=0;
	$data=array();
	foreach ($result as $record) {                
		$data[$i]['num']=$record->num;		
		$i++;
	}
	return $data;
}