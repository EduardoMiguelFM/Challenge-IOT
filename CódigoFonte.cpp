#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

// -------------------- Wi-Fi e MQTT --------------------
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

// -------------------- LEDs --------------------
#define LED_A 19  
#define LED_B 18  
#define LED_C 5   
#define LED_D 17  
#define LED_E 16  
#define LED_F 4   
#define LED_G 2   

const int leds[7] = {LED_A, LED_B, LED_C, LED_D, LED_E, LED_F, LED_G};
const char* nomes[7] = {
  "DISPONIVEL", "RESERVADA", "MANUTENCAO",
  "FALTA_PECA", "INDISPONIVEL", "DANOS_ESTRUTURAIS", "SINISTRO"
};
const char* setores[7] = {"A", "B", "C", "D", "E", "F", "G"};

int contagem[7] = {0,0,0,0,0,0,0};
long tempoAnterior = 0;

// -------------------- Sensor HC-SR04 (Setor A) --------------------
#define TRIG_A 13
#define ECHO_A 12

// -------------------- DHT22 --------------------
#define DHTPIN 15     
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);

// -------------------- HC-SR04 substituindo SW-420 --------------------
#define TRIG_B 27
#define ECHO_B 26

// -------------------- Funções --------------------
void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
}

int medirDistancia(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duracao = pulseIn(echo, HIGH, 30000); // timeout de 30ms
  return duracao * 0.034 / 2;
}

void publicarSetor(int i) {
  String payload = "{\"setor\":\"" + String(setores[i]) +
                   "\",\"status\":\"" + String(nomes[i]) +
                   "\",\"quantidade\":" + String(contagem[i]) + "}";
  String topico = "patio/setor/" + String(setores[i]) + "/contagem";
  client.publish(topico.c_str(), payload.c_str());
  Serial.println("MQTT -> " + topico + ": " + payload);
}

void publicarDHT() {
  float temp = dht.readTemperature();
  float umid = dht.readHumidity();

  if (!isnan(temp) && !isnan(umid)) {
    String payload = "{\"temperatura\":" + String(temp, 1) +
                     ",\"umidade\":" + String(umid, 1) + "}";
    client.publish("patio/ambiente/dht22", payload.c_str());
    Serial.println("MQTT -> patio/ambiente/dht22: " + payload);
  }
}

void acenderLedSomente(int index) {
  for (int i = 0; i < 7; i++) digitalWrite(leds[i], (i == index));
}

// -------------------- Setup --------------------
void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 7; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }

  pinMode(TRIG_A, OUTPUT); pinMode(ECHO_A, INPUT);
  pinMode(TRIG_B, OUTPUT); pinMode(ECHO_B, INPUT);

  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  randomSeed(analogRead(0));
}

// -------------------- Loop --------------------
void loop() {
  if (!client.connected()) client.connect("MotoPatioESP32");
  client.loop();

  // -------------------- SETOR A --------------------
  int distanciaA = medirDistancia(TRIG_A, ECHO_A);
  if (distanciaA > 0 && distanciaA < 15 && millis() - tempoAnterior > 2000) {
    contagem[0]++;
    acenderLedSomente(0);
    publicarSetor(0);
    tempoAnterior = millis();
  }

  // -------------------- SETOR B --------------------
  int distanciaB = medirDistancia(TRIG_B, ECHO_B);
  if (distanciaB > 0 && distanciaB < 10) {
    contagem[1]++;
    acenderLedSomente(1);
    publicarSetor(1);
  }

  // -------------------- Publicar dados DHT22 --------------------
  publicarDHT();

  // -------------------- Setores simulados --------------------
  for (int i = 2; i < 7; i++) {
    int caso = random(0, 3); // 0=normal,1=desaparecida,2=errada

    switch(caso) {
      case 0: contagem[i]++; acenderLedSomente(i); publicarSetor(i); break;
      case 1:
        Serial.println("ALERTA: Moto desaparecida no setor " + String(setores[i]));
        for(int j=0; j<2; j++){ 
          digitalWrite(leds[i], HIGH); delay(150);
          digitalWrite(leds[i], LOW); delay(150);
        }
        publicarSetor(i); 
        break;
      case 2:
        Serial.println("ALERTA: Moto no lugar errado no setor " + String(setores[i]));
        digitalWrite(leds[i], HIGH); publicarSetor(i);
        delay(300); digitalWrite(leds[i], LOW);
        break;
    }
  }

  delay(700);
}
