#define USE_ARDUINO_INTERRUPTS false
#include <PulseSensorPlayground.h>
#include <LiquidCrystal.h>



//  Variables
const int PULSE_INPUT = 36;
const int PULSE_BLINK = 22;
const int THRESHOLD = 550;

const int OUTPUT_TYPE = SERIAL_PLOTTER;
byte samplesUntilReport;
const byte SAMPLES_PER_SERIAL_SAMPLE = 10;

int flag = 0;
int stopflag = 0;

hw_timer_t * weartimer = NULL;
hw_timer_t * checktimer = NULL;
int weartiming = 0;
int checktiming = 0;

int freq = 2000;
int resolution = 8;
int BUZZER = 0; //channel

int buttonstate = 0;


LiquidCrystal lcd(19, 23, 18, 17, 16, 15);
PulseSensorPlayground pulseSensor; 

byte heart[8] = {0b00000, 0b01010, 0b11111, 0b11111, 0b11111, 0b01110, 0b00100, 0b00000};
byte heart1[8] = {0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b01110, 0b00000}; //middle
byte heart2[8] = {0b00000, 0b00000, 0b00000, 0b00000, 0b10001, 0b11111, 0b11111, 0b11111}; //top
byte heart3[8] = {0b00000, 0b00000, 0b00000, 0b01110, 0b11111, 0b11111, 0b11111, 0b11111}; //left
byte heart4[8] = {0b00000, 0b00000, 0b00000, 0b01110, 0b11111, 0b11111, 0b11111, 0b11111}; //right
byte heart5[8] = {0b11111, 0b11110, 0b11100, 0b11000, 0b10000, 0b00000, 0b00000, 0b00000}; //botright
byte heart6[8] = {0b11111, 0b01111, 0b00111, 0b00011, 0b00001, 0b00000, 0b00000, 0b00000}; //botleft
byte stick[8] = {0b01110, 0b01110, 0b00100, 0b01110, 0b10101, 0b00100, 0b01010, 0b10001};





void setup() {

  lcd.createChar(0, heart);
  lcd.createChar(1, heart1);
  lcd.createChar(2, heart2);
  lcd.createChar(3, heart3);
  lcd.createChar(4, heart4);
  lcd.createChar(5, heart5);
  lcd.createChar(6, heart6);
  lcd.createChar(7, stick);
  weartimer = timerBegin(0, 80, true);
  checktimer = timerBegin(1, 80, true);
  timerAttachInterrupt(weartimer, &wearTimer, true);
  timerAttachInterrupt(checktimer, &checkTimer, true);
  timerAlarmWrite(weartimer, 1000000, true);
  timerAlarmWrite(checktimer, 1000000, true);

  ledcSetup(BUZZER, freq, resolution);
  ledcAttachPin(2, BUZZER);

  pinMode(21, INPUT);

  lcd.begin(20, 4);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Place your finger");
  lcd.setCursor(0,2);
  lcd.print("on the sensor :)");
  Serial.begin(115200); 
  pulseSensor.analogInput(PULSE_INPUT);   
  pulseSensor.blinkOnPulse(PULSE_BLINK);   
  pulseSensor.setThreshold(THRESHOLD);
  pulseSensor.setSerial(Serial);
  pulseSensor.setOutputType(OUTPUT_TYPE);
  samplesUntilReport = SAMPLES_PER_SERIAL_SAMPLE;
  if (!pulseSensor.begin()) {
    for(;;) {
      // Flash the led to show things didn't work.
      digitalWrite(PULSE_BLINK, LOW);
      delay(50);
      digitalWrite(PULSE_BLINK, HIGH);
      delay(50);
    }
  }


}

// The Main Loop Function
void loop() {
  int myBPM = pulseSensor.getBeatsPerMinute();
   if ((digitalRead(21) == HIGH) && (stopflag==1)){
    flag = 0;
    stopflag = 0;
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Place your finger");
    lcd.setCursor(0,2);
    lcd.print("on the sensor :)");
    }
  
   if (pulseSensor.sawNewSample()) {

    if (--samplesUntilReport == (byte) 0) {
      samplesUntilReport = SAMPLES_PER_SERIAL_SAMPLE;

      pulseSensor.outputSample();

      if ((pulseSensor.sawStartOfBeat()) && (stopflag == 0)) {
        pulseSensor.outputBeat();
        flag = 1;
        timerAlarmEnable(weartimer);
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Measuring Your");
        lcd.setCursor(0,2);
        lcd.print("Heart Rate...");
      }
      if ((flag==1) && (weartiming > 5)){
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Your Heart Rate: ");
        lcd.print(myBPM);
        lcd.setCursor(9,2);
        lcd.write((byte) 3);
        lcd.setCursor(10,2);
        lcd.write((byte) 2);
        lcd.setCursor(11,2);
        lcd.write((byte) 4);
        lcd.setCursor(9,3);
        lcd.write((byte) 6);
        lcd.setCursor(10,3);
        lcd.write((byte) 1);
        lcd.write((byte) 5);
        timerAlarmDisable(weartimer);
        timerAlarmEnable(checktimer);

        if (checktiming == 20){
          lcd.clear();
          lcd.setCursor(0,1);
          stopflag = 1;
          flag = 0;
          timerAlarmDisable(checktimer);
          checktiming = 0;
          if (myBPM < 60){
            lcd.print("     Low BPM!");
            lcd.setCursor(0,2);
            lcd.print("   See a doctor!");
            lcd.write((byte) 7);
            warning();
          }
          else if (myBPM > 100){ 
            lcd.print("      High BPM");
            lcd.setCursor(0,2);
            lcd.print("   See a doctor!");
            lcd.write((byte) 7);
            warning();
          }
          else{
            lcd.print("     Normal BPM");
            lcd.setCursor(0,2);
            lcd.print("   Good & Healthy!");
          }
          
        }
      }
      if ((myBPM == 0) && (flag==1)){
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Check your finger");
        lcd.setCursor(0,2);
        lcd.print("placement!!!");
        flag = 0;
        weartiming = 0;
        buzzerwear();
        timerAlarmDisable(weartimer);
        timerAlarmDisable(checktimer);
        checktimer = 0;
        stopflag = 0;
        }
      
    }

    /*******
      Here is a good place to add code that could take up
      to a millisecond or so to run.
    *******/
  }

}


void wearTimer() {  //To count total wear time
weartiming = weartiming + 1;
}

void checkTimer() { 
checktiming = checktiming + 1;
}


void buzzerwear() {
ledcWriteTone(BUZZER, 1000);
delay(500);
ledcWriteTone(BUZZER, 500);
delay(500);
ledcWriteTone(BUZZER, 700);
delay(500);
ledcWriteTone(BUZZER, 0);
}

void warning() {
ledcWriteTone(BUZZER, 1000);
delay(500);
ledcWriteTone(BUZZER, 500);
delay(500);
ledcWriteTone(BUZZER, 1000);
delay(500);
ledcWriteTone(BUZZER, 500);
delay(500);
ledcWriteTone(BUZZER, 1000);
delay(500);
ledcWriteTone(BUZZER, 0);
}
