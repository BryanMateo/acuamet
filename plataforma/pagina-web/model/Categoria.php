<?php 

require "../config/Conexion.php";

class Categoria {

    public $cnx;

    function __construct()
    {
        $this->cnx = Conexion::ConectarDB();
    }

    //FUNCION PARA LISTAR LOS DATOS DE LAS CATEGORIAS
    function ListarCategorias()
    {
        $query = "SELECT * FROM categoria";
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

    //FUNCION PARA OBTENER UNA CATEGORIA POR ID
    function ObtenerCategoriaPorId($categoria_id)
    {
        $query = "SELECT * FROM categoria WHERE categoria_id = ? ";
        $result = $this->cnx->prepare($query);
        $result->bindParam(1,$categoria_id);
        if($result->execute()){
            return $result->fetch(PDO::FETCH_ASSOC);
        }
        return false;
    }

    //FUNCION PARA REGISTRAR UNA CATEGORIA
    function RegistrarCategoria($nombre,$descripcion)
    {
        $query = "INSERT INTO categoria(nombre,descripcion) VALUES (?, ?)";
        $result = $this->cnx->prepare($query);
        $result->bindParam(1,$nombre);
        $result->bindParam(2,$descripcion);
        if($result->execute())
        {
            return true;
        }
        return false;
    }

    //FUNCION PARA ACTUALIZAR UN REGISTRO
    function ActualizarCategoria($categoria_id,$nombre,$descripcion)
    {
        $query = "UPDATE categoria SET nombre= ?, descripcion = ? WHERE categoria_id = ?";
        $result = $this->cnx->prepare($query);
        $result->bindParam(1,$nombre);
        $result->bindParam(2,$descripcion);
        $result->bindParam(3,$categoria_id);
        if($result->execute()){
            return true;
        } 
        return false;
    }

    //FUNCION PARA ELIMINAR LOGICAMENTE UN REGISTRO
    function EliminarCategoria($categoria_id)
    {
        $query = "UPDATE categoria SET estado = 0 WHERE categoria_id= ?";
        $result = $this->cnx->prepare($query);
        $result->bindParam(1,$categoria_id);
        if($result->execute()){
            return true;
        }
        return false;
    }

    //FUNCION PARA ACTIVAR LOGICAMENTE UN REGISTRO
    function ActivarCategoria($categoria_id)
    {
        $query = "UPDATE categoria SET estado = 1 WHERE categoria_id= ?";
        $result = $this->cnx->prepare($query);
        $result->bindParam(1,$categoria_id);
        if($result->execute()){
            return true;
        }
        return false;
    }

    //FUNCION PARA LISTAR CATEGORIAS EN UN SELECT
    function ListarCategoriasSelect()
    {
        $query = "SELECT * FROM categoria WHERE estado = 1";
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

    function ListarCategoriasSelectEdit($subcategoria_id)
    {
        $query = "SELECT cat.categoria_id,cat.nombre FROM categoria cat 
        INNER JOIN subcategoria subcat ON cat.categoria_id = subcat.categoria_id 
        WHERE subcat.subcategoria_id = ? UNION SELECT categoria_id,nombre FROM 
        categoria WHERE estado = 1";
        $result = $this->cnx->prepare($query);
        $result->bindParam(1,$subcategoria_id);
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