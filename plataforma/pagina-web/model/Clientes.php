<?php 
 require "../config/Conexion.php";

class Clientes {
    public $cnx;

    function __construct()
    {
        $this->cnx = Conexion::ConectarDB();
    }

    function ListarClientes()
    {
        $query = "SELECT * FROM clientes";
        $result = $this->cnx->prepare($query);
        if($result->execute())
        {
            if($result->rowCount() > 0){
                while($fila = $result->fetch(PDO::FETCH_ASSOC)){
                    $datos[] = $fila;
                }
                return $datos;
            }
        }
        return false;
    }

}

?>