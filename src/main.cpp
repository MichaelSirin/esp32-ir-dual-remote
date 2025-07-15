#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRutils.h>

// Change this to DATA pin on which you connected the IR receiver
const uint8_t  IR_RECV_PIN  = 2;      // GPIO2 (pin 4)
const uint8_t  IR_SEND_PIN = 1;      // GPIO21 (pin 2)
const uint8_t  LED_PIN  = 3;          // GPIO3 (pin 5)

IRrecv irrecv(IR_RECV_PIN);
decode_results results;

IRsend irsend(IR_SEND_PIN);

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

  // Just to know which program is running on my Arduino
  Serial.println(F("START " __FILE__ " from " __DATE__));
  Serial.println("Setup complete");

  pinMode(LED_PIN, OUTPUT);
  Serial.println("Setup complete");
}

void loop() {

    // LG Volume Up в NEC-формате (32 бита, код 0x20DF40BF)
    digitalWrite(LED_PIN, HIGH);
    irsend.sendNEC(0x20DF40BF, 32);
    delay(1000);          // Delay to prevent flooding the IR transmitter
    digitalWrite(LED_PIN, LOW);

    Serial.println("Sent LG Volume Up");
    delay(1000);          // Delay to prevent flooding the IR transmitter

  /*
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
    */

}
