<?php

require "../model/Clientes.php";

$cat = new Clientes();

switch($_REQUEST["operador"]){

    case "listar_clientes":

        $datos = $cat->ListarClientes();
        if($datos){
            for($i=0; $i<count($datos); $i++){
                $list[] = array (
                    "Valvula OFF" =>'<div class="btn-group">
                                <button class="btn btn-info dropdown-toggle btn-sm" data-toggle="dropdown" 
                                    aria-haspopup="true" aria-expanded="true"><i class="icon-gear"></i>
                                </button>
                                <div class="dropdown-menu">
                                    <a class="dropdown-item"><i class="icon-gear"></i> Valvula Apt.1</a>
                                    <a class="dropdown-item"><i class="icon-gear"></i> Valvula Apt.2</a>
                                    <a class="dropdown-item"><i class="icon-gear"></i> Valvula Apt.3</a>
                                    <a class="dropdown-item"><i class="icon-gear"></i> Valvula Apt.4</a>
                                </div>
                    </div>',
                    "Valvula ON" =>'<div class="btn-group">
                                <button class="btn btn-info dropdown-toggle btn-sm" data-toggle="dropdown" 
                                    aria-haspopup="true" aria-expanded="true"><i class="icon-gear"></i>
                                </button>
                                <div class="dropdown-menu">
                                    <a class="dropdown-item"><i class="icon-gear"></i> Valvula Apt.1</a>
                                    <a class="dropdown-item"><i class="icon-gear"></i> Valvula Apt.2</a>
                                    <a class="dropdown-item"><i class="icon-gear"></i> Valvula Apt.3</a>
                                    <a class="dropdown-item"><i class="icon-gear"></i> Valvula Apt.4</a>
                                </div>
                    </div>',
                    "ID" =>$datos[$i]['cliente_id'],
                    "Apt1" =>$datos[$i]['acum_apt1'],
                    "Apt2" =>$datos[$i]['acum_apt2'],
                    "Apt3" =>$datos[$i]['acum_apt3'],
                    "Apt4" =>$datos[$i]['acum_apt4']

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

}

?>