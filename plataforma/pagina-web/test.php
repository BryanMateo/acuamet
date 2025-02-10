<?php

require("model/Usuario.php");

$usuario = new Usuario();

$n_documento = "12345";
$nombre = "Usuario1";
$apellidos = "Prueba1";
$correo = "test1@test.com";
$clave = "test123";

// if ($usuario->RegistrarUsuario($n_documento,$nombre,$apellidos,$correo,$clave)) {
//     echo "Usuario registrado con exito";
// } else {
//     echo "Error";
// }


if ($usuario->ValidarUsuario($correo,$clave)) {
    echo "Usuario encontrado";
} else {
     echo "No encontrado";
}

?>