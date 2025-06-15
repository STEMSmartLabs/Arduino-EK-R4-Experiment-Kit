#include <IRremote.hpp>

int RECV_PIN = 6; // Pin connected to the IR receiver

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(RECV_PIN, DISABLE_LED_FEEDBACK); 
}

void loop() {
  if (IrReceiver.decode()) {
    // Only print if the decoded value is NOT 0
    if (IrReceiver.decodedIRData.decodedRawData != 0) { 
      Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
    }
    // Always resume the receiver regardless of what was decoded
    IrReceiver.resume(); 
  }
}
