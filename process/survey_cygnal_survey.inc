<?php
	/* -----------------------------------------------------------------------------------------
	| Generación de archivo de Encuesta.
	|-------------------------------------------------------------------------------------------
	| @copyright: Achb
	| @author: ALFONSO CHAVEZ B <alfonso.chb@gmail.com>
	| @Description: Este archivo contiene todas las funciones necesarias y suficientes para 
	|               la generación del documento de una encuesta.
	|-------------------------------------------------------------------------------------------
	*/
	

	/**
	 * Función: principal que es llamada desde el enlace directo desde el listado de encuestas
	 *          o desde el registro directo de la Quota
	 */
	function _survey_cygnal_survey_file(){
		$nid=0;
		
	 	if(is_numeric(arg(4))){#Se asigna el id como el $nid directo
	 			$nid=arg(4); //Asignación directa
	 	}
	  
	 	if(is_numeric($nid) && $nid!=0){#si el $nid existe y es numerico y diferente de cero consulta el nodo
	 		$node=node_load($nid); #load the node with information of the quota	 		
	 		$datos=_survey_cygnal_survey_get_info_node($node);#Llena la estructura de generación del archivo de quotas con la información del nodo
	 		_survey_cygnal_survey_build_file($datos);#Inicia la construcción del archivo de Script, este archivo sera un word
	 	}else{
	 		echo "Data Not Found ";#Si retorna esto, es porque el id enviado no es correcto, o la Quota no existe
	 		return false;
	 	}

	 	return true;	
	}

/**
 * Función: Llena una estructura con la información de la Quota obtenida desde el nodo de la Quota enviado
 */
function _survey_cygnal_survey_get_info_node($node){
		$path = drupal_get_path('module', 'survey_cygnal');
		


		$fecha = date("Y-m-d");
		$fecha = date("l, F j, Y").' - '.date('l, F j, Y', strtotime($fecha. ' + 5 days'));

		$datos=array();
		$questions=array();
		$demographics=array();
		
		$text_client="";
		if(isset($node->field_survey_consumer[LANGUAGE_NONE])){
			$id_consumer=$node->field_survey_consumer[LANGUAGE_NONE][0]["nid"];
			$node_consumer=node_load($id_consumer);
			//$term_customer=taxonomy_term_load($id_customer);
			$text_client=$node_consumer->field_consumer_name[LANGUAGE_NONE][0]["value"];
			
			#Obtiene configuración del customer para generación del pdf
			if(isset($node_consumer->field_consumer_file_pdf[LANGUAGE_NONE])){
				$id_config_pdf=$node_consumer->field_consumer_file_pdf[LANGUAGE_NONE][0]["value"];
				$item_config_pdf=entity_load('field_collection_item', array($id_config_pdf));
				#Obtiene el colleccion de propiedades
				if(isset($item_config_pdf[$id_config_pdf]->field_cll_consumer_prope_file[LANGUAGE_NONE])){
					$id_show=$item_config_pdf[$id_config_pdf]->field_cll_consumer_prope_file[LANGUAGE_NONE][0]["value"];
					$item_show=entity_load('field_collection_item', array($id_show));
					
					#Obtiene la configuración de show / hide watermark
					$show_watermark=0;
					if(isset($item_show[$id_show]->field_cll_cll_prope_file_water[LANGUAGE_NONE])){
						$show_watermark=$item_show[$id_show]->field_cll_cll_prope_file_water[LANGUAGE_NONE][0]["value"];
					}

					#Obtiene la configuración de show / hide logo
					$show_customer_logo=0;
					if(isset($item_show[$id_show]->field_cll_cll_prope_file_logo_cu[LANGUAGE_NONE])){
						$show_customer_logo=$item_show[$id_show]->field_cll_cll_prope_file_logo_cu[LANGUAGE_NONE][0]["value"];
					}

					#Obtiene la configuración de show / hide logo
					$show_cygnal_logo=0;
					if(isset($item_show[$id_show]->field_cll_cll_prope_file_logo_cy[LANGUAGE_NONE])){
						$show_cygnal_logo=$item_show[$id_show]->field_cll_cll_prope_file_logo_cy[LANGUAGE_NONE][0]["value"];
					}

					#Obtiene la configuración de show / hide logo
					$show_header_pdf=0;
					if(isset($item_show[$id_show]->field_cll_cll_prope_file_header[LANGUAGE_NONE])){
						$show_header_pdf=$item_show[$id_show]->field_cll_cll_prope_file_header[LANGUAGE_NONE][0]["value"];
					}

					#Obtiene la configuración de show / hide logo
					$show_footer_message=0;
					if(isset($item_show[$id_show]->field_cll_cll_prope_file_footerm[LANGUAGE_NONE])){
						$show_footer_message=$item_show[$id_show]->field_cll_cll_prope_file_footerm[LANGUAGE_NONE][0]["value"];
					}

					#Obtiene la configuración de show / hide logo
					$show_date_creation=0;
					if(isset($item_show[$id_show]->field_cll_cll_prope_file_date[LANGUAGE_NONE])){
						$show_date_creation=$item_show[$id_show]->field_cll_cll_prope_file_date[LANGUAGE_NONE][0]["value"];
					}
				}
			}
			
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
		
		
		if(isset($node->field_survey_questions[LANGUAGE_NONE])){
			foreach ($node->field_survey_questions[LANGUAGE_NONE] as $key => $question) {
				$id_cll_question=$question["value"];
				$item_cll_question=entity_load('field_collection_item', array($id_cll_question));	
				
				
				
				/*if(isset($item_cll_question[$id_cll_question]->field_cll_survey_question_option[LANGUAGE_NONE])){
					
										
					foreach ($item_cll_question[$id_cll_question]->field_cll_survey_question_option[LANGUAGE_NONE] as $key_opt => $value_opt) {
						$id_option=$value_opt["value"];	
						$item_option=entity_load('field_collection_item', array($id_option));

						$answer_text="";
						if(isset($item_option[$id_option]->field_cll_survey_quest_opt_label[LANGUAGE_NONE])){
							$answer_text=$item_option[$id_option]->field_cll_survey_quest_opt_label[LANGUAGE_NONE][0]["value"];
						}

						$answer_info="";
						if(isset($item_option[$id_option]->field_cll_survey_quest_opt_desc[LANGUAGE_NONE])){
							$answer_info=$item_option[$id_option]->field_cll_survey_quest_opt_desc[LANGUAGE_NONE][0]["value"];
						}

						//$answers[]=array('answer_text' 		=> $answer_text, 
						//				 'answer_info' 		=> $answer_info
						//				);
						$answers[]=$answer_text;
					}
				}*/

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

					$answers=array();
					if(isset($item_question[$id_question]->field_cll_survey_question_option[LANGUAGE_NONE])){

						$id_option=$item_question[$id_question]->field_cll_survey_question_option[LANGUAGE_NONE][0]["value"];	
						$item_option=entity_load('field_collection_item', array($id_option));

						#Arma la estructura de las opciones de respuesta de la pregunta
						/*echo "<pre>";
						print_r($item_option[$id_option]);
						echo "</pre>";
						exit(0);*/
						
						$answer_text="";
						if(isset($item_option[$id_option]->field_cll_survey_quest_opt_txt[LANGUAGE_NONE])){
							$answer_text=$item_option[$id_option]->field_cll_survey_quest_opt_txt[LANGUAGE_NONE][0]["value"];
						}

						$answers = explode("\r\n", $answer_text);

						/*
						$answer_info="";
						if(isset($item_option[$id_option]->field_cll_survey_quest_opt_desc[LANGUAGE_NONE])){
							$answer_info=$item_option[$id_option]->field_cll_survey_quest_opt_desc[LANGUAGE_NONE][0]["value"];
						}*/

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

		/*echo "<pre>";
		print_r($questions);
		echo "</pre>";
		exit(0);

		if(isset($node->field_survey_demographics[LANGUAGE_NONE])){
			foreach ($node->field_survey_demographics[LANGUAGE_NONE] as $key_add => $demographics) {
				$id_cll_demographics=$demographics["value"];
				$item_cll_demographics=entity_load('field_collection_item', array($id_cll_demographics));

				#Arma la estructura de las preguntas
				if(isset($item_cll_demographics[$id_cll_demographics]->field_survey_question_control[LANGUAGE_NONE])){
					$id_demographics=$item_cll_demographics[$id_cll_demographics]->field_survey_question_control[LANGUAGE_NONE][0]["value"];	
					$item_demographics=entity_load('field_collection_item', array($id_demographics));


				$aditional_text="";
				if(isset($item_add_question[$id_add_question]->field_cll_survey_quest_question[LANGUAGE_NONE])){
					$aditional_text=$item_add_question[$id_add_question]->field_cll_survey_quest_question[LANGUAGE_NONE][0]["value"];
				}

				$aditional_info="";
				if(isset($item_add_question[$id_add_question]->field_cll_survey_quest_options[LANGUAGE_NONE])){
					$aditional_info=$item_add_question[$id_add_question]->field_cll_survey_quest_options[LANGUAGE_NONE][0]["value"];
				}
				

				$demographics[]=array(
					'aditional_text' 		=> $aditional_text,
					'aditional_info' 		=> $aditional_info,
				);
			}	
		}*/
		
		$datos['node']['logo_cygnal'] 		= $path."/theme/img/logo_cygnal_transparent.png";
		$datos['node']['logo_custamer'] 	= $path."/theme/img/logos/logo_empresa.png";
		$datos['node']['text_header'] 		= "Cygnal Pulse Survey Platform";
		$datos['node']['show_text_header'] 	= 1; // 0:No  1:Si
		$datos['node']['text_district'] 	= $text_district;
		$datos['node']['text_survey_of'] 	= $text_survey_of;
		$datos['node']['text_client'] 		= $text_client;
		$datos['node']['text_status'] 		= "- ".$text_status." -";
		$datos['node']['text_project_id'] 	= $text_project_id;
		$datos['node']['text_title'] 		= $node->title;
		$datos['node']['text_date'] 		= $fecha;		
		$datos['node']['text_intro']		= $text_intro;		
		$datos['node']['questions']			= $questions;
		$datos['node']['text_close']		= $text_close;		
		$datos['node']['demographics']		= $demographics;		



		$datos['node']['text_color_title'] 				= "39, 55, 70";
		$datos['node']['text_color_title_one'] 			= "211, 84, 0";
		$datos['node']['text_color_title_two'] 			= "51, 51, 51";
		$datos['node']['text_color_text'] 				= "23, 32, 42";
		$datos['node']['text_color_text_script_header'] = "74, 35, 90";
		$datos['node']['text_color_text_script_footer']	= "0, 0, 0";
		$datos['node']['text_color_num_question'] 		= "255, 0, 0";
		$datos['node']['text_color_question'] 			= "51, 51, 51";
		$datos['node']['text_color_response'] 			= "39, 55, 70";

		$datos['node']['watermark'] 					= $path."/theme/img/body2.png";
		$datos['node']['logo_custamer'] 				= $path."/theme/img/logos/logo_empresa.png";
		$datos['node']['logo_cygnal'] 					= $path."/theme/img/logo_cygnal_transparent.png";
		$datos['node']['header_pdf'] 					= $text_client; // Indicar valor o asignar en vacio. (Cygnal Pulse Survey Platform)
		$datos['node']['creation_date'] 				= date("F j, Y");
		$datos['node']['footer_message'] 				= "All rights reserved CYGNAL";

		
		$datos['node']['show_watermark'] 				= $show_watermark; // 0:No  1:Si
		$datos['node']['show_customer_logo'] 			= $show_customer_logo; // 0:No  1:Si
		$datos['node']['show_cygnal_logo'] 				= $show_cygnal_logo; // 0:No  1:Si
		$datos['node']['show_header_pdf'] 				= $show_header_pdf; // 0:No  1:Si
		$datos['node']['show_number_pages'] 			= 1; // 0:No  1:Si
		$datos['node']['show_footer_message'] 			= $show_footer_message; // 0:No  1:Si
		$datos['node']['show_date_creation'] 			= $show_date_creation; // 0:No  1:Si



		$datos['node']['text_project_id'] 	= $text_project_id;
		$datos['node']['questions']			= $questions;
		$datos['node']['quotas']			=array(); 
		$datos['node']['title'] 			= $node->title;
		//$datos['node']['presentation'] 		= $node->body[LANGUAGE_NONE][0]["value"];
		$datos['node']['fecha_creacion'] 	= "Fecha creación: ".$fecha;
		$datos['node']['script_start'] 		= $text_intro;
		$datos['node']['script_end'] 		= $text_close;

		/*echo "<pre>";
		print_r($datos);
		echo "</pre>";
		exit(0);*/

		return $datos;
}

function _survey_cygnal_survey_build_file($datos,$out="display"){


	// Los logos a escala ratio is: (16 : 9)
	$path = drupal_get_path('module', 'survey_cygnal');
	//require_once $path."/includes/survey_cygnal.pdf.inc";
	$data= $datos;
	/*$data = array();
	# Colores de texto, en formato rgb ("xxx, xxx, xxx")
	$data['node']['text_color_title'] 				= "39, 55, 70";
	$data['node']['text_color_title_one'] 			= "211, 84, 0";
	$data['node']['text_color_title_two'] 			= "52, 73, 94";
	$data['node']['text_color_text'] 				= "23, 32, 42";
	$data['node']['text_color_text_script_header'] 	= "74, 35, 90";
	$data['node']['text_color_text_script_footer'] 	= "0, 0, 0";
	$data['node']['text_color_question'] 			= "20, 90, 50";
	$data['node']['text_color_response'] 			= "39, 55, 70";

	$data['node']['watermark'] 						= $path."/theme/img/body2.png";
	$data['node']['logo_custamer'] 					= $path."/theme/img/logos/logo_empresa.png";
	$data['node']['logo_cygnal'] 					= $path."/theme/img/logo_cygnal_transparent.png";
	$data['node']['header_pdf'] 					= "Empresa Promesa LTDA."; // Indicar valor o asignar en vacio. (Cygnal Pulse Survey Platform)
	$data['node']['creation_date'] 					= date("F j, Y");
	$data['node']['footer_message'] 				= "All rights reserved CYGNAL";

	$data['node']['show_watermark'] 				= 1; // 0:No  1:Si
	$data['node']['show_customer_logo'] 			= 1; // 0:No  1:Si
	$data['node']['show_cygnal_logo'] 				= 1; // 0:No  1:Si
	$data['node']['show_header_pdf'] 				= 1; // 0:No  1:Si
	$data['node']['show_number_pages'] 				= 1; // 0:No  1:Si
	$data['node']['show_footer_message'] 			= 1; // 0:No  1:Si

	$data['node']['title'] = 'Encuesta para medir las tendencias del consumo de las personas en tiempo actual';
	$data['node']['presentation'] = "Esté es un procedimiento para la recogida de información en una población concreta y a su vez la técnica que se utiliza para guiar la recogida de la misma. La finalidad es presentar un ejemplo de  como se visualizá una encuesta realizada por CYGNAL. Por favor, invierta unos pocos minutos de su tiempo para rellenar el siguiente cuestionario.";
	$data['node']['fecha_creacion'] = "Fecha creación: ".date("Y-m-d");

	$data['node']['script_start'] = "Aquí se implementa el script de cabecera. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam ut neque gravida, suscipit risus sed, finibus lorem. Nulla leo dolor, feugiat sit amet magna in, dapibus eleifend risus. Suspendisse potenti. Pellentesque aliquet ullamcorper diam, eu egestas urna ullamcorper et. Praesent velit risus, elementum in eleifend eu, tincidunt non mauris. Fusce eget tempus est. Suspendisse imperdiet vel ex at sollicitudin.";
	$data['node']['script_end'] = "Aquí se implementa el script de pie de página. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam ut neque gravida, suscipit risus sed, finibus lorem. Nulla leo dolor, feugiat sit amet magna in, dapibus eleifend risus. Suspendisse potenti. Pellentesque aliquet ullamcorper diam, eu egestas urna ullamcorper et. Praesent velit risus, elementum in eleifend eu, tincidunt non mauris. Fusce eget tempus est. Suspendisse imperdiet vel ex at sollicitudin.";
	


	
	$data['node']['questions'][] = array(
		'question_type' => '1',
		'question' => '¿Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus tincidunt, purus vitae?',
		'answers' => array(
			'a' => 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam ut neque gravida, suscipit risus sed, finibus lorem. Nulla leo dolor, feugiat sit amet magna in, dapibus eleifend risus. Suspendisse potenti.',
			'b' => 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam ut neque gravida, suscipit risus sed, finibus lorem. Nulla leo dolor, feugiat sit amet magna in, dapibus eleifend risus. Suspendisse potenti. Pellentesque aliquet ullamcorper diam, eu egestas urna ullamcorper et.',
			'c' => 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam ut neque gravida, suscipit risus sed, finibus lorem. Nulla leo dolor, feugiat sit amet magna in, dapibus eleifend risus. Suspendisse potenti.',
			'd' => 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam ut neque gravida, suscipit risus sed.',
			'e' => 'Lorem ipsum dolor sit amet, consectetur adipiscing elit.',
			'f' => 'Lorem ipsum dolor sit amet.'
		)
	);
	$data['node']['questions'][] = array(
		'question_type' => '2',
		'question' => '¿Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus tincidunt, purus vitae viverra pharetra, orci nulla viverra urna, non bibendum elit libero ut risus.?',
		'answers' => array(
			'a' => 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam ut neque gravida, suscipit risus sed, finibus lorem. Nulla leo dolor, feugiat sit amet magna in, dapibus eleifend risus. Suspendisse potenti. Pellentesque aliquet ullamcorper diam, eu egestas urna ullamcorper et. Suspendisse potenti. Pellentesque aliquet ullamcorper diam, eu egestas urna ullamcorper et.',
			'b' => 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam ut neque gravida, suscipit risus sed, finibus lorem. Nulla leo dolor, feugiat sit amet magna in, dapibus eleifend risus. Suspendisse potenti. Pellentesque aliquet ullamcorper diam, eu egestas urna ullamcorper et.',
			'c' => 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam ut neque gravida, suscipit risus sed, finibus lorem. Nulla leo dolor, feugiat sit amet magna in, dapibus eleifend risus. Suspendisse potenti.',
			'd' => 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam ut neque gravida, suscipit risus sed.',
			'e' => 'Lorem ipsum dolor sit amet, consectetur adipiscing elit.',
			'f' => 'Lorem ipsum dolor sit amet.'
		)
	);
	$data['node']['questions'][] = array(
		'question_type' => '3',
		'question' => 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus tincidunt:',
		'answers' => array()
	);
	$data['node']['questions'][] = array(
		'question_type' => '4',
		'question' => 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus tincidunt:',
		'answers' => array()
	);
	$data['node']['questions'][] = array(
		'question_type' => '5',
		'question' => 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus tincidunt:',
		'answers' => array(
			'0' => 'Does not respond', // No responde
			'1' => 'Hardly ever', // Casi nunca
			'2' => 'Rarely', // Rara vez
			'3' => 'Ever', // Alguna vez
			'4' => 'Often', // A menudo
			'5' => 'Almost always' // Casi siempre
		)	
	);
	

	$data['node']['quotas']['columns'] = array('Quota group', 'Of respondents', 'Cellphone respondents', 'Landline respondents');
	$data['node']['quotas']['groups'][] = array('Edad (15 - 35)', '443', '4464', '56');
	$data['node']['quotas']['groups'][] = array('Hombres', '878', '566', '6');
	$data['node']['quotas']['groups'][] = array('Mujeres', '7767', '7777777', '565');
	$data['node']['quotas']['groups'][] = array('Catolicos', '46', '757', '775');
	$data['node']['quotas']['groups'][] = array('Con educación 778', '5757', '5757', '7757');
	$data['node']['quotas']['groups'][] = array('Otros', '88', '57', '57');*/

	return _survey_cygnal_survey_pdf( $data,$out);
}
	

function _survey_cygnal_survey_pdf( $data,$out="display" ){  
		$filename =_survey_cygnal_survey_pdf_file($data,$out);
		return $filename;	
	}


	function _survey_cygnal_survey_pdf_file( $data ,$out="display" ){	
		global $base_url;
		$path = DRUPAL_ROOT;
		$path_libraries = libraries_get_path('fpdf');
		$path_module = drupal_get_path('module', 'survey_cygnal');

		require_once $path.'/'.$path_libraries.'/'.'fpdf.php';  
		
		if (!class_exists('PDF')) {
			//Implementacion de Clase FPDF para sobreescribir los metodos Header y Footer
			class PDF extends FPDF
			{
				public $path;
				public $data;
				public $max_width=216;
				public $max_height=279;
				public $margin_top=15;
				public $margin_left=15;
				
				
				public function initSetData( $path, $data=array() ) {
					$this->path=$path;
					$this->data=$data;
				}

		

				// Cabecera de página
				public function Header() {
			        $fecha = !empty($this->data['node']['creation_date']) ? $this->data['node']['creation_date'] : date("F j, Y");
			        $header_pdf = !empty($this->data['node']['text_status']) ? $this->data['node']['text_status'] : "Cygnal Pulse Survey Platform";

			        # ----------------------------------------------------------------------
			        $this->SetDrawColor( 128, 139, 150 ); // Establece el color de graficación
			        $this->SetLineWidth( 0.9 );
			        $this->Line( 0, 0, 216, 0);

			        # ----------------------------------------------------------------------
			        # Los logos a escala ratio is: (16 : 9)
			        if ($this->data['node']['show_watermark']==1 and file_exists($this->data['node']['watermark'])) {
			        	$this->Image( $this->data['node']['watermark'], 0, 0, 216, 280); // Fondo de página.
			        }
			        if ($this->data['node']['show_customer_logo']==1 and file_exists($this->data['node']['logo_custamer'])) {
			        	$this->Image( $this->data['node']['logo_custamer'], $this->margin_left, 4, 0, 20); // Logo de la Empresa cliente.
			        }
			        if ($this->data['node']['show_cygnal_logo']==1 and file_exists($this->data['node']['logo_cygnal'])) {
			        	$this->Image( $this->data['node']['logo_cygnal'], 165, 4, 0, 20); // Logo de Cygnal.
			        }		        
			    
			        # ----------------------------------------------------------------------
			        # Designación del documento.
			        $this->SetFont('Arial', '', 16);
			        $this->SetTextColorRgb( $this->data['node']['text_color_title'] );
			        $this->SetY(10);
			        if ($this->data['node']['show_header_pdf']==1) {
				        $this->SetX(60);
				        $this->MultiCell( 100, 6, utf8_decode($header_pdf), 0, 'C', false);
			        }

			        # ----------------------------------------------------------------------
			        # Fecha en que se generá el documento.
			        $this->SetX(60);
			        $this->SetFont('Arial', '', 10);
			        $this->SetTextColorRgb( $this->data['node']['text_color_text'] );
					$this->MultiCell( 100, 6, $fecha, 0, 'C', false);

			        # ----------------------------------------------------------------------
			        # Linea de separación de cabecera y cuerpo de documento.
					$this->SetDrawColor(12, 87, 127);
					$this->SetLineWidth( 0.1 ); // mm
					$this->Line($this->margin_left, $this->margin_top+14, $this->max_width-15, $this->margin_top+14);

			        # ----------------------------------------------------------------------
			        # Cordenadas despúes del header.
					$this->SetXY($this->margin_left, 34);
				}

				public function Footer(){
					$this->SetXY($this->margin_left, 265);
					$this->SetFont('Arial', 'I', 8);
					$this->SetTextColorRgb( $this->data['node']['text_color_text'] );
					# ----------------------------------------------------------------------
					if ($this->data['node']['show_number_pages']==1) {
						$page = 'Page '.$this->PageNo().' of {nb}';
						$this->Cell(23, 8, utf8_decode( $page ), 0, 0, 'L');
					}else{
						$this->Cell(23, 8, utf8_decode( '' ), 0, 0, 'L');
					}
					if ($this->data['node']['show_footer_message']==1) {
						$this->Cell(163, 8, utf8_decode( $this->data['node']['footer_message'] ), 0, 0, 'R');
					}
					$this->Ln();
					/*
			        $this->SetLineWidth( 0.5 );
			        $this->Line( 15, 0, 15, 279);
			        $this->Line( 201, 0, 201, 279);
			        $this->Line( 15, 0, 201, 279);
			        $this->Line( 201, 0, 15, 279);
					*/
				}

				public function SetTextColorRgb( $colorRgb="0,0,0" ){
			        $rgb=explode(",", $colorRgb);
			        $this->SetTextColor( $rgb[0], $rgb[1], $rgb[2] );
				}

				public function putTitleOne( $text, $align="L"){
					$this->SetXY( $this->margin_left, $this->GetY() );
					$this->SetFont('Arial','B', 14);
					$this->SetTextColorRgb( $this->data['node']['text_color_title_one'] );
					$this->MultiCell( $this->max_width-30, 7, utf8_decode( $text ), 0, $align, false);
				}

				public function putTitleTwo( $text,$align="J" ){
					$this->SetXY( $this->margin_left, $this->GetY() );
			        $this->SetFont('Arial', 'B', 11);
			        $this->SetTextColorRgb( $this->data['node']['text_color_title_two'] );
					$this->MultiCell( $this->max_width-30, 5, utf8_decode( $text ), 0, $align, false);
				}

				public function putTitleQuestion( $pregNum, $text ){
					$this->SetXY( $this->margin_left, $this->GetY() );
			        $this->SetFont('Arial', 'B', 11);
			        $this->SetTextColorRgb( $this->data['node']['text_color_num_question'] );
			        $this->Cell(6, 6, $pregNum.".", 0, 0, 'L', false);
			        $this->SetTextColorRgb( $this->data['node']['text_color_question'] );
					$this->MultiCell( $this->max_width-36, 5, utf8_decode( $text ), 0, 'L', false);
				}

				public function putNormalText( $text ){
					$this->SetXY( $this->margin_left, $this->GetY() );
					$this->SetFont('Arial', 'I', 10);
					$this->SetTextColorRgb( $this->data['node']['text_color_text'] );
					$this->MultiCell( $this->max_width-30, 4, utf8_decode( $text ), 0, 'J', false);
				}

				/*public function putField( $field, $value ){
					$this->SetXY( $this->margin_left, $this->GetY() );
					$this->SetFont('Arial', 'I', 10);
					$this->SetTextColorRgb( $this->data['node']['text_color_text'] );
					$this->MultiCell( $this->max_width-30, 4, utf8_decode( $text ), 0, 'J', false);
				}*/

				public function putTextScriptHeader( $text ){
					$this->SetXY( $this->margin_left, $this->GetY() );
					$this->SetFont('Arial', 'I', 12);
					$this->SetTextColorRgb( $this->data['node']['text_color_text_script_header'] );
					$this->MultiCell( $this->max_width-30, 4, utf8_decode( $text ), 0, 'J', false);
				}

				public function putTextScriptFooter( $text ){
					$this->SetXY( $this->margin_left, $this->GetY() );
					$this->SetFont('Arial', 'I', 10);
					$this->SetTextColorRgb( $this->data['node']['text_color_text_script_footer'] );
					$this->MultiCell( $this->max_width-30, 4, utf8_decode( $text ), 0, 'J', false);
				}

				public function calculatesPageJump( $array ){

					$linea=6; // Altura de una linea de titulo.

					$maxPregunta=100;
					$totalChartsPr=strlen($array['question']);
					if ( $totalChartsPr>$maxPregunta) {
						$linea=(ceil( $totalChartsPr/$maxPregunta)*6);
					}

					if (count($array['answers'])>0 and $array['question_type']!=5) {
						$maxRespuesta=120;
						foreach ($array['answers'] as $key => $val) {
							$totalChartsLineRes=strlen( $val );
							if ( $totalChartsLineRes>$maxRespuesta ) {
								$linea+=(ceil( $totalChartsLineRes/$maxRespuesta)*4);
							}else{
								$linea+=4;
							}
						}
					}

					$linea+=(count($array['answers'])*2);// Total preguntas mas dos lineas de salto entre preguntas.
					$linea+=6; // Mas la altura en salto de pregunta.

					if ($array['question_type']==3) {
						$linea+=16; // Altura de la caja de texto.
					}	
					if ($array['question_type']==4) {
						$linea+=6; // Altura de la caja de texto.
					}
					if ($array['question_type']==5) {
						$linea+=8; // Altura de la caja de texto.
					}

					$y = $this->GetY();
					if ( ($y+$linea)>=265) {
						$this->AddPage();
					}
					return;
				}

				public function putSimpleQuestion( $array, $pregNum ){
					$this->calculatesPageJump($array);
					$this->putTitleQuestion( $pregNum, $array['question'] );
					$this->Ln(4);
			        $this->SetFont('Arial', '', 10);
			        $this->SetTextColorRgb( $this->data['node']['text_color_response'] );
			        foreach ($array['answers'] as $key => $val) {
						$x = ($this->margin_left+7);
						$y = round( $this->GetY() );
						$this->Image($this->path.'/theme/img/radio.png', $x, $y, 4, 4);
						$this->SetXY( $x+5, $y );
						$this->MultiCell( $this->max_width-42, 4, utf8_decode( $val ), 0, 'J', false);
						$this->Ln(2);
			        }
				}

				public function putMultipleQuestion( $array, $pregNum ){
					$this->calculatesPageJump($array);
					$this->putTitleQuestion( $pregNum, $array['question'] );
					$this->Ln(4);
			        $this->SetFont('Arial', '', 10);
			        $this->SetTextColorRgb( $this->data['node']['text_color_response'] );
			        foreach ($array['answers'] as $key => $val) {
						$x = ($this->margin_left+7);
						$y = round( $this->GetY() );
						$this->Image($this->path.'/theme/img/checkbox.png', $x, $y, 4, 4);
						$this->SetXY( $x+5, $y );
						$this->MultiCell( $this->max_width-42, 4, utf8_decode( $val ), 0, 'J', false);
						$this->Ln(2);
			        }
				}

				public function putOpenQuestion( $array, $pregNum ){
					$this->calculatesPageJump($array);
					$this->putTitleQuestion( $pregNum, $array['question'] );
					$this->Ln(4);
			        $this->SetFont('Arial', '', 10);
			        $this->SetTextColorRgb( $this->data['node']['text_color_response'] );
			        $this->SetDrawColor( 191, 201, 202 );
			        $this->SetFillColor( 251, 252, 252 );
					$x = ($this->margin_left+7);
					$y = round( $this->GetY() );
					$this->SetXY( $x, $y );
					$this->MultiCell( $this->max_width-37, 12, utf8_decode( ' ' ), 1, 'L', true);
					$this->Ln(2);
				}

				public function putTrueFalseQuestion( $array, $pregNum ){
					$this->calculatesPageJump($array);
					$this->putTitleQuestion( $pregNum, $array['question'] );
					$this->Ln(2);

					$x = ($this->margin_left+8);
					$y = round( $this->GetY() );
			        $this->SetFont('Arial', 'B', 10);
			        $this->SetDrawColor( 191, 201, 202 );
			        $this->SetFillColor( 251, 252, 252 );
			        $this->SetTextColorRgb( $this->data['node']['text_color_response'] );
					$this->SetXY( $x, $y );
					$this->Image($this->path.'/theme/img/radio.png', 58, $y+2, 4, 4, "PNG");
					$this->Image($this->path.'/theme/img/radio.png', 146, $y+2, 4, 4, "PNG");
			        $this->Cell(89, 8, utf8_decode( 'YES' ), 1, 0, 'C');
			        $this->Cell(89, 8, utf8_decode( 'NOT' ), 1, 0, 'C');
			        $this->Ln(10);
				}

				public function putSixOptionsQuestion( $array, $pregNum ){
					$this->calculatesPageJump($array);
					$this->putTitleQuestion( $pregNum, $array['question'] );
					$this->Ln(2);
			        $this->SetFont('Arial', 'B', 10);
			        $this->SetDrawColor( 191, 201, 202 );
			        $this->SetFillColor( 251, 252, 252 );
			        $this->SetTextColorRgb( $this->data['node']['text_color_response'] );
			        # ----------------------------------------------------------------------
			        # Dibujar el cuerpo de la tabla.
				    $this->SetFillColor( 234, 237, 237 ); // color de celda
					$this->SetFont('Arial', '', 9);
					$this->SetTextColorRgb( $this->data['node']['text_color_text'] );
				    
					$x = ($this->margin_left+8);
					$this->SetXY( $x, $this->GetY() );
				    $n=0;
				    for ($i=0; $i<2; $i++) {
				    	$y = $this->GetY();
					    for ($j=0; $j<3; $j++) { 
					    	$this->Cell(8, 8, utf8_decode(' '), 1, 0, 'C');
					    	$this->Cell(51, 8, utf8_decode($array['answers'][$n]), 1, 0, 'L');
					    	$n++;
					    }
					    $this->Image($this->path.'/theme/img/radio.png', 25, $y+2, 4, 4, "PNG");
					    $this->Image($this->path.'/theme/img/radio.png', 84, $y+2, 4, 4, "PNG");
					    $this->Image($this->path.'/theme/img/radio.png', 143, $y+2, 4, 4, "PNG");
					    $this->Ln();
					    $this->SetXY( $x, $this->GetY() );
				    }
			        $this->Ln(2);
				}


				public function putPieGoogle(){
					$this->Image('http://chart.googleapis.com/chart?cht=p3&chd=t:60,40&chs=250x100&chl=Hello|World',60,30,90,0,'PNG');
				}

				public function setTablaQuotas( $header, $data ){
					$anchoTotal = ($this->max_width - 30);
					$anchoCel = floor( $anchoTotal/count($header) );
					$text = $anchoTotal.'='.$anchoCel;
			        # ----------------------------------------------------------------------
			        # Dibujar la cabecera de la tabla.
				    $this->SetX($this->margin_left);
				    $this->SetDrawColor( 93, 109, 126 );
				    $this->SetFillColor(86, 101, 115); // color de celda
					$this->SetFont('Arial','B',9);
					$this->SetTextColor( 255, 255, 255 );
					$this->MultiCell( $this->max_width-32, 2, utf8_decode( ' ' ), 0, 'J', false);
					for ($i=0; $i<count($header); $i++) { 
						$this->Cell($anchoCel, 6, $header[$i], 1, 0, 'C', true);
					}
					$this->Ln(6);
			        # ----------------------------------------------------------------------
			        # Dibujar el cuerpo de la tabla.
				    $this->SetFillColor( 234, 237, 237 ); // color de celda
					$this->SetFont('Arial', '', 9);
					$this->SetTextColorRgb( $this->data['node']['text_color_text'] );
				    $fill = false;
				    foreach($data as $row){
				    	foreach ($row as $key => $val) {
				    		$this->Cell($anchoCel, 6, utf8_decode($val), 1, 0, 'C', $fill);
				    	}
						$fill = !$fill;
				        $this->Ln(6);
				    }
				    $this->Ln(2);
				}

				public function printEncuesta(){

					$this->putTitleTwo("Project ID: ".$this->data['node']['text_project_id'],"R");
					$this->Ln( 1 );

					$this->putTitleTwo("CLIENT: ".$this->data['node']['text_client'],"R");
					$this->Ln( 1 );

					$this->putTitleOne($this->data['node']['text_district'],"C");
					$this->Ln( 1 );

					$this->putTitleTwo("Surcey of ".$this->data['node']['text_survey_of'],"C");
					$this->Ln( 4 );	

			        //$this->putTextScriptHeader( $this->data['node']['script_start'] );
			        //$this->Ln( 4 );

					$this->putTitleOne("Questions");
					$this->Ln( 2 );

					$i=1;
					foreach ($this->data['node']['questions'] as $key => $val) {
						# Recorrido de cada pregunta.
						switch ( $val['question_type'] ) {
							case 1: $this->putSimpleQuestion( $val, $i ); break;
							case 2: $this->putMultipleQuestion( $val, $i ); break;
							case 3: $this->putOpenQuestion( $val, $i ); break;
							case 4: $this->putTrueFalseQuestion( $val, $i ); break;
							case 5: $this->putSixOptionsQuestion( $val, $i ); break;
							default: break;
						}
				        $this->Ln(4);
				        $i++;
					}
					$this->Ln( 2 );

			        //$this->putTextScriptFooter( $this->data['node']['script_end'] );
			        //$this->Ln( 2 );

					# ----------------------------------------------------------------------
			        # Definir quotas.
					/*if (99999==99999) {
				        $this->AddPage();
						$this->putTitleOne("Example survey, Quotas section");
						$this->setTablaQuotas( 
							$this->data['node']['quotas']['columns'], 
							$this->data['node']['quotas']['groups']
						);
						$this->Ln( 2 );
					}*/
				}


			}
		}
		
		
		// Creación del objeto de la clase heredada
		$pdf = new PDF('P','mm','letter');
		$pdf->initSetData( $path_module, $data );

		$pdf->SetMargins(15, 0, 15);
		$pdf->AcceptPageBreak();
		$pdf->SetAutoPageBreak(true, 20);
		$pdf->AliasNbPages();
		$pdf->AddPage();		
		$pdf->printEncuesta();		
		$pdf->Close();
		
		$namefile=$data['node']['text_project_id'];			
		$docname 	= $namefile.'-Survey-'.date("Ymd-His").'.pdf';
		$document 	= $path.'/'.$path_module.'/tmp/pdf/'.$docname;						
		$route		= $base_url.'/'.$path_module.'/tmp/pdf/'.$docname;
		$doc=$pdf->Output($document,'F');			
		if($out=="display"){
			header('Location:'.  $route );
			exit(0);
		}else{
			return $document;
		}
		
	}