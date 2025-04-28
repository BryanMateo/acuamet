<?php

//  require "../config/Conexion.php";
require(__DIR__ . '/../config/Conexion.php');

class Usuario {

    public $cnx;

    function __construct()
    {
        $this->cnx = Conexion::ConectarDB();
    }

    function RegistrarUsuario($n_documento,$nombre,$apellidos,$correo,$clave)
    {
        $query = "INSERT INTO usuarios(n_documento,nombre,apellidos,correo,clave) VALUES (?, ?, ?, ?, ?)";
        $result = $this->cnx->prepare($query);
        $result->bindParam(1,$n_documento);
        $result->bindParam(2,$nombre);
        $result->bindParam(3,$apellidos);
        $result->bindParam(4,$correo);
        $clave_hash = password_hash($clave,PASSWORD_DEFAULT);
        $result->bindParam(5,$clave_hash);
        if($result->execute()){
            return true;
        }
        return false;
    }


    function ValidarUsuario($correo,$clave)
    {
        $query = "SELECT * FROM usuarios WHERE correo = ? ";
        $result = $this->cnx->prepare($query);
        $result->bindParam(1,$correo);
        $result->execute();
        $fila = $result->fetch();
        if(password_verify($clave,$fila["clave"])){
            return $fila;
        }
        return false;
    }

}




?>