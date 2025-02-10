console.clear();

import createExpressServer from 'express';
const PORT = 3000;
const expressApp = createExpressServer();

expressApp.listen(PORT, () =>
    console.log(`Servidor en puerto ${PORT}`)

);

//mysql
import mysql from 'mysql';

var con = mysql.createConnection({
    host: "localhost",
    user: "root",
    password: "",
    database: "mydb"
});
//mysql

//mqtt
import mqtt from 'mqtt';
const client = mqtt.connect("mqtt://test.mosquitto.org");
//mqtt

//conectarse a base de datos
con.connect(function (err) {
    if (err) throw err;
    console.log("Conectado a la base de datos");
});

//Suscribirse a topico mqtt
client.on("connect", () => {
    client.subscribe("acuamet_mqtt", (err) => {
        if (!err) {
            console.log("Suscrito a topico mqtt");
        }
    });
});