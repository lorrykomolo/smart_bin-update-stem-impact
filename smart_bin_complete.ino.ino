#include <Simpletimer.h>
#define BLYNK_PRINT Serial
#include <HCSR04.h>

HCSR04 hc(22, 23); //initialisation class HCSR04 (trig pin , echo pin)
#define DHTPIN 4

//#include <SimpleTimer.h>;
#include "DHT.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE);



char auth[] = "u2gvh25kK8uPC_3P914fX7Ea1d_jlsJg";

char ssid[] = "AMERICAN_SPACE_NRB";
char pass[] = "muadmin2020as";
int val ,l;
SimpleTimer timer;

void setup() {
  Serial.begin(115200);
  dht.begin();

  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  int wifi_ctr = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
    Serial.print(".");
  }

  Serial.println("WiFi connected");

  Blynk.begin("u2gvh25kK8uPC_3P914fX7Ea1d_jlsJg", ssid, pass);
  timer.setInterval(1000L, sendSensor);

}

void loop() {
  Blynk.run();
  timer.run(); // Initiates SimpleTimer
}

void sendSensor()
{

  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }


  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F(" >>full  Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));


  
  l = hc.dist();
  val = map(l, 0, 187, 100, 0);

  Serial.print(val);
  delay(5);




  Blynk.virtualWrite(V2, h); // Humidity for gauge
  Blynk.virtualWrite(V1, t); // Temperature for gauge
  Blynk.virtualWrite(V3, val); // garbage level
  //Blynk.virtualWrite(V8, t); // Temperature for graph
}
