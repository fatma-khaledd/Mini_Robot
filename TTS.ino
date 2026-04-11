#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include "Audio.h"


const char* ssid = "";
const char* password = "";
const char* server = "IP address"; 

using namespace websockets; // as classes
WebsocketsClient client;
Audio audio;

void setup() {
    Serial.begin(115200);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) { 
        
     Serial.print("Wait...");

        delay(500); 
        
         }

    
    audio.setInternalDAC();  // convert from digital to analog   sound get from(automatically)  pin: GPIO 25 
    audio.setVolume(21);  //  from 0 to 21 (21 is max in the library)&PAM8403 take signal and max for speaker


    client.onMessage(onMessageCallback);
    
    if(client.connect(server)) {
        Serial.println("Connected to Python Server!");
    } else {
        Serial.println("Connection Failed! Check IP or Server.");
    }
    
    
    Serial.println("Ready!");
}

void loop() {
    audio.loop();
    client.poll();

    
}

void onMessageCallback(WebsocketsMessage message) {
    if (message.isBinary()) {
        //convert data from python to bytes
        uint8_t* data = (uint8_t*)message.raw().c_str();   // type casting & 8? data from 0 to 255 & data send byte by byte
        size_t len = message.raw().length();  //kam byte (1024  as in python)
        
        // sound play
        audio.playChunk(data, len); // put in audio library
    }
}


// for ESP32
