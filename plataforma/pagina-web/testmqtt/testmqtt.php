<?php

$url = "http://localhost:3000/cuenta/testphpdd";


switch($_REQUEST["operador"]){
    
    case "mqttbtn":
$ch = curl_init($url);
curl_setopt($ch, CURLOPT_POST, true);
curl_setopt($ch, CURLOPT_POSTFIELDS, ""); // Intentar un POST vacío
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
curl_setopt($ch, CURLOPT_HTTPHEADER, [ "Content-Length: 0" ]); // Forzar envío vacío

$response = curl_exec($ch);
curl_close($ch);
header("location:../");
    break;


// $response = file_get_contents($url, false, $context);

// // Mostrar la respuesta del servidor (opcional)
// echo $response;
}


?>

