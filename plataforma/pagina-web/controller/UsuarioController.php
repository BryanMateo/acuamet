<?php 
session_start();
require "../model/Usuario.php";

$usu = new Usuario();

switch($_REQUEST["operador"]){


    case "validar_usuario":

            if(isset($_POST["correo"],$_POST["clave"]) && !empty($_POST["correo"]) && !empty($_POST["clave"])){

                if($user = $usu->ValidarUsuario($_POST["correo"],$_POST["clave"])){
                    foreach($user as $campos => $valor){
                        $_SESSION["user"][$campos] = $valor;
                    }
                    $response = "success";
                } else {
                    $response = "notfound";
                }

            } else {
                $response ="requerid";
            }

            echo $response;

    break;


    case "cerrar_sesion":

            unset($_SESSION["user"]);
            header("location:../");

    break;

}


?>