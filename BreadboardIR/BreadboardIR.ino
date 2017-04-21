#define SYSCLOCKTIMER 16000000
#define TIMER_ENABLE_PWM        (TCCR0A |= _BV(COM0A0))
#define TIMER_DISABLE_PWM       (TCCR0A &= ~(_BV(COM0A0)))
#define TIMER_ENABLE_INTR       (TIMSK0 |= _BV(OCIE0A)
#define TIMER_DISABLE_INTR      (TIMSK0 &= ~(_BV(OCIE0A)))
#define TIMER_INTR_NAME         TIMER0_COMPA_vect

// Pin Definitions
#define IR_RECEIVER A0
#define TIMER_PWM_PIN 6
#define BUTTON_PIN 8

// IR Receiver
unsigned short receiverVal;
unsigned char droneHits;
int counter = 0;

// IR Transmitter

// Firing
bool buttonPressed;
bool buttonState = false;

void setup() {
  pinMode(IR_RECEIVER, INPUT);
  pinMode(TIMER_PWM_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  timerConfigKHz(44);

  Serial.begin(9600);
}

void loop() {
  receiverVal = analogRead(IR_RECEIVER);

  if(receiverVal < 1000)
  {
    // droneHits++;
    //Serial.print("Hit: ");
    //Serial.println(receiverVal);
  }


  // Firing
  buttonPressed = !(digitalRead(BUTTON_PIN));
  if(buttonPressed && buttonState == false)
  {
    sendIR(200);
    counter++;
    Serial.print("FIRE ");
    Serial.println(counter);
    buttonState = true;
  }
  else if(!buttonPressed && buttonState == true)
  {
    buttonState = false;
  }
  

}

void timerConfigKHz(int freq)
{
  const uint8_t pwmval = SYSCLOCKTIMER / 2000 / freq;
  TCCR0A = _BV(WGM01) | _BV(WGM00);
  TCCR0B = _BV(WGM02) | _BV(CS00);
  OCR0A = pwmval;
}

void sendIR(uint8_t numBursts)
{
  for(int i = 0; i < numBursts; i++)
  {
      TIMER_ENABLE_PWM;
      delay(1);
      TIMER_DISABLE_PWM;
  }
}

