<?php
	/* -----------------------------------------------------------------------------------------
	| GENERADOR ENCUESTAS PDF.
	|-------------------------------------------------------------------------------------------
	| @copyright: Achb
	| @author: ALFONSO CHAVEZ B <alfonso.chb@gmail.com>
	| @Description: Archivo para funciones de generacion de pdf.
	|-------------------------------------------------------------------------------------------
	*/
	
function _survey_cygnal_send_mail($data){
	 
	$message["To"]=$data["To"];					// A quien va dirigido el correo
	$message["ToName"]=$data["ToName"];				// Nombre del propietario del correo	
	$message["Subject"]=$data["Subject"];	        // Asunto
	$message["Body"]=$data["Body"];					// Mensaje completo, incluido todo el codigo HTML 
	$message["WithAttach"]=$data["WithAttach"];		// Mensaje completo, incluido todo el codigo HTML 
	
	if($data["WithAttach"]){							//Evalua si el correo solicita un attach	
		$message["Attach"]=$data["Attach"];				// Ruta del archivo que se Adjunta, con el nombre del archivo
		$message["AttachName"]=$data["AttachName"];		// Nombre que aparecera en el archivo Adjunto
		$message["AttachBase"]=$data["AttachBase"];		// Base usada para enviar el archivo
		$message["AttachType"]=$data["AttachType"];// Tipo de archivo o Mime a utilizar en el adjunto					
	}	
	
	$return=_survey_cygnal_send_mail_finaly($message);
	return $return;
 }
 
/**
 * @function
 * Funcion que envia correos a los usuarios indicados
 */
  
 function _survey_cygnal_approved_mail($data){
	
	if(isset($data["users"])){
		foreach($data["users"] as $usuario){	
							
			 $message=array( 
				//"To"=>$usuario["Mail"], 	// A quien va dirigido el correo		
				"To"			=>"mackpipe@gmail.com",																	
				//"ToName"		=>$user_data->field_user_first_name[LANGUAGE_NONE][0]['value'].' '.$user_data->field_user_last_name[LANGUAGE_NONE][0]['value'],					// Nombre del propietario del correo	
				"ToName"		=>$usuario["firstname"].' '.$usuario["lastname"],					// Nombre del propietario del correo	
				"Subject"		=>$data["subject"],	        	// Asunto
				"Body"			=>_survey_cygnal_approved_html_mail($data),	// Mensaje completo, incluido todo el codigo HTML 					
				"WithAttach"	=>$data["withAttach"],
				"Attach"		=>$data["attach"]["attach"],
				"AttachName"	=>$data["attach"]["attachName"],
				"AttachBase"	=>$data["attach"]["attachBase"],
				"AttachType"	=>$data["attach"]["attachType"],
				);	
					
			_survey_cygnal_send_mail($message);	
		}
	}//Fin if de usuarios  
	
}
 
/**
 * @function
 * Funcion que construye el html que va a incluirse en el mail en la opcion Body, estafuncion
 * hace el llamado a las funciones que construyen la cebecera, el cuertopo y el pie del correo,
 * No hay necesidad de modificarla, ya que el objetivo es mantener una estrucutra sintactica uniforme.
 * y esta funcion retorna esta estructura 
 */
function _survey_cygnal_approved_html_mail($data){
	$output =_survey_cygnal_approved_html_mail_body($data);
	return $output;	 
 }
 
 function _survey_cygnal_approved_html_mail_header(){	
	$output=_survey_cygnal_html_mail_header();	
	return $output;	 
 }

 function _survey_cygnal_approved_html_mail_body($data){
	global $base_url;
	
	$output = theme('survey_mail_approved',array('data' => $data)); 
	return $output;	
 }

 function _survey_cygnal_approved_html_mail_footer(){	
	$output=_survey_cygnal_html_mail_footer();	
	return $output;	 
 }


function _survey_cygnal_send_mail_finaly($message){ 
	global $base_url;	
	$path=DRUPAL_ROOT;
	$path_libraries=libraries_get_path('phpmailer');	
	require_once $path.'/'.$path_libraries.'/'.'class.phpmailer.php';
	$mail = new PHPMailer();
	
	$mail->IsHTML(true);							   // Habilita el envio de HTML	
	$mail->SMTPAuth   = true;                          // Habilita la autenticación del servidor SMTP
	
	$info["smtp_host"]=variable_get("smtp_host");
	$info["smtp_port"]=variable_get("smtp_port");
	$info["smtp_username"]=variable_get("smtp_username");
	$info["smtp_password"]=variable_get("smtp_password");
	$info["smtp_from"]=variable_get("smtp_from");
	$info["smtp_fromname"]=variable_get("smtp_fromname");
	$info["ToName"]=$message["ToName"];
	$info["Subject"]=$message["Subject"];
	$info["Body"]=$message["Body"];
	$info["WithAttach"]=$message["WithAttach"];
	$info["Attach"]=$message["Attach"];
	$info["AttachName"]=$message["AttachName"];
	$info["AttachBase"]=$message["AttachBase"];
	$info["AttachType"]=$message["AttachType"];


	#$mail->SMTPDebug = 1;
	$mail->IsSMTP(); 								   // Si es protocolo SMTP pide Host, puerto y datos si es autenticado.	
	$mail->SMTPSecure = 'ssl'; 
	$mail->Host       = variable_get("smtp_host");     // Define el host 
	$mail->Port       = variable_get("smtp_port");     // Define el puerto por el cual se va a comunicar con el SMTP
	$mail->Username   = variable_get("smtp_username"); // Usuario con acceso a SMTP
	$mail->Password	  = variable_get("smtp_password"); // Clave del usuario	 
	
	$mail->Helo 	  = "mackpipe@gmail.com";//variable_get("smtp_from");	   // Muy importante para que llegue a hotmail y otros
	$mail->From		  = "mackpipe@gmail.com";//variable_get("smtp_from");     // Desde que correo se envia el Mail. 
	$mail->FromName	  = "Alberto Chaves";//variable_get("smtp_fromname"); // Nombre de la persona dueña de la cuenta saliente	
	$mail->AddAddress($message["To"], $message["ToName"]);	
	//$mail->AddCC("chaves-9@hotmail.com","Wilson Chaves");
	//$mail->AddBCC("mackpipe@hotmail.com","Wilson Chaves");
	$mail->Subject = $message["Subject"];
	$mail->MsgHTML($message["Body"]);
	//Evalua si se definio un attach para enviar.
	if($message["WithAttach"]){
		$mail->AddAttachment($message["Attach"],$message["AttachName"], $message["AttachBase"], $message["AttachType"]);
	}
	
	if(!$mail->Send()) {
		exit(0);
		drupal_set_message("Error sending mail",'error');	
		return false;
	}	
	return true;
}


/**
 * @function
 * Funcion que construye la cebecera del html, esta cabecera se considera una cabecera generica, para todos los 
 * Correos, en caso de querer una cebecera con logos o estructura diferente, no se debe incluir el llamado de esta
 * funcion, y en cambio construir en el modulo propiamente, la funcion que construye esta cabecera.
 */
function _survey_cygnal_html_mail_header(){
	global $base_url; 
	$path = drupal_get_path('module', 'atenea');
	$output="<table width='100%' border='0'>";
		$output .="<tr style='background:#4B97ED;'>";
			$output .="<td style='text-align:center; padding:5px;'>";
				$output .="<img src='".$base_url.'/'.$path."/theme/img/logo_cygnal_transparent.png' width='80' height='20'>";
			$output .="</td>";
		$output .="</tr>";
	$output .="</table>";
	return $output;	 
 }


/**
 * @function
 * Funcion que construye el footer del html, este footer se considera un pie de pagina generico, para todos los 
 * Correos, en caso de querer un pie de pagina con logos o estructura diferente, no se debe incluir el llamado de esta
 * funcion, y en cambio construir en el modulo propiamente, la funcion que construye este pie de pagina.
 */
 function _survey_cygnal_html_mail_footer(){
	global $base_url; 
	$path = drupal_get_path('module', 'atenea'); 
	$output="<table width='100%' border='0'>";
		$output .="<tr>";
			$output .="<td width='100'>";
				//$output .="<img src='".$base_url.'/'.$path."/theme/images/mail/logo_interactivo.png' width='99' height='80'>";
			$output .="</td>";
			$output .="<td width='100%' style='font-size:10px; text-align:justify; color:#666;'>";
				$output .="Protecci&oacute;n de Datos Con la expedici&oacute;n de la ley 1581 de 2012 y el Decreto 1377 de 2013, se desarrolla el principio constitucional que tienen todas las personas a conocer, actualizar y rectificar todo tipo de informaci&oacute;n recogida o, que haya sido objeto de tratamiento de datos personales en bancos o bases de datos y, en archivos de entidades p&uacute;blicas y/o privadas.En El Evento En Que Usted Considere Que Nuestra Empresa ha dado uso inadecuado o contrario al autorizado y a las leyes aplicables, podr&aacute; contactarnos a trav&eacute;s de una comunicaci&oacute;n motivada dirigida a la Gerencia general de la Compa&ntilde;&iacute;a a trav&eacute;s del correo contactenos@interactivo.com.co";
			$output .="</td>";
		$output .="</tr>";
	$output .="</table>";
	return $output;	
 }


