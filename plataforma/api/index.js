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

//conectarse a base de datos
db.connect(function (err) {
    if (err) throw err;
    console.log("Conectado a la base de datos");
});



//                              mqtt
import mqtt from 'mqtt';
const mqttclient = mqtt.connect("mqtt://test.mosquitto.org");

//Suscribirse a topico mqtt
mqttclient.on("connect", () => {
    mqttclient.subscribe("acuamet_mqtt/#", (err) => {
        if (!err) {
            console.log("Suscrito a topico mqtt");
        }
    });
});
 

mqttclient.on("message", (topic, message) => {
    //console.log('Topico: ', topic.toString(), 'Mensaje:', message.toString());
    let strMessage = message.toString();
    let objMessage = JSON.parse(message);
    console.log(strMessage);

    // if (topic === "acuamet_mqtt/consola_cliente") {
    //     console.log("topico correcto");
    // } else console.log("topico incorrecto");
    
})
 