#include <BluetoothSerial.h>
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT22

BluetoothSerial BT;
DHT dht(DHTPIN, DHTTYPE);

uint8_t endSlave[] = {0x14, 0x33, 0x5C, 0x51, 0xF7, 0x3A};

bool primeiraLeitura = true;
float ultimaHumidade;
float ultimaTemp;

void setup() {
  Serial.begin(9600);
  Serial.println(F("Iniciando DHT22 e Bluetooth..."));
  dht.begin();

  if (BT.begin("EspSamuelEvan", true)) {
    Serial.println("Bluetooth iniciado (MASTER)");
  } else {
    Serial.println("Erro ao iniciar Bluetooth");
    while (true);
  }

  Serial.print("Tentando conectar ao slave: ");
  Serial.println("14:33:5C:51:F7:3A");

  if (BT.connect(endSlave)) {
    Serial.println("Conectado com sucesso ao slave!");
  } else {
    Serial.println("Falha na conexão — verifique se o slave está ligado e emparelhado.");
  }
}

void loop() {
  float humidade = dht.readHumidity();
  float temperaturaC = dht.readTemperature();
  float fahrenheit = dht.readTemperature(true);

  if (isnan(humidade) || isnan(temperaturaC) || isnan(fahrenheit)) {
    Serial.println(F("Falha ao ler o sensor DHT!"));
    return;
  }

  Serial.print(F("Umidade: "));
  Serial.print(humidade);
  Serial.print(F("% Temperatura: "));
  Serial.print(temperaturaC);
  Serial.print(F("°C "));
  Serial.print(fahrenheit);
  Serial.println(F("°F"));

  if (primeiraLeitura || humidade != ultimaHumidade || temperaturaC != ultimaTemp) {
    BT.print(F("Umidade: "));
    BT.print(humidade);
    BT.print(F("% Temperatura: "));
    BT.print(temperaturaC);
    BT.print(F("C "));
    BT.print(fahrenheit);
    BT.println(F("F"));

    ultimaHumidade = humidade;
    ultimaTemp = temperaturaC;
    primeiraLeitura = false;
  }
  delay(500);
}