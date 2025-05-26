🏍️ MotoPátio Inteligente – Projeto IoT (FIAP Sprint 1)

> Solução baseada em IoT para organizar o controle de motos em pátios logísticos, como os utilizados pela empresa Mottu.

---

📌 Descrição

O projeto MotoPátio Inteligente foi desenvolvido para resolver o problema de desorganização e falta de visibilidade em pátios de motos. Utilizando sensores, LEDs e comunicação MQTT, a solução monitora o status de cada setor do pátio e envia as informações para um dashboard em tempo real.

---

🧠 Tecnologias Utilizadas

| Tecnologia       | Finalidade                                          |
|------------------|-----------------------------------------------------|
| ESP32            | Microcontrolador principal (Wokwi e físico)         |
| HC-SR04          | Sensor ultrassônico para detecção no Setor A        |
| LEDs             | Indicadores de status por setor (7 cores)           |
| WiFi (ESP32)     | Conectividade para enviar dados MQTT                |
| MQTT             | Protocolo de comunicação (broker.hivemq.com)        |
| Node-RED         | Dashboard interativo para visualização dos dados    |
| Wokwi            | Simulador para testes e prototipagem                |

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

📡 ESP32
1. Acesse o Wokwi e abra o projeto.
2. Compile o código motopatio_esp32.ino com:
   - Sensor HC-SR04 no GPIOs 13 (TRIG) e 12 (ECHO)
   - LEDs conectados aos GPIOs 19 (Setor A), 18 (B), 5 (C), 17 (D), 16 (E), 4 (F) e 2 (G)

📊 Node-RED Dashboard
1. Acesse seu Node-RED (http://localhost:1880)
2. Importe o fluxo nodered_fluxo_completo.json
3. Instale o pacote node-red-dashboard (caso não tenha)
4. Abra o painel: http://localhost:1880/ui

---

🎯 Funcionalidades Atuais

- ✅ Detecção real de motos no Setor A com sensor ultrassônico
- ✅ Acionamento de LEDs conforme status por setor
- ✅ Simulação dos setores B a G
- ✅ Publicação MQTT por tópico: patio/setor/X/contagem
- ✅ Dashboard visual com:
  - Status textual
  - Gauge de contagem
  - Gráfico histórico

---

📸 Demonstração Visual

- Protótipo Físico: circuito com ESP32, LEDs e sensor
- Dashboard: exibição Node-RED em tempo real

---

🚀 Resultados Parciais

- MVP funcional entregue com sensor ativo e dashboard operando
- Código escalável para múltiplos sensores e visão computacional futura
- Comunicação robusta via MQTT
- Testado com simulação e execução local

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
- FIAP – 2025 | Disruptive Architectures
