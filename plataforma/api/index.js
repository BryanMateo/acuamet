console.clear();

//                              servidor express http
import createExpressServer from 'express';

const PORT = 3000;
const expressApp = createExpressServer();

expressApp.listen(PORT, () =>
    console.log(`Servidor en puerto ${PORT}`)

);



//                              mysql
import mysql from 'mysql';

var db = mysql.createConnection({
    host: "localhost",
    user: "root",
    password: "",
    database: "acuamet_db"
});

const lista_clientes = new Set();

//conectarse a base de datos
db.connect(function (err) {
    if (err) throw err;
    console.log("Conectado a la base de datos");
});

// db.query("SELECT EXISTS(SELECT 1 FROM usuarios WHERE usuario_id = 4) AS existe", [], function (err, result) {
//     if (err) {
//         console.error("Error al seleccionar:", err);
//     }
//     else {
//         console.log(result[0].existe ? "El cliente existe" : "El cliente no existe");
//     }
// });

//                              mqtt
import mqtt from 'mqtt';
const mqttclient = mqtt.connect("mqtt://localhost");

//Suscribirse a topico mqtt
mqttclient.on("connect", () => {
    mqttclient.subscribe("acuamet/#", (err) => {
        if (!err) {
            console.log("Suscrito a topico mqtt");
        }
    });
});

//                              funciones
import { sprintf } from 'sprintf-js';

function crear_topico_control(cliente_id, control_template) {
    const topico_control = sprintf("acuamet/%s/control", cliente_id);
    var mensaje = control_template;

    mqttclient.publish(topico_control, mensaje, { qos: 1, retain: true }, (err) => {
        if (err) {
            console.error("Error al publicar:", err);
        }
    });
}

mqttclient.on("message", (topic, message) => {

    const topico = topic.toString();
    const mensaje = JSON.parse(message); //convierte el mensaje del topico en un objeto JSON
    const segmentos_topico = topico.split('/');
    const cliente_id = segmentos_topico[1];

    if (segmentos_topico.length === 3 && segmentos_topico[2] === 'info') { //discrimina los mensajes mqtt

        db.query("SELECT * FROM clientes WHERE cliente_id = ?", [cliente_id], function (err, result) {

            //Verificacion y actualizacion de los clientes con su llave de usuario
            if (err) {
                console.error("Error al seleccionar:", err);
            } else if (result[0]) { //si existe va a verificar la llave

                if (result[0].key_usuario != mensaje.key) { //si la llave no coincide la actualiza
                    db.query("UPDATE `clientes` SET `key_usuario` = ? WHERE `clientes`.`cliente_id` = ?", [mensaje.key, cliente_id], function (err, result) {
                        console.log(result)
                        if (err) throw (err);
                        db.query("SELECT * FROM clientes WHERE cliente_id = ?", [cliente_id], function (err, result) {
                            if (err) throw (err);
                            var control_template = result[0].control;
                            crear_topico_control(cliente_id, control_template); //Crea el topico de control, o vuelve a aplicar el control que este en la base de datos si ya existia
                        });

                    });
                } else {
                    //funcion que suma el resultado en la base de datos y envia 
                    sumar_consumo(cliente_id);
                    //console.log(result[0]);
                }

            } else { //si no existe ningun registro del cliente, lo crea
                db.query("INSERT INTO clientes (cliente_id, key_usuario) VALUES (?, ?)", [cliente_id, mensaje.key], function (err, result) {
                    if (err) throw (err);
                    console.log("Cliente agregado ", cliente_id);

                    db.query("SELECT * FROM clientes WHERE cliente_id = ?", [cliente_id], function (err, result) {
                        if (err) throw (err);
                        var control_template = result[0].control;
                        crear_topico_control(cliente_id, control_template);
                    });
                });
            }
        });
    }

})

function sumar_consumo(cliente_id, mensaje) {

    db.query("SELECT info FROM clientes WHERE cliente_id = ?", [cliente_id], function (err, result) {
        if (err) throw (err);

        if (result[0].info != null) {
            const info = JSON.parse(result[0].info);
            console.log(cliente_id, info, mensaje);
            //const acum_apt1 = info.flujo_acumulado_apt1 + mensaje.flujo_apt1;
        }

    });

    // db.query("UPDATE `clientes` SET `key_usuario` = ? WHERE `clientes`.`cliente_id` = ?", [mensaje.key, cliente_id], function (err, result) {
    //     console.log(result)
    //     if (err) throw (err);
    //     db.query("SELECT * FROM clientes WHERE cliente_id = ?", [cliente_id], function (err, result) {
    //         if (err) throw (err);
    //         var control_template = result[0].control;
    //         crear_topico_control(cliente_id, control_template); //Crea el topico de control, o vuelve a aplicar el control que este en la base de datos si ya existia
    //     });
    // });

}