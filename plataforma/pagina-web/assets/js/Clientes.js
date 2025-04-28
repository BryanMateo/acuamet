var table;

init();

//FUNCION QUE SE EJECUTARA AL INICIO
function init()
{
    LlenarTablaClientes();
}

//FUNCION PARA LLENAR LA TABLA CATEGORIA
function LlenarTablaClientes()
{
    table = $('#table_clientes').DataTable({
        pageLength: 10,
        responsive: true,
        processing: true,
        ajax: "../controller/ClientesController.php?operador=listar_clientes",
        columns: [
            { data: 'Valvula OFF' },
            { data : 'Valvula ON'},
            { data : 'ID'},
            { data : 'Apt1'},
            { data : 'Apt2'},
            { data : 'Apt3'},
            { data : 'Apt4'},
        ]
        
    });
}