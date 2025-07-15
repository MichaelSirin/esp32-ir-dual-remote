#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRutils.h>
#include <IRCodes.h>

#include <WiFi.h>
#include <ESPAsyncWebServer.h>  // https://github.com/me-no-dev/ESPAsyncWebServer
#include <SPIFFS.h>
#include <ArduinoJson.h>        // https://arduinojson.org/v6/doc/installation/
#include "security.h"


// Web-server
String request;
DynamicJsonDocument doc(200);
AsyncWebServer server(80);
bool IRpending = false;
uint32_t IRcmd     = 0;
uint8_t  IRlen     = 0;

// Change this to DATA pin on which you connected the IR receiver
const uint8_t  IR_RECV_PIN  = 2;      // GPIO2 (pin 4)
const uint8_t  IR_SEND_PIN = 1;      // GPIO1 (pin 10)
const uint8_t  LED_PIN  = 3;          // GPIO3 (pin 5)

IRrecv irrecv(IR_RECV_PIN);
IRsend irsend(IR_SEND_PIN);
decode_results results;

void onRequest(AsyncWebServerRequest *request) {
    // dummy callback function for handling params, etc.
}

void onFileUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final)
{
    // dummy callback function signature, not in used in our code
}

// This is the main function that will receive request from the webpage when button is pressed
// and then based on the data that is sent, decide what should be done
void onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
  DeserializationError error = deserializeJson(doc, (char*)data);

  if (!error) {
    const char* command = doc["cmd"];
    Serial.printf("command=%s, parsed=", command);

    // Check which button was pressed
    if( strncmp(command,"pwr",3) == 0 )
    {
      //IrSender.sendSAMSUNG(RemoteCodes.btnOnOff, RemoteCodes.codeLen);
      IRcmd = RemoteCodes.btnOnOff;
      IRlen = 32;
      IRpending = true;
      Serial.println("OnOff");
    }
    else if(  strncmp(command,"up",2) == 0  )
    {
      //IrSender.sendSAMSUNG(RemoteCodes.btnUp, RemoteCodes.codeLen);
      IRcmd = RemoteCodes.btnUp;
      IRlen = 32;
      IRpending = true;
      Serial.println("up");
    }

    else if(  strncmp(command,"left",4) == 0  )
    {
      //IrSender.sendSAMSUNG(RemoteCodes.btnLeft, RemoteCodes.codeLen);
      IRcmd = RemoteCodes.btnLeft;
      IRlen = 32;
      IRpending = true;
      Serial.println("left");
    }

    else if(  strncmp(command,"ok",2) == 0  )
    {
      //IrSender.sendSAMSUNG(RemoteCodes.btnOK, RemoteCodes.codeLen);
      IRcmd = RemoteCodes.btnOK;
      IRlen = 32;
      IRpending = true;
      Serial.println("ok");
    }

    else if(  strncmp(command,"right",5) == 0  )
    {
      //IrSender.sendSAMSUNG(RemoteCodes.btnRight, RemoteCodes.codeLen);
      IRcmd = RemoteCodes.btnRight;
      IRlen = 32;
      IRpending = true;
      Serial.println("right");
    }

    else if(  strncmp(command,"down",4) == 0  )
    {
      //IrSender.sendSAMSUNG(RemoteCodes.btnDown, RemoteCodes.codeLen);
      IRcmd = RemoteCodes.btnDown;
      IRlen = 32;
      IRpending = true;
      Serial.println("down");
    }

    else if(  strncmp(command,"back",4) == 0  )
    {
      //IrSender.sendSAMSUNG(RemoteCodes.btnReturn, RemoteCodes.codeLen);
      IRcmd = RemoteCodes.btnReturn;
      IRlen = 32;
      IRpending = true;
      Serial.println("back");
    }

    else if(  strncmp(command,"home",4) == 0  )
    {
      //IrSender.sendSAMSUNG(RemoteCodes.btnHome, RemoteCodes.codeLen);
      IRcmd = RemoteCodes.btnHome;
      IRlen = 32;
      IRpending = true;
      Serial.println("home");
    }

    else if(  strncmp(command,"play",4) == 0  )
    {
      //IrSender.sendSAMSUNG(RemoteCodes.btnPlayPause, RemoteCodes.codeLen);
      IRcmd = RemoteCodes.btnPlayPause;
      IRlen = 32;
      IRpending = true;
      Serial.println("play");
    }

    else if(  strncmp(command,"volup",5) == 0  )
    {
      //IrSender.sendSAMSUNG(RemoteCodes.btnVolUp, RemoteCodes.codeLen);
      IRcmd = RemoteCodes.btnVolUp;
      IRlen = 32;
      IRpending = true;
      Serial.println("volume-up");
    }

    else if(  strncmp(command,"chup",4) == 0  )
    {
      //IrSender.sendSAMSUNG(RemoteCodes.btnChUp, RemoteCodes.codeLen);
      IRcmd = RemoteCodes.btnChUp;
      IRlen = 32;
      IRpending = true;
      Serial.println("channel-up");
    }

    else if(  strncmp(command,"volmute",7) == 0  )
    {
      //IrSender.sendSAMSUNG(RemoteCodes.btnVolEnter, RemoteCodes.codeLen);
      IRcmd = RemoteCodes.btnVolEnter;
      IRlen = 32;
      IRpending = true;
      Serial.println("volume-mute");
    }

    else if(  strncmp(command,"chmiddle",8) == 0  )
    {
      //IrSender.sendSAMSUNG(RemoteCodes.btnChEnter, RemoteCodes.codeLen);
      IRcmd = RemoteCodes.btnChEnter;
      IRlen = 32;
      IRpending = true;
      Serial.println("channel-return");
    }

    else if(  strncmp(command,"volminus",8) == 0  )
    {
      //IrSender.sendSAMSUNG(RemoteCodes.btnVolDown, RemoteCodes.codeLen);
      IRcmd = RemoteCodes.btnVolDown;
      IRlen = 32;
      IRpending = true;
      Serial.println("volume-minus");
    }

    else if(  strncmp(command,"chminus",7) == 0  )
    {
      //IrSender.sendSAMSUNG(RemoteCodes.btnChDown, RemoteCodes.codeLen);
      IRcmd = RemoteCodes.btnChDown;
      IRlen = 32;
      IRpending = true;
      Serial.println("channel-minus");
    }
    else
    {
      Serial.println("Don't recognize cmd");
    }

    // Return HTTP 200 code (success) to the client
    request->send(200, "text/plain", "Ok");
  }
}

void setup() {
  // Initialize serial communication at 115200 baud rate
  Serial.begin(115200);
  delay(500);

  // Initialize IR receiver
  irrecv.enableIRIn();
  Serial.println("IR Receiver ready");

  // Initialize IR transmitter
  pinMode(IR_SEND_PIN, OUTPUT);
  irsend.begin();
  Serial.println("IR Transmitter ready");

  // Initialize LED
  pinMode(LED_PIN, OUTPUT);

  // Wi-Fi
  WiFi.begin(ssid, password);
  Serial.printf("Connecting to Wi-Fi '%s' …\n", ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print('.');
  }
  Serial.println();
  Serial.printf("IP: %s\n", WiFi.localIP().toString().c_str());

  // SPIFFS (for serving files)
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  } 

  // List files in SPIFFS
  Serial.println("SPIFFS files:");
  File root = SPIFFS.open("/");
  if (!root) {
    Serial.println("Failed to open directory");
  } else {
    File file = root.openNextFile();
    while (file) {
      Serial.printf("  %s (%d bytes)\n", file.name(), file.size());
      file = root.openNextFile();
    }
  }

  // Web server setup
  server.serveStatic("/", SPIFFS, "/")
        .setDefaultFile("index.html")
        .setCacheControl("max-age=86400");

  // Which functions to use for processing the /api requests
  server.on("/api", HTTP_POST, onRequest, onFileUpload, onBody);
  
  server.onNotFound([](AsyncWebServerRequest* r){
    r->send(404, "text/plain", "Not Found");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Wait for a new command
  if (IRpending) {

    // LG Volume Up в NEC-формате (32 бита, код 0x20DF40BF)
    digitalWrite(LED_PIN, HIGH);
    irsend.sendLG(IRcmd, IRlen, 2);
    Serial.printf("Sent LG code 0x%X\n", IRcmd);
    IRpending = false;
    delay(100);  // Give some time for the IR signal to be sent
    digitalWrite(LED_PIN, LOW);
  }

  delay(10);
}
