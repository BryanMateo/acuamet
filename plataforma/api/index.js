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
import { createHash } from 'crypto';

function generar_token_cliente(cliente_id) {
    const timestamp = Date.now();
    const combinado = `${cliente_id}-${timestamp}`;
    return createHash('md5').update(combinado).digest('hex');
}

function crear_topico_control(cliente_id, control_template) {
    const topico_control = sprintf("acuamet/%s/control", cliente_id);
    var mensaje = control_template;

    mqttclient.publish(topico_control, mensaje, { qos: 1, retain: true }, (err) => {
        if (err) {
            console.error("Error al publicar:", err);
        }
    });
}

function pub_token_sesion(cliente_id, token) {
    const topico_token = sprintf("acuamet/%s/token/res", cliente_id);
    var mensaje = sprintf('{"token": "%s"}', token);
    console.log(mensaje);

    mqttclient.publish(topico_token, mensaje, { qos: 1, retain: false }, (err) => {
        if (err) {
            console.error("Error al publicar:", err);
        }
    });
}


//////
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
                    sumar_consumo(cliente_id, mensaje);
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

    else if (segmentos_topico.length === 4 && segmentos_topico[2] === 'token' && segmentos_topico[3] === 'req') {
        if (mensaje.solicitar_token === true) {
            var token = generar_token_cliente(cliente_id);
            pub_token_sesion(cliente_id, token);
            // db.query("UPDATE `cliente_consumos_anterior` SET `token_sesion` = ? WHERE `cliente_consumos_anterior`.`cliente_id` = ?", [token, cliente_id], function (err, result) {
            //     if (err) throw (err);
            //     console.log(result);   
            // });
        }
    }
})

function sumar_consumo(cliente_id, mensaje) {

    db.query("SELECT token_sesion, apt1, apt2, apt3, apt4 FROM cliente_consumos_anterior WHERE cliente_id = ?", [cliente_id], function (err, result) { // consumo anterior en la DB
        if (err) throw (err);

        if (result[0]) {
            if (mensaje.token === result[0].token_sesion) {
                if (mensaje.flujo_apt_1 > result[0].apt1 || mensaje.flujo_apt_2 > result[0].apt2 || mensaje.flujo_apt_3 > result[0].apt3 || mensaje.flujo_apt_4 > result[0].apt4) {
                    var acum_apt1 = mensaje.flujo_apt_1 - result[0].apt1;
                    var acum_apt2 = mensaje.flujo_apt_2 - result[0].apt2;
                    var acum_apt3 = mensaje.flujo_apt_3 - result[0].apt3;
                    var acum_apt4 = mensaje.flujo_apt_4 - result[0].apt4;

                    db.query("SELECT acum_apt1, acum_apt2, acum_apt3, acum_apt4 FROM clientes WHERE cliente_id = ?", [cliente_id], function (err, result) { // consumo anterior en la DB
                        if (err) throw (err);
                        //Suma el acumulado de la db mas el nuevo
                        acum_apt1 += result[0].acum_apt1;
                        acum_apt2 += result[0].acum_apt2;
                        acum_apt3 += result[0].acum_apt3;
                        acum_apt4 += result[0].acum_apt4;

                        db.query("UPDATE `clientes` SET `acum_apt1` = ?, `acum_apt2` = ?, `acum_apt3` = ?, `acum_apt4` = ? WHERE `cliente_id` = ?", [acum_apt1, acum_apt2, acum_apt3, acum_apt4, cliente_id], function (err, result) {
                            if (err) throw (err);
                            // console.log(result);

                            db.query("UPDATE `cliente_consumos_anterior` SET `apt1` = ?, `apt2` = ?, `apt3` = ?, `apt4` = ? WHERE `cliente_id` = ?", [mensaje.flujo_apt_1, mensaje.flujo_apt_2, mensaje.flujo_apt_3, mensaje.flujo_apt_4, cliente_id], function (err, result) {
                                if (err) throw (err);
                                console.log("Consumo acum actualizado");
                            });
                        });
                    });

                } else console.log("Consumo igual");
           
            } else {
                if (mensaje.flujo_apt_1 > result[0].apt1 || mensaje.flujo_apt_2 > result[0].apt2 || mensaje.flujo_apt_3 > result[0].apt3 || mensaje.flujo_apt_4 > result[0].apt4) {
                    db.query("SELECT acum_apt1, acum_apt2, acum_apt3, acum_apt4 FROM clientes WHERE cliente_id = ?", [cliente_id], function (err, result) { // consumo anterior en la DB
                        if (err) throw (err);

                        var acum_apt1 = result[0].acum_apt1 + mensaje.flujo_apt_1;
                        var acum_apt2 = result[0].acum_apt2 + mensaje.flujo_apt_2;
                        var acum_apt3 = result[0].acum_apt3 + mensaje.flujo_apt_3;
                        var acum_apt4 = result[0].acum_apt4 + mensaje.flujo_apt_4;

                        db.query("UPDATE `clientes` SET `acum_apt1` = ?, `acum_apt2` = ?, `acum_apt3` = ?, `acum_apt4` = ? WHERE `cliente_id` = ?", [acum_apt1, acum_apt2, acum_apt3, acum_apt4, cliente_id], function (err, result) {
                            if (err) throw (err);
                            // console.log(result);

                            db.query("UPDATE `cliente_consumos_anterior` SET `token_sesion` = ?, `apt1` = ?, `apt2` = ?, `apt3` = ?, `apt4` = ? WHERE `cliente_id` = ?", [mensaje.token, mensaje.flujo_apt_1, mensaje.flujo_apt_2, mensaje.flujo_apt_3, mensaje.flujo_apt_4, cliente_id], function (err, result) {
                                if (err) throw (err);
                                console.log("Token nuevo y consumo anterior al nuevo");
                            });
                        });
                    });
                } else console.log("Consumo igual");
            }



        } else console.log("Cons anterior no encontrado");

        //console.log(typeof result[0].cons_anterior); // verifica esto
        // console.log(JSON.parse(result[0].cons_anterior));
        // const cons_anterior = JSON.parse(result[0].cons_anterior);


        // console.log(consumo_anterior);

        // if (result[0]) {
        //     var apt_1 = mensaje.flujo_apt_1 - result[0].acum_apt_1


        // }

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