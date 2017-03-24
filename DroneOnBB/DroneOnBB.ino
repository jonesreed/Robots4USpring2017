#include <IRremote.h>

IRsend irsend;

int voltage = 150; //89 pwm == 1.55 Volts
int checkingVoltage = 19;
float pwmValue;
float pwmPercent;
float checkedVoltage;
unsigned int delayTime = 13; // will be used in uSeconds
int thisIsFrequency = 39;

int oncePerButton = 0;
int shooting = 0;

int trigger = 9;
int IRpin = 3;
int redPin = 13;
int greenPin = 11;
int bluePin = 8;
int IRreciever = 19;

// Fire
//IRsend irsend;                               // Class from IRremote.h to send IR signal
unsigned int rawCode[1] = {0xAAA};           // Sends 0b101010101010 encoded when IR signal is sent
int oncePerPress = 0;
boolean isFiring = false;



// Detection of IR light

boolean vulnerable = true;                  // acts as a shield after being hit 
uint8_t hits = 0;                           // counter for the number of hits recieved by a drone
int irSensorValue;
unsigned long previousMillis = 0;
unsigned long timeWhenHit = 0;
unsigned long currentTimeForHits = 0;
unsigned long difInTime = 0;
unsigned long invulnerabilityTime = 1000;   // After drone is hit, will be invunerable for 5 seconds

boolean shouldBeBlinking = false;
unsigned long redBlinkOn;
unsigned long redBlinkOff;
unsigned long redBlinkCurrentTimeOFF;
unsigned long redBlinkCurrentTimeON;
unsigned long redBlinkDifferenceON;
unsigned long redBlinkDifferenceOFF;
unsigned long blinkTime = 175;
int redLEDstate = 0;
int firstTimeBlink = 0;

void setup() {
  pinMode(trigger, INPUT);
  //pinMode(checkingVoltage, INPUT);
  //pinMode(LEDpin, OUTPUT);
  Serial.begin(115200);
  pinMode(IRpin,OUTPUT); // set to output 
  pinMode(redPin,OUTPUT); // set to output
  pinMode(greenPin,OUTPUT); // set to output
  pinMode(bluePin,OUTPUT); // set to output
  pinMode(IRreciever,INPUT); // set to INPUT

  

}



void loop() {
  // Code to sense IR shots
  irSensorValue = analogRead(IRreciever);
  //Serial.println(irSensorValue);
  if( irSensorValue == 0 && vulnerable == true){
    hits++;
    vulnerable = false;
    timeWhenHit = millis();
  }
  currentTimeForHits = millis();
  difInTime = currentTimeForHits - timeWhenHit;
  if( difInTime >= invulnerabilityTime ){ /*time since last being hit is >= 5seconds */
    vulnerable = true;
  }
  else{
    vulnerable = false;
  }


  
  // Code to turn on RBG Health indicator

  switch (hits){
        case 0:
          digitalWrite(greenPin, HIGH);
          digitalWrite(redPin, LOW);
          digitalWrite(bluePin, LOW);
          shouldBeBlinking = false;
          break;
        case 1:
          digitalWrite(greenPin, HIGH);
          digitalWrite(redPin, HIGH);
          digitalWrite(bluePin, LOW);
          shouldBeBlinking = false;
          break;
        case 2:
          digitalWrite(redPin, HIGH);
          digitalWrite(bluePin, LOW);
          digitalWrite(greenPin, LOW);
          shouldBeBlinking = false;
          break;
        case 3:
          shouldBeBlinking = true;
          //invulnerabilityTime = 5000;
          break;
        case 4:
          digitalWrite(bluePin, HIGH);
          digitalWrite(greenPin, LOW);
          digitalWrite(redPin, HIGH);
          shouldBeBlinking = false;
          //invulnerabilityTime = 5000;
          break;    
        default:
          hits = 0;
          firstTimeBlink = 0;
          shouldBeBlinking = false;
          //invulnerabilityTime = 5000;
          break;
      }
    if(shouldBeBlinking){
      if(redLEDstate == 0 && firstTimeBlink == 0){
        digitalWrite(redPin, HIGH);
        digitalWrite(bluePin, LOW);
        digitalWrite(greenPin, LOW);
        redLEDstate = 1;
        firstTimeBlink = 1;
        redBlinkOn = millis();
      }
      redBlinkCurrentTimeON = millis();
      redBlinkDifferenceON = redBlinkCurrentTimeON - redBlinkOn;
      
      if(redBlinkDifferenceON >= blinkTime && redLEDstate == 1){
        digitalWrite(redPin, LOW);
        digitalWrite(bluePin, LOW);
        digitalWrite(greenPin, LOW);
        redLEDstate = 0;
        redBlinkOff = millis();
      }
      redBlinkCurrentTimeOFF = millis();
      redBlinkDifferenceOFF = redBlinkCurrentTimeOFF - redBlinkOff;
      
      if(redBlinkDifferenceOFF >= blinkTime && redLEDstate == 0){
        digitalWrite(redPin, HIGH);
        digitalWrite(bluePin, LOW);
        digitalWrite(greenPin, LOW);
        redLEDstate = 1;
        redBlinkOn = millis();
      }
     }
      
      
    
    // Code to enable IR LED to 'Shoot'
    if(digitalRead(trigger) == HIGH){
      if(oncePerPress == 0){
        irsend.sendRaw( rawCode, sizeof(rawCode) / sizeof(rawCode[0]), 38); // Tells the IR to send signal rawCode with frequency of 38 kHz
        delay(50);
      }
      else{}
      oncePerPress = 1;
    }
    else{
      isFiring = false;
      oncePerPress = 0;
    }
}
