#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#include "HardwareSerial.h"

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
void setup() {
  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, 16, 17);  // RX = 16, TX = 17
   if (!myDFPlayer.begin(mySerial)) {
    Serial.println("DFPlayer Mini not detected!");
    while (1)
      ;
  }
 Serial.println("DFPlayer Mini ready!");
 myDFPlayer.volume(25);
 playIntro();
 playGreeting();
}

void loop() {
//mini robot code
}
