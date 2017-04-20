
// Pin Definitions
#define IR_RECEIVER A0

// IR Receiver
unsigned short receiverVal;
unsigned char droneHits;

// IR Transmitter

void setup() {
  pinMode(IR_RECEIVER, INPUT);

  Serial.begin(9600);
}

void loop() {
  receiverVal = analogRead(IR_RECEIVER);

//  if(receiverVal == 0)
//  {
//    droneHits++;
//    Serial.print("Hits: ");
//    Serial.println(droneHits);
//  }

  Serial.println(receiverVal);

}
