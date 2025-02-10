<?php

require "../model/SubCategoria.php";

$subcat = new SubCategoria();

switch($_REQUEST["operador"]){

    case "listar_subcategorias":

        $datos = $subcat->ListarSubCategorias();
        if($datos){
            for($i=0; $i<count($datos); $i++){
                $list[] = array (
                    "op"=>($datos[$i]['estado']==1)?
                            '<div class="btn-group">
                                <button class="btn btn-info dropdown-toggle btn-sm" data-toggle="dropdown" 
                                    aria-haspopup="true" aria-expanded="true"><i class="icon-gear"></i>
                                </button>
                                <div class="dropdown-menu">
                                    <a class="dropdown-item" data-toggle="modal" data-target="#update_subcategoria" 
                                    onclick="ObtenerSubCategoriaPorId('.$datos[$i]['subcategoria_id'].",'editar'".');">
                                    <i class="icon-edit"></i> Editar</a>
                                    <a class="dropdown-item" onclick="ObtenerSubCategoriaPorId('.$datos[$i]['subcategoria_id'].",'eliminar'".');"><i class="icon-trash"></i> Eliminar</a>
                                </div>
                        </div>':
                        '<div class="btn-group">
                        <button class="btn btn-info dropdown-toggle btn-sm" data-toggle="dropdown" 
                            aria-haspopup="true" aria-expanded="true"><i class="icon-gear"></i>
                        </button>
                        <div class="dropdown-menu">
                            <a class="dropdown-item" onclick="ObtenerSubCategoriaPorId('.$datos[$i]['subcategoria_id'].",'activar'".');"><i class="icon-check"></i> Activar</a>
                        </div>
                    </div>',
                    "id"=>$datos[$i]['subcategoria_id'],
                    "nombre"=>$datos[$i]['nombre'],
                    "categoria"=>$datos[$i]['categoria'],
                    "estado"=>($datos[$i]['estado'] == 1)?'<div class="tag tag-success">Activo</div>':
                                                        '<div class="tag tag-danger">Inactivo</div>'
                );  
            }
            $resultados = array(
                "sEcho" => 1,
                "iTotalRecords" => count($list),
                "iTotalDisplayRecords" => count($list),
                "aaData" => $list
            );
        }
        echo json_encode($resultados);

    break;

    case "obtener_subcategoria_por_id":

        if(isset($_POST["subcategoria_id"]) && !empty($_POST["subcategoria_id"])){
            $data = $subcat->ObtenerSubCategoriaPorId($_POST["subcategoria_id"]);
            if($data){
                $list[] = array(
                    "subcategoria_id"=>$data['subcategoria_id'],
                    "nombre"=>$data['nombre'],
                    "categoria_id"=>$data['categoria_id']
                );
                echo json_encode($list);
            }
        }

    break;

    case "registrar_subcategoria":

        if(isset($_POST["nombre"],$_POST["categoria_id"]) && !empty($_POST["nombre"]) && !empty($_POST["categoria_id"])){

            $nombre = $_POST["nombre"];
            $categoria_id = $_POST["categoria_id"];
            if($subcat->RegistrarSubCategoria($nombre,$categoria_id)){
                $response = "success";
            } else {
                $response = "error";
            }

        } else {
            $response = "requerid";
        }
        echo $response;
        
    break;

    case "eliminar_subcategoria":

        if(isset($_POST["subcategoria_id"]) && !empty($_POST["subcategoria_id"])){
            if($subcat->EliminarSubCategoria($_POST["subcategoria_id"])){
                $response = "success";
            } else {
                $response = "error";
            }
        } else {
            $response = "error";
        }
        echo $response;

    break;

    case "activar_subcategoria":

        if(isset($_POST["subcategoria_id"]) && !empty($_POST["subcategoria_id"])){
            if($subcat->ActivarSubCategoria($_POST["subcategoria_id"])){
                $response = "success";
            } else {
                $response = "error";
            }
        } else {
            $response = "error";
        }
        echo $response;

    break;

}


?>