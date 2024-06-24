#include <DHT.h>
#include <SoftwareSerial.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define RELAY_PIN 3
#define PIR_PIN 4
#define MQ2_PIN A0
#define LDR_PIN A1

DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial esp8266(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  pinMode(PIR_PIN, INPUT);
  esp8266.begin(115200);
  initWiFi();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int mq2 = analogRead(MQ2_PIN);
  int ldr = analogRead(LDR_PIN);
  int pir = digitalRead(PIR_PIN);

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C");

  if (t > 30) {
    digitalWrite(RELAY_PIN, HIGH);
  } else {
    digitalWrite(RELAY_PIN, LOW);
  }

  if (pir == HIGH) {
    Serial.println("Motion detected!");
  }

  sendDataToServer(h, t, mq2, ldr, pir);
  delay(2000);
}

void initWiFi() {
  esp8266.println("AT+CWMODE=1");
  delay(1000);
  esp8266.println("AT+CWJAP=\"SSID\",\"PASSWORD\""); // Replace with your SSID and Password
  delay(5000);
}

void sendDataToServer(float h, float t, int mq2, int ldr, int pir) {
  String data = "GET /update?humidity=" + String(h) + "&temperature=" + String(t) + "&gas=" + String(mq2) + "&light=" + String(ldr) + "&motion=" + String(pir) + " HTTP/1.1\r\n";
  data += "Host: yourserver.com\r\n"; // Replace with your server's IP or domain
  data += "Connection: close\r\n\r\n";

  esp8266.println("AT+CIPSTART=\"TCP\",\"yourserver.com\",80"); // Replace with your server's IP or domain
  delay(2000);
  esp8266.println("AT+CIPSEND=" + String(data.length()));
  delay(2000);
  esp8266.print(data);
  delay(2000);
  esp8266.println("AT+CIPCLOSE");
}
