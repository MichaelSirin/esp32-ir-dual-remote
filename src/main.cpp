#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

// Change this to DATA pin on which you connected the IR receiver
int RECV_PIN  = 2;      // GPIO2 (pin 4)
int LED_PIN  = 3;      // GPIO3 (pin 5)

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  // 1) Инициализируем Serial
  Serial.begin(115200);
  delay(500);

  irrecv.enableIRIn();
  Serial.println("IR Receiver ready");

  // Just to know which program is running on my Arduino
  Serial.println(F("START " __FILE__ " from " __DATE__));
  Serial.println("Ready to receive IR codes");
  Serial.println("Point your remote to IR receiver and start pressing buttons one-by-one.");
  Serial.println("");

  pinMode(LED_PIN, OUTPUT);
  Serial.println("Setup complete");
}

// void loop() {
//   digitalWrite(LED_PIN, HIGH);
//   delay(500);
//   digitalWrite(LED_PIN, LOW);
//   delay(500);
// }

void loop() {

  // Inside the main loop, wait for IRremote library to successfully parse
  // incomming IR code and then print out over serial what is the
  // HEX code received and what type was detected.
  if (irrecv.decode(&results))
  {
    Serial.print(results.value, HEX);
    Serial.print(" - ");
    switch (results.decode_type){
        case NEC:           Serial.println("NEC");          break;
        case SONY:          Serial.println("SONY");         break;
        case RC5:           Serial.println("RC5");          break;
        case RC6:           Serial.println("RC6");          break;
        case DISH:          Serial.println("DISH");         break;
        case SHARP:         Serial.println("SHARP");        break;
        case JVC:           Serial.println("JVC");          break;
        case SANYO:         Serial.println("SANYO");        break;
        case MITSUBISHI:    Serial.println("MISUBISHI");    break;
        case SAMSUNG:       Serial.println("SAMSUNG");      break;
        case LG:            Serial.println("LG");           break;
        case WHYNTER:       Serial.println("WHYNTER");      break;
        case AIWA_RC_T501:  Serial.println("AIWARC_T501");  break;
        case PANASONIC:     Serial.println("PNASONIC");     break;
        case DENON:         Serial.println("DENON");        break;
      default:
        case UNKNOWN:       Serial.println("UNKNOWN");      break;
      }
    irrecv.resume(); // Receive the next value
  }

  // Delay to prevent flooding our serial output
  delay(300);
}
