#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

float tempLocal = 22.0;
float humLocal = 70.0;
int nivel = 1800;
bool chuva = false;

unsigned long lastChuvaToggle = 0;
const unsigned long chuvaInterval = 10000;
unsigned long lastCidadeChange = 0;
const unsigned long cidadeChangeInterval = 10000;
float tempStep = 0.1;
float humStep = 0.15;
int nivelStep = 20;

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqttServer = "broker.hivemq.com"; 
const int mqttPort = 1883;
const char* mqttTopic = "sysweather/dados";

WiFiClient espClient;
PubSubClient client(espClient);

struct ClimaSimulado {
  String cidade;
  String descricao;
  float temperatura;
  String horario;
};

const String cidadesSP[] = {
  "São Paulo", "Campinas", "Santos", "São José dos Campos", "Ribeirão Preto",
  "Sorocaba", "São Bernardo do Campo", "Jundiaí", "Mogi das Cruzes", "Piracicaba"
};
const int cidadesSPCount = sizeof(cidadesSP) / sizeof(cidadesSP[0]);

const String descricoes[] = {
  "Ensolarado", "Parcialmente nublado", "Nublado", "Chuvoso", "Tempestade"
};
const int descricoesCount = sizeof(descricoes) / sizeof(descricoes[0]);

ClimaSimulado clima;
int cidadeIndex = 0;

void setupWifi() {
  Serial.print("Conectando ao Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" conectado!");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println(" conectado!");
    } else {
      Serial.print(" erro, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);

  setupWifi();
  client.setServer(mqttServer, mqttPort);

  clima.cidade = cidadesSP[cidadeIndex];
  clima.descricao = descricoes[0];
  clima.temperatura = 28.0;
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  tempLocal += tempStep;
  if (tempLocal > 30 || tempLocal < 20) tempStep = -tempStep;

  humLocal += humStep;
  if (humLocal > 80 || humLocal < 60) humStep = -humStep;

  nivel += nivelStep;
  if (nivel > 2000 || nivel < 1500) nivelStep = -nivelStep;

  if (millis() - lastChuvaToggle > chuvaInterval) {
    chuva = !chuva;
    lastChuvaToggle = millis();
  }

  clima.temperatura += 0.1;
  if (clima.temperatura > 35) clima.temperatura = 20;

  int hora = (millis() / 1000 / 60) % 24;
  int min = (millis() / 1000) % 60;
  clima.horario = String(hora) + ":" + (min < 10 ? "0" : "") + String(min);

  int descIndex = (millis() / 30000) % descricoesCount;
  clima.descricao = descricoes[descIndex];

  if (millis() - lastCidadeChange > cidadeChangeInterval) {
    cidadeIndex = (cidadeIndex + 1) % cidadesSPCount;
    clima.cidade = cidadesSP[cidadeIndex];
    lastCidadeChange = millis();
  }

  
  String intensidadeChuva = "Nenhuma";
  if (chuva) {
    if (nivel > 1900) intensidadeChuva = "Forte";
    else if (nivel > 1700) intensidadeChuva = "Moderada";
    else intensidadeChuva = "Leve";
  }

  String risco = "Nenhum";
  if (nivel > 1900 && chuva) risco = "Alto";
  else if (nivel > 1700 && chuva) risco = "Médio";
  else if (chuva) risco = "Baixo";

  
  StaticJsonDocument<512> doc;
  doc["cidade"] = clima.cidade;
  doc["descricao"] = clima.descricao;
  doc["temperatura"] = clima.temperatura;
  doc["humidade"] = humLocal;
  doc["chuva"] = chuva;
  doc["intensidade_chuva"] = intensidadeChuva;
  doc["nivel"] = nivel;
  doc["risco"] = risco;

  char buffer[512];
  serializeJson(doc, buffer);
  Serial.println(buffer); 
  client.publish(mqttTopic, buffer);

  delay(2000);
}