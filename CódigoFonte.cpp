#include <WiFi.h>
#include <PubSubClient.h>

// Wi-Fi e Broker
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

// Pinos LED por setor
#define LED_A 19  // Verde
#define LED_B 18  // Azul
#define LED_C 5   // Amarelo
#define LED_D 17  // Laranja
#define LED_E 16  // Branco
#define LED_F 4   // Vermelho
#define LED_G 2   // Preto

// Sensor Setor A (HC-SR04)
#define TRIG_A 13
#define ECHO_A 12

// Arrays
const int leds[7] = {LED_A, LED_B, LED_C, LED_D, LED_E, LED_F, LED_G};
const char* nomes[7] = {
  "DISPONIVEL", "RESERVADA", "MANUTENCAO",
  "FALTA_PECA", "INDISPONIVEL", "DANOS_ESTRUTURAIS", "SINISTRO"
};
const char* setores[7] = {"A", "B", "C", "D", "E", "F", "G"};

// Variáveis
int contagem[7] = {0, 0, 0, 0, 0, 0, 0};
long tempoAnterior = 0;

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < 7; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }

  pinMode(TRIG_A, OUTPUT);
  pinMode(ECHO_A, INPUT);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

int medirDistancia() {
  digitalWrite(TRIG_A, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_A, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_A, LOW);

  long duracao = pulseIn(ECHO_A, HIGH);
  int distancia = duracao * 0.034 / 2;
  return distancia;
}

void publicarSetor(int i) {
  String payload = "{\"setor\":\"" + String(setores[i]) +
                   "\",\"status\":\"" + String(nomes[i]) +
                   "\",\"quantidade\":" + String(contagem[i]) + "}";

  String topico = "patio/setor/" + String(setores[i]) + "/contagem";
  client.publish(topico.c_str(), payload.c_str());
  Serial.println("MQTT -> " + topico + ": " + payload);
}

void acenderLedSomente(int index) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(leds[i], (i == index) ? HIGH : LOW);
  }
}

void loop() {
  if (!client.connected()) {
    client.connect("MotoPatioESP32");
  }
  client.loop();

  // SETOR A COM SENSOR REAL
  int distancia = medirDistancia();
  if (distancia > 0 && distancia < 15 && millis() - tempoAnterior > 2000) {
    contagem[0]++;
    acenderLedSomente(0);
    publicarSetor(0);
    tempoAnterior = millis();
  }

  // SETORES B A G COM SIMULAÇÃO
  for (int i = 1; i < 7; i++) {
    contagem[i]++; // incremento fictício
    digitalWrite(leds[i], HIGH);
    publicarSetor(i);
    delay(400);
    digitalWrite(leds[i], LOW);
  }

  delay(800);
}
