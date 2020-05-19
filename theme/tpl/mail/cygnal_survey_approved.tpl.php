<?php 
  global $base_url;
  $path = drupal_get_path('module', 'survey_cygnal');
  /*require_once "$path/includes/pqrd.pdf.inc"; 
  $mensaje=_pqrd_pdf_replace($data["mensaje"],$data);//Se hacen los reemplazos que apliquen
  $caracteres=strlen($mensaje);*/
  
?>

<table border="0" cellpadding="0" cellspacing="0" height="100%" width="100%" style="height:100%;margin:0;padding:0;width:100%;border-collapse:collapse;background-color:#f5f5f5">
    <tbody><tr>
        <td align="center" valign="top" style="height:100%;margin:0;padding:0;width:100%;padding-top:40px;padding-bottom:40px">
            <table border="0" cellpadding="0" cellspacing="0" width="600" style="border-collapse:separate;background-color:#ffffff;border:1px solid #dddddd">
        <tbody><tr>
                    <td align="center" valign="bottom">
                        <table border="0" cellpadding="0" cellspacing="0" width="100%" style="border-collapse:collapse">
                            <tbody><tr>
                                <td align="center" valign="bottom">
                                    
                                    <table border="0" cellpadding="0" cellspacing="0" width="600" style="border-collapse:collapse">
                                        <tbody><tr>
                                            <td valign="bottom" width="600" style="padding-top:20px;padding-right:20px;padding-left:20px">
                                                <table align="Left" border="0" cellpadding="0" cellspacing="0" width="260" style="border-collapse:collapse">
                                                    <tbody><tr>
                                                        <td valign="bottom" style="color:#444;font-weight:600;letter-spacing:-1px;vertical-align:bottom;text-align:left">
                                                            <h3 style="margin:0;padding:0;color:#53585f;font-family:Helvetica;font-size:25px;line-height:100%;text-align:left"><span class="il"></span> Cygnal </h3>
                                                        </td>
                                                    </tr>
                                                </tbody></table>
                                            </td>
                                        </tr>
                                        
                                    </tbody></table>
                                    <hr width="95%" style="display:block;min-height:1px;border:0;border-top:1px solid #ccc;padding:0;width:560px">
                                    
                                </td>
                            </tr>
                        </tbody></table>
                        
                    </td>
                </tr>         
                <tr>
                    <td align="center" valign="top">
                        
                        <table border="0" cellpadding="0" cellspacing="0" width="100%" style="border-collapse:collapse">
                            <tbody><tr>
                                <td align="center" valign="top">
                                    
                                    <table border="0" cellpadding="0" cellspacing="0" width="600" style="border-collapse:collapse">
                                        <tbody><tr>
                                            <td align="center" valign="top" width="600" style="padding-top:20px;padding-right:20px;padding-left:20px">

                                                <table border="0" cellpadding="0" cellspacing="0" width="100%" style="border-collapse:collapse">
                                                    <tbody><tr>
                                                        <td valign="top" style="padding-bottom:0px">
                                                            <img width="560" style="max-width:560px;border:0;outline:none;text-decoration:none;min-height:auto">
                                                        </td>
                                                    </tr>
                                                </tbody></table>
                                                
                                            </td>
                                        </tr>
                                    </tbody></table>
                                    
                                </td>
                            </tr>
                        </tbody></table>
                        
                    </td>
                </tr>
                <tr>
                    <td align="center" valign="top">
                        <table border="0" cellpadding="0" cellspacing="0" width="100%" style="border-collapse:collapse">
                            <tbody><tr>
                                <td align="center" valign="top">
                                    <table border="0" cellpadding="0" cellspacing="0" width="600" style="border-collapse:collapse">
                                        <tbody><tr>
                                            <td align="center" valign="top" width="600" style="padding-top:0px;padding-right:20px;padding-left:20px">
                                                <table border="0" cellpadding="0" cellspacing="0" width="100%" style="border-collapse:collapse">
                                                    <tbody><tr>
                                                        <td valign="top" style="color:#404040;font-family:Helvetica;font-size:16px;line-height:125%;text-align:left;padding-bottom:5px">
                                                            <h1 style="margin:0;padding:0;color:#53585f;font-family:Helvetica;font-size:30px;line-height:100%;text-align:center;margin-bottom:0.8em;text-transform:uppercase"><?=utf8_decode("Cygnal")?></h1>                                                            
                                                        </td>
                                                    </tr>
                                                </tbody></table></td>
                                        </tr>
                                    </tbody></table>
                                </td>
                            </tr>
                        </tbody></table>
                        
                    </td>
                </tr>
                <tr>
                    <td align="center" valign="top">
                       <table border="0" cellpadding="0" cellspacing="0" width="600" style="border-collapse:collapse">
                                        <tbody><tr>
                                          <td align="center" valign="middle" style="padding-top:20px;padding-right:20px;padding-left:20px">
                                              <div style="font-size:14px">
                                                <p style="margin:1em 0;color:#53585f;font-family:Helvetica;text-align:justify; line-height:20px;"> <?php // echo utf8_decode($mensaje);?></p>
                                              </div>
                                            </td>
                                        </tr>   
                                    </tbody></table>
                    </td>
                </tr>
                <tr>
                    <td align="center" valign="top">
                        
                        <table border="0" cellpadding="0" cellspacing="0" width="100%" style="border-collapse:collapse">
                            <tbody><tr>
                                <td align="center" valign="top" style="padding-top:3em;padding-bottom:1em">&nbsp;
                                </td>
                            </tr>
                        </tbody></table>
                        
                    </td>
                </tr>
                

            </tbody></table>
            
        </td>
    </tr>
    
</tbody></table>