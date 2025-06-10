#include <IRremote.h> //Library IRremote 

int RECV_PIN = A0; // Pin connected to the IR receiver
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(9600); // Start serial communication
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX); // Print received value in HEX format
    irrecv.resume(); // Prepare for the next signal
  }
}
