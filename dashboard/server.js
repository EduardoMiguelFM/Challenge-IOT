const mqtt = require("mqtt");
const fs = require("fs");
const express = require("express");
const http = require("http");
const { Server } = require("socket.io");
const createCsvWriter = require("csv-writer").createObjectCsvWriter;

// ---------------- MQTT ----------------
const broker = "mqtt://broker.hivemq.com";
const topics = [
  "patio/ambiente/dht22",
  "patio/setor/+/contagem"
];

// ---------------- CSV ----------------
const csvWriter = createCsvWriter({
  path: "dados.csv",
  header: [
    {id: "timestamp", title: "TIMESTAMP"},
    {id: "topic", title: "TOPIC"},
    {id: "payload", title: "PAYLOAD"}
  ],
  append: true
});

// ---------------- Server ----------------
const app = express();
const server = http.createServer(app);
const io = new Server(server);

app.use(express.static("public"));

// ---------------- Conexão MQTT ----------------
const client = mqtt.connect(broker);

client.on("connect", () => {
  console.log("✅ Conectado ao broker:", broker);
  topics.forEach(topic => client.subscribe(topic));
});

client.on("message", (topic, message) => {
  const payload = message.toString();
  const data = {
    timestamp: new Date().toISOString(),
    topic,
    payload
  };

  // Salvar no CSV
  csvWriter.writeRecords([data]).catch(err => console.error(err));

  // Enviar para dashboard em tempo real
  io.emit("mqtt_message", data);
});
  
// ---------------- Servidor HTTP ----------------
server.listen(3000, () => {
  console.log("Servidor rodando em http://localhost:3000");
});
