🏍️ MotoPátio Inteligente – Projeto IoT (FIAP Sprint 1)

> Solução baseada em IoT para organizar o controle de motos em pátios logísticos, como os utilizados pela empresa Mottu.

---

📌 Descrição

O projeto MotoPátio Inteligente foi desenvolvido para resolver o problema de desorganização e falta de visibilidade em pátios de motos. Utilizando sensores, LEDs e comunicação MQTT, a solução monitora o status de cada setor do pátio e envia as informações para um dashboard em tempo real.

---

🧠 Tecnologias Utilizadas

| Tecnologia       | Finalidade                                         |
|-----------------|----------------------------------------------------|
| ESP32            | Microcontrolador principal (simulado e físico)    |
| HC-SR04          | Sensor ultrassônico para detecção de motos        |
| DHT22            | Sensor de temperatura e umidade                   |
| LEDs             | Indicadores de status por setor                   |
| WiFi (ESP32)     | Conexão para envio de dados MQTT                  |
| MQTT             | Protocolo de comunicação entre ESP32 e servidor  |
| Node.js          | Servidor para receber dados MQTT e atualizar CSV  |
| Socket.IO        | Atualização do dashboard em tempo real           |
| HTML/CSS         | Dashboard web interativo                          |
| CSV              | Armazenamento de histórico em arquivos           |
| Wokwi            | Simulação do ESP32 e sensores                     |

---

🗂️ Estrutura do Projeto

MotoPatio/
├── src/
│   └── motopatio_esp32.ino       # Código principal do ESP32
├── dashboard/
│   └── nodered_fluxo_completo.json  # Fluxo Node-RED com os 7 setores
├── images/
│   ├── circuito.png              # Protoboard com ESP32 + LEDs + Sensor
│   └── patio_simulado.png        # Simulação visual do pátio real
└── README.md

---

⚙️ Como Usar

## ⚙️ Como Usar

### 📡 ESP32
1. Abra o projeto no Wokwi ou no seu ESP32 físico.
2. Configure os sensores:
   - **HC-SR04**:
     - TRIG A → GPIO 13, ECHO A → GPIO 12  
     - TRIG B → GPIO 27, ECHO B → GPIO 26
   - **DHT22** → GPIO 15
   - **LEDs** → GPIOs: 19 (A), 18 (B), 5 (C), 17 (D), 16 (E), 4 (F), 2 (G)
3. Compile e faça upload do código `motopatio_esp32.ino`.

### 📡 Servidor Node.js
1. Abra o terminal na pasta `server/`.
2. Instale as dependências:

---
npm init -y
npm install express socket.io mqtt fs
---

🎯 Funcionalidades Atuais

- ✅ Detecção de motos no Setor A com sensor ultrassônico
- ✅ Simulação dos setores B a G
- ✅ Acionamento de LEDs conforme status de cada setor
- ✅  Publicação MQTT por tópico: patio/setor/X/contagem, patio/ambiente/dht22
- ✅ Dashboard web com setores separados em cards
- ✅ Atualização em tempo real via Socket.IO
- ✅ Armazenamento automático em CSV

---

📸 Demonstração Visual

- Protótipo físico: circuito com ESP32, LEDs, HC-SR04 e DHT22
- Dashboard: exibição em tempo real com setores separados e monitoramento do ambiente

---

🚀 Resultados Parciais

- MVP funcional com sensores ativos e dashboard operando
- Comunicação confiável via MQTT
- Dados armazenados em tempo real em CSV
- Código escalável para integração com múltiplos sensores ou visão computacional

---

🧩 Próximos Passos

- Integração com câmeras (Visão Computacional)
- Identificação automática por imagem
- Alertas automatizados por condição crítica
- Armazenamento histórico com banco de dados

---

👨‍💻 Equipe

- Eduardo Miguel Forato Monteiro -RM55871
- Cicero Gabriel Oliveira Serafim -RM556996
- Murillo Sant'Anna - RM557183
- FIAP – 2025 | Disruptive Architectures
