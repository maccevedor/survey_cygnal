<?php
	/* -----------------------------------------------------------------------------------------
	| Generación de archivo de Quotas.
	|-------------------------------------------------------------------------------------------
	| @copyright: Achb
	| @author: ALFONSO CHAVEZ B <alfonso.chb@gmail.com>
	| @Description: Este archivo contiene todas las funciones necesarias y suficientes para 
	|               la generación del documento de Quotas de una encuesta.
	|-------------------------------------------------------------------------------------------
	*/
	

	/**
	 * Función: principal que es llamada desde el enlace directo desde el listado de encuestas
	 *          o desde el registro directo de la Quota
	 */
	function _survey_cygnal_quota_file(){
		$nid=0;
		$qid=0;
		$path = drupal_get_path('module', 'survey_cygnal');
	 	require_once "$path/includes/survey_cygnal.db.inc";

	 	if(arg(4)=="foreign"){ #Si el llamado se hace desde el listado de encuestas o desde la encuesta directamente
	 		if(is_numeric(arg(5))){#Debe llegar el id de la encuesta, y debe ser númerico
	 			$data=array("survey_id"=>arg(5));
	 			$qid=_survey_cygnal_db_get_quota_id($data);#Se obtiene el ultimo qid de las Quotas de la encuesta enviada
	 		}
	 	}else{#Si el enlace se construye directamente en el contenido de la Quota, o desde el listado de Quotas
	 		if(is_numeric(arg(4))){#Se asigna el id como el $qid directo
	 			$qid=arg(4); //Asignación directa del id de quota
	 			$nid=arg(5); //Asignación directa del id de survey
	 		}
	 	}
	    
	 	if(is_numeric($qid) && $qid!=0){#Si se encontre el $qid del nodo de la cuota, se pasa a la construcción del archivo
	 		$node_quota=node_load($qid); #load the node with information of the quota
	 		$node_survey=node_load($nid); #load the node with information of the quota
	 		
	 		$text_project_id="";
			if(isset($node_survey->field_survey_project_id[LANGUAGE_NONE])){
				$text_project_id=strtoupper($node_survey->field_survey_project_id[LANGUAGE_NONE][0]["value"]);
			}
			$info=array("text_project_id"=>$text_project_id,
				        "view"=>"all", #all,landline,cell
				        "celda"=>"formula", #numerica, formula
				        "export"=>"yes"); #yes , no
	 		$quotas=_survey_cygnal_quota_get_info_node_quota($node_quota,$info);#Llena la estructura de generación del archivo de quotas con la información del nodo
	 		_survey_cygnal_quota_build_file($quotas);#Inicia la construcción del archivo de Quotas, este archivo sera un excel
	 	}else{
	 		echo "Data Not Found ";#Si retorna esto, es porque el id enviado no es correcto, o la Quota no existe
	 		return false;
	 	}

	 	return true;	
	}

	/**
	 * Función: Llena una estructura con la información de la Quota obtenida desde el nodo de la Quota enviado
	 */
	function _survey_cygnal_quota_get_info_node_quota($node,$info){
		
		$path_module = drupal_get_path('module', 'survey_cygnal');
		$datos=array();
		$quotas=array();		
		$valores=array();

		#_debug($node,"node",false,true); #Variable, exit, print
		#Se inicializa el target "overall", ya que debe ser el principal y este no viene en la estructura de los target
		$datos['node']['groups']["overall"]=array();
			
		//Crea los target
		if(isset($node->field_quota_target[LANGUAGE_NONE])){
			foreach ($node->field_quota_target[LANGUAGE_NONE] as $target) {
				$id_target=$target["tid"];
				$term_target=taxonomy_term_load($id_target);
				
				$datos['node']['groups'][strtolower($term_target->field_target_alias[LANGUAGE_NONE][0]["value"])]=array();
			}
			
		}

		if(isset($node->field_quota_variables[LANGUAGE_NONE])){// se verifica si hay variables de la quota
			foreach($node->field_quota_variables[LANGUAGE_NONE] as $item){ // recorre cada una de las variables de la quota
				$id_variable=$item["value"];
				$item_variable=entity_load('field_collection_item', array($id_variable));	// se obtiene la colleccion completa del 
				//codigos
				if(isset($item_variable[$id_variable]->field_cll_quotas_code[LANGUAGE_NONE])){
					$code=$item_variable[$id_variable]->field_cll_quotas_code[LANGUAGE_NONE][0]["value"];						
				}

				//variables
				if(isset($item_variable[$id_variable]->field_cll_quotas_values[LANGUAGE_NONE])){
					foreach($item_variable[$id_variable]->field_cll_quotas_values[LANGUAGE_NONE] as $vars){ // recorre cada uno de los valores de cada variable
						$id_vars=$vars["value"];
						$item_vars=entity_load('field_collection_item', array($id_vars));	// se obtiene la colleccion completa del 	
						
						$valores[]=array("name"=>$code, 	
						                 "code"=>$item_vars[$id_vars]->field_cll_quotas_values_code[LANGUAGE_NONE][0]["value"], 	
						                 "value"=>$item_vars[$id_vars]->field_cll_quotas_values_value[LANGUAGE_NONE][0]["value"], 		
						                 "Population"=>0, 	
						                 "percent"=>0);//4.66
						
						if(is_numeric($item_vars[$id_vars]->field_cll_quotas_values_quotas[LANGUAGE_NONE][0]["value"])){
							$id_quota=$item_vars[$id_vars]->field_cll_quotas_values_quotas[LANGUAGE_NONE][0]["value"];
							$item_quota=entity_load('field_collection_item', array($id_quota));

							
							foreach ($datos["node"]["groups"] as $key => $value) {
								$code_var=$item_vars[$id_vars]->field_cll_quotas_values_code[LANGUAGE_NONE][0]["value"];
								$value_quota=0;
								if(isset($item_quota[$id_quota]->{'field_cll_quotas_values_'.$key}[LANGUAGE_NONE])){
									$value_quota=$item_quota[$id_quota]->{'field_cll_quotas_values_'.$key}[LANGUAGE_NONE][0]["value"];
								}
								$quotas[$code_var][$key]=$value_quota;							
							}						
						}
					}
				}
			
			}
		}

	
		
		$id_quota_general=$node->field_quota_quota_general[LANGUAGE_NONE][0]["value"];
		$item_quota_general=entity_load('field_collection_item', array($id_quota_general));

		$id_percent_general=$node->field_quota_percent_target[LANGUAGE_NONE][0]["value"];
		$item_percent_general=entity_load('field_collection_item', array($id_percent_general));


		$id_population=$item_quota_general[$id_quota_general]->field_cll_quotas_values_quotas[LANGUAGE_NONE][0]["value"];
		$item_population=entity_load('field_collection_item', array($id_population));


		$id_percent=$item_percent_general[$id_percent_general]->field_cll_quotas_values_quotas[LANGUAGE_NONE][0]["value"];
		$item_percent=entity_load('field_collection_item', array($id_percent));

		
		$datos['node']['view'] =$info['view']; //all,landline,cell 
		$datos['node']['celda'] =$info['celda']; //formulada, numerica 
		$datos['node']['export'] =$info['export']; //formulada, numerica 
	    $datos['node']['logo_cygnal'] 		= $path_module."/theme/img/logo-excel.png";
		$datos['node']['logo_custamer'] 	= $path_module."/theme/img/logos/logo_empresa.png";
		$datos['node']['text_header'] 		= "Cygnal Pulse Survey Platform";
		$datos['node']['text_title'] 		= "Survey of Likely General Election Voters";
		$datos['node']['text_project_id'] 	= $info["text_project_id"];
		$datos['node']['sample_size'] 		= array('label' => "Sample size", 	
		                                  			'value' => $node->field_quota_sample_size[LANGUAGE_NONE][0]["value"]);


		foreach ($datos['node']['groups'] as $key1 => $value1) {
			foreach ($valores as $key2 => $value2) {
				$code=$valores[$key2]["code"];
				$valores[$key2]["Population"]=$quotas[$code][$key1];
			}
			$population=0;		
			if(isset($item_population[$id_population]->{'field_cll_quotas_values_'.$key1}[LANGUAGE_NONE])){
				$population=$item_population[$id_population]->{'field_cll_quotas_values_'.$key1}[LANGUAGE_NONE][0]["value"];			
			}

			$percent=0;
			if(isset($item_percent[$id_percent]->{'field_cll_quotas_values_'.$key1}[LANGUAGE_NONE])){
				$percent=$item_percent[$id_percent]->{'field_cll_quotas_values_'.$key1}[LANGUAGE_NONE][0]["value"]/100;
			}


			$datos['node']['groups'][$key1]=$valores;
			$datos['node']["target"][]=array(	'code' => $key1,
												'title_population' => ucfirst($key1)." n",
												'population' => $population,
												'title_percent' => ucfirst($key1[0]).'%',
												'percent' => $percent,
												'title_data' => ucfirst($key1),
												'cols_data' => array(ucfirst($key1[0]).'Pop', ucfirst($key1[0]).'PopC', ucfirst($key1[0]).'QC', ucfirst($key1[0]).'Quota'),		
												'cols_params' => array(ucfirst($key1), 'CodeKey', 'Meaning', 'Pop', '%'),
												);
		}

		return $datos;
	}

	function _survey_cygnal_quota_build_file($quotas){

		
		// Los logos a escala ratio is: (16 : 9)
		$path = DRUPAL_ROOT;
		#$path_module = drupal_get_path('module', 'survey_cygnal');
		#require_once $path_module."/includes/survey_cygnal.excel.inc";
		
		$data = $quotas;
		/*$data=array();
		$data['node']['logo_cygnal'] 	= $path_module."/theme/img/logo-excel.png";
		$data['node']['logo_custamer'] 	= $path_module."/theme/img/logos/logo_empresa.png";
		$data['node']['text_header'] 	= "Cygnal Pulse Survey Platform";
		$data['node']['text_title'] 	= "Survey of Likely General Election Voters";

		# ---------------------------------------------------------------
		# Valores para la tabla "General".
		$data['node']['sample_size'] = array('label' => "Sample size", 	'value' => "600");

		# ---------------------------------------------------------------
		# Valores para la tabla del grupo "Overall".
		$data['node']["target"][] = array(
			'code' => 'overall',
			'title_population' => 'Overall n',
			'population' => '5769635',
			'title_percent' => '',
			'percent' => '',
			'title_data' => 'Overall',
			'cols_data' => array('OPop', 'OPopC', 'OQC', 'OQuota'),		
			'cols_params' => array('Overall', 'CodeKey', 'Meaning', 'Pop', '%')
		);
		$data['node']['groups']["overall"][] = array("name"=>"AgeGen", 	"code"=>"AG1", 	"value"=>"F; 18-34", 		"Population"=>"268930", 	"percent"=>"4,66%");//4.66
		$data['node']['groups']["overall"][] = array("name"=>"AgeGen", 	"code"=>"AG2", 	"value"=>"F; 35-49", 		"Population"=>"575934", 	"percent"=>"9,97%");
		$data['node']['groups']["overall"][] = array("name"=>"AgeGen", 	"code"=>"AG3", 	"value"=>"F; 50-64", 		"Population"=>"994443", 	"percent"=>"17,22%");
		$data['node']['groups']["overall"][] = array("name"=>"AgeGen", 	"code"=>"AG4", 	"value"=>"F; 65+", 			"Population"=>"1333349", 	"percent"=>"23,08%");
		$data['node']['groups']["overall"][] = array("name"=>"AgeGen", 	"code"=>"AG5", 	"value"=>"M; 18-34", 		"Population"=>"208589", 	"percent"=>"3,61%");
		$data['node']['groups']["overall"][] = array("name"=>"AgeGen", 	"code"=>"AG6", 	"value"=>"M; 35-49", 		"Population"=>"451459", 	"percent"=>"7,82%");
		$data['node']['groups']["overall"][] = array("name"=>"AgeGen", 	"code"=>"AG7", 	"value"=>"M; 50-64", 		"Population"=>"835372", 	"percent"=>"14,46%");
		$data['node']['groups']["overall"][] = array("name"=>"AgeGen", 	"code"=>"AG8", 	"value"=>"M; 65+", 			"Population"=>"1108021", 	"percent"=>"19,18%");
		$data['node']['groups']["overall"][] = array("name"=>"Party", 	"code"=>"P1", 	"value"=>"GOP", 			"Population"=>"2581690", 	"percent"=>"44,75%");
		$data['node']['groups']["overall"][] = array("name"=>"Party", 	"code"=>"P2", 	"value"=>"Dem+Others", 		"Population"=>"3187945", 	"percent"=>"55,25%");
		$data['node']['groups']["overall"][] = array("name"=>"Geo", 	"code"=>"G1", 	"value"=>"North Florida", 	"Population"=>"1138076", 	"percent"=>"19,73%");
		$data['node']['groups']["overall"][] = array("name"=>"Geo", 	"code"=>"G2", 	"value"=>"Orlando", 		"Population"=>"1201236", 	"percent"=>"20,82%");
		$data['node']['groups']["overall"][] = array("name"=>"Geo", 	"code"=>"G3", 	"value"=>"Miami", 			"Population"=>"1635532", 	"percent"=>"28,35%");
		$data['node']['groups']["overall"][] = array("name"=>"Geo", 	"code"=>"G4", 	"value"=>"Tampa", 			"Population"=>"1794791", 	"percent"=>"31,11%");

		# ---------------------------------------------------------------
		# Valores para la tabla del grupo "LandLine".
		$data['node']["target"][] = array(
			'code' => 'landline',
			'title_population' => 'Land n',
			'population' => '2979226',
			'title_percent' => 'L%',
			'percent' => '0.7',	
			'title_data' => 'Land Line',
			'cols_data' => array('LPop', 'LPopC', 'LQC', 'LQuota'),
			'cols_params' => array('Landline', 'CodeKey', 'Meaning', 'Pop', '%')
		);
		$data['node']['groups']["landline"][] = array("name"=>"AgeGen", 	"code"=>"AG1", 	"value"=>"F; 18-34", 		"Population"=>"92600", 		"percent"=>"4,66%");
		$data['node']['groups']["landline"][] = array("name"=>"AgeGen", 	"code"=>"AG2", 	"value"=>"F; 35-49", 		"Population"=>"242397", 	"percent"=>"9,97%");
		$data['node']['groups']["landline"][] = array("name"=>"AgeGen", 	"code"=>"AG3", 	"value"=>"F; 50-64", 		"Population"=>"525247", 	"percent"=>"17,22%");
		$data['node']['groups']["landline"][] = array("name"=>"AgeGen", 	"code"=>"AG4", 	"value"=>"F; 65+", 			"Population"=>"792057", 	"percent"=>"23,08%");
		$data['node']['groups']["landline"][] = array("name"=>"AgeGen", 	"code"=>"AG5", 	"value"=>"M; 18-34", 		"Population"=>"78097", 		"percent"=>"3,61%");
		$data['node']['groups']["landline"][] = array("name"=>"AgeGen", 	"code"=>"AG6", 	"value"=>"M; 35-49", 		"Population"=>"181450", 	"percent"=>"7,82%");
		$data['node']['groups']["landline"][] = array("name"=>"AgeGen", 	"code"=>"AG7", 	"value"=>"M; 50-64", 		"Population"=>"428581", 	"percent"=>"14,46%");
		$data['node']['groups']["landline"][] = array("name"=>"AgeGen", 	"code"=>"AG8", 	"value"=>"M; 65+", 			"Population"=>"642388", 	"percent"=>"19,18%");
		$data['node']['groups']["landline"][] = array("name"=>"Party", 		"code"=>"P1", 	"value"=>"GOP", 			"Population"=>"1337722", 	"percent"=>"44,75%");
		$data['node']['groups']["landline"][] = array("name"=>"Party", 		"code"=>"P2", 	"value"=>"Dem+Others", 		"Population"=>"1641504", 	"percent"=>"55,25%");
		$data['node']['groups']["landline"][] = array("name"=>"Geo", 		"code"=>"G1", 	"value"=>"North Florida", 	"Population"=>"581120", 	"percent"=>"19,73%");
		$data['node']['groups']["landline"][] = array("name"=>"Geo", 		"code"=>"G2", 	"value"=>"Orlando", 		"Population"=>"633328", 	"percent"=>"20,82%");
		$data['node']['groups']["landline"][] = array("name"=>"Geo", 		"code"=>"G3", 	"value"=>"Miami", 			"Population"=>"838764", 	"percent"=>"28,35%");
		$data['node']['groups']["landline"][] = array("name"=>"Geo", 		"code"=>"G4", 	"value"=>"Tampa", 			"Population"=>"925657", 	"percent"=>"31,11%");

		# ---------------------------------------------------------------
		# Valores para la tabla del grupo "Cell".
		$data['node']["target"][] = array(
			'code' => 'cell',
			'title_population' => 'Cell n',
			'population' => '1439736',
			'title_percent' => 'C%',
			'percent' => '0.3',	
			'title_data' => 'Cell',
			'cols_data' => array('CPop', 'CPopC', 'CQC', 'CQuota'),
			'cols_params' => array('Cell', 'CodeKey', 'Meaning', 'Pop', '%')
		);
		$data['node']['groups']["cell"][] = array("name"=>"AgeGen", 	"code"=>"AG1", 	"value"=>"F; 18-34", 		"Population"=>"93111", 		"percent"=>"6,46%");
		$data['node']['groups']["cell"][] = array("name"=>"AgeGen", 	"code"=>"AG2", 	"value"=>"F; 35-49", 		"Population"=>"177118", 	"percent"=>"12,30%");
		$data['node']['groups']["cell"][] = array("name"=>"AgeGen", 	"code"=>"AG3", 	"value"=>"F; 50-64", 		"Population"=>"259294", 	"percent"=>"18,00%");
		$data['node']['groups']["cell"][] = array("name"=>"AgeGen", 	"code"=>"AG4", 	"value"=>"F; 65+", 			"Population"=>"237236", 	"percent"=>"16,47%");
		$data['node']['groups']["cell"][] = array("name"=>"AgeGen", 	"code"=>"AG5", 	"value"=>"M; 18-34", 		"Population"=>"68448", 	   	"percent"=>"4,75%");
		$data['node']['groups']["cell"][] = array("name"=>"AgeGen", 	"code"=>"AG6", 	"value"=>"M; 35-49", 		"Population"=>"143732", 	"percent"=>"9,98%");
		$data['node']['groups']["cell"][] = array("name"=>"AgeGen", 	"code"=>"AG7", 	"value"=>"M; 50-64", 		"Population"=>"228573", 	"percent"=>"15,87%");
		$data['node']['groups']["cell"][] = array("name"=>"AgeGen", 	"code"=>"AG8", 	"value"=>"M; 65+", 			"Population"=>"232755", 	"percent"=>"16,16%");
		$data['node']['groups']["cell"][] = array("name"=>"Party", 		"code"=>"P1", 	"value"=>"GOP", 			"Population"=>"647631", 	"percent"=>"44,98%");
		$data['node']['groups']["cell"][] = array("name"=>"Party", 		"code"=>"P2", 	"value"=>"Dem+Others", 		"Population"=>"792105", 	"percent"=>"55,02%");
		$data['node']['groups']["cell"][] = array("name"=>"Geo", 		"code"=>"G1", 	"value"=>"North Florida", 	"Population"=>"300990", 	"percent"=>"20,91%");
		$data['node']['groups']["cell"][] = array("name"=>"Geo", 		"code"=>"G2", 	"value"=>"Orlando", 		"Population"=>"273570", 	"percent"=>"19,00%");
		$data['node']['groups']["cell"][] = array("name"=>"Geo", 		"code"=>"G3", 	"value"=>"Miami", 			"Population"=>"419941", 	"percent"=>"29,17%");
		$data['node']['groups']["cell"][] = array("name"=>"Geo", 		"code"=>"G4", 	"value"=>"Tampa", 			"Population"=>"445235", 	"percent"=>"30,92%");


		# Pasar texto exacto, para la impresión:
		#
		# all, landline, cell
		#
		$data['node']['view'] ='all';*/

		_survey_cygnal_quota_build_matrix($data,$data['node']["target"],$data['node']['view']);
		
		#_debug($data,"data-after-table",true,true); #Variable, exit, print

		if($data['node']['export']=="yes"){# indica que se esta exportando la quota, se deja continuar, en caso contrario, solo se trae los datos y no se genera archivo
			_survey_cygnal_quota_excel( $data );
			return true;
		}else{
			return $data;
		}
	}

	function _debug($variable,$title,$exit=false,$print=true){
		if($print){
			echo "<pre><br>....................INICIA ".strtoupper($title)."........................<br>";
			print_r($variable);
			echo "<br>...................FIN ".strtoupper($title)."........................<br></pre>";
			if($exit){
				exit(0);
			}
		}
	}
	# ---------------------------------------------------------------
	# Llamado a los procesos para construir el tabulado de datos para el excel.
	function _survey_cygnal_quota_build_matrix( &$data=array(), $target=array(), $element ){
		$fila = 5; // Donde inicia construcción de tabulados en el excel.
		$codes = _survey_cygnal_quota_build_array_vars($data['node']['groups']["overall"] );
		#_debug($codes,"codes",false,true); #Variable, exit, print
		#_debug($data,"data-before",false,true); #Variable, exit, print
		_survey_cygnal_quota_build_array_vars_population_percent($data);
		#_debug($data,"data-after",true,true); #Variable, exit, print
		
		$combinations = _survey_cygnal_quota_build_codes_combinations( $codes );
		#_debug($combinations,"combinations",false,true); #Variable, exit, print
		#construye $data['node']['table']
		_survey_cygnal_quota_build_table($data,$combinations, $fila, $target, $element );
		unset($target, $codes, $combinations);
		
		return true;
	}

	# ---------------------------------------------------------------
	# Construye el arreglo multidimensional.
	function _survey_cygnal_quota_build_array_vars( $data=array() ){
		$i=-1;	
		$aux='';
		$codes=array();
		//Se usa Overall unicamente, ya que es el grupo principal y siempre va a venir ese grupo
		foreach ($data as $group => $gvalue) {
			if ( $aux != $gvalue['name']) {
				$aux = $gvalue['name'];
				$i++;
			}
			$codes[$i][] = $gvalue;
		}			
		return $codes;
	}

	# ---------------------------------------------------------------
	# Construye el arreglo multidimensional.
	function _survey_cygnal_quota_build_array_vars_population_percent(&$data){
		$total_codes=array();

		#_debug($data['node']['groups'],"node",false,true); #Variable, exit, print	
		foreach ($data['node']['groups'] as $group => $group_value) {
			#_debug($group_value,"group_value",false,true); #Variable, exit, print			
			
			$codes[$group] = _survey_cygnal_quota_build_array_vars($group_value);
			#_debug($codes,"codes",false,true); #Variable, exit, print	}

				#Genera el total de cada uno de las variables en cada grupo
				foreach ($codes[$group] as $code => $code_value) {
					if(empty($total_codes[$group][$code])){
						$total_codes[$group][$code]=0;
					}
					#_debug($code,"code_value",false,true); #Variable, exit, print	}
					
					$total=0;					
					foreach ($code_value as $xcode => $xvalue) {
						#_debug($var_value["code"]."---".$xvalue["code"],"pregunta",false,true); #Variable, exit, print	}
						
							#_debug($total."---".$xvalue["Population"],"total",false,true); #Variable, exit, print	}
							$total=$total+$xvalue["Population"];							
						
					}
					#_debug($total,"total",true,true); #Variable, exit, print	}
					$total_codes[$group][$code]=$total;	
			
				}

				# Calcula el valor porcentaje de cada variable, con el total de cada grupo 
				foreach ($codes[$group] as $code => $code_value) {
					foreach ($code_value as $xcode => $xvalue) {
						#_debug($codes[$group][$code],"datos",true,true); #Variable, exit, print	}
						$codes[$group][$code][$xcode]["percent"]=(($codes[$group][$code][$xcode]["Population"]/$total_codes[$group][$code]));
						
					}	
					
				}
							
							
		}
		#_debug($total_codes,"totales",false,true); #Variable, exit, print	}

		#Se llena el campo porcentaje, en la confifuracion de los grupos iniciales
		/*foreach ($data['node']['groups'] as $group => $value) {

		}*/	
		#_debug($codes,"codes-total",true,true); #Variable, exit, print	

		/*foreach ($data['node']['groups'] as $group => $group_value) {
			foreach ($data['node']['groups'] as $group => $group_value) {
				
			}	
		}*/	

		foreach ($data['node']['groups'] as $group => $group_value) {
			#_debug($group,"group",false,true); #Variable, exit, print
			#_debug($group_value,"value",false,true); #Variable, exit, print	
			foreach ($group_value as $var => $var_value) {
				#_debug($var,"var",false,true); #Variable, exit, print
				#_debug($var_value,"var_value",false,true); #Variable, exit, print
				#_debug($codes[$group],"codes-group",false,true); #Variable, exit, print	
				foreach ($codes[$group] as $code_var => $code_value) {
					#_debug($code_var,"code_var",false,true); #Variable, exit, print
					#_debug($code_value,"code_value",false,true); #Variable, exit, print
					foreach ($code_value as $xcode => $xvalue) {
						#_debug($xcode,"xcode",false,true); #Variable, exit, print
						#_debug($xvalue,"xvalue",false,true); #Variable, exit, print
						if($var_value["code"]==$xvalue["code"]){
							#_debug($var_value["code"]."-".$xvalue["code"],"var-value-----xvalue",false,true); #Variable, exit, print	
							$data['node']['groups'][$group][$var]["percent"]=$xvalue["percent"];
						}
					}
				}					
			}
		}	
		#_debug($data['node']['groups'],"grupos-original",true,true); #Variable, exit, print	
		$data['node']['groups2']=$codes;
		return true;
	}

	# ---------------------------------------------------------------
	# Está es la construccion de combinaciones entre variables.
	function _survey_cygnal_quota_build_codes_combinations( $codes=array() ){
		$combinations = array();
		$vars_count=array();
		$x = count($codes);
		foreach ($codes as $i => $value) {
			$x--;
			$vars_count[$x]['name'] = $codes[$x][0]['name'];
			$vars_count[$x]['cuantos'] = count($codes[$x]);
			$vars_count[$x]['conteo']=0; # Este valor siempre inicia en cero.
			$vars_count[$x]['aux']=0; # Este valor siempre inicia en cero.
			$combinations = _survey_cygnal_quota_build_concat_recursive( $combinations, $codes[$x] );
		}
		return array(
			"combinations"=>$combinations,
	        "vars_count"=>$vars_count
	    );
	}

	# ---------------------------------------------------------------
	# Está es la recursividad de concatenar para construir los grupos por combinación.
	function _survey_cygnal_quota_build_concat_recursive( $combinations, $codes ){
		if ( empty($combinations) ) {
			foreach ($codes as $key => $val) {
				$combinations[]=$val['code'];
			}
		}else{
			$aux = $combinations;
			$combinations = array();
			foreach ($aux as $key => $val) {
				foreach ($codes as $k => $v) {
					$combinations[]=$val.'||'.$v['code'];
				}
			}
		}
		return $combinations;
	}

	# ---------------------------------------------------------------
	# Aquí se construye el tabulado del excel.
	function _survey_cygnal_quota_build_table(&$data=array(),$arreglo, $fila, $target=array(), $element ){
		ini_set('memory_limit', '-1');
		set_time_limit(180);

		#_debug($data,"data-build",false,true); #Variable, exit, print
		#_debug($target,"target",false,true); #Variable, exit, print

		$table = array();

		$cell='C';
		for ($j=0; $j<count($target); $j++) {		
			foreach ($target[$j]['cols_data'] as $key => $val) {
				$data2[$cell] = array( 'formula'=>'', 'vars'=>$arreglo['vars_count'] );
				$cell++;
			}
		}
		#_debug($data2,"data",false,true); #Variable, exit, print
		#_debug($target,"target",false,true); #Variable, exit, print
		if ( $element=='all' ) {
			// Recorrido por el total de registros a tabular.
			for ($i=0; $i<count($arreglo['combinations']); $i++) { 
		 		$cell='C';
		 		$cell_prev='B';
		 		$table['A'][ $fila ] = ($i+1);
		 		$table['B'][ $fila ] = str_replace("||", "", $arreglo['combinations'][$i]);
				#_debug($target,"target-a",false,true); #Variable, exit, print
				for ($j=0; $j<count($target); $j++) { // Recorrido por el numero de elementos.
					$cols_data = $target[$j]['cols_data'];
					$changeVar = ($j+1);
					#_debug($changeVar,"changeVar",false,true); #Variable, exit, print
					foreach ($cols_data as $key => $val) { // Recorrido por construcción de formulas.
						switch (substr($val, 1)) {
							case 'Pop': 	$data2 = _survey_cygnal_quota_build_formula_column_general_Pop($data, $data2, $cell, $changeVar,$target[$j]);	break;
							case 'PopC': 	$data2 = _survey_cygnal_quota_build_formula_column_general_PopC($data, $data2, $cell, $cell_prev, $changeVar,$target[$j], $fila, $table );	break;
							case 'QC': 		$data2 = _survey_cygnal_quota_build_formula_column_general_QC($data, $data2, $cell, $cell_prev, $changeVar, $target[$j], $fila, $table ); 	break;
							case 'Quota': 	$data2 = _survey_cygnal_quota_build_formula_column_general_Quota($data, $data2, $cell, $cell_prev, $changeVar, $fila, $table ); break;
						}
						#$data['formula']='aaaa';
						#_debug($data2,"Data formula",true,true); #Variable, exit, print
						if($data['node']['celda']=="numerica"){
							$table[ $cell ][ $fila ] = $data2['formula']; //Formula con datos numericos
						}else{
							$table[ $cell ][ $fila ] = $data2['formula2']; // Formula con datos variables	
						}
						$cell++;
						$cell_prev++;
						#_debug($cell,"cell--combina",false,true); #Variable, exit, print
					}
				}
		 		$fila++;
		 		#_debug($table,"table",false,true); #Variable, exit, print
		 		#_debug($fila,"fila",true,true); #Variable, exit, print
			}
			
		}else{
			// Recorrido por el total de registros a tabular.
			for ($i=0; $i<count($arreglo['combinations']); $i++) { 
		 		$cell='I';
		 		$cell_prev='H';
		 		$table['G'][ $fila ] = ($i+1);
		 		$table['H'][ $fila ] = str_replace("||", "", $arreglo['combinations'][$i]);
				for ($j=0; $j<count($target); $j++) { // Recorrido por el numero de elementos.
					if ( $element==$target[$j]['code'] ) {
						$cols_data = $target[$j]['cols_data'];
						foreach ($cols_data as $key => $val) { // Recorrido por construcción de formulas.
							switch (substr($val, 1)) {
								case 'Pop': 	$data2 = _survey_cygnal_quota_build_formula_column_simple_Pop($data, $data2, $cell, 1, $target[$j] ); 	break;
								case 'PopC': 	$data2 = _survey_cygnal_quota_build_formula_column_simple_PopC($data, $data2, $cell, $cell_prev,  1, $target[$j], $fila, $table ); 	break;
								case 'QC': 		$data2 = _survey_cygnal_quota_build_formula_column_simple_QC($data, $data2, $cell, $cell_prev, 1, $target[$j], $fila, $table ); 	break;
								case 'Quota': 	$data2 = _survey_cygnal_quota_build_formula_column_simple_Quota($data, $data2, $cell, $cell_prev, 1, $target[$j], $fila, $table ); break;
							}
							#$data['formula']='aaaa';
							$table[ $cell ][ $fila ] = $data2['formula'];
							$cell++;
							$cell_prev++;
						}
					}
				}
		 		$fila++;
			}
		}
		unset($fila, $target, $data2, $cell);
		$data['node']['table']=$table;
		$data['node']['codes']=$arreglo;
		_survey_cygnal_quota_build_table_quota($data);
		
		return true;
	}

	# ---------------------------------------------------------------
	# Aquí se construye el tabulado del excel.
	function _survey_cygnal_quota_build_table_quota(&$data=array()){
		#_debug($data,"data-build",false,true); #Variable, exit, print
		#_debug($target,"target",false,true); #Variable, exit, print

		$table2 = array();
		$fila=1;
		
		#_debug($data2,"data",false,true); #Variable, exit, print
		#_debug($target,"target",false,true); #Variable, exit, print
		
			// Recorrido por el total de registros a tabular.
			for ($i=0; $i<count($data['node']['codes']['combinations']); $i++) { 
		 		
		 		#Se debe sumar 5 a la fila en los datos ya almacenados, ya que esto construye el excel, y comienza en la fila 5
		 		$table2['A'][ $fila ] =($data['node']['table']["D"][$fila+4]*100); # Se multiplica por 100 para generarlo en porcentaje, y se toma la columna que contienen el porcentaje de quota general
		 		$table2['B'][ $fila ] ="";
		 		$table2['C'][ $fila ] = ($i+1);
		 		$table2['D'][ $fila ] ="";
		 		
				#_debug($target,"target-a",false,true); #Variable, exit, print
				
		 		$fila++;
		 		#_debug($table,"table",false,true); #Variable, exit, print
		 		#_debug($fila,"fila",true,true); #Variable, exit, print
			}
			
		
		unset($fila);
		$data['node']['table_quota']=$table2;

		return true;
	}


	function _survey_cygnal_quota_letters(){
		return array(
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 
			'AA', 'AB', 'AC', 'AD', 'AE', 'AF', 'AG', 'AH', 'AI', 'AJ', 'AK', 'AL', 'AM', 'AN', 'AO', 'AP', 'AQ', 'AR', 'AS', 'AT', 'AU', 'AV', 'AW', 'AX', 'AY', 'AZ', 
			'BA', 'BB', 'BC', 'BD', 'BE', 'BF', 'BG', 'BH', 'BI', 'BJ', 'BK', 'BL', 'BM', 'BN', 'BO', 'BP', 'BQ', 'BR', 'BS', 'BT', 'BU', 'BV', 'BW', 'BX', 'BY', 'BZ',
		);
	}

	function _survey_cygnal_quota_build_formula_column_general_Pop(&$data=array(), $data2, $cell, $changeVar,$target ){
		$string='';
		$fila_ini_config = 3; // Se restan estas 3 filas que ya estan ocupadas por las cabeceras.
		#_debug($cell,"cell",false,true); #Variable, exit, print

		#_debug($data["node"]["groups"][$target["code"]],"grupo",false,true); #Variable, exit, print
		$files_group = 3; // Ocupa las lineas: titulo, total, espacio.
		#_debug($data2[$cell]['vars'],"data-vars",false,true); #Variable, exit, print
		foreach ($data2[$cell]['vars'] as $key => $val) {
			$files_group += $val['cuantos'];
		}
		#_debug($files_group,"files_group",false,true); #Variable, exit, print

		# Inicia en cambio de variable x numero de filas que ocupa el grupo mas tres filas iniciales.
		$fila_ini_x_var = ((($changeVar-1)*$files_group)+$fila_ini_config);
		#_debug($fila_ini_x_var,"fila_ini_x_var",false,true); #Variable, exit, print
		$increment = ((($changeVar-1)*$files_group)+$fila_ini_config);
		$increment2=0;
		#_debug($increment,"increment",false,true); #Variable, exit, print

		for ($i=0; $i<count($data2[$cell]['vars']); $i++) { 
			
			if ($i==0) {
				if ($data2[$cell]['vars'][$i]['conteo']==$data2[$cell]['vars'][$i]['cuantos']) {
					$data2[$cell]['vars'][$i]['conteo']=0;
					$data2[$cell]['vars'][$i]['aux']=1;
				}
				$data2[$cell]['vars'][$i]['conteo']++;
			}

			if ($i>0) {
				if ($data2[$cell]['vars'][$i]['conteo']==0) {
					$data2[$cell]['vars'][$i]['conteo']=1;
				}
				if ($data2[$cell]['vars'][$i-1]['aux']==1) {
					$data2[$cell]['vars'][$i-1]['aux']=0;
					if ($data2[$cell]['vars'][$i]['conteo']==$data2[$cell]['vars'][$i]['cuantos']) {
						$data2[$cell]['vars'][$i]['conteo']=0;
						$data2[$cell]['vars'][$i]['aux']=1;
					}
					$data2[$cell]['vars'][$i]['conteo']++;
				}
			}

			if(isset($data2[$cell]['vars'][$i-1])){
				if ($data2[$cell]['vars'][$i-1]['aux']==1) {
					$data2[$cell]['vars'][$i-1]['aux']++;
				}
			}
			

			# V O Y     A  Q  U  I 
			#   El problema es que a este nivel, se esta referenciando la formula con las columnas del excel 
			#   Entonces aqui aplica el incremento, pero en el caso del valor directo, se debe evaluar este incremento
            #   ya que la primer vez siempre comienza en cero, y de ahi en adelante el incremento si sera acorde 
            #   a las variables u todo el calculo

			# ---------------------------------------------------------------
			# Construcción del string.
			if ($i==0) {
				$number = ( $data2[$cell]['vars'][$i]['conteo'] + $increment );
				$number2 = ( $data2[$cell]['vars'][$i]['conteo'] + $increment2 ) -1;
				$string='*Config!$I'.$number;
				#_debug($number2,"number2 =0",false,true); #Variable, exit, print
				$numeric=$data["node"]["groups"][$target["code"]][$number2]["percent"];
				#_debug($numeric,"numeric =0",false,true); #Variable, exit, print
				#_debug($string,"string",false,true); #Variable, exit, print
				#_debug($target["code"],"grp",false,true); #Variable, exit, print
				#_debug($data["node"]["groups"],"grupos",false,true); #Variable, exit, print
				#_debug($number,"number",false,true); #Variable, exit, print
				#_debug($increment,"increment",false,true); #Variable, exit, print
				#_debug($fila_ini_config,"fila_ini_config",false,true); #Variable, exit, print
				
				#_debug($numeric,"numeric ==0",false,true); #Variable, exit, print
				#_debug($number,"number ==0",false,true); #Variable, exit, print
			}else{
				$increment +=$data2[$cell]['vars'][$i-1]['cuantos'];
				$increment2 +=$data2[$cell]['vars'][$i-1]['cuantos'];
				$number = ( $data2[$cell]['vars'][$i]['conteo'] + $increment );
				$number2 = ( $data2[$cell]['vars'][$i]['conteo'] + $increment2 )-1;
				$string='*Config!$I$'.$number.$string;
				#_debug($number2,"number2 >0",false,true); #Variable, exit, print
				$numeric=$numeric*$data["node"]["groups"][$target["code"]][$number2]["percent"];
				#_debug($numeric,"numeric >0",false,true); #Variable, exit, print
				#_debug($number,"number > 0",false,true); #Variable, exit, print
				
				#_debug($string,"string",false,true); #Variable, exit, print
				#_debug($data["node"]["groups"][$target["code"]],"code",false,true); #Variable, exit, print
				#_debug($number,"number",false,true); #Variable, exit, print
				#_debug($increment,"increment",false,true); #Variable, exit, print
				#_debug($fila_ini_config,"fila_ini_config",false,true); #Variable, exit, print
				
				#_debug($numeric,"numeric ==0",false,true); #Variable, exit, print
				#_debug($number,"number > 0",false,true); #Variable, exit, print
			}
		}

		#_debug($target,"target",false,true); #Variable, exit, print
		#_debug($data["node"]["groups"],"grupos",false,true); #Variable, exit, print}

		$ini = ($fila_ini_config + $changeVar);
		#_debug($ini,"init",false,true); #Variable, exit, print
		#_debug($string,"string",false,true); #Variable, exit, print

		$data2['formula2'] = '=Config!$B$'.$ini.$string;
		#$data2['formula'] = round($target["population"]*$numeric);
		$data2['formula'] = $target["population"]*$numeric;
		#_debug($data2,"data2",true,true); #Variable, exit, print
		return $data2;

		#$data['formula'] = 'AAA-'.($fila_ini_config + $changeVar);
		#return $data;
	}

	function _survey_cygnal_quota_build_formula_column_general_PopC(&$data=array(), $data2, $cell, $cell_prev, $changeVar, $target, $fila, $table ){
		#_debug($target,"target",false,true); #Variable, exit, print
		#_debug($table,"table",false,true); #Variable, exit, print
		
		#_debug($table[$cell_prev][$fila],"valor",false,true); #Variable, exit, print
		#_debug($target["population"],"population",false,true); #Variable, exit, print
		$abc = _survey_cygnal_quota_letters();
		$key = (array_search($cell, $abc) - 1);
		$nexGroup = (3+$changeVar); // Las tres primeras filas ocupadas.
		$data2['formula2'] = '='.$abc[$key].$fila.'/'.'Config!$B'.$nexGroup;
		#$data2['formula']=number_format(($table[$cell_prev][$fila]/$target["population"]),6);
		$data2['formula']=($table[$cell_prev][$fila]/$target["population"]);
		#_debug($cell,"cell",false,true); #Variable, exit, print
		#_debug($cell-1,"cell_prev",false,true); #Variable, exit, print
		#_debug($data2,"data2",true,true); #Variable, exit, print
		return $data2;
	}

	function _survey_cygnal_quota_build_formula_column_general_QC(&$data=array(), $data2, $cell, $cell_prev, $changeVar, $target, $fila, $table ){
		#_debug($table,"table",false,true); #Variable, exit, print
		#_debug($cell_prev,"table",false,true); #Variable, exit, print
		#_debug($fila,"table",false,true); #Variable, exit, print
		#_debug($target,"target",false,true); #Variable, exit, print
		$abc = _survey_cygnal_quota_letters();
		$key = (array_search($cell, $abc) - 1);
		#$n = (int)count($data2[$cell]['vars']);
		$n = (int)count($data['node']['groups']);
		/*echo "<pre>"; 
		print_r($data);
		echo "</pre>";
		exit(0);*/
		$cellZice = ($n+4);
		if ($changeVar==1) {
			$data2['formula2'] = '='.$abc[$key].$fila.'*Config!$B$'.$cellZice;
			#$data2['formula']=number_format(($table[$cell_prev][$fila]*$data['node']['sample_size']["value"]),6);
			$data2['formula']=($table[$cell_prev][$fila]*$data['node']['sample_size']["value"]);
		}else{
			$nexGroup = (3+$changeVar);
			$data2['formula2'] = '='.$abc[$key].$fila.'*Config!$C$'.$nexGroup.'*Config!$B$'.$cellZice;
			#$data2['formula']=number_format(($table[$cell_prev][$fila]*$target['percent']*$data['node']['sample_size']["value"]),6);
			$data2['formula']=($table[$cell_prev][$fila]*$target['percent']*$data['node']['sample_size']["value"]);
		}
		
		return $data2;
	}

	function _survey_cygnal_quota_build_formula_column_general_Quota(&$data=array(), $data2, $cell, $cell_prev, $changeVar, $fila, $table){
		$abc = _survey_cygnal_quota_letters();
		$key = (array_search($cell, $abc) - 1);
		$data2['formula2'] = '='.$abc[$key].$fila;
		$data2['formula'] = round($table[$cell_prev][$fila]);  //Verificar que dato exacto se debe tener aqui, porque se tenia number format
		//$data2['formula'] = round(number_format($table[$cell_prev][$fila],6,".",""));
		return $data2;
	}



	function _survey_cygnal_quota_build_formula_column_simple_Pop(&$data=array(), $data2, $cell, $changeVar, $target ){
		$string='';
		$fila_ini_config = 4; // Se restan estas 4 filas que ya estan ocupadas por las cabeceras y una fila en blanco.
		
		#_debug($cell,"cell",false,true); #Variable, exit, print
		#_debug($target,"target",false,true); #Variable, exit, print
		#_debug($data,"data",false,true); #Variable, exit, print

		$increment =8;
		$increment2=0;
		for ($i=0; $i<count($data2[$cell]['vars']); $i++) { 
			
			if ($i==0) {
				if ($data2[$cell]['vars'][$i]['conteo']==$data2[$cell]['vars'][$i]['cuantos']) {
					$data2[$cell]['vars'][$i]['conteo']=0;
					$data2[$cell]['vars'][$i]['aux']=1;
				}
				$data2[$cell]['vars'][$i]['conteo']++;
			}

			if ($i>0) {
				if ($data2[$cell]['vars'][$i]['conteo']==0) {
					$data2[$cell]['vars'][$i]['conteo']=1;
				}
				if ($data2[$cell]['vars'][$i-1]['aux']==1) {
					$data2[$cell]['vars'][$i-1]['aux']=0;
					if ($data2[$cell]['vars'][$i]['conteo']==$data2[$cell]['vars'][$i]['cuantos']) {
						$data2[$cell]['vars'][$i]['conteo']=0;
						$data2[$cell]['vars'][$i]['aux']=1;
					}
					$data2[$cell]['vars'][$i]['conteo']++;
				}
			}

			if ($data2[$cell]['vars'][$i-1]['aux']==1) {
				$data2[$cell]['vars'][$i-1]['aux']++;
			}
			
			# ---------------------------------------------------------------
			# Construcción del string.
			if ($i==0) {
				$number = ( $data2[$cell]['vars'][$i]['conteo'] + $increment );
				$number2 = ( $data2[$cell]['vars'][$i]['conteo'] + $increment2 ) -1;
				$string='*$E'.$number;
				$numeric=$data["node"]["groups"][$target["code"]][$number2]["percent"];
			}else{
				$increment +=$data2[$cell]['vars'][$i-1]['cuantos'];
				$increment2 +=$data2[$cell]['vars'][$i-1]['cuantos'];
				$number = ( $data2[$cell]['vars'][$i]['conteo'] + $increment );
				$number2 = ( $data2[$cell]['vars'][$i]['conteo'] + $increment2 )-1;
				$string='*$E$'.$number.$string;
				$numeric=$numeric*$data["node"]["groups"][$target["code"]][$number2]["percent"];
			}
		}


		$ini = ($fila_ini_config + $changeVar);
		if($data['node']["celda"]=="formulada"){
			$data2['formula'] = '=$B$'.$ini.$string;
		}else{
			$data2['formula'] = $target["population"]*$numeric;
		}
		#_debug($data2['formula'],"formula",true,true); #Variable, exit, print
		return $data2;

		#$data['formula'] = 'AAA-'.($fila_ini_config + $changeVar);
		#return $data;
	}

	function _survey_cygnal_quota_build_formula_column_simple_PopC(&$data=array(), $data2, $cell, $cell_prev, $changeVar, $target, $fila, $table ){
		$abc = _survey_cygnal_quota_letters();
		$key = (array_search($cell, $abc) - 1);
		$nexGroup = 5; // Las cuatro primeras filas ocupadas.
		$data['formula2'] = '='.$abc[$key].$fila.'/'.'$B'.$nexGroup;
		$data2['formula']=($table[$cell_prev][$fila]/$target["population"]);
		return $data2;
	}

	function _survey_cygnal_quota_build_formula_column_simple_QC(&$data=array(), $data2, $cell, $cell_prev, $changeVar, $target, $fila, $table ){
		$abc = _survey_cygnal_quota_letters();
		$key = (array_search($cell, $abc) - 1);
		$data2['formula2'] = '='.$abc[$key].$fila.'*$C$5*$B$6';
		$data2['formula']=($table[$cell_prev][$fila]*$target['percent']*$data['node']['sample_size']["value"]);
		return $data2;
	}

	function _survey_cygnal_quota_build_formula_column_simple_Quota(&$data=array(), $data2, $cell, $cell_prev, $changeVar, $target, $fila, $table ){
		$abc = _survey_cygnal_quota_letters();
		$key = (array_search($cell, $abc) - 1);
		$data2['formula2'] = '='.$abc[$key].$fila;
		$data2['formula'] = round(number_format($table[$cell_prev][$fila],6,".",""));
		return $data2;
	}

	function _survey_cygnal_quota_excel( $data ){
		#die("Stop");
		$filename =_survey_cygnal_quota_excel_file( $data );
		return $filename;	
	}

	function _survey_cygnal_quota_excel_file( $data ){	
		global $base_url;
		$path = DRUPAL_ROOT;
		$path_module = drupal_get_path('module', 'survey_cygnal');
		$path_libraries = libraries_get_path('phpexcel'); 

		if (PHP_SAPI == 'cli'){
			// Este ejemplo solo debe ejecutarse desde un navegador web
			die('This example should only be run from a Web Browser');
		}

		//require_once $path_libraries."/Classes/PHPExcel.php";	
		require_once $path.'/'.$path_libraries.'/Classes/PHPExcel.php'; 	
		
		# ------------------------------------------------------------------
		if (!class_exists('EXCEL')) {
			
			class EXCEL {

				private $PHPEXCEL;
				private $path;
				private $base_url;
				private $path_module;
				private $path_libraries;
				private $node;
				private $line_params;
				private $section;
				private $cells;
				private $header; 
				private $h1; 
				private $headerTop;
				private $bgYellow;
				private $namefile;



				public function EXCEL( $params ){
					$this->PHPEXCEL 		= $params['PHPEXCEL'];
					$this->path 			= $params['path'];
					$this->base_url			=$params['base_url'];
					$this->path_module 		= $params['path_module'];
					$this->path_libraries 	= $params['path_libraries'];
					$this->node 			= $params['node'];
					$this->namefile 		= $params['namefile'];

					$this->header = array( 
						'font' => array('bold' => true, 'size' => 18, 'name' => 'Verdana', 'color' => array('rgb' => '427590')), 
						'alignment' => array('vertical' => 'center', 'horizontal' => 'left'),
						'borders' => array('top' => array('style' => PHPExcel_Style_Border::BORDER_THIN,),),
						'fill' => array(
							'type' => PHPExcel_Style_Fill::FILL_GRADIENT_LINEAR,
							'rotation' => 90,
							'startcolor' => array('argb' => 'FFF2F2F2',),
							'endcolor' => array('argb' => 'FFFFFFFF',),
						),
					);	
					$this->headerTop = array(
						'font' => array('bold' => true, 'size' => 16, 'name' => 'Calibri', 'color' => array('rgb' => '427590')),
						'alignment' => array('vertical' => 'center', 'horizontal' => 'center'),
						'borders' => array('bottom' => array('style' => PHPExcel_Style_Border::BORDER_THIN)),
						'fill' => array(
							'type' => PHPExcel_Style_Fill::FILL_GRADIENT_LINEAR,
							'rotation' => 90,
							'startcolor' => array('argb' => 'FFD8D8D8',),
							'endcolor' => array('argb' => 'FFFFFFFF',),
						),
					);			
					$this->h1 = array( 
						'font' => array('bold' => true, 'size' => 11, 'name' => 'Tahoma', 'color' => array('rgb' => 'FFFFFF')), 
						'borders' => array('allborders' => array('style' => 'thin')), 
						'alignment' => array('vertical' => 'center', 'horizontal' => 'center'),
						'fill' => array(
							'type' => PHPExcel_Style_Fill::FILL_SOLID,
							'color' => array('rgb' => '427590')
						)
					); 
					$this->h2 = array( 
						'font' => array('bold' => true, 'size' => 10, 'name' => 'Tahoma', 'color' => array('rgb' => 'FFFFFF')), 
						'borders' => array('allborders' => array('style' => 'thin')), 
						'alignment' => array('vertical' => 'center', 'horizontal' => 'center'),
						'fill' => array(
							'type' => PHPExcel_Style_Fill::FILL_SOLID,
							'color' => array('rgb' => '145A32')
						)
					);
					$this->bgGreen = array(
						'borders' => array('allborders' => array('style' => 'thin')), 
						'fill' => array(
							'type' => PHPExcel_Style_Fill::FILL_SOLID,
							'color' => array('rgb' => 'C4D79B')
						)
					);
					$this->bgYellow = array(
						'borders' => array('allborders' => array('style' => 'thin')), 
						'fill' => array(
							'type' => PHPExcel_Style_Fill::FILL_SOLID,
							'color' => array('rgb' => 'FFFF00')
						)
					);		

					$array = array( 
						'font' => array('bold' => true, 'size' => 14, 'name' => 'Calibri', 'color' => array('rgb' => 'FFFFFF')), 
						'borders' => array('allborders' => array('style' => 'thin')), 
						'alignment' => array('vertical' => 'center', 'horizontal' => 'center'),
					);
					$this->bgTitle[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'0F243E') )));
					$this->bgTitle[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'1D1B10') )));
					$this->bgTitle[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'632523') )));
					$this->bgTitle[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'4F6228') )));
					$this->bgTitle[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'403151') )));
					$this->bgTitle[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'244062') )));
					$this->bgTitle[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'215967') )));
					$this->bgTitle[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'974706') )));
					$this->bgTitle[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'808080') )));

					$array = array( 
						'font' => array('bold' => true, 'size' => 11, 'name' => 'Calibri', 'color' => array('rgb' => '101010')), 
						'borders' => array('allborders' => array('style' => 'thin')), 
						'alignment' => array('vertical' => 'center', 'horizontal' => 'center'),
					);
					$this->bgSubTitle[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'366092') )));
					$this->bgSubTitle[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'948A54') )));
					$this->bgSubTitle[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'DA9694') )));
					$this->bgSubTitle[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'C4D79B') )));
					$this->bgSubTitle[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'B1A0C7') )));
					$this->bgSubTitle[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'95B3D7') )));
					$this->bgSubTitle[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'92CDDC') )));
					$this->bgSubTitle[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'E26B0A') )));
					$this->bgSubTitle[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'A6A6A6') )));

					$arrayCero = array( 
						'font' => array('bold' => false, 'size' => 11, 'name' => 'Calibri', 'color' => array('rgb' => '000000')), 
						'borders' => array('allborders' => array('style' => 'thin')), 
						'alignment' => array('vertical' => 'center', 'horizontal' => 'center'),
					);
					$array = array( 
						'font' => array('bold' => false, 'size' => 11, 'name' => 'Calibri', 'color' => array('rgb' => '000000')), 
						'borders' => array('allborders' => array('style' => 'thin')), 
						'alignment' => array('vertical' => 'center'),
					);
					$this->bgData[] = array_merge($arrayCero, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'C5D9F1') )));
					$this->bgData[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'DDD9C4') )));
					$this->bgData[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'F2DCDB') )));
					$this->bgData[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'EBF1DE') )));
					$this->bgData[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'E4DFEC') )));
					$this->bgData[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'DCE6F1') )));
					$this->bgData[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'DAEEF3') )));
					$this->bgData[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'FDE9D9') )));
					$this->bgData[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'F2F2F2') )));


					/*echo "<pre>";
					print_r( $this->bgTitle );
					echo "</pre>";
					die;*/
				}

				public function excel_document_properties(){
					// Set document properties
					$this->PHPEXCEL->getProperties()
						->setCreator("Alfonso Chavez - Alberto Chaves")
						->setLastModifiedBy("Alfonso Chavez - Alberto Chaves")
						->setTitle("Office 2007 XLSX Test Document")
						->setSubject("Office 2007 XLSX Test Document")
						->setDescription("Test document for Office 2007 XLSX, generated using PHP classes.")
						->setKeywords("office 2007 openxml php")
						->setCategory("Test result file");
					return;
				}

				public function excel_sheet_one(){
					$heet=0;
					$this->PHPEXCEL->getActiveSheet()->setTitle('Data');
					$this->excel_set_lines_headers( $heet );
					return;
				}

				public function excel_sheet_two(){
					$heet=1;
					$this->PHPEXCEL->createSheet();
					$this->PHPEXCEL->setActiveSheetIndex( $heet );
					$this->PHPEXCEL->getActiveSheet()->setTitle('Config');
					$this->excel_set_lines_headers( $heet );
					return;
				}

				public function excel_sheet_tree(){
					$heet=2;
					$this->PHPEXCEL->createSheet();
					$this->PHPEXCEL->setActiveSheetIndex( $heet );
					$this->PHPEXCEL->getActiveSheet()->setTitle('Resources');
					$this->excel_set_lines_headers( $heet );
					return;
				}

				public function excel_set_lines_headers( $sheet ){
					
					# -----------------------------------------------------------------------------------
					# Toda la fila uno tiene un alto definido.
					$this->PHPEXCEL->getActiveSheet()->getRowDimension('1')->setRowHeight(58);
					
					# -----------------------------------------------------------------------------------
					# Se pone el logo de la compañia al inicio.
					$gdImage = imagecreatefrompng($this->node['logo_cygnal']);
					$objDrawing = new PHPExcel_Worksheet_MemoryDrawing();
					$objDrawing->setName('Cygnal');
					$objDrawing->setDescription('Cygnal');
					$objDrawing->setImageResource($gdImage);
					$objDrawing->setRenderingFunction(PHPExcel_Worksheet_MemoryDrawing::RENDERING_PNG);
					$objDrawing->setMimeType(PHPExcel_Worksheet_MemoryDrawing::MIMETYPE_DEFAULT);
					$objDrawing->setHeight(74);
					$objDrawing->setWorksheet($this->PHPEXCEL->getActiveSheet());

					# -----------------------------------------------------------------------------------
					# Las celdas se combinan.
					$this->PHPEXCEL->setActiveSheetIndex( $sheet )->mergeCells('A1:B1');


					if ( $this->node['view']!='all' ) {
						$countCells = 12;
					}else{
						$countCells = ((count($this->node['target'])*4)+6);
					}
					
					$nextC='A';
					for ($i=1; $i<$countCells; $i++) { 
						$nextC++;
					}
					$merge = 'C1:'.$nextC.'1';
					#die("=".$merge);
					$this->PHPEXCEL->setActiveSheetIndex( $sheet )->mergeCells( $merge );

					# -----------------------------------------------------------------------------------
					# Se pone el texto descriptivo de la aplicación.
					$this->PHPEXCEL->setActiveSheetIndex( $sheet )->setCellValue('C1', $this->node['text_header']);
					$this->PHPEXCEL->getActiveSheet()->getStyle( $merge )->applyFromArray( $this->header );

					# -----------------------------------------------------------------------------------
					# Todas las columnas tienen un ancho definido.
					$cell='A';
					for ($i=0; $i<27; $i++) { 
						$this->PHPEXCEL->getActiveSheet()->getColumnDimension( $cell )->setWidth(14);
						$cell++;
					}
					
					# -----------------------------------------------------------------------------------
					# Definir la altura de la fila.
					$this->PHPEXCEL->getActiveSheet()->getRowDimension('2')->setRowHeight(28);

					# -----------------------------------------------------------------------------------
					# Combinar las celdas de la fila actual.
					$merge = 'A2:'.$nextC.'2';
					$this->PHPEXCEL->setActiveSheetIndex( $sheet )->mergeCells( $merge );

					# -----------------------------------------------------------------------------------
					# Setear el titulo de la encuesta.
					$this->PHPEXCEL->setActiveSheetIndex( $sheet )->setCellValue('A2', $this->node['text_title']);

					# -----------------------------------------------------------------------------------
					# Aplicar los estilos correspondientes.
					$this->PHPEXCEL->getActiveSheet()->getStyle( $merge )->applyFromArray($this->headerTop);
					return;
				}

				public function excel_sheet_two_params(){
					
					$heet=1;	# 1 Para iniciar en la hoja 2.
					$index=3; 	# Para iniciar en A3.

					$style = $this->PHPEXCEL->setActiveSheetIndex( $heet );
		        	$this->PHPEXCEL->setActiveSheetIndex( $heet )
		        		->setCellValue( "A$index", 'Target' )
		        		->setCellValue( "B$index", 'Population' )
		        		->setCellValue( "C$index", 'Percent' );
					$style->getStyle( "A$index:C$index" )->applyFromArray( $this->h2 );
					$index++;
					foreach ($this->node['target'] as $key => $val) {
		        		$this->PHPEXCEL->setActiveSheetIndex( $heet )
		        			->setCellValue("A$index", $val['title_population'])
		        			->setCellValue("B$index", $val['population']);

		        		if (!empty($val['percent'])) {
		        			$this->PHPEXCEL->setActiveSheetIndex( $heet )->setCellValue("C$index", $val['percent']);
		        		}

						$style->getStyle( "A$index:C$index" )->applyFromArray( $this->bgYellow );
						
						$this->PHPEXCEL->getActiveSheet()->getStyle("B$index")->getNumberFormat()->setFormatCode('#,##0');
						$this->PHPEXCEL->getActiveSheet()->getStyle("C$index")->getNumberFormat()->applyFromArray(array('code' => PHPExcel_Style_NumberFormat::FORMAT_PERCENTAGE ));
						
						if ($key==0) {
							$style->getStyle( "A$index:C$index" )->applyFromArray( $this->bgGreen );
						}
						$index++;
					}
					$this->PHPEXCEL->setActiveSheetIndex( $heet )
						->setCellValue("A$index", $this->node['sample_size']['label'])
						->setCellValue("B$index", $this->node['sample_size']['value']);
					$style->getStyle( "A$index:C$index" )->applyFromArray( $this->bgGreen );
					$index++;
			        return;
				}

				public function excel_sheet_two_params_groups(){
					$heet=1;	# 1 Para iniciar en la hoja 2.
					$index=3; 	# Para iniciar en A1.

					$style = $this->PHPEXCEL->setActiveSheetIndex( $heet );

					foreach ($this->node['target'] as $key => $val) {
						
						$this->PHPEXCEL->setActiveSheetIndex( $heet )
							->setCellValue("E$index", $val['cols_params'][0])
							->setCellValue("F$index", $val['cols_params'][1])
							->setCellValue("G$index", $val['cols_params'][2])
							->setCellValue("H$index", $val['cols_params'][3])
							->setCellValue("I$index", $val['cols_params'][4]);
						$style->getStyle( "E$index:I$index" )->applyFromArray( $this->h2 );
						$index++;
						
						$rows = $this->node['groups'][$val['code']];

						
						# ------------------------------------------
						$arreglo = array();
						$formula = array();
						$i = $index;
						$change='';
						foreach ($rows as $id => $info) {
							if ($info['name']!=$change) {
								$change = $info['name'];
								$ini = $i;
							}
							$arreglo[$i]['text'] = 'H'.$i.'/SUM($H$'.$ini.':$H$';
							$arreglo[$i]['name'] = $info['name'];
							$i++;
						}
						$change='';
						$formula = array();
						for ($j=($i-1); $j>=$index; $j--) { 
							if ($arreglo[$j]['name']!=$change) {
								$change = $arreglo[$j]['name'];
								$fin = $j;
							}
							$formula[$j] = '='.$arreglo[$j]['text'].$fin.')';
						}
						
						#$this->imprimir($formula); 
						# ------------------------------------------
						
						$sumIni = $sumFin = $index;
						$aux = $rows[0]['name'];
						foreach ($rows as $id => $info) {
							if ($aux==$info['name']) {
								$sumFin++;
							}
							$this->PHPEXCEL->setActiveSheetIndex( $heet )
								->setCellValue("E$index", $info['name'])
								->setCellValue("F$index", $info['code'])
								->setCellValue("G$index", $info['value'])
								->setCellValue("H$index", $info['Population'])
								->setCellValue("I$index", "$formula[$index]");

							$this->PHPEXCEL->getActiveSheet()->getStyle("H$index")->getNumberFormat()->setFormatCode('#,##0');
							$this->PHPEXCEL->getActiveSheet()->getStyle("I$index")->getNumberFormat()->applyFromArray(array('code' => PHPExcel_Style_NumberFormat::FORMAT_PERCENTAGE_00 ));
							
							$style->getStyle( "E$index:I$index" )->applyFromArray( $this->bgYellow );
							$index++;
						}
						unset($arreglo, $formula);

						$sumFin=($sumFin-1);
						$formula="=SUM(H$sumIni:H$sumFin)";
						$this->PHPEXCEL->setActiveSheetIndex( $heet )->setCellValue("E$index", 'Total');
						$style->getStyle( "E$index:G$index" )->applyFromArray( array('borders' => array('allborders' => array('style' => 'thin')), 'font' => array('bold' => true), 'alignment' => array('vertical' => 'center', 'horizontal' => 'right'),'fill' => array('type' => PHPExcel_Style_Fill::FILL_SOLID,'color' => array('rgb' => 'FFFF00'))));

						$this->PHPEXCEL->getActiveSheet()->getStyle("H$index")->getNumberFormat()->setFormatCode('#,##0');
						$this->PHPEXCEL->setActiveSheetIndex( $heet )->setCellValue("H$index", $formula);
						$style->getStyle( "H$index:I$index" )->applyFromArray( $this->bgYellow );

						$this->PHPEXCEL->setActiveSheetIndex( $heet )->mergeCells( "E$index:G$index" );
						$index=($index+2);
					}
					return;
				}

				public function excel_sheet_one_titles(){
					$heet=0;	# 0 Para iniciar en la hoja 1.
					$index=3; 	# Para iniciar en A3.
					
					$style = $this->PHPEXCEL->setActiveSheetIndex( $heet );

					$this->PHPEXCEL->getActiveSheet()->getRowDimension( $index )->setRowHeight(22);

					$this->PHPEXCEL->setActiveSheetIndex( $heet )->mergeCells( "A$index:B$index" );
					$this->PHPEXCEL->setActiveSheetIndex( $heet )->setCellValue("A$index", "Groups");
					$style->getStyle( "A$index:B$index" )->applyFromArray( $this->bgTitle[0] );

					$n=1;
					$cIni='B';
					$cFin='E';
					$cicle = (count($this->node['target'])+1);
					for ($i=0; $i<$cicle; $i++) { 					
						$cIni++;
						$cFin++;
						$merge = $cIni.$index.':'.$cFin.$index;
						$write = $cIni.$index;	
						$this->PHPEXCEL->setActiveSheetIndex( $heet )->mergeCells( $merge );
						if (isset($this->node['target'][$i])) {
							$this->PHPEXCEL->setActiveSheetIndex( $heet )->setCellValue( $write, $this->node['target'][$i]['title_data']);
						}else{
							$this->PHPEXCEL->setActiveSheetIndex( $heet )->setCellValue( $write, "OTHERS");
						}
						$style->getStyle( $merge )->applyFromArray( $this->bgTitle[$n] );
						for ($j=0; $j<3; $j++) {
							$cIni++;
							$cFin++;
						}
						$n++;
					}
					return;
				}

				public function excel_sheet_one_sub_titles(){
					$heet=0;	# 0 Para iniciar en la hoja 1.
					$index=4; 	# Para iniciar en A4.
					$style = $this->PHPEXCEL->setActiveSheetIndex( $heet );
					$this->PHPEXCEL->getActiveSheet()->getRowDimension( $index )->setRowHeight(20);
					$this->PHPEXCEL->setActiveSheetIndex( $heet )
						->setCellValue("A$index", "Number")
						->setCellValue("B$index", "Code");
					$style->getStyle( "A$index:B$index" )->applyFromArray( $this->bgSubTitle[0] );

					$n=1;
					$cell='C';
					for ($i=0; $i<count($this->node['target']); $i++) {
						foreach ($this->node['target'][$i]['cols_data'] as $key => $val) {
							$indexCell=$cell.$index;
							$this->PHPEXCEL->setActiveSheetIndex( $heet )->setCellValue($indexCell, $val);
							$style->getStyle( $indexCell )->applyFromArray( $this->bgSubTitle[$n] );
							$cell++;
						}
						$n++;
					}	

					$array = array('Pop%', 'Sam', 'Group', 'Weight');
					foreach ($array as $key => $val) {
						$indexCell=$cell.$index;
						$this->PHPEXCEL->setActiveSheetIndex( $heet )->setCellValue($indexCell, $val);
						$style->getStyle( $indexCell )->applyFromArray( $this->bgSubTitle[$n] );
						$cell++;
					}

					return;
				}

				public function excel_sheet_one_data(){
					$heet=0;	# 0 Para iniciar en la hoja 1.
					$index=5; 	# Para iniciar en A5.

					if ( isset($this->node['table']['A']) ) {

						$style = $this->PHPEXCEL->setActiveSheetIndex( $heet );

						$lettersPop=array('C', 'G', 'K', 'O', 'S', 'W', 'AA', 'AE', 'AI', 'AM');
						$lettersPopC=array('D', 'H', 'L', 'P', 'T', 'X', 'AB', 'AF', 'AJ', 'AN');
						$lettersQC=array('E', 'I', 'M', 'Q', 'U', 'Y', 'AC', 'AG', 'AK', 'AO');
						$lettersQuota=array('F', 'J', 'N', 'R', 'V', 'Z', 'AD', 'AH', 'AL', 'AP');
						# ---------------------------------------------------------
						# Escribir el excel.
						foreach ($this->node['table'] as $key => $value) {
							foreach ($value as $k => $val) {
								$posicition=$key.$k;
								$this->PHPEXCEL->setActiveSheetIndex( $heet )->setCellValue( $posicition, $val );
								if (in_array($key, $lettersPop)) {
									$this->PHPEXCEL->getActiveSheet()->getStyle( $posicition )->getNumberFormat()->setFormatCode('#,##0');
								}
								if (in_array($key, $lettersPopC)) {
									$this->PHPEXCEL->getActiveSheet()->getStyle( $posicition )->getNumberFormat()->setFormatCode('#,##0.000000');
								}
								if (in_array($key, $lettersQC)) {
									$this->PHPEXCEL->getActiveSheet()->getStyle( $posicition )->getNumberFormat()->setFormatCode('#,##0.00000');
								}
								if (in_array($key, $lettersQuota)) {
									$this->PHPEXCEL->getActiveSheet()->getStyle( $posicition )->getNumberFormat()->applyFromArray(array('code' => PHPExcel_Style_NumberFormat::FORMAT_NUMBER ));
								}
							}
						}

						# ---------------------------------------------------------
						# Cuatro columnas finales.
						$cell=$key;
						for ($i=0; $i<4; $i++) { // Desplaza 4 columnas.
							$cell++;
							$n=1;
							foreach ($this->node['table']['A'] as $key => $val) {
								$posicition = $cell.$key;
								switch ( $i ) {
									case 0:
										$asign = '=D'.$key;
										$this->PHPEXCEL->setActiveSheetIndex( $heet )->setCellValue( $posicition, $asign );
										$this->PHPEXCEL->getActiveSheet()->getStyle( $posicition )->getNumberFormat()->applyFromArray(array('code' => PHPExcel_Style_NumberFormat::FORMAT_PERCENTAGE_00 ));
									break;
									case 2:
										$this->PHPEXCEL->setActiveSheetIndex( $heet )->setCellValue( $posicition, $n );
										$this->PHPEXCEL->getActiveSheet()->getStyle( $posicition )->getNumberFormat()->applyFromArray(array('code' => PHPExcel_Style_NumberFormat::FORMAT_NUMBER ));
									break;
								}
								$n++;
							}
						}

						# ---------------------------------------------------------
						# Estilo para columnas A y B.
						$iniHeight = key( $this->node['table']['A'] );
						$finHeight = (count($this->node['table']['A'])+($index-1));
						$indexCell='A'.$iniHeight.':B'.$finHeight;
						$style->getStyle( $indexCell )->applyFromArray( $this->bgData[0] );
						
						# ---------------------------------------------------------
						# Estilo para las otras columnas.
						$n=1;
						$cell='C';
						$cicle = (count($this->node['target'])+1);// Esto por el grupo de columnas que no son de variables.
						for ($i=0; $i<$cicle; $i++) { 
							$indexCell=$cell.$iniHeight;
							for ($j=0; $j<3; $j++) {
								$cell++;
							}
							$indexCell.=':'.$cell.$finHeight;
							$cell++;
							$style->getStyle( $indexCell )->applyFromArray( $this->bgData[$n] );
							$n++;
						}

						# ---------------------------------------------------------
						# Totales del tabulado.
						$cell = 'C';
						$cellQuota = 'F';
						$fila = ($finHeight+1);
						$increment = 3;
						for ($i=0; $i<($cicle-1); $i++) {
							$increment++;

							$formula = '=SUM('.$cell.$iniHeight.':'.$cell.$finHeight.')';
							$posicition = $cell.$fila;
							$this->PHPEXCEL->setActiveSheetIndex( $heet )->setCellValue( $posicition, $formula );
							$this->PHPEXCEL->getActiveSheet()->getStyle( $posicition )->getNumberFormat()->setFormatCode('#,##0');
							$style->getStyle( $posicition )->applyFromArray( $this->bgGreen );

							$formulaQuota = '=SUM('.$cellQuota.$iniHeight.':'.$cellQuota.$finHeight.')';
							$posicitionQuota = $cellQuota.$fila;
							$this->PHPEXCEL->setActiveSheetIndex( $heet )->setCellValue( $posicitionQuota, $formulaQuota );
							$this->PHPEXCEL->getActiveSheet()->getStyle( $posicitionQuota )->getNumberFormat()->applyFromArray(array('code' => PHPExcel_Style_NumberFormat::FORMAT_NUMBER ));
							$style->getStyle( $posicitionQuota )->applyFromArray( $this->bgGreen );

							$diff = '=Config!$B'.$increment.'-'.$cell.$fila;
							$posicition2 = $cell.($fila+1);
							$this->PHPEXCEL->setActiveSheetIndex( $heet )->setCellValue( $posicition2, $diff );
							$this->PHPEXCEL->getActiveSheet()->getStyle( $posicition2 )->getNumberFormat()->applyFromArray(array('code' => PHPExcel_Style_NumberFormat::FORMAT_NUMBER ));

							for ($j=0; $j<4; $j++) {
								$cell++;
								$cellQuota++;
							}
						}
						# ---------------------------------------------------------
					}
					unset( $this->node['table'] );
					return;
				}

				public function excel_generate(){
					
					$docname = $this->namefile.'-Quota-'.date("Ymd-His").'.xlsx';
					$document 	= $this->path.'/'.$this->path_module.'/tmp/xls/'.$docname;
					$route		=$this->base_url.'/'.$this->path_module.'/tmp/xls/'.$docname;

					// Redirect output to a client’s web browser (Excel5)
					/*header('Content-Type: application/vnd.ms-excel');
					header('Content-Disposition: attachment;filename='.$docname);
					header('Cache-Control: max-age=0');

					// If you're serving to IE 9, then the following may be needed
					header('Cache-Control: max-age=1');

					// If you're serving to IE over SSL, then the following may be needed
					header ('Expires: Mon, 26 Jul 1997 05:00:00 GMT'); // Date in the past
					header ('Last-Modified: '.gmdate('D, d M Y H:i:s').' GMT'); // always modified
					header ('Cache-Control: cache, must-revalidate'); // HTTP/1.1
					header ('Pragma: public'); // HTTP/1.0*/

					$objWriter = PHPExcel_IOFactory::createWriter($this->PHPEXCEL, 'Excel2007');
					$objWriter->save( $document );
					header('Location:'.  $route );
					exit;
				}



				public function excel_sheet_simple_params(){
					$heet=0;	# 0 Para iniciar en la hoja 1.
					$index=4; 	# Para iniciar en A3.

					$style = $this->PHPEXCEL->setActiveSheetIndex( $heet );

		        	$this->PHPEXCEL->setActiveSheetIndex( $heet )
		        		->setCellValue( "A$index", 'Target' )
		        		->setCellValue( "B$index", 'Population' )
		        		->setCellValue( "C$index", 'Percent' );
					$style->getStyle( "A$index:C$index" )->applyFromArray( $this->h2 );
					$index++;

					foreach ($this->node['target'] as $key => $val) {
		        		if ( $this->node['view'] == $val['code']) {
			        		$this->PHPEXCEL->setActiveSheetIndex( $heet )
			        			->setCellValue("A$index", $val['title_population'])
			        			->setCellValue("B$index", $val['population']);

			        		if (!empty($val['percent'])) {
			        			$this->PHPEXCEL->setActiveSheetIndex( $heet )->setCellValue("C$index", $val['percent']);
			        		}

							$style->getStyle( "A$index:C$index" )->applyFromArray( $this->bgYellow );
							
							$this->PHPEXCEL->getActiveSheet()->getStyle("B$index")->getNumberFormat()->setFormatCode('#,##0');
							$this->PHPEXCEL->getActiveSheet()->getStyle("C$index")->getNumberFormat()->applyFromArray(array('code' => PHPExcel_Style_NumberFormat::FORMAT_PERCENTAGE ));
							
							if ($key==0) {
								$style->getStyle( "A$index:C$index" )->applyFromArray( $this->bgGreen );
							}
							$index++;
		        		}
					}
					$this->PHPEXCEL->setActiveSheetIndex( $heet )
						->setCellValue("A$index", $this->node['sample_size']['label'])
						->setCellValue("B$index", $this->node['sample_size']['value']);
					$style->getStyle( "A$index:C$index" )->applyFromArray( $this->bgGreen );
					$index++;
					$index++;



					foreach ($this->node['target'] as $key => $val) {
						if ( $this->node['view'] == $val['code']) {
							$this->PHPEXCEL->setActiveSheetIndex( $heet )
								->setCellValue("A$index", $val['cols_params'][0])
								->setCellValue("B$index", $val['cols_params'][1])
								->setCellValue("C$index", $val['cols_params'][2])
								->setCellValue("D$index", $val['cols_params'][3])
								->setCellValue("E$index", $val['cols_params'][4]);
							$style->getStyle( "A$index:E$index" )->applyFromArray( $this->h2 );
							$index++;
							
							$rows = $this->node['groups'][$val['code']];

							# ------------------------------------------
							$arreglo = array();
							$formula = array();
							$i = $index;
							$change='';
							foreach ($rows as $id => $info) {
								if ($info['name']!=$change) {
									$change = $info['name'];
									$ini = $i;
								}
								$arreglo[$i]['text'] = 'D'.$i.'/SUM($D$'.$ini.':$D$';
								$arreglo[$i]['name'] = $info['name'];
								$i++;
							}
							$change='';
							$formula = array();
							for ($j=($i-1); $j>=$index; $j--) { 
								if ($arreglo[$j]['name']!=$change) {
									$change = $arreglo[$j]['name'];
									$fin = $j;
								}
								$formula[$j] = '='.$arreglo[$j]['text'].$fin.')';
							}
							
							
							$sumIni = $sumFin = $index;
							$aux = $rows[0]['name'];
							foreach ($rows as $id => $info) {
								if ($aux==$info['name']) {
									$sumFin++;
								}
								$this->PHPEXCEL->setActiveSheetIndex( $heet )
									->setCellValue("A$index", $info['name'])
									->setCellValue("B$index", $info['code'])
									->setCellValue("C$index", $info['value'])
									->setCellValue("D$index", $info['Population'])
									->setCellValue("E$index", "$formula[$index]");

								$style->getStyle( "A$index:E$index" )->applyFromArray( $this->bgYellow );
								$this->PHPEXCEL->getActiveSheet()->getStyle("D$index")->getNumberFormat()->setFormatCode('#,##0');
								$this->PHPEXCEL->getActiveSheet()->getStyle("E$index")->getNumberFormat()->applyFromArray(array('code' => PHPExcel_Style_NumberFormat::FORMAT_PERCENTAGE_00 ));
								$index++;
							}
							unset($arreglo, $formula);
						}
					}
			        return;
				}		

				public function excel_sheet_simple_values(){

					$heet=0;	# 0 Para iniciar en la hoja 1.
					$index=4; 	# Para iniciar en A5.

					if ( isset($this->node['table']['G']) ) {
						$style = $this->PHPEXCEL->setActiveSheetIndex( $heet );

						# Seteal los titulos.
						$this->PHPEXCEL->setActiveSheetIndex( $heet )
							->setCellValue("G$index", 'Group')
							->setCellValue("H$index", 'Code');
						$style->getStyle( "G$index:H$index" )->applyFromArray( $this->bgTitle[0] );

						$cell = 'I';
						$cellIni=$cell;
						$netxCell = 2;
						foreach ($this->node['target'] as $key => $value) {
							if ($value['code'] == $this->node['view'] ) {
								$titles = $value['cols_data'];
								foreach ($titles as $k => $val) {
									$posicition = $cell.$index;
									$this->PHPEXCEL->setActiveSheetIndex( $heet )->setCellValue( $posicition, $val );
									$cell++;
								}
								$style->getStyle( $cellIni.$index.':'.$posicition )->applyFromArray( $this->bgTitle[2] );
								$netxCell = ($netxCell+($key*4));
							}
						}
						

						$n=0;
						$cell='G';
						foreach ($this->node['table'] as $key => $val) {
							foreach ($val as $k => $v) {
								$indexCell=$key.$k;
								$this->PHPEXCEL->setActiveSheetIndex( $heet )->setCellValue($indexCell, $v);
							}
							$bg=$cell.'5:'.$cell.$k;

							if ( $cell=='I' ) {
								$this->PHPEXCEL->getActiveSheet()->getStyle( $bg )->getNumberFormat()->setFormatCode('#,##0');
							}
							if ( $cell=='J' ) {
								$this->PHPEXCEL->getActiveSheet()->getStyle( $bg )->getNumberFormat()->setFormatCode('#,##0.000000');
							}
							if ( $cell=='K' ) {
								$this->PHPEXCEL->getActiveSheet()->getStyle( $bg )->getNumberFormat()->setFormatCode('#,##0.00000');
							}
							if ( $cell=='L' ) {
								$this->PHPEXCEL->getActiveSheet()->getStyle( $bg )->getNumberFormat()->applyFromArray(array('code' => PHPExcel_Style_NumberFormat::FORMAT_NUMBER ));
							}

							if ( $n < 2 ) {
								$style->getStyle( $bg )->applyFromArray( $this->bgData[0] );
							}else{
								$style->getStyle( $bg )->applyFromArray( $this->bgData[2] );
							}
							$n++;
							$cell++;
						}


						$formula='=SUM(I5:I68)';
						$posicition = 'I'.($k+1);
						$this->PHPEXCEL->setActiveSheetIndex( $heet )->setCellValue($posicition, $formula);
						$this->PHPEXCEL->getActiveSheet()->getStyle( $posicition )->getNumberFormat()->setFormatCode('#,##0');
						$style->getStyle( $posicition )->applyFromArray( $this->bgGreen );
						

						$formula='=SUM(L5:L'.$k.')';
						$posicition = 'L'.($k+1);
						$this->PHPEXCEL->setActiveSheetIndex( $heet )->setCellValue('L'.($k+1), $formula);
						$this->PHPEXCEL->getActiveSheet()->getStyle( $posicition )->getNumberFormat()->applyFromArray(array('code' => PHPExcel_Style_NumberFormat::FORMAT_NUMBER ));
						$style->getStyle( $posicition )->applyFromArray( $this->bgGreen );
						


					}
			        return;
				}

				

			}
		}

		// Creating the new document XLS...
		$PHPEXCEL = new PHPExcel();
		$params = array(
			'PHPEXCEL' 			=> $PHPEXCEL, 
			'path' 				=> $path, 
			'base_url'			=> $base_url,
			'path_module' 		=> $path_module, 
			'path_libraries' 	=> $path_libraries, 
			'node' 				=> $data['node'],
			'namefile'			=> $data['node']['text_project_id'],
		);

		$objXLS = new EXCEL( $params );
		switch ( $data['node']['view'] ) {
			case 'all':
				$objXLS->excel_document_properties();
				$objXLS->excel_sheet_one();
				$objXLS->excel_sheet_two();
				//$objXLS->excel_sheet_tree();
				$objXLS->excel_sheet_two_params();
				$objXLS->excel_sheet_two_params_groups();
				$objXLS->excel_sheet_one_titles();
				$objXLS->excel_sheet_one_sub_titles();
				$objXLS->excel_sheet_one_data();
				$objXLS->excel_generate();
			break;
			default:
				$objXLS->excel_document_properties();
				$objXLS->excel_sheet_one();
				$objXLS->excel_sheet_simple_params();
				$objXLS->excel_sheet_simple_values();
				$objXLS->excel_generate();
			break;
		}

	}

