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
	function _survey_cygnal_script_file(){
		$nid=0;
		
	 	if(is_numeric(arg(4))){#Se asigna el id como el $nid directo
	 			$nid=arg(4); //Asignación directa
	 	}
	   
	 	if(is_numeric($nid) && $nid!=0){#Si se encontre el $nid del nodo de la cuota, se pasa a la construcción del archivo
	 		$node=node_load($nid); #load the node with information of the quota
	 		$datos=_survey_cygnal_script_get_info_node($node);#Llena la estructura de generación del archivo de quotas con la información del nodo
	 		_survey_cygnal_script_build_file($datos);#Inicia la construcción del archivo de Script, este archivo sera un word
	 	}else{
	 		echo "Data Not Found ";#Si retorna esto, es porque el id enviado no es correcto, o la Quota no existe
	 		return false;
	 	}

	 	return true;	
	}

	/**
	 * Función: Llena una estructura con la información de la Quota obtenida desde el nodo de la Quota enviado
	 */
	function _survey_cygnal_script_get_info_node($node){
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
		$datos['node']['demographics_text']	="[FROM VOTER FILE]";		
		
		/*echo "<pre>";
		echo "<br>---------<br>";
		print_r($datos);		
		echo "<br>---------<br>";
		echo "</pre>";
		exit(0);*/

		return $datos;
	}


	function _survey_cygnal_script_build_file($datos){
	

		// Los logos a escala ratio is: (16 : 9)
		#$path = drupal_get_path('module', 'survey_cygnal');
		#require_once $path."/includes/survey_cygnal.word.inc";
		
		$data=$datos;
		/*$data = array();
		$data['node']['logo_cygnal'] 		= $path."/theme/img/logo_cygnal_transparent.png";
		$data['node']['logo_custamer'] 		= $path."/theme/img/logos/logo_empresa.png";
		$data['node']['text_header'] 		= "Cygnal Pulse Survey Platform";

		$data['node']['show_text_header'] 	= 1; // 0:No  1:Si

		$data['node']['text_zone'] 			= "Florida Statewide";
		$data['node']['text_client'] 		= "Frontline Strategies";
		$data['node']['text_status'] 		= "- APROVED -";
		$data['node']['text_project_id'] 	= "171229L-FLS-FL";
		$data['node']['text_title'] 		= "Survey of Likely General Election Voters";
		$data['node']['text_date'] 			= $fecha;
		$data['node']['text_intro'] 		= "Hi. I’m _________________ with Seminole [SIM-uh-nole] Research. We are conducting an issue survey in Florida today and need your help. Your confidential responses will play an important role in the legislative process.";
		
		# ---------------------------------------------------------------------------
		# PREGUNTA 1
		$data['node']['questions'][] = array(
			'question_type' 	=> '2',	
			'question_head' 	=> array(),
			'question' 			=> 'How likely are you to vote in the 2018 General Election for Senate, Congress, Governor, state legislature, and local offices next November?',
			'answer_head' 		=> array(),
			'answers' => array(
				'a' => array(
					'answer_text' => 'Definitely voting', 
					'answer_info' => ''
				),
				'b' => array(
					'answer_text' => 'Probably voting',
					'answer_info' => ''
				),
				'c' => array(
					'answer_text' => 'Probably not voting',
					'answer_info' => ''
				),
				'd' => array(
					'answer_text' => 'Definitely not voting',
					'answer_info' => ''
				),
				'e' => array(
					'answer_text' => 'Not sure',
					'answer_info' => '[DO NOT READ]'
				),
			),
			'question_footer' => array(
				'IF DEF VOTING, PROB VOTING OR NOT SURE, GO TO Q2',
				'IF NOT VOTING, SAY “Thank You” AND TERMINATE'
			),
		);

		
		# ---------------------------------------------------------------------------
		// ADICIONALES.
		$data['node']['aditionals'][] = array(
			'aditional_text' => 'Congressional District',
			'aditional_info' => '[FROM VOTER FILE]',
		);
		*/

		return _survey_cygnal_script_word( $data );
	}

	function _survey_cygnal_script_word( $data ){
		$filename =_survey_cygnal_script_word_file( $data );
		return $filename;	
	}


	function _survey_cygnal_script_word_file( $data ){	
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
						'spaceAfter'=>40, 
						'spacing'=>40,						
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
						'name'=>'Arial', 
						'size'=>13, 
						'color'=>'283747',
						'bold'=>false 
					));
					$this->PHPWord->addFontStyle('fontNormal', array(
						'name'=>'Arial', 
						'size'=>11, 
						'color'=>'1B2631',
					));
					$this->PHPWord->addFontStyle('fontBold', array(
						'name'=>'Arial', 
						'size'=>11, 
						'color'=>'1B2631',
						'bold'=>true 
					));

					$this->PHPWord->addFontStyle('fontBoldTitle', array(
						'name'=>'Arial', 
						'size'=>12, 
						'color'=>'1B2631',
						'bold'=>true 
					));

					$this->PHPWord->addFontStyle('fontZone', array(
						'name'=>'Arial', 
						'size'=>14, 
						'color'=>'1B5E20',
						'italic'=>true,
						'bold'=>true 
					));
					$this->PHPWord->addFontStyle('fontDate', array(
						'name'=>'Arial', 
						'size'=>8, 
						'color'=>'1B5E20',
						'italic'=>true
					));
					$this->PHPWord->addFontStyle('fontRed', array(
						'color'=>'FF0000',
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
						'marginLeft'=>1200, 
						'marginTop'=>600, 
						'marginRight'=>1200, 
						'marginBottom'=>1200, 
						//'borderColor'=>'F2F4F4', 
						//'borderSize'=>12
					    //'colsNum' => 2,
					);
					$this->section = $this->PHPWord->addSection($sectionStyle);
				}

				public function word_header(){
					$header = $this->section->createHeader();
					
					$table = $header->addTable();
					$table->addRow();
					$table->addCell(4000)->addImage($this->node['logo_cygnal'], array('width'=>100, 'height'=>56, 'align'=>'left'));
					/*if ($this->node['show_text_header']==1 and !empty($this->node['text_header'])) {
						$cell = $table->addCell(5500);
						$cell->addTextBreak();
						$cell->addText($this->node['text_header'], 'fontHeader', array('align'=>'right'));
					}*/

					if(!empty($this->node['text_status'])){
						$cell = $table->addCell(5500);
						//$cell->addTextBreak();
						$cell->addText($this->node['text_status'], 'fontHeader', array('align'=>'center'));
					}
					/*$text->addText('Status: ', 'fontNormal');
					$text->addText(htmlspecialchars($this->node['text_status'], ENT_COMPAT, 'UTF-8'), 'fontBold');
					$text = $cell->createTextRun('pStyleR');*7

					/*$header->addWatermark($this->path.'/'.$this->path_module.'/theme/img/body2.png', array(
						'marginTop' => -200, 
						'marginLeft' => -300
					));*/
				}

				public function word_intro(){
					$max_width=9500;
					$this->PHPWord->addTableStyle('tableOne', array(
						'cellMargin'=>80, 
						//'borderSize'=>6, 
						//'borderColor'=>'D5D8DC', 
						//'bgColor'=>'ECEFF1'
					));
					$table = $this->section->addTable('tableOne');
					$table->addRow(600);

					$cell = $table->addCell(5500);					
					$cell = $table->addCell(4000);
					
					
					// proyecti ID
					$text = $cell->createTextRun('pStyleR');
					$text->addText('Project ID: ', 'fontBold');
					$text->addText(htmlspecialchars($this->node['text_project_id'], ENT_COMPAT, 'UTF-8'), 'fontNormal');
					$this->section->addTextBreak();
					
					// Cliente
					$text = $cell->createTextRun('pStyleR');
					$text->addText('CLIENT: ', 'fontBold');
					$text->addText(htmlspecialchars($this->node['text_client'], ENT_COMPAT, 'UTF-8'), 'fontNormal');
					
					

					// Distrito
					$this->section->addText(htmlspecialchars($this->node['text_district'], ENT_COMPAT, 'UTF-8'), 'fontZone', 'pStyleCenter');
					// Título
					$this->section->addText(htmlspecialchars("Survey of ".$this->node['text_survey_of'], ENT_COMPAT, 'UTF-8'), 'fontBoldTitle', 'pStyleCenter');
					// Fecha
					$this->section->addText(htmlspecialchars($this->node['text_date'], ENT_COMPAT, 'UTF-8'), 'fontDate', 'pStyleCenter');
					$this->section->addTextBreak();

					if(trim($this->node['text_intro'])!=""){
						$textrun = $this->section->createTextRun('pStyle');
						$textrun->addText('INTRO - ', 'fontRed');
						// Intro
						$textrun->addText(htmlspecialchars($this->node['text_intro'], ENT_COMPAT, 'UTF-8'), 'fontNormal');

						$this->section->addTextBreak();
					}

					$lineStyle = array('weight' => 1, 'width' => 450, 'height' => 0, 'color' => '4E4A4A', 'align'=>'center');
					$this->section->addLine($lineStyle);
				}

				public function word_question(){
					//Inicio de Question
					$i=1;
					foreach ($this->node['questions'] as $key => $val) {

						if ( isset($val['question']) and !empty($val['question'])) {
							// Encabezado de la pregunta.
							if ( !empty($val['question_head']) ) {
								foreach ($val['question_head'] as $k => $v) {
									$textrun = $this->section->createTextRun('pStyle');
									$textrun->addText(htmlspecialchars($v, ENT_COMPAT, 'UTF-8'), 'fontRed');
								}
								$this->section->addTextBreak();
							}
							

							// La pregunta.
							$textrun = $this->section->createTextRun('pStyle');
							$textrun->addText('Q'.$i, 'fontRed');
							$textrun->addText(htmlspecialchars(' - '.$val['question'], ENT_COMPAT, 'UTF-8'), 'fontBold');

							// Cabeceras de pregunta.
							if ( !empty($val['answer_head']) ) {
								foreach ($val['answer_head'] as $k => $v) {
									//$listItemRun = $this->section->addListItemRun(0, null, 'listStyle');
									//$listItemRun->addText(htmlspecialchars($v, ENT_COMPAT, 'UTF-8'), 'fontRed', null, 'listStyle');
									$textrun = $this->section->createTextRun('pStyle');
									$textrun->addText('      ', 'fontRed');									
									$textrun->addText(htmlspecialchars($v, ENT_COMPAT, 'UTF-8'), 'fontRed');

								}
							}

							// Las respuestas.
							foreach ($val['answers'] as $k => $v) {
								$listItemRun = $this->section->addListItemRun(0, null, 'listStyle');

								$listItemRun->addText(htmlspecialchars($v['answer_text'].' ', ENT_COMPAT, 'UTF-8'), 'fontNormal', null, 'listStyle');
								if ( !empty($v['answer_info']) && $v['answer_info']!=".") {
									$listItemRun->addText(htmlspecialchars($v['answer_info'], ENT_COMPAT, 'UTF-8'), 'fontRed', null, 'listStyle');
								}
							}

							// Comentarios de pie de página.
							if ( !empty($val['question_footer']) ) {
								$this->section->addTextBreak();
								foreach ($val['question_footer'] as $k => $v) {
									$textrun = $this->section->createTextRun('pStyle');
									$textrun->addText( htmlspecialchars( $v, ENT_COMPAT, 'UTF-8'), 'fontRed');
								}
							}
							$this->section->addTextBreak();
							$i++;
						}

					}
					//Final de Questions

					// Comentarios de pie de página.
					if ( !empty($this->node['text_close']) ) {
						$this->section->addTextBreak();
						
						$lineStyle = array('weight' => 1, 'width' => 450, 'height' => 0, 'color' => '4E4A4A', 'align'=>'center');
						$this->section->addLine($lineStyle);

						$textrun = $this->section->createTextRun('pStyle');
						$textrun->addText( htmlspecialchars( 'CLOSE - ', ENT_COMPAT, 'UTF-8'), 'fontRed');
						$textrun->addText( htmlspecialchars( $this->node['text_close'], ENT_COMPAT, 'UTF-8'), 'fontNormal');

						$this->section->addTextBreak();
					}


					// Instrucciones de demographics.
					if ( !empty($this->node['demographics_instruction']) ) {						
						$textrun = $this->section->createTextRun('pStyle');
						$textrun->addText(htmlspecialchars($this->node['demographics_instruction'], ENT_COMPAT, 'UTF-8'), 'fontRed');
						$this->section->addTextBreak();
					}
					
					// Demographics
					$i=1;					
					foreach ($this->node['demographics'] as $key => $val) {

						if ( isset($val['demographics']) and !empty($val['demographics'])) {
													

							// La pregunta.
							$textrun = $this->section->createTextRun('pStyle');
							$textrun->addText('D'.$i, 'fontRed');
							$textrun->addText(htmlspecialchars(' - '.$val['demographics'], ENT_COMPAT, 'UTF-8'), 'fontBold');
							$textrun->addText(strtoupper(htmlspecialchars(' '.$this->node['demographics_text'], ENT_COMPAT, 'UTF-8')), 'fontRed');
							
							// Las respuestas.
							/*foreach ($val['answers'] as $k => $v) {
								$listItemRun = $this->section->addListItemRun(0, null, 'listStyle');

								$listItemRun->addText(htmlspecialchars($v['answer_text'].' ', ENT_COMPAT, 'UTF-8'), 'fontNormal', null, 'listStyle');
								if ( !empty($v['answer_info']) && $v['answer_info']!=".") {
									$listItemRun->addText(htmlspecialchars($v['answer_info'], ENT_COMPAT, 'UTF-8'), 'fontRed', null, 'listStyle');
								}
							}*/
							$this->section->addTextBreak();
							$i++;
						}

					}	
					//Final de Demographics







					// Adicionales.
					/*if ( isset($this->node['aditionals']) and !empty($this->node['aditionals']) ) {
						$this->section->addTextBreak();
						
						$lineStyle = array('weight' => 1, 'width' => 450, 'height' => 0, 'color' => '4E4A4A', 'align'=>'center');
						$this->section->addLine($lineStyle);

						$i=1;
						foreach ($this->node['aditionals'] as $key => $val) {
							// La pregunta.
							$textrun = $this->section->createTextRun('pStyle');
							$textrun->addText('D'.$i, 'fontRed');
							$textrun->addText(htmlspecialchars(' - '.$val['aditional_text'], ENT_COMPAT, 'UTF-8'), 'fontNormal');
							if ( isset($val['aditional_info']) and !empty($val['aditional_info']) ) {
								$textrun->addText(htmlspecialchars(' '.$val['aditional_info'], ENT_COMPAT, 'UTF-8'), 'fontRed');
							}
							$i++;
						}
					}*/
				}

				public function word_footer(){
					// Add footer
					$footer = $this->section->createFooter();
					$footer->addPreserveText('Page {PAGE} of {NUMPAGES}.', array('align'=>'center'));
				}

				public function word_generate(){
					error_reporting(0);
					$docname = $this->namefile.'-Script-'.date("Ymd-His").'.docx';
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


	

