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
	
	function _debug_c($variable,$title,$exit=false,$print=true){
		if($print){
			echo "<pre><br>....................INICIA ".strtoupper($title)."........................<br>";
			print_r($variable);
			echo "<br>...................FIN ".strtoupper($title)."........................<br></pre>";
			if($exit){
				exit(0);
			}
		}
	}
	/**
	 * Función: principal que es llamada desde el enlace directo desde el listado de encuestas
	 *          o desde el registro directo de la Quota
	 */
	function _survey_cygnal_crosstab_file(){
		$nid=0;
		
	 	if(is_numeric(arg(4))){#Se asigna el id como el $nid directo
	 			$nid=arg(4); //Asignación directa
	 	}
	   
	 	if(is_numeric($nid) && $nid!=0){#Si se encontre el $nid del nodo de la cuota, se pasa a la construcción del archivo
	 		$node=node_load($nid); #load the node with information of the quota
	 		$datos=_survey_cygnal_crosstab_get_info_node($node);#Llena la estructura de generación del archivo de quotas con la información del nodo
	 			
	 		$quota=_survey_cygnal_crosstab_get_quota($nid);
	 		$ss=_survey_cygnal_crosstab_get_ss($node,$quota);

	 		_survey_cygnal_crosstab_build_file($datos,$ss);#Inicia la construcción del archivo de Script, este archivo sera un word
	 	}else{
	 		echo "Data Not Found ";#Si retorna esto, es porque el id enviado no es correcto, o la Quota no existe
	 		return false;
	 	}

	 	return true;	
	}


	/**
	 * Función: 
	 *         
	 */
	function _survey_cygnal_crosstab_get_quota($nid){
		$path = drupal_get_path('module', 'survey_cygnal');
	 	require_once "$path/includes/survey_cygnal.db.inc";
	 	require_once "$path/process/survey_cygnal_quota.inc";
	 	
	 	$data=array("survey_id"=>$nid);
	 	$qid=_survey_cygnal_db_get_quota_id($data);#Se obtiene el ultimo qid de las Quotas de la encuesta enviada
	 	$node_quota=node_load($qid); #load the node with information of the quota
		$info=array("text_project_id"=>"1000",
				        "view"=>"all", #all,landline,cell
				        "celda"=>"numerica", #numerica, formula
				        "export"=>"no"); #yes , no
 		$quotas=_survey_cygnal_quota_get_info_node_quota($node_quota,$info);#Llena la estructura de generación del archivo de quotas con la información del nodo
 		$data=_survey_cygnal_quota_build_file($quotas);#Inicia la construcción del archivo de Quotas, este archivo sera un excel
		#_debug_t($data,"data-after-table",false,true); #Variable, exit, print
		return $data;
	}

	function _survey_cygnal_crosstab_letters(){
		return array(
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 
			'AA', 'AB', 'AC', 'AD', 'AE', 'AF', 'AG', 'AH', 'AI', 'AJ', 'AK', 'AL', 'AM', 'AN', 'AO', 'AP', 'AQ', 'AR', 'AS', 'AT', 'AU', 'AV', 'AW', 'AX', 'AY', 'AZ', 
			'BA', 'BB', 'BC', 'BD', 'BE', 'BF', 'BG', 'BH', 'BI', 'BJ', 'BK', 'BL', 'BM', 'BN', 'BO', 'BP', 'BQ', 'BR', 'BS', 'BT', 'BU', 'BV', 'BW', 'BX', 'BY', 'BZ',
		);
	}

	/**
	 * Función: 
	 *         
	 */
	function _survey_cygnal_crosstab_get_ss($node,&$quota){
		$path = DRUPAL_ROOT;
		$path_libraries = libraries_get_path('phpexcel'); 
			
		require_once $path.'/'.$path_libraries.'/Classes/PHPExcel.php'; 
		
		$data=array();
		$col_quota="";
		$abc=_survey_cygnal_crosstab_letters(); # se obtiene un vector con las letras
		#_debug_t($key,"key",false,true); #Variable, exit, print

		$fid=$node->field_survey_ss_file[LANGUAGE_NONE][0]["fid"]; # Se obtiene el identificador del archivo de ss
		#_debug_t($fid,"fid",false,true); #Variable, exit, print
		$file=file_load($fid); # Se carga el archivo
		$archivo = drupal_realpath($file->uri);  # Se obtiene la ruta real del archivo que se va a abrir
	 	
	 	$inputFileType = PHPExcel_IOFactory::identify($archivo); # se identifica el tipo de archivo a abrir
		$objReader = PHPExcel_IOFactory::createReader($inputFileType); # se crea objeto para leer el archvio
		$objPHPExcel = $objReader->load($archivo); # se lee el archivo
		$sheet = $objPHPExcel->getSheet(0); # Se obtiene la hoja empezando desde el indice 0
		$highestRow = $sheet->getHighestRow(); # Se obtiene la fila mayor de los resultados
		$highestColumn = $sheet->getHighestColumn(); # se obtiene la maxima columna 
		$key = (array_search($highestColumn, $abc)); # se obtiene la posición de la maxima columna

		$data["rows"]=$highestRow-1; #Se almacena la cantidad de registros, menos la cabecera
		#_debug_t($highestRow,"highestRow",false,true); #Variable, exit, print
		#_debug_t($highestColumn,"highestColumn",false,true); #Variable, exit, print
		for ($row = 1; $row <= $highestRow; $row++){ # Se recorre hasta la maxima fila de resultados
			for ($col = 0; $col <= $key; $col++){ # se recorre hasta la maxima column
				$data["datos"][$row-1][$col]=$sheet->getCell($abc[$col].$row)->getValue(); # Se almacena en la matrix de resultados, se resta 1 a la fila actual, para que el array se almacene desde la posición cero				
				#Si es la primera columna, se pregunta cual de estas es la de quota
				if($row==1){
					#_debug_t($col,"col",false,true); #Variable, exit, print
					#_debug_t($data["datos"][$row-1][$col],"celda",false,true); #Variable, exit, print
					if(strtoupper($data["datos"][$row-1][$col])==strtoupper("QUOTA")){
						$data["col_quota"]=$col;
					}
				}
			}
		}
	 	
	 	_survey_cygnal_crosstab_set_sum_quotas($data);
	 	_survey_cygnal_crosstab_set_porcentaje_quotas($data);
	 	_survey_cygnal_crosstab_set_sam_quotas($data,$quota);
	 	_survey_cygnal_crosstab_set_height_ss($data,$quota);	 	
	    #_debug_t($data,"data",true,true); #Variable, exit, print
	    return $data;
	}

	function _survey_cygnal_crosstab_set_sum_quotas(&$data){
		$quotas=array();
		#Recorrer la matrix de datos, inicializa en 1, para que no tome la fila de cabecera
		#_debug_t($data["col_quota"],"col_quota",false,true); #Variable, exit, print
		for($row=1;$row<count($data["datos"]);$row++){
			$quota=$data["datos"][$row][$data["col_quota"]];
			if(isset($quotas[$quota])){
				$quotas[$quota]=$quotas[$quota]+1;
			}else{
				$quotas[$quota]=1;
			}
		}

		ksort($quotas);
		$data["quotas"]["cantidades"]=$quotas;
	}

	function _survey_cygnal_crosstab_set_porcentaje_quotas(&$data){
		$porcentajes=array();
		#Recorrer la matrix de datos, inicializa en 1, para que no tome la fila de cabecera
		
		$row=1;
		foreach ($data["quotas"]["cantidades"] as $key => $value) {
			$porcentajes[$row]=$value/$data["rows"];
			$row++;
		}
		$data["quotas"]["porcentajes"]=$porcentajes;	
	}

	function _survey_cygnal_crosstab_set_sam_quotas(&$data,&$quota){
		#_debug_t($quota["node"]["table_quota"],"table_quota",true,true); #Variable, exit, print
		#Asigna en la columna B, de la información de las quotas, representa a la solumna SAM 
		foreach ($quota["node"]["table_quota"]['B'] as $key => $value) {
			$quota["node"]["table_quota"]['B'][$key]=round(($data["quotas"]["porcentajes"][$key]*100),2);
			$quota["node"]["table_quota"]['D'][$key]=($quota["node"]["table_quota"]['A'][$key]/$quota["node"]["table_quota"]['B'][$key]);
		}

	}	

	function _survey_cygnal_crosstab_set_height_ss(&$data,&$quota){
		#_debug_t($quota["node"]["table_quota"],"table-quota",false,true); #Variable, exit, print
		#Asigna en la columna B, de la información de las quotas, representa a la solumna SAM 
		foreach ($data["datos"] as $key => $value) {
			if($key==0){
				$data["datos"][$key]['height']="HEIGHT";
			}else{
				$code=$data["datos"][$key][$data["col_quota"]];
				#_debug_t($quota,"Quota",false,true); #Variable, exit, print
				$data["datos"][$key]['height']=$quota["node"]["table_quota"]['D'][$code];	
			}			
		}

	}

	function _survey_cygnal_crosstab_excluir_columnas($col_value){
		$retorno=true;
		if(strtoupper($col_value)=="LALVOTERID"){ # no se incluye la columna inicial
			$retorno=false;
		}
		if(strtoupper($col_value)=="HEIGHT"){ # no se incluye la columna inicial
			$retorno=false;
		}
		if(strtoupper($col_value)=="QUOTA"){ # no se incluye la columna inicial
			$retorno=false;
		}
		return $retorno;
	}

	/**
	 * Función: Llena una estructura con la información de la Quota obtenida desde el nodo de la Quota enviado
	 */
	function _survey_cygnal_crosstab_get_info_node($node){
		$path = drupal_get_path('module', 'survey_cygnal');
		

		$fecha = date("Y-m-d");
		$fecha = date("l, F j, Y").' - '.date('l, F j, Y', strtotime($fecha. ' + 5 days'));

		$datos=array();
		$questions=array();
		$demographics_answers=array();
		
		$text_client="";
		if(isset($node->field_survey_consumer[LANGUAGE_NONE])){
			$id_consumer=$node->field_survey_consumer[LANGUAGE_NONE][0]["nid"];
			$node_consumer=node_load($id_consumer);
			$text_client=$node_consumer->field_consumer_name[LANGUAGE_NONE][0]["value"];
		}

		$text_status="";
		if(isset($node->field_survey_status[LANGUAGE_NONE])){
			$id_status=$node->field_survey_status[LANGUAGE_NONE][0]["tid"];
			$term_status=taxonomy_term_load($id_status);
			$text_status=strtoupper($term_status->name);
		}

		$text_project_id="";
		if(isset($node->field_survey_project_id[LANGUAGE_NONE])){
			$text_project_id=strtoupper($node->field_survey_project_id[LANGUAGE_NONE][0]["value"]);
		}

		$text_district="";
		if(isset($node->field_survey_district[LANGUAGE_NONE])){
			$text_district=$node->field_survey_district[LANGUAGE_NONE][0]["value"];
		}

		$text_survey_of="";
		if(isset($node->field_survey_survey_of[LANGUAGE_NONE])){
			$text_survey_of=$node->field_survey_survey_of[LANGUAGE_NONE][0]["value"];
		}


		$text_intro="";
		if(isset($node->field_survey_script_intro[LANGUAGE_NONE])){
			$text_intro=$node->field_survey_script_intro[LANGUAGE_NONE][0]["value"];
		}

		$text_close="";
		if(isset($node->field_survey_script_close[LANGUAGE_NONE])){
			$text_close=$node->field_survey_script_close[LANGUAGE_NONE][0]["value"];
		}

		$text_demographics_instruction="";
		if(isset($node->field_survey_demo_instructions[LANGUAGE_NONE])){
			$text_demographics_instruction=$node->field_survey_demo_instructions[LANGUAGE_NONE][0]["value"];
		}
			
		
		#Arma estructura de todo preguntas
		if(isset($node->field_survey_questions[LANGUAGE_NONE])){
			foreach ($node->field_survey_questions[LANGUAGE_NONE] as $key => $question) {
				$id_cll_question=$question["value"];
				$item_cll_question=entity_load('field_collection_item', array($id_cll_question));			

				#Arma la estructura de las preguntas
				if(isset($item_cll_question[$id_cll_question]->field_cll_survey_question[LANGUAGE_NONE])){
					$id_question=$item_cll_question[$id_cll_question]->field_cll_survey_question[LANGUAGE_NONE][0]["value"];
					$item_question=entity_load('field_collection_item', array($id_question));
					
					$question_type="";
					if(isset($item_question[$id_question]->field_cll_survey_question_type[LANGUAGE_NONE])){
						$type_question_tid=$item_question[$id_question]->field_cll_survey_question_type[LANGUAGE_NONE][0]['tid'];
						$term_type=taxonomy_term_load($type_question_tid);
						$question_type=$term_type->field_tax_type_question_code[LANGUAGE_NONE][0]["value"];
					}

					$question="";
					if(isset($item_question[$id_question]->field_cll_survey_question_script[LANGUAGE_NONE])){
						$question=$item_question[$id_question]->field_cll_survey_question_script[LANGUAGE_NONE][0]['value'];
					}	

					$question_head=array();
					if(isset($item_question[$id_question]->field_cll_survey_question_txt_he[LANGUAGE_NONE])){
						foreach ($item_question[$id_question]->field_cll_survey_question_txt_he[LANGUAGE_NONE] as $key_desc => $value_desc) {
							$question_head[]=$value_desc["value"];
						}
					}

					$question_txt_footer=array();
					if(isset($item_question[$id_question]->field_cll_survey_question_txt_fo[LANGUAGE_NONE])){
						foreach ($item_question[$id_question]->field_cll_survey_question_txt_fo[LANGUAGE_NONE] as $key_desc => $value_desc) {
							$question_txt_footer[]=$value_desc["value"];
						}
					}

					$answer_head=array();
					if(isset($item_question[$id_question]->field_cll_survey_question_txt_op[LANGUAGE_NONE])){
						foreach ($item_question[$id_question]->field_cll_survey_question_txt_op[LANGUAGE_NONE] as $key_desc => $value_desc) {
							$answer_head[]=$value_desc["value"];
						}
					}						

					#Arma la estructura de las opciones de respuesta de la pregunta	
					$answers=array();
					if(isset($item_question[$id_question]->field_cll_survey_question_option[LANGUAGE_NONE])){

						$id_option=$item_question[$id_question]->field_cll_survey_question_option[LANGUAGE_NONE][0]["value"];	
						$item_option=entity_load('field_collection_item', array($id_option));									
						
						$answer_text="";
						if(isset($item_option[$id_option]->field_cll_survey_quest_opt_txt[LANGUAGE_NONE])){
							$answer_text=$item_option[$id_option]->field_cll_survey_quest_opt_txt[LANGUAGE_NONE][0]["value"];
						}
						$answer_text_vec = explode("\r\n", $answer_text);

						$answer_inst="";
						if(isset($item_option[$id_option]->field_cll_survey_quest_opt_inst[LANGUAGE_NONE])){
							$answer_inst=$item_option[$id_option]->field_cll_survey_quest_opt_inst[LANGUAGE_NONE][0]["value"];
						}
						$answer_inst_vec = explode("\r\n", $answer_inst);
						
						/*echo "<pre>";
						echo "<br>---------<br>";
						print_r($answer_text_vec);
						echo "<br>---------<br>";
						print_r($answer_inst_vec);
						echo "<br>---------<br>";
						echo "</pre>";*/

						foreach($answer_text_vec as $key=>$value){
							$answers[]=array('answer_text'=>$answer_text_vec[$key],
								             'answer_info'=>isset($answer_inst_vec[$key])?$answer_inst_vec[$key]:'');
						}
						

					}


					$questions[]=array('question_type' 			=> $question_type,	
									 'question_head' 			=> $question_head,
									 'question' 				=> $question,
									 'answer_head' 				=> $answer_head,
									 'answers' 					=> $answers,
									 'question_footer'  		=> $question_txt_footer,
									); 
				}

			}
		}

		#Arma estructura de todo demographics
		if(isset($node->field_survey_demographics[LANGUAGE_NONE])){
			foreach ($node->field_survey_demographics[LANGUAGE_NONE] as $key_add => $demographic) {
				$id_cll_demographics=$demographic["value"];
				$item_cll_demographics=entity_load('field_collection_item', array($id_cll_demographics));

				#Arma la estructura de las demographics
				if(isset($item_cll_demographics[$id_cll_demographics]->field_survey_question_control[LANGUAGE_NONE])){
					$id_demographics=$item_cll_demographics[$id_cll_demographics]->field_survey_question_control[LANGUAGE_NONE][0]["value"];
					$item_demographics=entity_load('field_collection_item', array($id_demographics));
										
					$demographics_label="";
					if(isset($item_demographics[$id_demographics]->field_cll_survey_quest_label[LANGUAGE_NONE])){
						$demographics_label=$item_demographics[$id_demographics]->field_cll_survey_quest_label[LANGUAGE_NONE][0]['value'];
					}	

					
					#Arma la estructura de las opciones de respuesta de la demographics	
					$demographics_answers=array();
	
					$answer_demographics_text="";
					if(isset($item_demographics[$id_demographics]->field_cll_survey_quest_opt_scr[LANGUAGE_NONE])){
						$answer_demographics_text=$item_demographics[$id_demographics]->field_cll_survey_quest_opt_scr[LANGUAGE_NONE][0]["value"];
					}
					$answer_demographics_text_vec = explode("\r\n", $answer_demographics_text);

					$answer_demographics_inst="";
					if(isset($item_demographics[$id_demographics]->field_cll_survey_quest_option_in[LANGUAGE_NONE])){
						$answer_demographics_inst=$item_demographics[$id_demographics]->field_cll_survey_quest_option_in[LANGUAGE_NONE][0]["value"];
					}
					$answer_demographics_inst_vec = explode("\r\n", $answer_demographics_inst);
					
					foreach($answer_demographics_text_vec as $key=>$value){
							$demographics_answers[]=array('answer_text'=>$answer_demographics_text_vec[$key],
								             			  'answer_info'=>isset($answer_demographics_inst_vec[$key])?$answer_demographics_inst_vec[$key]:'');
						}

					$demographics[]=array('demographics'	=> $demographics_label,
										  'answers'			=> $demographics_answers);	

				}
				
			}	
		}
		
		$datos['node']['logo_cygnal'] 				= $path."/theme/img/logo_cygnal_transparent.png";
		$datos['node']['logo_custamer'] 			= $path."/theme/img/logos/logo_empresa.png";
		$datos['node']['text_header'] 				= "Cygnal Pulse Survey Platform";
		$datos['node']['show_text_header'] 			= 1; // 0:No  1:Si
		$datos['node']['text_district'] 			= $text_district;
		$datos['node']['text_survey_of'] 			= $text_survey_of;
		$datos['node']['text_client'] 				= $text_client;
		$datos['node']['text_status'] 				= "- ".$text_status." -";
		$datos['node']['text_project_id'] 			= $text_project_id;
		$datos['node']['text_title'] 				= $node->title;
		$datos['node']['text_date'] 				= $fecha;		
		$datos['node']['text_intro']				= $text_intro;		
		$datos['node']['questions']					= $questions;
		$datos['node']['text_close']				= $text_close;		
		$datos['node']['demographics']				= $demographics;
		$datos['node']['demographics_instruction']	=$text_demographics_instruction;		
		
		/*echo "<pre>";
		echo "<br>---------<br>";
		print_r($datos);		
		echo "<br>---------<br>";
		echo "</pre>";
		exit(0);*/

		return $datos;	
	}

	function _survey_cygnal_crosstab_set_sum_height($datos,$question,$answer){
		
		#Asigna en la columna B, de la información de las quotas, representa a la solumna SAM 
		$sum=0;
		foreach ($datos as $row => $row_value) {			
			#_debug_t($row_value,"row_value",false,true); #Variable, exit, print
			if($row_value[$question]==$answer){
				$sum=$sum + $row_value['height'];
			}
			
			/*foreach ($row_value as $col => $col_value) {
				#_debug_t($row,"row",false,true); #Variable, exit, print
				if($row==0){
					#_debug_t($col,"col",false,true); #Variable, exit, print
					if(_survey_cygnal_toplines_excluir_columnas($col_value)){ # valida que se incluyan solo las columnas de preguntas y demographics
						$sum[$question]["key"]=$question;
						//$sum[$question]["value"]=$col_value;
						$sum[$question]["answers"]=array();
					}
				}else{
					#_debug_t($data["datos"][0][$col],"col",false,true); #Variable, exit, print
					if(_survey_cygnal_toplines_excluir_columnas($datos[0][$col])){ # valida que se incluyan solo las columnas de preguntas y demographics
						
						if(isset($sum[$question]["answers"][$col_value])){
							$sum[$question]["answers"][$col_value]=$sum[$col]["answers"][$col_value] + $row_value['height'];
						}else{
							$sum[$question]["answers"][$col_value]=$row_value['height'];
						}
					}	
				}
			}*/



		}		

		#_debug_t($question,"question",true,true); #Variable, exit, print	
		return $sum;
	}


	function _survey_cygnal_crosstab_filter_question($datos,$question,$answer){
		$filter=array();
		$i=0;
		foreach ($datos as $key => $value) {
			#_debug_t($value,"value",false,true); #Variable, exit, print
			if($key>0){
				if($value[$question]==$answer){
					$filter[$i]=$value;
					$i++;
				}
			}
		}
		#_debug_t($filter,"filter",false,true); #Variable, exit, print
		return $filter;
	}


	function _survey_cygnal_crosstab_build_crosstab($data,$ss){
		$crosstab=array();
		foreach ($data["node"]["questions"] as $key => $value) {
			$q="Q".($key+1);
			//if($key==0){
				$crosstab[$q]['question']=$value['question'];
				foreach ($value["answers"] as $key2 => $value2) {
					$filter=_survey_cygnal_crosstab_filter_question($ss["datos"],$key+1,$key2+1); // Las opciones de pregunta inician en la opcion cero, pero los resultados vienen desde la opcion 1, entonces se incrementa en 1
					#_debug_t($filter,"filter",true,true); #Variable, exit, print
					//Funcion que debe obtener la suma de los height de una pregunta, y para una opcion de respuesta. 
					$sum=_survey_cygnal_crosstab_set_sum_height($ss["datos"],$key+1,$key2+1);
					#_debug_t($key2,"key2",false,true); #Variable, exit, print
					#En el primer registro del array se define, el titulo y su valor
					$crosstab[$q]['rows'][0]='Total';		
					$crosstab[$q]['total'][0]=0;

					$crosstab[$q]['rows'][$key2+1]=$value2["answer_text"];					
					$crosstab[$q]['total'][$key2+1]=$sum;

					foreach ($data["node"]["questions"] as $key3 => $value3) {					
						//if($key3==0){
							$subtotal=array();
							$total_general=array();
							foreach ($value3["answers"] as $key4 => $value4) {
								$crosstab[$q]['groups'][$key3]['name']=$value3['question'];
								$crosstab[$q]['groups'][$key3]['vars'][$key4+1]=$value4['answer_text'];

								$sum_general=_survey_cygnal_crosstab_set_sum_height($ss["datos"],$key3+1,$key4+1);
								$sum=_survey_cygnal_crosstab_set_sum_height($filter,$key3+1,$key4+1);
																						
								$subtotal[]=$sum;
								$total_general[]=$sum_general;
							} 					
							
							#_debug_c($total,"total",false,true); #Variable, exit, print
							$crosstab[$q]['groups'][$key3]['data_vars'][0]=$total_general;
							$crosstab[$q]['groups'][$key3]['data_vars'][$key2+1]=$subtotal;
						//}
					}		

				}
			//}	
		}

		foreach ($crosstab as $key => $value) {
			$totalq=0;

			foreach ($value["total"] as $key0 => $value0) {
				if($key0>0){
					$totalq=$totalq+$value0;
					#$totalq=$totalq+$value0;
				}
			}
			$crosstab[$key]['total'][0]=round($totalq);

			foreach ($value["groups"] as $key2 => $value2) {						
				foreach ($value2["data_vars"] as $key3 => $value3) {
					foreach ($value3 as $key4 => $value4) {
						#_debug_c($value4,"value4",false,true); #Variable, exit, print
						$total="";
						#_debug_c($key3,"key3",false,true); #Variable, exit, print																
						$percent="";
						#_debug_c($value2["data_vars"][0][$key4],"aaaaa",false,true); #Variable, exit, print							
						if($value2["data_vars"][0][$key4]!=0){
							$percent=$value4/$value2["data_vars"][0][$key4];								
						}	
						$total=(round($value4))." - ".round(($percent*100),1)."%";
						#_debug_c($total,"total",true,true); #Variable, exit, print
						if($key3!=0){
							$crosstab[$key]['groups'][$key2]['data_vars'][$key3][$key4]=$total;
						}
						$crosstab[$key]['groups'][$key2]['data_vars'][0][$key4]=round($crosstab[$key]['groups'][$key2]['data_vars'][0][$key4]);
						
					}
				}
			}


		}


		#_debug_c($crosstab,"crosstab",true,true); #Variable, exit, print
		return $crosstab;
	}

	function _survey_cygnal_crosstab_build_file($datos,$ss){
		#_debug_c($datos,"datos",false,true); #Variable, exit, print
		// Los logos a escala ratio is: (16 : 9)
		$path = DRUPAL_ROOT;
		$path_module = drupal_get_path('module', 'survey_cygnal');
		#require_once $path_module."/includes/survey_cygnal.excel_crosstabs.ecxel.inc";
		$data = $datos;
		#_debug_c($data,"data",true,true); #Variable, exit, print
		$data['node']['logo_cygnal'] 	= 	$path_module."/theme/img/logo-excel.png";
		$data['node']['logo_custamer'] 	= 	$path_module."/theme/img/logos/logo_empresa.png";
		$data['node']['text_header'] 	= 	"Cygnal Pulse Survey Platform";
		$data['node']['text_title'] 	= 	$data["node"]["text_title"];
		$data['node']['text_project_id']=	$data["node"]["text_project_id"];

		$data['node']['crosstab']=_survey_cygnal_crosstab_build_crosstab($data,$ss);
		#_debug_c($data,"data",true,true); #Variable, exit, print

		// Pregunta numero N.
		/*$data['node']['crosstab']['Q1'] = array(
			// Enunciado de la pregunta.
			'question' => "How likely are you to vote in the March 2018 Republican Primary Election for US Senate, Congress, governor, legislature, and local offices?",
			// Columna izquierda "A", items mostrados por cada fila en el excel.
			'rows' => array( 
				'Total', 
				'Definitely voting', 
				'Probably voting', 
				'Not sure' 
			),
			// Columna izquierda "B", valores totales correspondientes a los items.
			'total' => array( 
				'304', 
				'240 - 79.1%', 
				'52 - 17.1%', 
				'12 - 3.9%' 
			),
			// Los grupos asignados a una pregunta.
			'groups' => array(
				'0' => array(
					// Nombre del grupo.
					'name' => 'Likelihood to vote', 
					// Variables de grupo.
					'vars' => array('Definitely voting', 'Probably voting', 'Not sure'),
					// Valores de las variables en cada grupo.
					'data_vars' => array(					
						array( '240', '52', '12' ),
						array( '240 - 100.0%', '', '' ),
						array( '', '52 - 100.0%', '' ),
						array( '', '', '12 - 100.0%' ),
					)
				),
				'1' => array(
					'name' => 'Gender', 
					'vars' => array( 'Female', 'Male' ),
					'data_vars' => array(
						array( '149', '155' ),
						array( '116 - 77.5%', '125 - 80.5%' ),
						array( '27 - 18.2%', '25 - 16.0%' ),
						array( '6 - 4.3%', '5 - 3.4%' ),
					)
				),
				'2' => array(
					'name' => 'What age range do you fall within?', 
					'vars' => array( '18-39', '40-54', '55-69', '70+' ),
					'data_vars' => array(
						array( '28', '77', '113', '85' ),
						array( '125 - 80.5% ', '21 - 74.1%', '57 - 74.3%', '91 - 80.3%' ),
						array( '25 -16.0%', '5 -19.2%', '17 -21.5%', '17 -14.6%' ),
						array( '2 - 6.7%', '3 - 4.2%', '6 - 5.1%', '1 - 1.1%' ),
					)
				),
				'3' => array(
					'name' => 'Ethnicity', 
					'vars' => array( 'White', 'Black', 'Hispanic', 'Other' ),
					'data_vars' => array(
						array( '248', '1', '30', '26' ),
						array( '198 -79.8%', '1 -100.0%', '21 -71.6%', '20 -79.1%' ),
						array( '39 - 15.8%', '', '8 - 28.4%', '4 - 16.9%' ),
						array( '11 - 4.3%', '', '', '1 - 4.0% ' ),
					)
				),
				'4' => array(
					'name' => 'Estimated Income', 
					'vars' => array( '<$25k-$49k', '$50k-$100k', '$100k' ),
					'data_vars' => array(
						array( '38', '150', '116' ),
						array( '33 -87.1% ', '118 -78.9%', '89 -76.7% ' ),
						array( '4 -9.6% ', '25 -17.0%', '23 -19.6%' ),
						array( '1 - 3.3%', '6 - 4.2%', '4 - 3.7%' ),
					)
				),
				'5' => array(
					'name' => 'Primary Voter Propensity', 
					'vars' => array('High', 'Medium', 'Low',),
					'data_vars' => array(
						array('130', '140', '34' ),
						array('116 - 89.1%', '102 - 72.7%', '23 - 66.9%' ),
						array('12 - 9.2%', '32 - 22.9%', '8 - 22.8%' ),
						array('2 1.7%', '6 4.4%', '4 10.3%' ),
					)
				),
				'6' => array(
					'name' => 'Political Party', 
					'vars' => array('R', 'D', 'I'),
					'data_vars' => array(
						array('292', '12', '0'),
						array('230 - 79.0%', '10 - 81.3%', ''),
						array('51 - 17.4%', '1 - 8.6%', ''),
						array('11 -3.6%', '1 - 10.1%', ''),
					)
				),
				'7' => array(
					'name' => 'College Attendance', 
					'vars' => array('College', 'No College'),
					'data_vars' => array(
						array('195', '109'),
						array('160 -81.9%', '81 - 74.0%'),
						array('31 -15.8%', '21 - 19.3%'),
						array('4 - 2.3%', '7 - 6.8%'),
					)
				),
				'8' => array(
					'name' => 'Education', 
					'vars' => array('White Female College', 'White Female No college', 'White Male College', 'White Male No College', 'Other Female College', 'Other Female No College', 'Other Male College', 'Other Male No College'),
					'data_vars' => array(
						array('79', '39', '84', '46', '18', '13', '14', '10'),
						array('64 -81.2%', '29 - 74.4%', '70 - 83.5%', '35 -75.5%', '13 -74.9%', '9 -68.3%', '12 -84.9%', '8 -73.0%'),
						array('12 -14.8%', '8 - 19.8%', '13 - 15.0%', '7 - 15.7%', '4 -25.1%', '3 - 24.0%', '2 - 15.1%', '3 -27.0%'),
						array('3 - 4.0%', '2 - 5.8%', '1 - 1.5%', '4 - 8.8%', '', '1 - 7.7%', '', ''),
					)
				),
				'9' => array(
					'name' => 'Phone Type', 
					'vars' => array('Landline', 'Cellphone'),
					'data_vars' => array(
						array('211', '93'),
						array('168 - 79.5%', '72 -78.1%'),
						array('33 - 15.5%', '19 -20.6%'),
						array('11 - 5.0%', '1 - 1.3%'),
					)
				),
				'10' => array(
					'name' => 'Geo', 
					'vars' => array('Landline', 'Cellphone'),
					'data_vars' => array(
						array('146', '158'),
						array('116 - 79.6%', '124 - 78.5%'),
						array('25 - 16.9%', '27 - 17.3%'),
						array('5 - 3.5%', '7 - 4.2%'),
					)
				),
				'11' => array(
					'name' => 'County Commissioner District', 
					'vars' => array('Williamson Cnty Comm Dist 1', 'Williamson Cnty Comm Dist 3', 'Williamson Cnty Comm Dist 4'),
					'data_vars' => array(
						array('89', '52', '163'),
						array('72 - 81.2%', '41 - 78.3%', '127 - 78.1%'),
						array('13 - 15.0%', '7 - 13.3%', '32 - 19.4%'),
						array('3 - 3.8%', '4 - 8.3%', '4 - 2.5%'),
					)
				),
				'12' => array(
					'name' => 'Age/Gender Combos', 
					'vars' => array('F 18-39', 'F 40-54', 'F 55-69', 'F 70+', 'M 18-39', 'M 40-54', 'M 55-69', 'M 70+'),
					'data_vars' => array(
						array('15', '36', '56', '42', '13', '41', '58', '43'),
						array('11 - 76.5%', '27 - 74.2%', '43 - 77.8%', '34 - 80.3%', '9 - 71.4%', '30 - 74.4%', '48 - 82.8%', '37 - 86.2% '),
						array('4 - 23.5%', '7 - 19.5%', '9 - 16.4%', '7 - 17.5%', '2 - 14.3%', '9 - 23.3%', '7 - 12.9%', '6 - 13.8%'),
						array('2 - 6.3%', '3 - 5.9%', '1 - 2.2%', '2 - 14.3%', '1 - 2.3%', '2 - 4.3%', '', ''),
					)
				),
				'13' => array(
					'name' => 'Donald Trump', 
					'vars' => array('Very favorable', 'Some favorable', 'Some unfav', 'Very unfav', 'Never heard of', 'Unsure'),
					'data_vars' => array(
						array('169', '42', '17', '53', '14', '10'), 
						array('147 - 87.5%', '30 - 70.9%', '8 - 46.3%', '40 - 74.9%', '9 - 63.8%', '7 - 69.5%'),
						array('18 - 10.6%', '12 - 29.1%', '8 - 48.1%', '10 - 18.5%', '2 -13.5%', '2 -20.2%'),
						array('3 - 1.9%', '', '1 - 5.6%', '3 - 6.6%', '3 - 22.7%', '1 - 10.3%'),
					)
				),
				'14' => array(
					'name' => 'Christopher Ward', 
					'vars' => array('Very favorable', 'Some favorable', 'Some unfav', 'Very unfav', 'Never heard of', 'Unsure'),
					'data_vars' => array(
						array('21', '41', '14', '9', '177', '42'),
						array('19 94.0%', '28 - 67.7%', '12 - 82.4%', '9 - 100.0%', '143 - 80.9%', '29 - 69.1%'),
						array('1 - 6.0%', '11 - 27.6%', '2 - 17.6%', '-', '28 - 15.5%', '9 - 22.4%'),
						array('', '2 - 4.6% ', '', '', '6 - 3.6%', '4 - 8.4%'),
					)
				),
				'15' => array(
					'name' => 'Cynthia Flores', 
					'vars' => array('Very fav', 'Some fav', 'Some unfav', 'Very unfav', 'Never heard of', 'Unsure'),
					'data_vars' => array(
						array('49', '47', '29', '14', '118', '47'),
						array('45 - 91.6%', '32 - 67.5%', '22 - 77.5%', '14 - 100.0%', '97 - 81.8%', '31 - 65.2%'),
						array('4 - 8.4%', '14 - 29.8%', '5 - 15.8%', '', '15 - 12.8%', '14 - 29.9%'),
						array('', '1 - 2.6% ', '2 - 6.6%', '', '6 - 5.4%', '2 - 4.9%'),
					)
				),
				'16' => array(
					'name' => 'Jeremy Story', 
					'vars' => array('Very fav', 'Some fav', 'Some unfav', 'Very unfav', 'Never heard of', 'Unsure'),
					'data_vars' => array(
						array('28', '32', '18', '6', '180', '40'),
						array('24 - 86.8%', '22 - 68.1%', '13 - 70.2%', '5 - 81.5%', '147 - 81.5%', '30 - 75.2%'),
						array('4 - 13.2% ', '8 - 26.0%', '5 - 29.8%', '1 - 18.5%', '27 - 15.0%', '6 - 15.9%'),
						array('', '2 - 5.9% ', '', '', '6 - 3.5%', '4 - 9.0%'),
					)
				), 
				'17' => array(
					'name' => 'If you were voting today in the March 2018 Republican Primary Election for state representative, who would you vote for?', 
					'vars' => array('Definitely Christopher Ward', 'Probably Christopher Ward', 'Definitely Jeremy Story', 'Probably Jeremy Story', 'Definitely Cynthia Flores', 'Probably Cynthia Flores', 'Unsure'),
					'data_vars' => array(
						array('14', '17', '15', '10', '46', '29', '173'),
						array('13 - 92.5%', '14 - 81.6%', '14 - 93.0%', '6 - 54.2%', '40 - 85.4% ', '18 - 62.9%', '137 - 79.0%'),
						array('', '3 - 18.4%', '1 - 7.0%', '4 - 36.7%', '5 - 10.5%', '11 - 37.1%', '28 - 16.4%'),
						array('1 - 7.5%', '', '', '1 - 9.1%', '2 - 4.1%', '', '8 - 4.6% '),
					)
				), 
				'18' => array(
					'name' => 'Which of the following issues is most important to you? Please pick one.', 
					'vars' => array('Jobs and the Economy', 'Border Security', 'Property Tax Reform', 'Taxes and Spending', 'Public Education', 'Healthcare', 'Crime/Public Safety', 'Social Issues', 'Other', 'Unsure'),
					'data_vars' => array(
						array('35', '81', '36', '32', '31', '36', '20', '9', '7', '16'),
						array('25 -72.1%', '69 - 85.3%', '31 - 85.3%', '27 - 83.9%', '24 - 76.5%', '27 - 75.8%', '13 - 63.7%', '7 - 79.5%', '6 - 87.1%', '11 - 66.4%'),
						array('10 - 27.9%', '11 - 13.5%', '3 - 9.2%', '3 - 10.0%', '6 - 19.6%', '9 - 24.2%', '6 - 30.3%', '1 - 6.8%', '', '3 - 19.1%'),
						array('', '1 - 1.2%', '2 - 5.5%', '2 - 6.1%', '1 - 3.9%', '', '1 - 6.1%', '1 - 13.7%', '1 - 12.9%', '2 - 14.5%'),
					)
				), 	 
				'19' => array(
					'name' => 'How would you describe your political views?', 
					'vars' => array('Very conserv', 'Some conserv', 'Moderate', 'Some liberal', 'Very liberal', 'Unsure'),
					'data_vars' => array(
						array('100', '109', '57', '12', '15', '11'),
						array('91 - 90.8%', '83 - 76.3%', '45 - 77.8%', '8 - 70.3%', '8 - 53.0%', '5 - 49.8%'),
						array('7 - 7.3%', '24 - 21.7%', '10 - 18.2%', '4 - 29.7%', '4 - 23.9%', '3 - 31.2% '),
						array('2 - 1.8%', '2 - 2.0% ', '2 - 4.0%', '', '3 - 23.1%', '2 - 19.0%'),
					)
				)
			)
		);
		$data['node']['crosstab']['Q2'] = array(
			'question' => "Donald Trump",
			'rows' => array( 
				'Total', 
				'Favorable', 
				'Very favorable', 
				'Some favorable', 
				'Unfav', 
				'Some unfav', 
				'Very unfav', 
				'Never heard of', 
				'Unsure'
			),
			'total' => array( 
				'304', 
				'210 - 69.2%', 
				'169 - 55.5%', 
				'42 - 13.7%', 
				'70 - 23.0%', 
				'17 - 5.5%', 
				'53 - 17.4%', 
				'14- 4.5%', 
				'10 - 3.4%'
			),
			'groups' => array(
				'0' => array(
					'name' => 'Likelihood to vote',
					'vars' => array('Definitely voting', 'Probably voting', 'Not sure'),
					'data_vars' => array(
						array( '240', '52', '12'),
						array( '177 - 73.6% ', '30 - 57.9%', '3 - 27.3%'),
						array( '147 - 61.3%', '18 - 34.6%', '3 - 27.3%'),
						array( '30 - 12.3%', '12 - 23.4%', '-'),
						array( '47 - 19.8%', '18 - 34.5%', '4 - 37.5%'),
						array( '8 - 3.2% ', '8 - 15.6%', '1 - 7.9%'),
						array( '40 - 16.5%', '10 - 18.9%', '3 - 29.6%'),
						array( '9 - 3.6%', '2 - 3.5%', '3 - 26.3%'),
						array( '7 - 3.0%', '2 - 4.0%', '1 - 9.0%'),
					)
				),
				'1' => array(
					'name' => 'Gender',
					'vars' => array('Female', 'Male'),
					'data_vars' => array(
						array( '149', '155'),
						array( '103 - 68.9%', '108 - 69.4%'),
						array( '87 - 58.2%', '82 - 52.8%'),
						array( '16 - 10.7% ', '26 - 16.6% '),
						array( '36 - 24.3%', '34 - 21.7%'),
						array( '10 - 6.7%', '7 - 4.4% '),
						array( '26 - 17.7%', '27 - 17.2%'),
						array( '5 - 3.7%', '8 - 5.3%'),
						array( '5 - 3.1%', '6 - 3.7% '),
					)
				),
				'2' => array(
					'name' => 'What age range do you fall within?',
					'vars' => array('18-39', '40-54', '55-69', '70+'),
					'data_vars' => array(
						array( '28', '77', '113', '85'),
						array( '15 - 53.4% ', '52 - 68.2%', '79 - 69.8%', '64 - 74.4% '),
						array( '12 - 42.2%', '36 - 47.3%', '61 - 54.2%', '59 - 68.8%'),
						array( '3 - 11.1% ', '16 - 20.8%', '18 - 15.6%', '5 - 5.6%'),
						array( '11 - 38.9%', '20 - 25.5%', '23 - 20.2%', '16 - 19.0%'),
						array( '3 - 9.9% ', '5 - 6.8%', '3 - 2.9%', '5 - 6.3% '),
						array( '8 - 29.0%', '14 - 18.7%', '20 - 17.3%', '11 - 12.7%'),
						array( '-', '3 - 3.7%', '8 - 7.0%', '3 - 3.3%'),
						array( '2 - 7.7%', '2 - 2.6% ', '3 - 3.0%', '3 - 3.2%'),
					)
				),
				'3' => array(
					'name' => 'Ethnicity',
					'vars' => array('White', 'Black', 'Hispanic', 'Other'),
					'data_vars' => array(
						array( '248', '1', '30', '26'),
						array( '172 - 69.4%', '-', '18 - 61.8%', '20 - 78.1%'),
						array( '139 - 56.0%', '-', '15 - 50.3%', '15 - 58.0%'),
						array( '33 - 13.4%', '-', '3 - 11.5%', '5 - 20.1%'),
						array( '56 - 22.6%', '1 - 100.0%', '10 - 35.1%', '2 - 8.8%'),
						array( '16 - 6.3% ', '-', '- ', '1 - 4.7%'),
						array( '41 - 16.4%', '1 - 100.0%', '10 - 35.1%', '1 - 4.0%'),
						array( '11 - 4.3%', '-', '1 - 3.1%', '2 - 8.3%'),
						array( '9 - 3.7%', '-', '-', '1 - 4.8%'),
					)
				),
				'4' => array(
					'name' => 'Estimated Income',
					'vars' => array('<$25k-$49k', '$50k-$100k', '$100k'),
					'data_vars' => array(
						array( '38', '150', '116'),
						array( '27 - 70.8%', '104 - 69.1%', '80 - 68.7%'),
						array( '26 - 68.3%', '86 - 56.9%', '57 - 49.3%'),
						array( '1 - 2.5%', '18 - 12.2%', '22 - 19.4%'),
						array( '7 - 18.7%', '35 - 23.5%', '28 - 23.7%'),
						array( '1 - 2.5%', '9 - 6.2%', '7 - 5.7% '),
						array( '6 - 16.2%', '26 - 17.3%', '21 - 18.0%'),
						array( '2 - 4.9%', '6 - 4.1%', '6 - 4.8%'),
						array( '2 - 5.7%', '5 - 3.3%', '3 - 2.8%'),
					)
				),
				'5' => array(
					'name' => 'Primary Voter Propensity',
					'vars' => array('High', 'Medium', 'Low'),
					'data_vars' => array(
						array( '130', '140 ', '34'),
						array( '91 - 70.3%', '95 - 68.2%', '23 - 69.0%'),
						array( '81 - 62.5%', '67 - 47.8%', '20 - 60.0%'),
						array( '10 - 7.8%', '29 - 20.4%', '3 - 9.0%'),
						array( '25 - 19.3%', '36 - 26.0%', '8 - 24.6%'),
						array( '6 - 4.8%', '9 - 6.6%', '1 - 3.6%'),
						array( '19 - 14.4%', '27 - 19.4%', '7 - 21.0%'),
						array( '9 - 6.6%', '3 - 2.0%', '2 - 6.4%'),
						array( '5 - 3.9%', '5 - 3.8%', '-'),
					)
				),
				'6' => array(
					'name' => 'Political Party',
					'vars' => array('R', 'D', 'I'),
					'data_vars' => array(
						array( '292', '12 ', '0'),
						array( '205 - 70.4%', '5 - 40.9%', '-'),
						array( '163 - 56.1%', '5 - 40.9%', '-'),
						array( '42 - 14.3%', '-', '-'),
						array( '62 - 21.4%', '7 - 59.1%', '-'),
						array( '16 - 5.4%', '1 - 8.6%', '-'),
						array( '47 - 16.0%', '6 - 50.5%', '-'),
						array( '14 - 4.7%', '-', '-'),
						array( '10 - 3.5%', '-', '-'),
					)
				),
				'7' => array(
					'name' => 'College Attendance',
					'vars' => array('College', 'No College'),
					'data_vars' => array(
						array( '195', '109'),
						array( '134 - 68.8% ', '76 - 69.9%'),
						array( '104 - 53.4%', '64 - 59.2%'),
						array( '30 - 15.4%', '12 - 10.7%'),
						array( '45 - 22.9%', '25 - 23.1%'),
						array( '7 - 3.8%', '9 - 8.6% '),
						array( '37 - 19.1%', '16 - 14.5%'),
						array( '8 - 4.3% ', '5 - 4.8%'),
						array( '8 - 4.1%', '2 - 2.2%'),
					)
				),
				'8' => array(
					'name' => 'Education',
					'vars' => array('White Female College', 'White Female No college', 'White Male College', 'White Male No College', 'Other Female College', 'Other Female No College', 'Other Male College', 'Other Male No College'),
					'data_vars' => array(
						array( '79', '39', '84', '46', '18', '13', '14', '10'),
						array( '53 - 66.8% ', '31 - 79.6%', '58 - 69.3%', '30 - 65.3%', '12 - 66.9%', '7 - 52.4%', '11 - 77.9%', '8 - 76.3%'),
						array( '45 - 56.6%', '27 - 68.4%', '44 - 52.2%', '24 - 51.6%', '9 - 47.5%', '7 - 52.4%', '7 - 49.6%', '7 - 67.1%'),
						array( '8 - 10.2%', '4 - 11.2%', '14 - 17.2%', '6 - 13.6%', '3 - 19.4%', '-', '4 - 28.3%', '1 - 9.2%'),
						array( '22 - 27.6%', '6 - 14.3%', '16 - 19.3%', '13 - 27.3%', '3 - 19.5%', '5 - 40.7%', '3 - 22.1%', '2 - 14.9%'),
						array( '4 - 5.5%', '4 - 11.2%', '2 - 2.3%', '5 - 10.8%', '1 - 6.8%', '- -', '- -', '- - '),
						array( '17 - 22.1%', '1 - 3.1%', '14 - 17.1%', '8 - 16.5%', '2 - 12.7%', '5 - 40.7%', '3 - 22.1%', '2 - 14.9%'),
						array( '2 -2.7%', '1 - 3.1%', '5 - 6.1%', '2 - 4.7%', '1 - 6.8%', '1 - 6.9%', '- -', '1 - 8.9%'),
						array( '2 - 2.9%', '1 - 3.0%', '4 - 5.3%', '1 - 2.7%', '1 - 6.9%', '- -', '- -', '- -'),
					)
				),
				'9' => array(
					'name' => 'Phone Type',
					'vars' => array('Landline', 'Cellphone'),
					'data_vars' => array(
						array( '211', '93'),
						array( '149 - 70.6%', '61 - 65.9%'),
						array( '123 - 58.3%', '45 - 48.9%'),
						array( '26 - 12.2%', '16 - 17.1%'),
						array( '45 - 21.2%', '25 - 27.1%'),
						array( '11 - 5.4%', '5 - 5.7%'),
						array( '33 - 15.7%', '20 - 21.4%'),
						array( '14 - 6.5%', '-'),
						array( '4 - 1.8%', '6 - 7.0%'),
					)
				),
				'10' => array(
					'name' => 'Geo',
					'vars' => array('Northwest', 'Southeast'),
					'data_vars' => array(
						array( '146', '158'),
						array( '110 - 75.5%', '100 - 63.3%'),
						array( '91 - 62.4%', '77 - 49.0%'),
						array( '19 - 13.0%', '23 - 14.3%'),
						array( '26 - 17.9%', '44 - 27.7%'),
						array( '5 - 3.4%', '12 - 7.5% '),
						array( '21 - 14.5%', '32 - 20.2%'),
						array( '7 - 4.5% ', '7 - 4.5%'),
						array( '3 - 2.2%', '7 - 4.5%'),
					)
				),
				'11' => array(
					'name' => 'County Commissioner District',
					'vars' => array('Williamson Cnty Comm Dist 1', 'Williamson Cnty Comm Dist 3', 'Williamson Cnty Comm Dist 4'),
					'data_vars' => array(
						array( '89', '52', '163'),
						array( '56 - 63.3%', '38 - 72.4%', '116 - 71.3%'),
						array( '44 - 49.3%', '29 - 56.1%', '95 - 58.6%'),
						array( '12 - 14.0%', '9 - 16.4%', '21 - 12.7%'),
						array( '24 - 26.4%', '10 - 18.7%', '36 - 22.4%'),
						array( '6 - 6.6%', '2 - 3.5%', '9 - 5.6%'),
						array( '18 - 19.9%', '8 - 15.2%', '27 - 16.8%'),
						array( '4 - 4.8%', '3 - 6.5%', '6 - 3.7%'),
						array( '5 - 5.4%', '1 - 2.4%', '4 - 2.6%'),
					)
				),
				'12' => array(
					'name' => 'Age/Gender Combos',
					'vars' => array('F 18-39', 'F 40-54', 'F 55-69', 'F 70+', 'M 18-39', 'M 40-54', 'M 55-69', 'M 70+'),
					'data_vars' => array(
						array( '15', '36', '56', '42', '13', '41', '58', '43'),
						array( '9 - 62.3%', '24 - 66.3%', '39 - 69.1%', '31 - 73.2%', '6 - 43.3%', '28 - 69.8%', '41 - 70.4%', '33 - 75.6%'),
						array( '8 - 54.1%', '18 - 50.7%', '30 - 54.6%', '30 - 71.0%', '4 - 28.8%', '18 - 44.3%', '31 - 53.8%', '29 - 66.7%'),
						array( '1 - 8.2%', '6 - 15.6%', '8 - 14.5%', '1 - 2.2%', '2 - 14.5%', '10 - 25.5%', '10 - 16.7%', '4 - 8.9%'),
						array( '4 - 29.5%', '11 - 30.8%', '12 - 22.2%', '8 - 19.7%', '7 - 49.6%', '8 - 20.8%', '11 - 18.3%', '8 - 18.4%'),
						array( '- -', '3 - 9.3%', '2 - 4.3%', '4 - 9.8%', '3 - 21.2%', '2 - 4.7%', '1 - 1.6%', '1 - 2.9%'),
						array( '4 - 29.5%', '8 - 21.5%', '10 - 17.9%', '4 - 9.8%', '4 - 28.4%', '7 - 16.2%', '10 - 16.7%', '7 - 15.5%'),
						array( '- -', '- -', '4 - 6.5%', '2 - 4.4%', '- -', '3 - 7.0%', '4 - 7.5%', '1 - 2.3%'),
						array( '1 - 8.2%', '1 - 2.9%', '1 - 2.2%', '1 - 2.7%', '1 - 7.1%', '1 - 2.3%', '2 - 3.8%', '2 - 3.7%'),				
					)
				),
				'13' => array(
					'name' => 'Donald Trump',
					'vars' => array('Very favorable', 'Some favorable', 'Some unfav', 'Very unfav', 'Never heard of', 'Unsure'),
					'data_vars' => array(
						array( '169', '42', '17', '53', '14', '10'),
						array( '169 - 100.0%', '42 - 100.0%', '-', '-', '-', '-'),
						array( '169 - 100.0%', '-', '-', '-', '-', '-'),
						array( 'xxx', '42 - 100.0%', '-', '-', '-', '-'),
						array( '-', '-', '17 - 100.0%', '53 - 100.0%', '-', '-'),
						array( '-', '-', '17 - 100.0%', '-', '-', '-'),
						array( '-', '-', '-', '53 - 100.0% ', '-', '-'),
						array( '-', '-', '-', '-', '14 - 100.0%', '-'),
						array( '-', '-', '-', '-', '-', '10 - 100.0%'),					
					)
				),
				'14' => array(
					'name' => 'Christopher Ward',
					'vars' => array('Very favorable', 'Some favorable', 'Some unfav', 'Very unfav', 'Never heard of', 'Unsure'),
					'data_vars' => array(
						array( '21', '41', '14', '9', '177', '42' ),
						array( '20 - 95.5%', '31 - 75.9%', '8 - 58.3%', '1 - 13.0%', '125 - 70.7%', '25 - 59.5% ' ),
						array( '19 - 90.7%', '23 - 56.4%', '7 - 49.4%', '1 - 13.0%', '103 - 58.4%', '15 - 36.5%' ),
						array( '1 - 4.8%', '8 - 19.5%', '1 - 8.8%', '- -', '22 - 12.3%', '10 - 23.0%' ),
						array( '1 - 4.5%', '9 - 22.5%', '6 - 41.7% ', '8 - 87.0%', '38 - 21.6%', '7 - 17.4%' ),
						array( '- -', '4 - 10.3%', '2 - 11.0%', '1 - 12.2%', '7 - 3.8% ', '3 - 7.3%' ),
						array( '1 - 4.5%', '5 - 12.2%', '4 - 30.7%', '7 - 74.9%', '31 - 17.8%', '4 - 10.1%' ),
						array( '- - ', '- -', '- -', '- -', '11 - 6.5%', '2 - 5.2%' ),
						array( '- -', '1 - 1.5%', '- -', '- -', '2 - 1.2%', '8 - 18.0%' ),					
					)
				),
				'15' => array(
					'name' => 'Cynthia Flores',
					'vars' => array('Very favorable', 'Some favorable', 'Some unfav', 'Very unfav', 'Never heard of', 'Unsure'),
					'data_vars' => array(
						array( '49', '47', '29', '14', '118', '47' ),
						array( '38 - 77.3%', '29 - 61.5%', '20 - 71.1%', '9 - 62.6%', '83 - 70.0%', '32 - 66.8%' ),
						array( '35 - 71.0%', '21 - 45.4%', '17 - 60.3%', '9 - 62.6%', '66 - 56.0%', '20 - 42.5%' ),
						array( '3 - 6.2%', '8 - 16.1%', '3 - 10.8%', '- -', '17 - 14.0%', '11 - 24.3%' ),
						array( '11 - 22.7%', '18 - 38.5%', '5 - 17.4%', '4 - 30.1%', '23 - 19.3%', '9 - 18.4%' ),
						array( '2 - 4.0%', '5 - 10.2%', '2 - 5.5%', '- -', '5 - 4.4%', '3 - 6.9%' ),
						array( '9 - 18.7%', '13 - 28.4%', '3 - 11.9%', '4 - 30.1%', '18 - 14.9%', '5 - 11.4%' ),
						array( '- -', '- -', '1 - 3.2%', '- -', '11 - 8.9%', '2 - 4.6% ' ),
						array( '- -', '- -', '2 - 8.3%', '1 - 7.2%', '2 - 1.8%', '5 - 10.2%' ),					
					)

				),				
			)
		);*/
		
		return _survey_cygnal_crosstab_excel( $data );
	}


	function _survey_cygnal_crosstab_excel( $data ){
		$filename =_survey_cygnal_crosstabs_excel_file( $data );
		return $filename;	
	}


	function _survey_cygnal_crosstabs_excel_file( $data ){	
		global $base_url;
		$path = DRUPAL_ROOT;
		$path_module = drupal_get_path('module', 'survey_cygnal');
		$path_libraries = libraries_get_path('phpexcel'); 
		if (PHP_SAPI == 'cli'){// Este ejemplo solo debe ejecutarse desde un navegador web.
			die('This example should only be run from a Web Browser');
		}
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
				private $h1;
				private $header; 
				private $headerTop;
				private $bgTitle;
				private $bgTitleGroup;
				private $bgSubTitle;
				private $bgYellow;
				private $bgData;
				private $bgLineTop;
				private $data_structure;
				private $namefile;

				public function EXCEL( $params ){
					$this->PHPEXCEL = $params['PHPEXCEL'];
					$this->path = $params['path'];
					$this->base_url = $params['base_url'];
					$this->path_module = $params['path_module'];
					$this->path_libraries = $params['path_libraries'];
					$this->node = $params['node'];
					$this->namefile= $params['namefile'];

					$this->header = array( 
						'font' => array('bold' => true, 'size' => 18, 'name' => 'Verdana', 'color' => array('rgb' => '427590')), 
						'alignment' => array('vertical' => 'center', 'horizontal' => 'left'),
						'borders' => array('allborders' => array('style' => PHPExcel_Style_Border::BORDER_NONE)), 
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
 						'borders' => array(
				            'allborders' => array(
				                'style' => PHPExcel_Style_Border::BORDER_THIN,
				                'color' => array('rgb' => 'BBBBBB')
				            )
				        ),
						'fill' => array(
							'type' => PHPExcel_Style_Fill::FILL_GRADIENT_LINEAR,
							'rotation' => 90,
							'startcolor' => array('argb' => 'FFD8D8D8',),
							'endcolor' => array('argb' => 'FFFFFFFF',),
						),
					);
					$this->bgTitle = array(
						'font' => array('bold' => true, 'size' => 8, 'name' => 'Verdana', 'color' => array('rgb' => '000000')), 
						'alignment' => array('vertical' => 'center', 'horizontal' => 'left'),
 						'borders' => array(
 							'allborders' => array(
				                'style' => PHPExcel_Style_Border::BORDER_THIN,
				                'color' => array('rgb' => '5D6D7E')
 							)
 						), 
						'fill' => array(
							'type' => PHPExcel_Style_Fill::FILL_SOLID,
							'color' => array('rgb' => 'AEB6BF')
						)
					);
					$this->bgYellow = array(
 						'borders' => array(
 							'allborders' => array(
				                'style' => PHPExcel_Style_Border::BORDER_THIN,
				                'color' => array('rgb' => '5D6D7E')
 							)
 						), 
						'fill' => array(
							'type' => PHPExcel_Style_Fill::FILL_SOLID,
							'color' => array('rgb' => 'FFFF00')
						)
					);
					$this->bgLineTop = array(
						'borders' => array('top' => array('style' => PHPExcel_Style_Border::BORDER_THIN,),),
					);
					$array = array(
						'font' => array('bold' => true, 'size' => 12, 'name' => 'Calibri', 'color' => array('rgb' => 'FFFFFF')), 
						'borders' => array('allborders' => array('style' => 'thin')), 
						'alignment' => array('vertical' => 'center', 'horizontal' => 'center')
					);
					$this->bgTitleGroup[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'0F243E') )));
					$this->bgTitleGroup[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'1D1B10') )));
					$this->bgTitleGroup[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'632523') )));
					$this->bgTitleGroup[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'4F6228') )));
					$this->bgTitleGroup[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'403151') )));
					$this->bgTitleGroup[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'244062') )));
					$this->bgTitleGroup[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'215967') )));
					$this->bgTitleGroup[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'974706') )));
					$this->bgTitleGroup[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'808080') )));
					$array = array(
						'font' => array('bold' => true, 'size' => 11, 'name' => 'Calibri', 'color' => array('rgb' => '101010')), 
						'borders' => array('allborders' => array('style' => 'thin')), 
						'alignment' => array('vertical' => 'center', 'horizontal' => 'center')
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
					$array = array( 
						'font' => array('bold' => false, 'size' => 11, 'name' => 'Calibri', 'color' => array('rgb' => '000000')), 
						'borders' => array('allborders' => array('style' => 'thin')), 
						'alignment' => array('vertical' => 'center', 'horizontal' => 'right'),
					);
					$this->bgData[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'C5D9F1') )));
					$this->bgData[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'DDD9C4') )));
					$this->bgData[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'F2DCDB') )));
					$this->bgData[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'EBF1DE') )));
					$this->bgData[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'E4DFEC') )));
					$this->bgData[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'DCE6F1') )));
					$this->bgData[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'DAEEF3') )));
					$this->bgData[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'FDE9D9') )));
					$this->bgData[] = array_merge($array, array( 'fill' => array( 'type' => PHPExcel_Style_Fill::FILL_SOLID, 'color' => array('rgb'=>'F2F2F2') )));
				}

				public function excel_set_lines_headers( $sheet ){

					# -----------------------------------------------------------------------------------
					# Toda la fila uno tiene un alto definido.
					$this->PHPEXCEL->getActiveSheet()->getRowDimension('1')->setRowHeight(58);
					
					# -----------------------------------------------------------------------------------
					# Todas las columnas tienen un ancho definido.
					$cell='A';
					for ($i=0; $i<12; $i++) { 
						$this->PHPEXCEL->getActiveSheet()->getColumnDimension( $cell )->setWidth( 10 );
						$cell++;
					}

					# -----------------------------------------------------------------------------------
					# Las celdas de la primera fila se combinan.
					$this->PHPEXCEL->setActiveSheetIndex( $sheet )->mergeCells('A1:B1');
					$this->PHPEXCEL->setActiveSheetIndex( $sheet )->mergeCells( 'C1:L1' );

					# -----------------------------------------------------------------------------------
					# Se pone el texto descriptivo de la aplicación.
					$this->PHPEXCEL->setActiveSheetIndex( $sheet )->setCellValue('C1', $this->node['text_header']);
					$this->PHPEXCEL->getActiveSheet()->getStyle( 'C1:L1' )->applyFromArray( $this->header );

					# -----------------------------------------------------------------------------------
					# Se pone el logo de la compañia al inicio.
					$gdImage = imagecreatefrompng($this->node['logo_cygnal']);
					$objDrawing = new PHPExcel_Worksheet_MemoryDrawing();
					$objDrawing->setName( 'Cygnal' );
					$objDrawing->setDescription( 'Cygnal' );
					$objDrawing->setImageResource( $gdImage );
					$objDrawing->setRenderingFunction( PHPExcel_Worksheet_MemoryDrawing::RENDERING_PNG );
					$objDrawing->setMimeType( PHPExcel_Worksheet_MemoryDrawing::MIMETYPE_DEFAULT );
					$objDrawing->setHeight( 74 );
					$objDrawing->setWorksheet( $this->PHPEXCEL->getActiveSheet() );

					# -----------------------------------------------------------------------------------
					# Definir la altura de la fila.
					$this->PHPEXCEL->getActiveSheet()->getRowDimension('2')->setRowHeight(20);

					# -----------------------------------------------------------------------------------
					# Combinar las celdas de la fila actual.
					$merge = 'A2:L2';
					$this->PHPEXCEL->setActiveSheetIndex( $sheet )->mergeCells( $merge );


					# -----------------------------------------------------------------------------------
					# Setear el titulo de la encuesta.
					$this->PHPEXCEL->setActiveSheetIndex( $sheet )->setCellValue('A2', $this->node['text_title']);

					# -----------------------------------------------------------------------------------
					# Aplicar los estilos correspondientes.
					$this->PHPEXCEL->getActiveSheet()->getStyle( $merge )->applyFromArray($this->headerTop);
					
					return;
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

				public function excel_create_new_sheet(){
					$heet=0;
					$this->PHPEXCEL->getActiveSheet()->setTitle('Crosstabs');
					$this->excel_set_lines_headers( $heet );
					return;
				}

				public function excel_new_data_structure(){
					$letras = array();
					$nuevoArray = array();
					$letra = 'a';
					for ($i=0; $i<200; $i++) { 
						$letras[] = $letra;
						$letra ++;
					}
					# ---------------------------------------------------------------------------------
					# SI EXISTEN PREGUNTAS:
					if (isset($this->node['crosstab']) and count($this->node['crosstab'])>0) {
						# DESGLOSE DE GRUPOS:
						foreach ($this->node['crosstab'] as $question_id => $val_question) {
							$n = 0;
							$cuantos = 0;
							# SI EXISTE PREGUNTA, FILAS Y GRUPOS:
							if (isset($val_question['question']) and isset($val_question['rows']) and isset($val_question['groups'])) {
								foreach ($val_question['groups'] as $k_group => $val_group) {
									if ( $cuantos + count($val_group['vars'])<11 ) {
										$pos = strtoupper($question_id.$letras[$n]);
										$cuantos += count($val_group['vars']);
										$nuevoArray['crosstab'][$pos]['question'] = $pos.': '.$val_question['question'];
										$nuevoArray['crosstab'][$pos]['rows'] = $val_question['rows'];
										$nuevoArray['crosstab'][$pos]['total'] = $val_question['total'];
										$nuevoArray['crosstab'][$pos]['groups'][] = $val_group;
									}else{
										$n++;
										$pos = strtoupper($question_id.$letras[$n]);
										$cuantos = count($val_group['vars']);
										$nuevoArray['crosstab'][$pos]['question'] = $pos.': '.$val_question['question'];
										$nuevoArray['crosstab'][$pos]['rows'] = $val_question['rows'];
										$nuevoArray['crosstab'][$pos]['total'] = $val_question['total'];
										$nuevoArray['crosstab'][$pos]['groups'][] = $val_group;
									}
									#echo "<br>".count($val_group['vars']).' -- '.$val_group['name'];
								}
							}
						}
					}
					$this->data_structure = $nuevoArray;
					unset( $this->node['crosstab'], $nuevoArray );
					return;
				}

				public function write_the_questions(){
					if ( !empty($this->data_structure) ) {
						
						// Se inicia en la fila 3 del documento excel.
						$index = 3;
						$sheet = 0;
						foreach( $this->data_structure['crosstab'] as $question_id => $val_question){
							
							# ---------------------------------------------------------------------------------
							# PONER EL ENUNCIADO GENERAL DE CADA PREGUNTA:
							$cell = 'A';
							$merge = $cell.$index.':L'.$index;
							$this->PHPEXCEL->setActiveSheetIndex( $sheet )->mergeCells( $merge );
							$this->PHPEXCEL->setActiveSheetIndex( $sheet )->setCellValue( $cell.$index, $val_question['question'] );
							$this->PHPEXCEL->getActiveSheet()->getStyle( $merge )->applyFromArray( $this->bgTitle );
							$this->PHPEXCEL->getActiveSheet()->getStyle( $cell.$index )->getAlignment()->setWrapText(true);
							$this->PHPEXCEL->getActiveSheet()->getRowDimension( $index )->setRowHeight(30);
							$index++;


							# ---------------------------------------------------------------------------------
							# DIBUJA EL ENUNCIOADO DE FILA SOBRE LA COLUMNA A.
							$index++;
							$this->PHPEXCEL->setActiveSheetIndex( $sheet )->mergeCells( $cell.($index-1).':'.$cell.$index );
							$this->PHPEXCEL->getActiveSheet()->getStyle( $cell.($index-1).':'.$cell.$index  )->applyFromArray( $this->bgYellow );
							$index++;
							foreach ($val_question['rows'] as $k => $val) {
								$this->PHPEXCEL->setActiveSheetIndex( $sheet )->setCellValue( $cell.$index, $val );
								$this->PHPEXCEL->getActiveSheet()->getStyle( $cell.$index )->getAlignment()->setWrapText(true);
								$this->PHPEXCEL->getActiveSheet()->getStyle( $cell.$index )->applyFromArray( $this->bgYellow );
								$index++;
							}


							# ---------------------------------------------------------------------------------
							# DIBUJA LOS TOTALES SOBRE LA COLUMNA B.
							$cell = 'B';
							$index = ($index - (count($val_question['rows'])+2) );
							$this->PHPEXCEL->getActiveSheet()->getStyle( $cell.$index )->applyFromArray( $this->bgTitleGroup[0] );
							$index++;
							$this->PHPEXCEL->setActiveSheetIndex( $sheet )->setCellValue( $cell.$index, 'Total' );
							$this->PHPEXCEL->getActiveSheet()->getStyle( $cell.$index )->applyFromArray( $this->bgSubTitle[0] );							
							$index++;
							foreach ($val_question['total'] as $k => $val) {
								$posicition = $cell.$index;
								$texto='';
								$val = trim($val);
								if (!empty($val)) {
									$pos = strpos($val, "-");
									if($pos === false) {
									    $texto = trim(round($val));
									}else{
										$partes = explode("-", $val);
										$texto = trim($partes[0])."\n".trim($partes[1]);
										if (empty(trim($texto))) {
											$texto = "-\n-";
										}
									}
								}else{
									$texto = "-\n-";
								}
								$this->PHPEXCEL->setActiveSheetIndex( $sheet )->setCellValue( $posicition, $texto );
								$this->PHPEXCEL->getActiveSheet()->getStyle( $posicition )->getAlignment()->setWrapText(true);
								$this->PHPEXCEL->getActiveSheet()->getStyle( $posicition )->applyFromArray( $this->bgData[0] );
								$index++;
							}


							# ---------------------------------------------------------------------------------
							# COMBINAR LAS CELDAS DE GRUPO E IMPRIME EL TITULO DE GRUPO.
							$index = ($index - (count($val_question['rows'])+2) );
							$cellAux = $cellIni = $cellFin = 'C';
							$num = 1;
							foreach($val_question['groups'] as $goup_id => $val_group) {
								for ($j=1; $j<count($val_group['vars']); $j++){
									$cellFin++;
								}								
								$merge = $cellIni.$index.':'.$cellFin.$index;
								$this->PHPEXCEL->setActiveSheetIndex( $sheet )->mergeCells( $merge );								
								$this->PHPEXCEL->setActiveSheetIndex( $sheet )->setCellValue( $cellIni.$index, $val_group['name'] );
								$this->PHPEXCEL->getActiveSheet()->getStyle( $cellIni.$index )->applyFromArray( $this->bgTitleGroup[$num] );
								$this->PHPEXCEL->getActiveSheet()->getStyle( $merge )->getAlignment()->setWrapText(true);
								$index++;


								# nombres de variables
								foreach ($val_group['vars'] as $var_id => $val_var) {
									$posicition = $cellIni.$index;
									$this->PHPEXCEL->setActiveSheetIndex( $sheet )->setCellValue( $posicition, $val_var );
									$this->PHPEXCEL->getActiveSheet()->getStyle( $posicition )->applyFromArray( $this->bgSubTitle[$num] );
									$this->PHPEXCEL->getActiveSheet()->getStyle( $posicition )->getAlignment()->setWrapText(true);
									$cellIni++;
								}
								$index++;


								$cellIni = $cellAux;
								foreach ($val_group['data_vars'] as $var_id => $val_var) {
									foreach ($val_var as $id => $value) {
										$posicition = $cellIni.$index;
										$texto='';
										$value = trim($value);
										if (!empty($value)) {
											$pos = strpos($value, "-");
											if($pos === false) {
											    $texto = trim($value);
											}else{
												$partes = explode("-", $value);
												$texto = trim($partes[0])."\n".trim($partes[1]);
												if (empty(trim($texto))) {
													$texto = "-\n-";
												}
											}
										}else{
											$texto = "-\n-";
										}
										$this->PHPEXCEL->setActiveSheetIndex( $sheet )->setCellValue( $posicition, $texto );
										$this->PHPEXCEL->getActiveSheet()->getStyle( $posicition )->applyFromArray( $this->bgData[$num] );
										$this->PHPEXCEL->getActiveSheet()->getStyle( $posicition )->getAlignment()->setWrapText(true);
										$cellIni++;
									}
									$index++;
									$cellIni = $cellAux;
								}
								$num++;
								$cellFin++;
								$cellAux = $cellIni = $cellFin;
								$index = ($index - (count($val_question['rows'])+2) );								
							}

							$index = ($index + (count($val_question['rows'])+2) );
							$this->PHPEXCEL->setActiveSheetIndex( $sheet )->mergeCells( 'A'.$index.':L'.$index );
							$this->PHPEXCEL->getActiveSheet()->getRowDimension( $index )->setRowHeight(30);
							$index++;
							#die("=".$index);
						}

					}
					return;
				}




				public function excel_print_data(){
					$sheet=0;	# cero indica que se inicia en la hoja 1.
					$index=$inicio=3; 	# Para iniciar en la fila 3.
					# ---------------------------------------------------------------------------------
					# SI EXISTEN PREGUNTAS:
					if (isset($this->node['crosstab']) and count($this->node['crosstab'])>0) {
						# DESGLOSE DE GRUPOS:
						foreach ($nuevoArray['crosstab'] as $key => $value) {
							$cell++;
							$index = ($index - (count($value['rows'])+2) );
							foreach ($value['groups'] as $k => $val) {
								$num_merges = count($val['vars']);
								echo "<br>".$num_merges;

							}

							/*
							# ---------------------------------------------------------------------------------
							# PONER LOS ITEMS DE CADA GRUPO
							$num = 0;
							$cellIni = 'B';
							foreach ($value['groups'] as $k => $val) {
								foreach ($val['sub-groups'] as $x => $item) {
									$this->PHPEXCEL->setActiveSheetIndex( $sheet )->setCellValue( $cellIni.$index, $item );
									$this->PHPEXCEL->getActiveSheet()->getStyle( $cellIni.$index )->getAlignment()->setWrapText(true);
									$this->PHPEXCEL->getActiveSheet()->getStyle( $cellIni.$index )->applyFromArray( $this->bgSubTitle[$num] );
									$cellIni++;
								}
								$num++;
							}
							$index++;

							# ---------------------------------------------------------------------------------
							# ESCRIBIR LOS RESULTADOS DE DATOS.
							$ng=0;
							$aux='';
							$cellIni='B';
							$cellPivote='B';
							$reinicioFila = ($index-count($value['rows']));
							foreach ($value['groups'] as $k => $val) {
								foreach ($val['data'] as $x => $rowvalues) {
									foreach ($rowvalues as $y => $info) {
										if (!empty($info)) {
											if (strpos($info, "-")) {
												$partes = explode("-", $info);
												$texto = trim($partes[0])."\n".trim($partes[1]);
											}else{
												$texto = trim($info);
											}
										}else{
											$texto = "-\n-";
										}
										$posicition = $cellPivote.$reinicioFila;
										$this->PHPEXCEL->setActiveSheetIndex( $sheet )->setCellValue( $posicition, $texto );
										$this->PHPEXCEL->getActiveSheet()->getStyle( $posicition )->getAlignment()->setWrapText(true);
										$this->PHPEXCEL->getActiveSheet()->getStyle( $posicition )->applyFromArray( $this->bgData[$ng]);
										$cellPivote++;
									}
									$aux=$cellPivote;
									$cellPivote=$cellIni;
									$reinicioFila++;
								}
								$reinicioFila = ($index-count($value['rows']));
								$cellIni=$aux;
								$cellPivote=$cellIni;
								$ng++;
							}

							# ---------------------------------------------------------------------------------
							$merge = 'A'.$index.':L'.$index;
							$this->PHPEXCEL->setActiveSheetIndex( $sheet )->mergeCells( $merge );
							$this->PHPEXCEL->getActiveSheet()->getStyle( $merge  )->applyFromArray( $this->bgLineTop );
							$index++;
							*/
						}
					}
					return;
				}



				public function excel_generate(){
					$docname = $this->namefile.'-Crosstab-'.date("Ymd-His").'.xlsx';
					$document 	= $this->path.'/'.$this->path_module.'/tmp/xls/'.$docname;
					$route		=$this->base_url.'/'.$this->path_module.'/tmp/xls/'.$docname;	

					//$docname = 'survey_crosstabs_'.date("Y_m_d_H_i_s").'.xlsx';
					//$document = $this->path.'/'.$this->path_module.'/tmp/xls/'.$docname;

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

			}
		}

		$PHPEXCEL = new PHPExcel();
		$params = array(
			'PHPEXCEL' 			=> $PHPEXCEL, 
			'base_url'			=> $base_url,
			'path' 				=> $path, 
			'path_module' 		=> $path_module, 
			'path_libraries' 	=> $path_libraries, 
			'node' 				=> $data['node'],
			'namefile'			=> $data['node']['text_project_id'],
		);
		$objXLS = new EXCEL( $params );
		$objXLS->excel_document_properties();
		$objXLS->excel_create_new_sheet();
		$objXLS->excel_new_data_structure();
		$objXLS->write_the_questions();


		#$objXLS->excel_print_data();
		$objXLS->excel_generate();
	}