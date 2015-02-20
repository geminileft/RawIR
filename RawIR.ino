#define SERIAL_BAUD_RATE 115200  //for arduino serial output

//ir sensor must be plugged into pin 2 or pin 3 for this to work
//value should be specified in setup()  depending on pin
#define PIN2_INT 0  //input from ir sensor in pin2
#define PIN3_INT 1  //input from ir sensor in pin3

#define MAX_WAIT_MICROS 15000 //how long to wait after last change state to record ir pulses
#define NUM_PULSES 200  //maximum number of pulses to capture

volatile boolean gInterruptCalled = false;
boolean gIncomingInput = false;
int gPulses[NUM_PULSES];
int gPulseIndex = 0;
unsigned long gStartMicros;

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  attachInterrupt(PIN2_INT, interruptCallback, CHANGE);
  Serial.println("Starting");
}

void loop() {
  unsigned long currentMicros = micros();
  if (gInterruptCalled) {
    if (!gIncomingInput) {
      resetInput();
    } else {
      if (gPulseIndex < NUM_PULSES) {
        gPulses[gPulseIndex++] = currentMicros - gStartMicros;
      }
    }
    gStartMicros = currentMicros;
    gInterruptCalled = false;
  } else {
    if (gIncomingInput && ((currentMicros - gStartMicros) > MAX_WAIT_MICROS)) {
      Serial.println("Start Capture Data");
      int i = 0;
      for (;i < gPulseIndex; ++i) {
        if ((i % 2) == 1) {
          Serial.print(",");
        }
        Serial.print(gPulses[i], DEC);
        if ((i % 2) == 1) {
          Serial.println("");
        }
      }
      if ((i %2) == 1) {
        Serial.println("");
      }
      Serial.print("End Capture Data: ");
      Serial.print(gPulseIndex, DEC);
      Serial.println("");
      gIncomingInput = false;
    }
  }
}

void printData(const char* label, int count, int* data) {
  Serial.println(label);
  for (int i = 0;i < count; ++i) {
    if ((i % 2) == 1) {
      Serial.print(",");
    }
    Serial.print(data[i], DEC);
    if ((i % 2) == 1) {
      Serial.println("");
    }
  }
  Serial.println("");
}

void resetInput() {
  gIncomingInput = true;
  gPulseIndex = 0;
  memset(gPulses, 0, sizeof(gPulses));
}

void interruptCallback() {
    gInterruptCalled = true;
}

