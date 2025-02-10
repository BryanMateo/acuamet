<?php 

require "../config/Conexion.php";

class SubCategoria {

    public $cnx;

    function __construct()
    {
        $this->cnx = Conexion::ConectarDB();
    }

    //FUNCION PARA LISTAR LOS DATOS DE LAS SUBCATEGORIAS
    function ListarSubCategorias()
    {
        $query = "SELECT sub.subcategoria_id,sub.nombre,ca.nombre AS categoria,sub.estado FROM categoria ca 
        INNER JOIN subcategoria sub ON ca.categoria_id = sub.categoria_id";
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
	//FUNCION PARA OBTENER UNA SUBCATEGORIA POR ID
    function ObtenerSubCategoriaPorId($subcategoria_id)
    {
        $query = "SELECT * FROM subcategoria WHERE subcategoria_id = ?";
        $result = $this->cnx->prepare($query);
        $result->bindParam(1,$subcategoria_id);
        if($result->execute()){
            return $result->fetch(PDO::FETCH_ASSOC);
        } 
        return false;
    }
    //FUNCION PARA REGISTRAR UNA SUBCATEGORIA
    function RegistrarSubCategoria($nombre,$categoria_id)
    {
        $query = "INSERT INTO subcategoria(nombre,categoria_id) VALUES (?,?)";
        $result = $this->cnx->prepare($query);
        $result->bindParam(1,$nombre);
        $result->bindParam(2,$categoria_id);
        if($result->execute()){
            return true;
        }
        return false;
    }
    //FUNCION PARA ELIMINAR LOGICAMENTE UN REGISTRO DE SUB CATEGORIAS
    function EliminarSubCategoria($subcategoria_id)
    {
        $query = "UPDATE subcategoria SET estado = 0 WHERE subcategoria_id= ?";
        $result = $this->cnx->prepare($query);
        $result->bindParam(1,$subcategoria_id);
        if($result->execute()){
            return true;
        }
        return false;
    }
    //FUNCION PARA ACTIVAR LOGICAMENTE UN REGISTRO DE SUB CATEGORIAS
    function ActivarSubCategoria($subcategoria_id)
    {
        $query = "UPDATE subcategoria SET estado = 1 WHERE subcategoria_id= ?";
        $result = $this->cnx->prepare($query);
        $result->bindParam(1,$subcategoria_id);
        if($result->execute()){
            return true;
        }
        return false;
    }


}



?>