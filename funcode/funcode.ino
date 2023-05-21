#include <Blynk.h>
#include <BlynkSimpleEsp32.h>
#include <WiFi.h>
#include <DHT.h>

#define DHTPIN 14
#define DHTTYPE DHT11

#define GAS_PIN 26
#define MQ2RL 1.1 // Résistance de charge en kilohms
#define GAS_RO 1

#define BLYNK_TEMPLATE_ID "TMPL2UdVMyjU2"
#define BLYNK_TEMPLATE_NAME "SmartApp"
#define BLYNK_AUTH_TOKEN "3Wa82rAGOB7wyZ3XoyHZ7BhOPhqmK27o"
#define BLYNK_PRINT Serial

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "ooredoo_ABA0E8";
char pass[] = "97JFWXAW3373C";
int moisturePin = 34;

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void setup() 
{
  Serial.begin(9600);
  Serial.println("Setup complete1");
  WiFi.begin(ssid, pass);

  Serial.print("Connecting to ");
  Serial.print(ssid);

  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }

  Serial.print("Connected");

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Setup complete2");

  dht.begin();
  Serial.println("Setup complete3");

  pinMode(moisturePin, INPUT);

  Serial.println("Setup complete4");
  Blynk.begin(auth, ssid, pass);

  Serial.println("Setup complete5");
}

void loop()
{
  Serial.println("Setup complete6");

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  int moisture = ((( 100.00 - ( (analogRead(moisturePin) / 1023.00) * 100.00 ) ))+300) ;


  int gasValue = analogRead(GAS_PIN);
  
  float gas_ADC = analogRead(GAS_PIN);
  float gas_Rs = (5.0 - gas_ADC / 1023.0 * 5.0) / gas_ADC * MQ2RL;
  float gas_ratio = gas_Rs / GAS_RO;
  float gas_ppm = pow(gas_ratio, -1.59) * 6.5;
  

  if (isnan(humidity) || isnan(temperature))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Moisture: ");
  Serial.println(moisture);

  Serial.print("Gas Value: ");
  Serial.println(gas_ppm);

  Serial.print("--------------------- \n");
  
  Serial.print("data sendit \n");

  Blynk.virtualWrite(V0, temperature);  // Send temperature value to virtual pin V0
  Blynk.virtualWrite(V1, humidity);     // Send humidity value to virtual pin V1
  Blynk.virtualWrite(V2, moisture);     // Send moisture value to virtual pin V2
  Blynk.virtualWrite(V3, gas_ppm);     // Send gas value to virtual pin V3

  delay(100);

  Blynk.run();
  timer.run();
}