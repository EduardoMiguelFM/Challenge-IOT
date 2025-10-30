const mqtt = require("mqtt");
const fs = require("fs");
const express = require("express");
const http = require("http");
const { Server } = require("socket.io");
const createCsvWriter = require("csv-writer").createObjectCsvWriter;
const bodyParser = require('body-parser');
const cors = require('cors');

// ---------------- MQTT ----------------
const broker = "mqtt://broker.hivemq.com";
const topics = [
  "patio/ambiente/dht22",
  "patio/setor/+/contagem",
  "patio/veiculo/event"
];

// ---------------- CSV ----------------
const csvWriter = createCsvWriter({
  path: "dados.csv",
  header: [
    {id: "timestamp", title: "timestamp"},
    {id: "topic", title: "topic"},
    {id: "payload", title: "payload"}
  ],
  append: true
});

const app = express();
app.use(cors());
app.use(bodyParser.json({limit:'1mb'}));
const server = http.createServer(app);
const io = new Server(server, { cors: { origin: "*" }});

const client = mqtt.connect(broker);

client.on("connect", () => {
  console.log("Conectado ao broker MQTT:", broker);
  topics.forEach(t => client.subscribe(t, (err)=>{ if(err) console.error("Subscribe err",err); }));
});

client.on("message", (topic, message) => {
  try {
    const payload = message.toString();
    console.log("MQTT", topic, payload);
    // save to CSV for simple persistence / demo
    csvWriter.writeRecords([{ timestamp: new Date().toISOString(), topic, payload }]).catch(()=>{});
    // publish to socket.io clients
    if(topic.startsWith("patio/setor/")){
      // topic like "patio/setor/A/contagem"
      const parts = topic.split('/');
      const setor = parts[2];
      let obj = { setor: setor, quantidade: Number(payload), status: "DISPONIVEL", timestamp: new Date().toISOString() };
      // try parse json payload if provided
      try { const p = JSON.parse(payload); if(p.status) obj.status = p.status; if(p.quantidade) obj.quantidade = p.quantidade; } catch(e){}
      io.emit('setor_update', obj);
    } else if(topic === "patio/ambiente/dht22"){
      io.emit('ambiente', payload);
    } else if(topic === "patio/veiculo/event"){
      let ev = {};
      try { ev = JSON.parse(payload); } catch(e){ ev = { raw: payload }; }
      io.emit('veiculo_event', ev);
    } else {
      io.emit('mqtt_raw', { topic, payload });
    }
  } catch(e){
    console.error("Erro ao processar mensagem MQTT", e);
  }
});

// REST endpoint to receive prepared payloads (integration point for DBs / Java / .NET / Mobile)
app.post('/api/push', (req, res)=>{
  // expected body: { source: "dashboard" | "app", data: { ... } }
  const body = req.body || {};
  // Basic validation
  if(!body.data){
    return res.status(400).json({ error: "Body must contain data property" });
  }
  // Save JSON to disk as proof-of-concept (in production, this is where you'd write to DB)
  const fname = `push_${Date.now()}.json`;
  fs.writeFileSync(fname, JSON.stringify(body, null, 2));
  // Also write a CSV record for easier inspection
  csvWriter.writeRecords([{ timestamp: new Date().toISOString(), topic: "/api/push", payload: JSON.stringify(body) }]).catch(()=>{});
  // Emit event to real-time clients
  io.emit('integration_push', body);
  return res.json({ ok: true, saved: fname });
});

// Serve static dashboard files
app.use(express.static('.'));

// Socket events from client
io.on('connection', (socket)=>{
  console.log('cliente conectado (socket.io)');
  socket.on('simulate_event', (ev)=> {
    // Allow dashboard to request simulated events be broadcast (useful for demos)
    console.log('Simulação recebida', ev);
    // broadcast to others as if it came from MQTT
    io.emit('veiculo_event', ev);
    // optionally store to disk
    fs.writeFileSync(`sim_${Date.now()}.json`, JSON.stringify(ev, null, 2));
  });
});

const PORT = process.env.PORT || 3000;
server.listen(PORT, ()=> console.log("Servidor rodando em http://localhost:"+PORT));
