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
	

	function _debug_t($variable,$title,$exit=false,$print=true){
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
	function _survey_cygnal_toplines_file(){
		$nid=0;
		
	 	if(is_numeric(arg(4))){#Se asigna el id como el $nid directo
	 			$nid=arg(4); //Asignación directa
	 	}
	   
	 	if(is_numeric($nid) && $nid!=0){#Si se encontre el $nid del nodo de la cuota, se pasa a la construcción del archivo
	 		$node=node_load($nid); #load the node with information of the quota
	 		$datos=_survey_cygnal_toplines_get_info_node($node);#Llena la estructura de generación del archivo de quotas con la información del nodo
	 		
	 		
	 		$quota=_survey_cygnal_toplines_get_quota($nid);
	 		$ss=_survey_cygnal_toplines_get_ss($node,$quota);
	 		#_debug_t($ss,"ss",true,true); #Variable, exit, print	
	 		_survey_cygnal_toplines_build_file($datos,$ss);#Inicia la construcción del archivo de Script, este archivo sera un word
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
	function _survey_cygnal_toplines_get_quota($nid){
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

	function _survey_cygnal_toplines_letters(){
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
	function _survey_cygnal_toplines_get_ss($node,&$quota){
		$path = DRUPAL_ROOT;
		$path_libraries = libraries_get_path('phpexcel'); 
			
		require_once $path.'/'.$path_libraries.'/Classes/PHPExcel.php'; 
		
		$data=array();
		$col_quota="";
		$abc=_survey_cygnal_toplines_letters(); # se obtiene un vector con las letras
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
		#_debug_t($quota,"INICIA QUOTA",true,true); #Variable, exit, print
	 	#_debug_t($data,"INICIA SS",false,true); #Variable, exit, print
	 	_survey_cygnal_toplines_set_sum_quotas($data);
	 	#_debug_t($data,"SUM SS",false,true); #Variable, exit, print
	 	_survey_cygnal_toplines_set_porcentaje_quotas($data);
	 	#_debug_t($data,"PORCENTAJE SS",false,true); #Variable, exit, print
	 	_survey_cygnal_toplines_set_sam_quotas($data,$quota);
	 	#_debug_t($data,"QUOTA SS",false,true); #Variable, exit, print
	 	_survey_cygnal_toplines_set_height_ss($data,$quota);
	 	#_debug_t($data,"weight SS",false,true); #Variable, exit, print
	 	_survey_cygnal_toplines_set_data_toplines($data,$quota);
	 	#_debug_t($data,"TOP SS",false,true); #Variable, exit, print
	    #_debug_t($data,"data",true,true); #Variable, exit, print
	    return $data;
	}	

	function _survey_cygnal_toplines_set_sum_quotas(&$data){
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

	function _survey_cygnal_toplines_set_porcentaje_quotas(&$data){
		$porcentajes=array();
		#Recorrer la matrix de datos, inicializa en 1, para que no tome la fila de cabecera
		
		$row=1;
		foreach ($data["quotas"]["cantidades"] as $key => $value) {
			$porcentajes[$row]=$value/$data["rows"];
			$row++;
		}
		$data["quotas"]["porcentajes"]=$porcentajes;	
	}

	function _survey_cygnal_toplines_set_sam_quotas(&$data,&$quota){
		#_debug_t($quota["node"]["table_quota"],"table_quota",true,true); #Variable, exit, print
		#Asigna en la columna B, de la información de las quotas, representa a la solumna SAM 
		foreach ($quota["node"]["table_quota"]['B'] as $key => $value) {
			$quota["node"]["table_quota"]['B'][$key]=round(($data["quotas"]["porcentajes"][$key]*100),2);
			$quota["node"]["table_quota"]['D'][$key]=($quota["node"]["table_quota"]['A'][$key]/$quota["node"]["table_quota"]['B'][$key]);
		}

	}	

	function _survey_cygnal_toplines_set_height_ss(&$data,&$quota){
		_debug_t($quota["node"]["table_quota"],"table-quota",false,true); #Variable, exit, print
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

	function _survey_cygnal_toplines_set_data_toplines(&$data,&$quota){
		#_debug_t($quota["node"]["table_quota"],"table-quota",false,true); #Variable, exit, print
		#Asigna en la columna B, de la información de las quotas, representa a la solumna SAM 
		$question=array();
		foreach ($data["datos"] as $row => $row_value) {
			foreach ($row_value as $col => $col_value) {
				#_debug_t($row,"row",false,true); #Variable, exit, print
				if($row==0){
					#_debug_t($col,"col",false,true); #Variable, exit, print
					if(_survey_cygnal_toplines_excluir_columnas($col_value)){ # valida que se incluyan solo las columnas de preguntas y demographics
						$question[$col]["key"]=$col;
						$question[$col]["value"]=$col_value;
						$question[$col]["answers"]=array();
					}
				}else{
					#_debug_t($data["datos"][0][$col],"col",false,true); #Variable, exit, print
					if(_survey_cygnal_toplines_excluir_columnas($data["datos"][0][$col])){ # valida que se incluyan solo las columnas de preguntas y demographics
						
						if(isset($question[$col]["answers"][$col_value])){
							$question[$col]["answers"][$col_value]=$question[$col]["answers"][$col_value] + $row_value['height'];
						}else{
							$question[$col]["answers"][$col_value]=$row_value['height'];
						}
					}	
				}
			}
		}		

		#_debug_t($question,"question",true,true); #Variable, exit, print	
		$data["toplines"]=$question;
	}

	function _survey_cygnal_toplines_excluir_columnas($col_value){
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
	function _survey_cygnal_toplines_get_info_node($node){
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

	function _survey_cygnal_toplines_build_file($datos,$ss){
	
		#_debug_t($datos["node"]["questions"],"questions",false,true); #Variable, exit, print
		#_debug_t($datos["node"]["demographics"],"demographics",false,true); #Variable, exit, print
		#_debug_t($ss["toplines"],"toplines",true,true); #Variable, exit, print

		

		$fecha = date("Y-m-d");
		$fecha = date("l, F j, Y").' - '.date('l, F j, Y', strtotime($fecha. ' + 5 days'));

		// Los logos a escala ratio is: (16 : 9)
		$path = drupal_get_path('module', 'survey_cygnal');
		//require_once $path."/includes/survey_cygnal.word_toplines.inc";
		

		$data = array();

		foreach ($datos["node"]["questions"] as $key => $value) {
			#_debug_t($key,"key",false,true); #Variable, exit, print

			$data['node']['questions'][$key]['question']=$value['question'];
			$total=0;
			#_debug_t($value,"value",true,true); #Variable, exit, print
			foreach ($value["answers"] as $key2 => $value2) {
				if(isset($ss["toplines"][$key+1]['answers'][$key2+1])){
					$subtotal=$ss["toplines"][$key+1]['answers'][$key2+1];
				}else{
					$subtotal=0;
				}
				#$total=$total+round($subtotal);
				$total=$total+$subtotal;
				#_debug_t($key2,"key2",false,true); #Variable, exit, print
				#_debug_t($value2,"value2",false,true); #Variable, exit, print
				$data['node']['questions'][$key]['labels'][$key2]['item']=$value2['answer_text'];
				$data['node']['questions'][$key]['labels'][$key2]['total']=round($subtotal);
				$data['node']['questions'][$key]['labels'][$key2]['total_complete']=$subtotal;
				$data['node']['questions'][$key]['labels'][$key2]['bold']=false;								
			}		

			foreach ($value["answers"] as $key2 => $value2) {				
				$data['node']['questions'][$key]['labels'][$key2]['percent']=round((($data['node']['questions'][$key]['labels'][$key2]['total_complete']/$total)*100),1);						
			}		

			$data['node']['questions'][$key]['total']=round($total);		
		}

		#_debug_t($data,"data",true,true); #Variable, exit, print


		$data['node']['logo_cygnal'] 		= $path."/theme/img/logo_cygnal_transparent.png";
		$data['node']['logo_custamer'] 		= $path."/theme/img/logos/logo_empresa.png";
		$data['node']['text_header'] 		= "Cygnal Pulse Survey Platform";

		$data['node']['show_text_header'] 	= 1; // 0:No  1:Si

		$data['node']['text_zone'] 			= $datos["node"]["text_title"];
		$data['node']['text_date'] 			= "";
		$data['node']['text_info'] 			= "";

		$data['node']['text_project_id']=$datos["node"]["text_project_id"];
		
		/*$data['node']['questions'][] = array(
			'question' 	=> 'How likely are you to vote in the 2018 Republican Primary Election for federal, state, and local offices next August? ',
			'total' => 461,
			'labels' => array(
				0 => array(
					'item' => 'Definitely voting',
					'total' => '412',
					'percent' => '89.3%',
					'bold' => false
				),
				1 => array(
					'item' => 'Probably voting',
					'total' => '44',
					'percent' => '9.5%',
					'bold' => false
				),
				2 => array(
					'item' => 'Not sure',
					'total' => '6',
					'percent' => '1.2%',
					'bold' => false
				)
			)
		);
		$data['node']['questions'][] = array(
			'question' 	=> 'What age range do you fall within?',
			'total' => 461,
			'labels' => array(
				0 => array(
					'item' => '18-39',
					'total' => '32',
					'percent' => '6.9%',
					'bold' => true
				),
				1 => array(
					'item' => '40-54',
					'total' => '89',
					'percent' => '19.2%',
					'bold' => false
				),
				2 => array(
					'item' => '55-69',
					'total' => '177',
					'percent' => '38.4%',
					'bold' => false
				),
				3 => array(
					'item' => '70+',
					'total' => '163',
					'percent' => '35.4%',
					'bold' => true
				),
			)
		);
		$data['node']['questions'][] = array(
			'question' 	=> 'Gender',
			'total' => 461,
			'labels' => array(
				0 => array(
					'item' => 'Male',
					'total' => '225',
					'percent' => '48.9%',
					'bold' => true
				),
				1 => array(
					'item' => 'Female',
					'total' => '236',
					'percent' => '51.1%',
					'bold' => false
				),
			)
		);
		$data['node']['questions'][] = array(
			'question' 	=> 'Jay Trumbull',
			'total' => 461,
			'labels' => array(
				0 => array(
					'item' => 'Very favorable',
					'total' => '61',
					'percent' => '13.3%',
					'bold' => true
				),
				1 => array(
					'item' => 'Somewhat favorable',
					'total' => '68',
					'percent' => '14.8%',
					'bold' => false
				),
				2 => array(
					'item' => 'Somewhat unfavorable',
					'total' => '14',
					'percent' => '3.1%',
					'bold' => false
				),
				3 => array(
					'item' => 'Very unfavorable',
					'total' => '15',
					'percent' => '3.3%',
					'bold' => false
				),
				4 => array(
					'item' => 'Never heard of',
					'total' => '237',
					'percent' => '51.4%',
					'bold' => false
				),
				5 => array(
					'item' => 'Unsure',
					'total' => '65',
					'percent' => '14.1%',
					'bold' => false
				),
			)
		);
		$data['node']['questions'][] = array(
			'question' 	=> 'Donald Trump',
			'total' => 461,
			'labels' => array(
				0 => array(
					'item' => 'Very favorable',
					'total' => '332',
					'percent' => '72.0%',
					'bold' => true
				),
				1 => array(
					'item' => 'Somewhat favorable',
					'total' => '55',
					'percent' => '11.8%',
					'bold' => false
				),
				2 => array(
					'item' => 'Somewhat unfavorable',
					'total' => '11',
					'percent' => '2.4%',
					'bold' => false
				),
				3 => array(
					'item' => 'Very unfavorable',
					'total' => '46',
					'percent' => '10.1%',
					'bold' => false
				),
				4 => array(
					'item' => 'Never heard of',
					'total' => '5',
					'percent' => '1.2%',
					'bold' => false
				),
				5 => array(
					'item' => 'Unsure',
					'total' => '12',
					'percent' => '2.5%',
					'bold' => false
				),
			)
		);
		$data['node']['questions'][] = array(
			'question' 	=> 'Rick Scott',
			'total' => 461,
			'labels' => array(
				0 => array(
					'item' => 'Very favorable',
					'total' => '234',
					'percent' => '50.7%',
					'bold' => true
				),
				1 => array(
					'item' => 'Somewhat favorable',
					'total' => '116',
					'percent' => '25.3%',
					'bold' => false
				),
				2 => array(
					'item' => 'Somewhat unfavorable',
					'total' => '32',
					'percent' => '7.0%',
					'bold' => false
				),
				3 => array(
					'item' => 'Very unfavorable',
					'total' => '35',
					'percent' => '7.6%',
					'bold' => false
				),
				4 => array(
					'item' => 'Never heard of',
					'total' => '11',
					'percent' => '2.4%',
					'bold' => false
				),
				5 => array(
					'item' => 'Unsure',
					'total' => '32',
					'percent' => '7.0%',
					'bold' => false
				),
			)
		);
		$data['node']['questions'][] = array(
			'question' 	=> 'George Gainer',
			'total' => 461,
			'labels' => array(
				0 => array(
					'item' => 'Very favorable',
					'total' => '98',
					'percent' => '21.3%',
					'bold' => true
				),
				1 => array(
					'item' => 'Somewhat favorable',
					'total' => '76',
					'percent' => '16.5%',
					'bold' => false
				),
				2 => array(
					'item' => 'Somewhat unfavorable',
					'total' => '25',
					'percent' => '5.4%',
					'bold' => false
				),
				3 => array(
					'item' => 'Very unfavorable',
					'total' => '22',
					'percent' => '4.8%',
					'bold' => false
				),
				4 => array(
					'item' => 'Never heard of',
					'total' => '172',
					'percent' => '37.3%',
					'bold' => false
				),
				5 => array(
					'item' => 'Unsure',
					'total' => '68',
					'percent' => '14.8%',
					'bold' => false
				),
			)
		);
		$data['node']['questions'][] = array(
			'question' 	=> 'Brad Drake',
			'total' => 461,
			'labels' => array(
				0 => array(
					'item' => 'Very favorable',
					'total' => '53',
					'percent' => '11.5%',
					'bold' => true
				),
				1 => array(
					'item' => 'Somewhat favorable',
					'total' => '80',
					'percent' => '17.4%',
					'bold' => false
				),
				2 => array(
					'item' => 'Somewhat unfavorable',
					'total' => '23',
					'percent' => '4.9%',
					'bold' => false
				),
				3 => array(
					'item' => 'Very unfavorable',
					'total' => '20',
					'percent' => '4.4%',
					'bold' => false
				),
				4 => array(
					'item' => 'Never heard of',
					'total' => '220',
					'percent' => '47.7%',
					'bold' => false
				),
				5 => array(
					'item' => 'Unsure',
					'total' => '65',
					'percent' => '14.2%',
					'bold' => false
				),
			)
		);
		$data['node']['questions'][] = array(
			'question' 	=> 'The National Rifle Association, known as the NRA',
			'total' => 461,
			'labels' => array(
				0 => array(
					'item' => 'Very favorable',
					'total' => '300',
					'percent' => '65.0%',
					'bold' => true
				),
				1 => array(
					'item' => 'Somewhat favorable',
					'total' => '72',
					'percent' => '15.6%',
					'bold' => false
				),
				2 => array(
					'item' => 'Somewhat unfavorable',
					'total' => '18',
					'percent' => '3.9%',
					'bold' => false
				),
				3 => array(
					'item' => 'Very unfavorable',
					'total' => '53',
					'percent' => '11.4%',
					'bold' => false
				),
				4 => array(
					'item' => 'Never heard of',
					'total' => '2',
					'percent' => '0.4%',
					'bold' => false
				),
				5 => array(
					'item' => 'Unsure',
					'total' => '17',
					'percent' => '3.6%',
					'bold' => false
				),
			)
		);
		$data['node']['questions'][] = array(
			'question' 	=> 'If you were voting today in the August 2018 Republican Primary Election for governor, which of the following candidates would you vote for?',
			'total' => 461,
			'labels' => array(
				0 => array(
					'item' => 'Richard Corcoran',
					'total' => '11',
					'percent' => '2.4%',
					'bold' => true
				),
				1 => array(
					'item' => 'Ron DeSantis',
					'total' => '35',
					'percent' => '7.6%',
					'bold' => false
				),
				2 => array(
					'item' => 'Jack Latvala',
					'total' => '6',
					'percent' => '1.2%',
					'bold' => false
				),
				3 => array(
					'item' => 'Adam Putnam',
					'total' => '64',
					'percent' => '13.9%',
					'bold' => false
				),
				4 => array(
					'item' => 'Unsure',
					'total' => '345',
					'percent' => '74.9%',
					'bold' => false
				),
			)
		);
		$data['node']['questions'][] = array(
			'question' 	=> 'Do you approve or disapprove of the job State Representative Jay Trumbull has done so far?',
			'total' => 461,
			'labels' => array(
				0 => array(
					'item' => 'Strongly approve',
					'total' => '63',
					'percent' => '13.6%',
					'bold' => true
				),
				1 => array(
					'item' => 'Somewhat approve',
					'total' => '91',
					'percent' => '19.6%',
					'bold' => false
				),
				2 => array(
					'item' => 'Somewhat disapprove',
					'total' => '25',
					'percent' => '5.4%',
					'bold' => false
				),
				3 => array(
					'item' => 'Strongly disapprove',
					'total' => '17',
					'percent' => '3.6%',
					'bold' => false
				),
				4 => array(
					'item' => 'No opinion',
					'total' => '190',
					'percent' => '41.2%',
					'bold' => false
				),
				5 => array(
					'item' => 'Unsure',
					'total' => '76',
					'percent' => '16.6%',
					'bold' => false
				),
			)
		);
		$data['node']['questions'][] = array(
			'question' 	=> ' Does Jay Trumbull deserve reelection to the state legislature or is it time for someone new?',
			'total' => 461,
			'labels' => array(
				0 => array(
					'item' => 'Yes, Jay Trumbull deserves another term',
					'total' => '99',
					'percent' => '21.4%',
					'bold' => true
				),
				1 => array(
					'item' => 'No, it’s time for someone new',
					'total' => '104',
					'percent' => '22.5%',
					'bold' => false
				),
				2 => array(
					'item' => 'Unsure',
					'total' => '259',
					'percent' => '56.1%',
					'bold' => false
				),				
			)
		);
		$data['node']['questions'][] = array(
			'question' 	=> 'Do you support or oppose private businesses receiving economic development funding if they promise to create jobs?',
			'total' => 461,
			'labels' => array(
				0 => array(
					'item' => 'Strongly support',
					'total' => '161',
					'percent' => '34.9%',
					'bold' => true
				),
				1 => array(
					'item' => 'Somewhat support',
					'total' => '145',
					'percent' => '31.3%',
					'bold' => false
				),
				2 => array(
					'item' => 'Somewhat oppose',
					'total' => '57',
					'percent' => '12.4%',
					'bold' => false
				),
				3 => array(
					'item' => 'Strongly oppose',
					'total' => '36',
					'percent' => '7.9%',
					'bold' => false
				),
				4 => array(
					'item' => 'Makes no difference',
					'total' => '17',
					'percent' => '3.6%',
					'bold' => false
				),
				5 => array(
					'item' => 'Undecided',
					'total' => '45',
					'percent' => '9.8%',
					'bold' => false
				),
			)
		);
		$data['node']['questions'][] = array(
			'question' 	=> 'Should Bay County and the State of Florida use settlement funds from the Deepwater Horizon oil spill settlement to spur economic development or invest in infrastructure improvements?',
			'total' => 461,
			'labels' => array(
				0 => array(
					'item' => 'Spur economic development',
					'total' => '80',
					'percent' => '17.4%',
					'bold' => true
				),
				1 => array(
					'item' => 'Invest in infrastructure improvements',
					'total' => '257',
					'percent' => '55.8%',
					'bold' => false
				),
				2 => array(
					'item' => 'Need more information',
					'total' => '87',
					'percent' => '18.9%',
					'bold' => false
				),
				3 => array(
					'item' => 'Undecided',
					'total' => '36',
					'percent' => '7.8%',
					'bold' => false
				),
			)
		);
		$data['node']['questions'][] = array(
			'question' 	=> 'Which of the following options do you believe is to be blamed for violence in schools?',
			'total' => 461,
			'labels' => array(
				0 => array(
					'item' => 'Too many guns',
					'total' => '39',
					'percent' => '8.6%',
					'bold' => true
				),
				1 => array(
					'item' => 'Failures in our mental health system',
					'total' => '64',
					'percent' => '13.9%',
					'bold' => false
				),
				2 => array(
					'item' => 'Declining moral values',
					'total' => '216',
					'percent' => '46.8%',
					'bold' => false
				),
				3 => array(
					'item' => 'Lack of security in our schools',
					'total' => '62',
					'percent' => '13.5%',
					'bold' => false
				),
				4 => array(
					'item' => 'Something else',
					'total' => '57',
					'percent' => '12.4%',
					'bold' => false
				),
				5 => array(
					'item' => 'Undecided',
					'total' => '22',
					'percent' => '4.8%',
					'bold' => false
				),
			)
		);
		$data['node']['questions'][] = array(
			'question' 	=> 'How do you believe the Florida legislature should address gun violence in our schools?',
			'total' => 461,
			'labels' => array(
				0 => array(
					'item' => 'Reinstitute prayer and moral training in our schools',
					'total' => '105',
					'percent' => '22.8%',
					'bold' => true
				),
				1 => array(
					'item' => 'Institute more gun control',
					'total' => '35',
					'percent' => '7.6%',
					'bold' => false
				),
				2 => array(
					'item' => 'Increase armed security measures in schools',
					'total' => '100',
					'percent' => '21.7%',
					'bold' => false
				),
				3 => array(
					'item' => 'All of the above',
					'total' => '165',
					'percent' => '35.7%',
					'bold' => false
				),
				4 => array(
					'item' => 'Something else',
					'total' => '40',
					'percent' => '8.6%',
					'bold' => false
				),
				5 => array(
					'item' => 'Undecided',
					'total' => '17',
					'percent' => '3.7%',
					'bold' => false
				),
			)
		);
		$data['node']['questions'][] = array(
			'question' 	=> 'More specifically, in addressing gun violence in our schools, do you believe the Florida  legislature should:',
			'total' => 461,
			'labels' => array(
				0 => array(
					'item' => 'Raise the age limit of those who can buy guns to the age of 21 or older',
					'total' => '136',
					'percent' => '29.6%',
					'bold' => true
				),
				1 => array(
					'item' => 'Institute a 3-day waiting period for anyone who buys an assault rifle',
					'total' => '41',
					'percent' => '8.9%',
					'bold' => false
				),
				2 => array(
					'item' => 'Ban assault rifles altogether',
					'total' => '87',
					'percent' => '18.8%',
					'bold' => false
				),
				3 => array(
					'item' => 'Place specially trained armed marshals on school campuses to boost security',
					'total' => '139',
					'percent' => '30.2%',
					'bold' => false
				),
				4 => array(
					'item' => 'Undecided',
					'total' => '58',
					'percent' => '12.5%',
					'bold' => false
				),
			)
		);
		$data['node']['questions'][] = array(
			'question' 	=> 'If you knew a candidate seeking re-election to the US Congress or Florida state legislature supported addressing school violence by instituting more gun control, would you be more or less likely to vote for him or her?',
			'total' => 461,
			'labels' => array(
				0 => array(
					'item' => 'Much more likely',
					'total' => '87',
					'percent' => '18.9%',
					'bold' => true
				),
				1 => array(
					'item' => 'Somewhat more likely',
					'total' => '43',
					'percent' => '9.3%',
					'bold' => false
				),
				2 => array(
					'item' => 'Somewhat less likely',
					'total' => '66',
					'percent' => '14.2%',
					'bold' => false
				),
				3 => array(
					'item' => 'Much less likely',
					'total' => '176',
					'percent' => '38.1%',
					'bold' => false
				),
				4 => array(
					'item' => 'Indifferent ',
					'total' => '37',
					'percent' => '8.1%',
					'bold' => false
				),
				5 => array(
					'item' => 'Unsure',
					'total' => '52',
					'percent' => '11.4%',
					'bold' => false
				),
			)
		);
		$data['node']['questions'][] = array(
			'question' 	=> 'Which of the following issues is most important to you? Please pick one',
			'total' => 461,
			'labels' => array(
				0 => array(
					'item' => 'Taxes & spending',
					'total' => '71',
					'percent' => '15.4%',
					'bold' => true
				),
				1 => array(
					'item' => 'Economy & jobs',
					'total' => '97',
					'percent' => '21.1%',
					'bold' => false
				),
				2 => array(
					'item' => 'Infrastructure improvements',
					'total' => '45',
					'percent' => '9.8%',
					'bold' => false
				),
				3 => array(
					'item' => 'Crime & public safety',
					'total' => '59',
					'percent' => '12.7%',
					'bold' => false
				),
				4 => array(
					'item' => 'Healthcare',
					'total' => '65',
					'percent' => '14.2%',
					'bold' => false
				),
				5 => array(
					'item' => 'Education',
					'total' => '40',
					'percent' => '8.7%',
					'bold' => false
				),
				6 => array(
					'item' => 'Social issues',
					'total' => '21',
					'percent' => '4.6%',
					'bold' => false
				),
				7 => array(
					'item' => 'Unsure',
					'total' => '62',
					'percent' => '13.5%',
					'bold' => false
				),
			)
		);*/
		return _survey_cygnal_toplines_word( $data );
	}

	


	function _survey_cygnal_toplines_word( $data ){
		$filename =_survey_cygnal_toplines_word_file( $data );
		return $filename;	
	}


	function _survey_cygnal_toplines_word_file( $data ){	
		global $base_url;
		$path = DRUPAL_ROOT;
		$path_module = drupal_get_path('module', 'survey_cygnal');
		$path_libraries = libraries_get_path('vendor'); 
		require_once $path_libraries."/autoload.php";		
		# ------------------------------------------------------------------
		if (!class_exists('WORD')) {
			class WORD {

				private $PHPWord;
				private $path;
				private $base_url;
				private $path_module;
				private $path_libraries;
				private $node;
				private $section;
				private $namefile;

				public function WORD( $params ){
					$this->PHPWord 			= $params['PHPWord'];
					$this->path 			= $params['path'];
					$this->base_url 		= $params['base_url'];
					$this->path_module 		= $params['path_module'];
					$this->path_libraries 	= $params['path_libraries'];
					$this->node 			= $params['node'];
					$this->namefile 		= $params['namefile'];
				}

				public function word_styles(){
					$this->PHPWord->addParagraphStyle('pStyle', array(
						'spaceAfter'=>40,
						'spacing'=>40					
					));
					$this->PHPWord->addParagraphStyle('pStyleCenter', array(
						'spaceAfter'=>40,
						'spacing'=>40, 
						'align'=>'center'
					));
					$this->PHPWord->addParagraphStyle('pStyleR', array(
						'spaceAfter'=>30, 
						'spacing'=>30,						
						'align'=>'right'
					));
					$this->PHPWord->addParagraphStyle('listStyle', array(
						'spaceAfter'    =>  0,
                        'spaceBefore'   =>  0,
                        'lineHeight'	=> 1.0,
                        'color' 		=> '635552',
                        //'content'		=> '', //?
                        //'format'		=> 'none', //?
                        //'type'			=> 'bullet',
                        //'underline'		=> 'dotted'
					));

					
					$this->PHPWord->addFontStyle('fontHeader', array(
						'name'=>'Verdana', 
						'size'=>14, 
						'color'=>'283747',
						'bold'=>false 
					));
					$this->PHPWord->addFontStyle('fontNormal', array(
						'name'=>'Verdana', 
						'size'=>9, 
						'color'=>'1B2631',
					));
					$this->PHPWord->addFontStyle('fontBold', array(
						'name'=>'Verdana', 
						'size'=>9, 
						'color'=>'1B2631',
						'bold'=>true 
					));					
					$this->PHPWord->addFontStyle('fontZone', array(
						'name'=>'Verdana', 
						'size'=>14, 
						'color'=>'1B5E20',
						'italic'=>true,
						'bold'=>true 
					));
					$this->PHPWord->addFontStyle('fontDate', array(
						'name'=>'Verdana', 
						'size'=>8, 
						'color'=>'1B5E20',
						'italic'=>true
					));
					$this->PHPWord->addFontStyle('fontRed', array(
						'color'=>'D32F2F',
						'bold'=>true 
					));
				}

				public function word_section(){
					// Información del documento.
					$properties = $this->PHPWord->getDocInfo();
					$properties->setCreator('INGS: Alfonso Chavez - Alberto Chaves');
					$properties->setCompany('web-dream');
					$properties->setTitle('Cygnal Pulse Survey Platform');
					$properties->setDescription('My description');
					$properties->setCategory('My category');
					$properties->setLastModifiedBy('My name');
					$properties->setCreated(date("Y-m-d H-i-s"));
					$properties->setModified(date("Y-m-d H-i-s"));
					$properties->setSubject('My subject');
					$properties->setKeywords('my, key, word');

					$sectionStyle = array(
					    //'orientation' => 'landscape', //(portrait, landscape)
						'marginLeft'=>1000, 
						'marginTop'=>800, 
						'marginRight'=>800, 
						'marginBottom'=>800, 
						//'borderColor'=>'F2F4F4', 
						//'borderSize'=>12,
					    //'colsNum' => 2,
					);
					$this->section = $this->PHPWord->addSection($sectionStyle);
				}

				public function word_header(){
					$header = $this->section->createHeader();
					
					$table = $header->addTable();
					$table->addRow();
					$table->addCell(4500)->addImage($this->node['logo_cygnal'], array('width'=>100, 'height'=>56, 'align'=>'left'));
					if ($this->node['show_text_header']==1 and !empty($this->node['text_header'])) {
						$cell = $table->addCell(5500);
						$cell->addTextBreak();
						$cell->addText($this->node['text_header'], 'fontHeader', array('align'=>'right'));
					}
				}

				public function word_intro(){
					$max_width=9500;
					/*$this->PHPWord->addTableStyle('tableOne', array(
						'cellMargin'=>80, 
						'borderSize'=>6, 
						'borderColor'=>'D5D8DC', 
						'bgColor'=>'cccccc'
					));*/
					$table = $this->section->addTable('tableOne');
					$table->addRow(600);
					$cell = $table->addCell(10000);

					$text = $cell->createTextRun('pStyle');
					$text->addText(htmlspecialchars($this->node['text_zone'], ENT_COMPAT, 'UTF-8'), 'fontZone');
					
					$text = $cell->createTextRun('pStyle');
					$text->addText(htmlspecialchars($this->node['text_date'], ENT_COMPAT, 'UTF-8'), 'fontBold');

					$text = $cell->createTextRun('pStyle');
					$text->addText(htmlspecialchars($this->node['text_info'], ENT_COMPAT, 'UTF-8'), 'fontBold');

					$this->section->addTextBreak();
					
					$lineStyle = array('weight' => 1, 'width' => 500, 'height' => 0, 'color' => '4E4A4A', 'align'=>'center');
					$this->section->addLine($lineStyle);
				}

				public function word_question(){
					/*echo "<pre>";
					print_r($this->node['questions']);
					echo "</pre>";
					die;*/
					$tablestyle = array(
						'borderSize' => 1, 
						'borderColor' => 'D5D8DC', 
						'afterSpacing' => 40, 
						'Spacing'=> 40, 
						'cellMargin'=> 40  
					);
					$cells = array(
						'cellMargin'=>80, 
						'borderSize'=>6, 
						'borderColor'=>'D5D8DC', 
						'bgColor'=>'dbe7f3'
					);

					if (!empty($this->node['questions'])) {
						$i=1;
						foreach ($this->node['questions'] as $key => $val) {
							// La pregunta.
							$textrun = $this->section->createTextRun('pStyle');
							$textrun->addText('Q'.$i, 'fontRed');
							$textrun->addText(htmlspecialchars(' - '.$val['question'], ENT_COMPAT, 'UTF-8'), 'fontBold');

							$table = $this->section->addTable($tablestyle);
							
							$table->addRow();
							$cell = $table->addCell(2500, $cells);
							$cell->addText( htmlspecialchars('Total', ENT_COMPAT, 'UTF-8'), 'fontBold');
							$cell = $table->addCell(2500, $cells);
							$cell->addText( htmlspecialchars($val['total'], ENT_COMPAT, 'UTF-8'), 'fontNormal', 'pStyleR');

							foreach ($val['labels'] as $k => $v) {
								$table->addRow();
								$textstyle='fontNormal';
								if ($v['bold']) {
									$textstyle='fontBold';
								}
								$cell = $table->addCell(2500);
								$cell->addText( htmlspecialchars($v['item'], ENT_COMPAT, 'UTF-8'), $textstyle);
								$cell = $table->addCell(2500);
								$cell->addText( htmlspecialchars($v['total'], ENT_COMPAT, 'UTF-8'), $textstyle, 'pStyleR');
								#$this->section->addTextBreak();
								$cell->addText( htmlspecialchars($v['percent']."%", ENT_COMPAT, 'UTF-8'), $textstyle, 'pStyleR');
							}
							$this->section->addTextBreak();
							$i++;
						}
					}
					

				}

				public function word_footer(){
					// Add footer
					$footer = $this->section->createFooter();
					$footer->addPreserveText('Page {PAGE} of {NUMPAGES}.', array('align'=>'center'));
				}

				public function word_generate(){
					
					error_reporting(0);
					$docname = $this->namefile.'-Toplines-'.date("Ymd-His").'.docx';
					$document = $this->path.'/'.$this->path_module.'/tmp/docs/'.$docname;
					$route		=$this->base_url.'/'.$this->path_module.'/tmp/docs/'.$docname;

					$objWriter = \PhpOffice\PhpWord\IOFactory::createWriter($this->PHPWord, 'Word2007');
					$objWriter->save($document);

					#$objWriter = \PhpOffice\PhpWord\IOFactory::createWriter($this->PHPWord, 'ODText');
					#$objWriter->save($document.'.odt');

					#$objWriter = \PhpOffice\PhpWord\IOFactory::createWriter($this->PHPWord, 'HTML');
					#$objWriter->save($document.'.html');
					header('Location:'.  $route );
					exit(0);
				}

				public function print_document(){
					$this->word_styles();
					$this->word_section();					
					$this->word_header();
					$this->word_intro();
					$this->word_question();
					$this->word_footer();
					$this->word_generate();
				}

			}
		}

		// Creating the new document...
		$PHPWord = new \PhpOffice\PhpWord\PhpWord();
		$params = array(
			'PHPWord' 			=> $PHPWord, 
			'path' 				=> $path,
			'base_url' 			=> $base_url, 
			'path_module' 		=> $path_module, 
			'path_libraries' 	=> $path_libraries, 
			'node' 				=> $data['node'],
			'namefile'			=> $data['node']['text_project_id'],
		);
		$objWORD = new WORD( $params );
		$objWORD->print_document();

	}