#include <Arduino.h>
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
// const uint8_t  IR_RECV_PIN  = 2;      // GPIO2 (pin 4)
const uint8_t  IR_SEND_PIN = 0;      // GPIO1 (pin 10)
const uint8_t  LED_PIN  = 3;          // GPIO3 (pin 5)
const uint8_t  CUSTOM_IR_PIN = IR_SEND_PIN; 

// IRrecv irrecv(IR_RECV_PIN);
// IRsend irsend(IR_SEND_PIN);

// TV model name
const char* tvModel_LG = "LG";
const char* tvModel_SAMSUNG = "SAMSUNG";
char* tvModel = nullptr;   

hw_timer_t* _timer = nullptr;
volatile bool _burst = false;
void sendNEC_custom(uint32_t data, uint8_t nbits);
void sendSAMSUNG_custom(uint32_t data, uint8_t nbits);
void sendRaw_custom(const uint16_t buf[], uint16_t len, uint16_t hz);

TaskHandle_t irTaskHandle = nullptr;

void onTimer() 
{
    if(_burst) 
    {
        // toggle pin each interrupt
        digitalWrite(CUSTOM_IR_PIN, !digitalRead(CUSTOM_IR_PIN));
    }
}

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
    const char* brand   = doc["brand"]   | "LG";    // LG is default
    
    // chose the remote codes based on the brand
    const Remote_t* codes = nullptr;
    if (strcasecmp(brand, "LG") == 0) {
        codes = &RemoteCodes_LG;
        IRlen = codes->codeLen;
        tvModel = (char*)tvModel_LG;
        request->send(200, "text/plain", "Ok");
    } else if (strcasecmp(brand, "SAMSUNG") == 0) {
        codes = &RemoteCodes_SAMSUNG;
        IRlen = codes->codeLen;
        tvModel = (char*)tvModel_SAMSUNG;
        request->send(200, "text/plain", "Ok");
    } else {
        tvModel = nullptr; // Unknown brand
        request->send(400, "text/plain", "Unknown brand");
        return;
    }

    // Check which button was pressed
    if( strncmp(command,"pwr",3) == 0 ) 
    {
      IRcmd = codes->btnOnOff;
    }
    else if(  strncmp(command,"up",2) == 0  )
    {
      IRcmd = codes->btnUp;
    }
    else if(  strncmp(command,"left",4) == 0  )
    {
      IRcmd = codes->btnLeft;
    }

    else if(  strncmp(command,"ok",2) == 0  )
    {
      IRcmd = codes->btnOK;
    }
    else if(  strncmp(command,"right",5) == 0  )
    {
      IRcmd = codes->btnRight;
    }
    else if(  strncmp(command,"down",4) == 0  )
    {
      IRcmd = codes->btnDown;
    }
    else if(  strncmp(command,"back",4) == 0  )
    {
      IRcmd = codes->btnReturn;
    }
    else if(  strncmp(command,"home",4) == 0  )
    {
      IRcmd = codes->btnHome;
    }
    else if(  strncmp(command,"play",4) == 0  )
    {
      IRcmd = codes->btnPlayPause;
    }
    else if(  strncmp(command,"volup",5) == 0  )
    {
      IRcmd = codes->btnVolUp;
    }
    else if(  strncmp(command,"chup",4) == 0  )
    {
      IRcmd = codes->btnChUp;
    }
    else if(  strncmp(command,"volmute",7) == 0  )
    {
      IRcmd = codes->btnVolEnter;
    }
    else if(  strncmp(command,"chmiddle",8) == 0  )
    {
      IRcmd = codes->btnChEnter;
    }

    else if(  strncmp(command,"volminus",8) == 0  )
    {
      IRcmd = codes->btnVolDown;
    }
    else if(  strncmp(command,"chminus",7) == 0  )
    {
      IRcmd = RemoteCodes_LG.btnChDown;
    }
    else
    {
      Serial.println("Don't recognize cmd");
      return;
    }

    Serial.printf("TV model: %s, Command: %s, HEX code: %08X\n", tvModel, command, IRcmd);
    request->onDisconnect([]()
    {
        IRpending = true;
    });
  }
}

void setup() {
  // Initialize serial communication at 115200 baud rate
  Serial.begin(115200);
  delay(500);

  // Initialize IR receiver
//   irrecv.enableIRIn();
//   Serial.println("IR Receiver ready");

  // Initialize IR transmitter
//   irsend.begin();
//   Serial.println("IR Transmitter ready");

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

  pinMode(CUSTOM_IR_PIN, OUTPUT);
  digitalWrite(CUSTOM_IR_PIN, LOW);

  // configure hardware timer 1, divider 80 -> 1MHz tick
  _timer = timerBegin(1, 80, true);
  timerAttachInterrupt(_timer, &onTimer, true);
}

void loop() {
    if (IRpending && tvModel != nullptr) 
    {
        IRpending = false;
        digitalWrite(LED_PIN, HIGH);
    
        if(tvModel == tvModel_LG) 
        {
            // irsend.sendNEC(IRcmd, IRlen, 1);
            sendNEC_custom(IRcmd, IRlen);
        } 
        else if(tvModel == tvModel_SAMSUNG) 
        {
            // irsend.sendSAMSUNG(IRcmd, IRlen);
            sendSAMSUNG_custom(IRcmd, IRlen);
        }
        delay(100);
        digitalWrite(LED_PIN, LOW);
    }
    // Yield to other tasks
    vTaskDelay(pdMS_TO_TICKS(1));
}

void enableCarrier(uint16_t khz) {
  // set alarm to half period in microseconds
  uint32_t interval = 1000UL / khz / 2;     // 1000 kHz
  timerAlarmWrite(_timer, interval, true);
  timerAlarmEnable(_timer);
}

void disableCarrier() {
  timerAlarmDisable(_timer);
  digitalWrite(CUSTOM_IR_PIN, LOW);
}

void mark(uint16_t usec) {
  _burst = true;
  delayMicroseconds(usec);
  _burst = false;
}

void space(uint16_t usec) {
  _burst = false;
  delayMicroseconds(usec);
}

void sendNEC_custom(uint32_t data, uint8_t nbits) {
  enableCarrier(38);
  // NEC header
  mark(9000); space(4500);
  // Data bits
  for (int8_t i = nbits - 1; i >= 0; i--) {
    if (data & (1UL << i)) { mark(560); space(1690); }
    else                   { mark(560); space(560); }
    }
  // NEC footer
  mark(560);
  disableCarrier();
}

void sendSAMSUNG_custom(uint32_t data, uint8_t nbits) {
  enableCarrier(38);
  // Samsung header
  mark(4500); space(4500);
  // Data bits
//   for (uint8_t i = 0; i < nbits; i++) {
  for (int8_t i = nbits - 1; i >= 0; i--) {

    if (data & (1UL << i)) { mark(560); space(560); }
    else                   { mark(560); space(1690); }
    data >>= 1;
  }
  // Samsung footer
  mark(560);
  disableCarrier();
}

void sendRaw_custom(const uint16_t buf[], uint16_t len, uint16_t hz) {
  enableCarrier(hz);
  for (uint16_t i = 0; i < len; i++) {
    if (i & 1) space(buf[i]); else mark(buf[i]);
  }
  disableCarrier();
}
