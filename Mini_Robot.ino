#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#include "HardwareSerial.h"

#define BATTERY_PIN 34
#define CHARGING_PIN 35
#define BATTERY_LOW 20
#define BATTERY_CRITICAL 10
#define BATTERY_FULL 95

HardwareSerial mySerial(1);  // Use UART1
DFRobotDFPlayerMini myDFPlayer;

void playIntro()
{
  Serial.println("Robot Introduce itself");
  myDFPlayer.play(1);  // Play Introduction sound (0001.mp3)
}
void playGreeting()
{
  Serial.println("Robot Greet User");
  myDFPlayer.play(2);  // Play Greeting sound (0002.mp3)
}
void requestInfo()
{
  //Robot processes the query(Integrtion)
}
void displayResponse(char choice)
{
if (choice=='I')
{
  Serial.println("Robot Speak about IoT Commitee");
  myDFPlayer.play(3); // Play IoT sound (0003.mp3)
}
else if(choice=='F')
{
  Serial.println("Robot Speak about Flutter Commitee");
  myDFPlayer.play(4); // Play Flutter sound (0004.mp3)
}
else if(choice=='A')
{
  Serial.println("Robot Speak about AI Commitee");
  myDFPlayer.play(5); // Play AI sound (0005.mp3)
}
else if(choice=='R')
{
  Serial.println("Robot Speak about Rov Commitee");
  myDFPlayer.play(6); // Play Rov sound (0006.mp3)
}
else if(choice=='M')
{
  Serial.println("Robot Speak about Media Commitee");
  myDFPlayer.play(7); // Play Media sound (0007.mp3)
}
//choice will be handeled with integration..
}

int getBatteryPercentage()
{
    int raw = analogRead(BATTERY_PIN);
    int percentage = map(raw, 1304, 1825, 0, 100);
    percentage = constrain(percentage, 0, 100);
    return percentage;
}
void monitorBattery() {
    int battery = getBatteryPercentage();

    Serial.print("BATTERY:");
    Serial.println(battery);

    if (battery <= BATTERY_CRITICAL) {
        Serial.println("BATTERY STATUS:CRITICAL");
        // light RGB (red)
    } 
    else if (battery <= BATTERY_LOW) {
        Serial.println("BATTERY STATUS:LOW");
        myDFPlayer.play(8);  //Play "Battary Low" sound (0008.mp3)
    } 
    else {
        Serial.println("BATTERY STATUS:OK");
        // light RGB (Green)
    }
}
void manageCharging() {
   bool isCharging = digitalRead(CHARGING_PIN);
   int battery = getBatteryPercentage();

  if (isCharging)
  {

        Serial.println("CHARGING:TRUE");

        if (battery >= BATTERY_FULL) 
        { Serial.println("CHARGING:FULL");
            // light RGB (red)}
  } 
  else {  Serial.println("CHARGING:FALSE");}
}

void setup() {
  Serial.begin(115200);
  pinMode(CHARGING_PIN, INPUT);
  mySerial.begin(9600, SERIAL_8N1, 16, 17);  // RX = 16, TX = 17
   if (!myDFPlayer.begin(mySerial)) 
   {
    Serial.println("DFPlayer Mini not detected!");
    while (1);  
   }
   Serial.println("DFPlayer Mini ready!");
   myDFPlayer.volume(25);

}

void loop() {
  monitorBattery();
  manageCharging();
  playIntro();
  playGreeting();
  delay(600000);

}
