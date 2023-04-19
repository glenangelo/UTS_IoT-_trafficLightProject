#define BLYNK_TEMPLATE_ID "TMPL6JNQh9KcX"
#define BLYNK_TEMPLATE_NAME "ESP8266"
#define BLYNK_AUTH_TOKEN "V0lHH6EPST6d_ZRjZou06OzQ1JbUcjuv"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#define BLYNK_GREEN "#23C48E"
#define BLYNK_RED "#D3435C"
#define BLYNK_YELLOW "#FFBF00"
#define BLYNK_WHITE "#FFFFFF"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "1234";
char pass[] = "11223344";

WidgetLCD lcd(V5);

#include "DHT.h"

#define DHTPIN 4     // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 

#define PIN_RED D9
#define PIN_GREEN D10
#define PIN_BLUE D11
#define buttonPin D2
#define buzzerPin D5
#define BLYNK_GREEN "#00FF00"
#define BLYNK_RED "#FF0000"

int redTime;
int yellowTime;
int greenTime;

int buttonState = 0;
int foul = 0;

WidgetLED led(V7);

DHT dht(DHTPIN, DHTTYPE);

BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  redTime = pinValue ;
}
BLYNK_WRITE(V2)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V2 to a variable
  yellowTime = pinValue ;
}
BLYNK_WRITE(V3)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V3 to a variable
  greenTime = pinValue ;
}
BLYNK_WRITE(V8)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V8 to a variable
  if (pinValue ==255){
    digitalWrite(buzzerPin, LOW);  
    foul++;  // Counting foul
  }
}

void setup() {
  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  dht.begin();
}

void loop() {
  Blynk.run();

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  lcd.print(1,0, "Smart TFlight");
  
  Blynk.virtualWrite(V4,"Suhu :", t,"C");
  Blynk.virtualWrite(V0,"Humidity :", h);
  
  Serial.print("Suhu: ");
  Serial.print(t);
  Serial.println();

  led.on();
  led.setColor(BLYNK_GREEN);
  setColor(255, 0, 0); // Red Color
  Blynk.virtualWrite(V9,"MERAH");
  for (int i = redTime; i >= 0; i--) {
    if (digitalRead(buttonPin) == LOW) {   // Check if the button is pressed
      digitalWrite(buzzerPin, HIGH);
      led.on();
      led.setColor(BLYNK_RED);
    }

    if (i<10){ 
      lcd.print(1,1, "R Counter: 0"+String(i));
      } else{
      lcd.print(1,1, "R Counter: "+String(i));}

    if (digitalRead(buzzerPin)== LOW){
      led.setColor(BLYNK_GREEN);
    }
    Blynk.virtualWrite(V6,"Foul :", foul);
    delay(1000);
    }

  setColor(255, 0, 120); // Yellow Color
  Blynk.virtualWrite(V9,"KUNING");
  for (int i = yellowTime; i >= 0; i--) {
    if (i<10){ 
      lcd.print(1,1, "Y Counter: 0"+String(i));
    } else{
      lcd.print(1,1, "Y Counter: "+String(i));}
    delay(1000);
  }

  setColor(0, 0, 255); // Green Color
  Blynk.virtualWrite(V9,"HIJAU");
  for (int i = greenTime; i >= 0; i--) {
    if (i<10){ 
      lcd.print(1,1, "G Counter: 0"+String(i));
    } else{
      lcd.print(1,1, "G Counter: "+String(i));}
    delay(1000);
  }
}

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(PIN_RED, redValue);
  analogWrite(PIN_GREEN, greenValue);
  analogWrite(PIN_BLUE, blueValue);
}