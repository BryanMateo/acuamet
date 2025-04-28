<?php

require "../model/Clientes.php";

$cat = new Clientes();

switch($_REQUEST["operador"]){

    case "listar_clientes":

        $datos = $cat->ListarClientes();
        if($datos){
            for($i=0; $i<count($datos); $i++){
                $clienteId = $datos[$i]['cliente_id'];
                $list[] = array (
                    "Valvula OFF" => '
                        <div class="btn-group">
                            <button class="btn btn-info dropdown-toggle btn-sm" data-toggle="dropdown"
                                aria-haspopup="true" aria-expanded="true"><i class="icon-gear"></i>
                            </button>
                            <div class="dropdown-menu">
                                <form action="http://localhost:3000/api/apagar" method="GET" target="hidden_iframe" id="formApt1_' . $clienteId . '">
                                    <input type="hidden" name="cliente_id" value="' . $clienteId . '">
                                    <input type="hidden" name="target" value="Apt1">
                                </form>
                                <a class="dropdown-item" href="#" onclick="document.getElementById(\'formApt1_' . $clienteId . '\').submit();"><i class="icon-gear"></i> Valvula Apt.1</a>
                                
                                <form action="http://localhost:3000/api/apagar" method="GET" target="hidden_iframe" id="formApt2_' . $clienteId . '">
                                    <input type="hidden" name="cliente_id" value="' . $clienteId . '">
                                    <input type="hidden" name="target" value="Apt2">
                                </form>
                                <a class="dropdown-item" href="#" onclick="document.getElementById(\'formApt2_' . $clienteId . '\').submit();"><i class="icon-gear"></i> Valvula Apt.2</a>

                                <form action="http://localhost:3000/api/apagar" method="GET" target="hidden_iframe" id="formApt3_' . $clienteId . '">
                                    <input type="hidden" name="cliente_id" value="' . $clienteId . '">
                                    <input type="hidden" name="target" value="Apt3">
                                </form>
                                <a class="dropdown-item" href="#" onclick="document.getElementById(\'formApt3_' . $clienteId . '\').submit();"><i class="icon-gear"></i> Valvula Apt.3</a>

                                <form action="http://localhost:3000/api/apagar" method="GET" target="hidden_iframe" id="formApt4_' . $clienteId . '">
                                    <input type="hidden" name="cliente_id" value="' . $clienteId . '">
                                    <input type="hidden" name="target" value="Apt4">
                                </form>
                                <a class="dropdown-item" href="#" onclick="document.getElementById(\'formApt4_' . $clienteId . '\').submit();"><i class="icon-gear"></i> Valvula Apt.4</a>

                                <form action="http://localhost:3000/api/apagar" method="GET" target="hidden_iframe" id="formBomba_' . $clienteId . '">
                                    <input type="hidden" name="cliente_id" value="' . $clienteId . '">
                                    <input type="hidden" name="target" value="Bomba">
                                </form>
                                <a class="dropdown-item" href="#" onclick="document.getElementById(\'formBomba_' . $clienteId . '\').submit();"><i class="icon-gear"></i> Bomba</a>
                                
                            </div>
                        </div>
                        <iframe name="hidden_iframe" style="display:none;"></iframe>
                        ',
                    "Valvula ON" =>'
                        <div class="btn-group">
                            <button class="btn btn-info dropdown-toggle btn-sm" data-toggle="dropdown"
                                aria-haspopup="true" aria-expanded="true"><i class="icon-gear"></i>
                            </button>
                            <div class="dropdown-menu">
                                <form action="http://localhost:3000/api/encender" method="GET" target="hidden_frame" id="formApt1' . $clienteId . '">
                                    <input type="hidden" name="cliente_id" value="' . $clienteId . '">
                                    <input type="hidden" name="target" value="Apt1">
                                </form>
                                <a class="dropdown-item" href="#" onclick="document.getElementById(\'formApt1' . $clienteId . '\').submit();"><i class="icon-gear"></i> Valvula Apt.1</a>
                                
                                <form action="http://localhost:3000/api/encender" method="GET" target="hidden_frame" id="formApt2' . $clienteId . '">
                                    <input type="hidden" name="cliente_id" value="' . $clienteId . '">
                                    <input type="hidden" name="target" value="Apt2">
                                </form>
                                <a class="dropdown-item" href="#" onclick="document.getElementById(\'formApt2' . $clienteId . '\').submit();"><i class="icon-gear"></i> Valvula Apt.2</a>

                                <form action="http://localhost:3000/api/encender" method="GET" target="hidden_frame" id="formApt3' . $clienteId . '">
                                    <input type="hidden" name="cliente_id" value="' . $clienteId . '">
                                    <input type="hidden" name="target" value="Apt3">
                                </form>
                                <a class="dropdown-item" href="#" onclick="document.getElementById(\'formApt3' . $clienteId . '\').submit();"><i class="icon-gear"></i> Valvula Apt.3</a>

                                <form action="http://localhost:3000/api/encender" method="GET" target="hidden_frame" id="formApt4' . $clienteId . '">
                                    <input type="hidden" name="cliente_id" value="' . $clienteId . '">
                                    <input type="hidden" name="target" value="Apt4">
                                </form>
                                <a class="dropdown-item" href="#" onclick="document.getElementById(\'formApt4' . $clienteId . '\').submit();"><i class="icon-gear"></i> Valvula Apt.4</a>

                                <form action="http://localhost:3000/api/encender" method="GET" target="hidden_frame" id="formBomba' . $clienteId . '">
                                    <input type="hidden" name="cliente_id" value="' . $clienteId . '">
                                    <input type="hidden" name="target" value="Bomba">
                                </form>
                                <a class="dropdown-item" href="#" onclick="document.getElementById(\'formBomba' . $clienteId . '\').submit();"><i class="icon-gear"></i> Bomba</a>
                                
                            </div>
                        </div>
                        <iframe name="hidden_frame" style="display:none;"></iframe>
                        ',
                    "ID" => $datos[$i]['cliente_id'],
                    "Apt1" => $datos[$i]['acum_apt1'],
                    "Apt2" => $datos[$i]['acum_apt2'],
                    "Apt3" => $datos[$i]['acum_apt3'],
                    "Apt4" => $datos[$i]['acum_apt4']

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