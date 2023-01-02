/*
  Since MH-ET LIVE MAX30102 breakout board seems outputting IR and RED swapped
  when Sparkfun's library is used.

  red = particleSensor.getFIFOIR();
  ir = particleSensor.getFIFORed();

  Is used in my code. If you have Sparkfun's MAX30105 breakout board , try to
  correct these lines.

  ## Tips:
  SpO2 is calculated as R=((square root means or Red/Red average )/((square root means of IR)/IR average))
  SpO2 = -23.3 * (R - 0.4) + 100;
  // taken from a graph in http://ww1.microchip.com/downloads/jp/AppNotes/00001525B_JP.pdf

*/
#include <Wire.h>
#include <Sparkfun_MAX30105.h> //sparkfun MAX3010X library

Sparkfun_MAX30105 particleSensor;

#define TIMEOUT 30 //Time out second to sleep

//HARDWARE DEFINITION

double aveRed = 0;          //DC component of RED signal
double aveIr = 0;           //DC component of IR signal
double sumIrRMS = 0;        //sum of IR square
double sumRedRMS = 0;       // sum of RED square
unsigned int i = 0;         //loop counter
#define SUM_CYCLE 100
int Num = SUM_CYCLE ;       //calculate SpO2 by this sampling interval
double SP02 = 95.0;        //initial value of estimated SpO2
double fSpO2 = 0.7;         //filter factor for estimated SpO2
double fRate = 0.95;        //low pass filter for IR/red LED value to eliminate AC component

#define TIMETOBOOT 3000     // wait for this time(msec) to output SpO2
#define SCALE 88.0          //adjust to display heart beat and SpO2 in Arduino serial plotter at the same time
#define SAMPLING 1          //if you want to see heart beat more precisely , set SAMPLING to 1
#define FINGER_ON 50000     // if ir signal is lower than this , it indicates your finger is not on the sensor
#define MINIMUM_SPO2 80.0
#define MAX_SPO2 100.0
#define MIN_SPO2 80.0

void sleepSensor() {
  //Setup to sense a nice looking saw tooth on the plotter
  byte ledBrightness = 0; //Options: 0=Off to 255=50mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  //Options: 1 = IR only, 2 = Red + IR on MH-ET LIVE MAX30102 board

  int sampleRate = 200; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 16384; //Options: 2048, 4096, 8192, 16384

  // Set up the wanted parameters
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
}

void initSensor() {
  //Setup to sense a nice looking saw tooth on the plotter
  byte ledBrightness = 0x7F; //Options: 0=Off to 255=50mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  //Options: 1 = IR only, 2 = Red + IR on MH-ET LIVE MAX30102 board

  int sampleRate = 200; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 16384; //Options: 2048, 4096, 8192, 16384

  // Set up the wanted parameters
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing...");

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    initSensor();
  }
}


// Heart Rate Monitor by interval of zero crossing at falling edge
// max 180bpm - min 45bpm
#define FINGER_ON 50000 // if ir signal is lower than this , it indicates your finger is not on the sensor
#define LED_PERIOD 100 // light up LED for this period in msec when zero crossing is found for filtered IR signal
#define MAX_BPS 180
#define MIN_BPS 45

double HRM_estimator( double fir , double aveIr)
{
  static double fbpmrate = 0.95; // low pass filter coefficient for HRM in bpm
  static uint32_t crosstime = 0; //falling edge , zero crossing time in msec
  static uint32_t crosstime_prev = 0;//previous falling edge , zero crossing time in msec
  static double bpm = 60.0;
  static double BPM = 60.0;
  static double eir = 0.0; //estimated lowpass filtered IR signal to find falling edge without notch
  static double firrate = 0.85; //IR filter coefficient to remove notch , should be smaller than fRate
  static double eir_prev = 0.0;


  // Heart Rate Monitor by finding falling edge
  eir = eir * firrate + fir * (1.0 - firrate); //estimated IR : low pass filtered IR signal
  if ( ((eir - aveIr) * (eir_prev - aveIr) < 0 ) && ((eir - aveIr) < 0.0)) { //find zero cross at falling edge
    crosstime = millis();//system time in msec of falling edge
    //Serial.print(crosstime); Serial.print(","); Serial.println(crosstime_prev);
    if ( ((crosstime - crosstime_prev ) > (60 * 1000 / MAX_BPS)) && ((crosstime - crosstime_prev ) < (60 * 1000 / MIN_BPS)) ) {
      bpm = 60.0 * 1000.0 / (double)(crosstime - crosstime_prev) ; //get bpm
      //   Serial.println("crossed");
      BPM = BPM * fbpmrate + (1.0 - fbpmrate) * bpm;//estimated bpm by low pass filtered
    }
    crosstime_prev = crosstime;
  }
  eir_prev = eir;
  return (BPM);
}

unsigned int loopCnt = 0;
void loop()
{
  uint32_t ir, red;     //raw data
  double fred, fir;     //floating point RED ana IR raw values
  double SpO2 = 0;      //raw SpO2 before low pass filtered
  double BPM;           //estimated Heart Rate (bpm)

  particleSensor.check(); //Check the sensor, read up to 3 samples

  while (particleSensor.available()) {

    red = particleSensor.getFIFORed(); //Sparkfun's MAX30105
    ir = particleSensor.getFIFOIR();  //Sparkfun's MAX30105

    i++; loopCnt++;
    fred = (double)red;
    fir = (double)ir;
    aveRed = aveRed * fRate + (double)red * (1.0 - fRate);//average red level by low pass filter
    aveIr = aveIr * fRate + (double)ir * (1.0 - fRate); //average IR level by low pass filter
    sumRedRMS += (fred - aveRed) * (fred - aveRed); //square sum of alternate component of red level
    sumIrRMS += (fir - aveIr) * (fir - aveIr);//square sum of alternate component of IR level

    BPM = HRM_estimator(fir, aveIr); //BPM is estimated BPM

    if ((i % SAMPLING) == 0) {
      if ( millis() > TIMETOBOOT) {

        float ir_forGraph = 2.0 * (fir - aveIr) / aveIr * SCALE + (MIN_SPO2 + MAX_SPO2) / 2.0;
        float red_forGraph = 2.0 * (fred - aveRed) / aveRed * SCALE + (MIN_SPO2 + MAX_SPO2) / 2.0;

        if ( ir < FINGER_ON) SP02 = MINIMUM_SPO2;                       //indicator for finger detached
      }
    }
    if ((i % Num) == 0) {
      double R = (sqrt(sumRedRMS) / aveRed) / (sqrt(sumIrRMS) / aveIr);
      // Serial.println(R);
      //#define MAXIMREFDESIGN

#define OFFSET 0.0
      SpO2 = -23.3 * (R - 0.4) + 100 - OFFSET ; //http://ww1.microchip.com/downloads/jp/AppNotes/00001525B_JP.pdf
      if (SpO2 > 100.0 ) SpO2 = 100.0;
      SP02 = fSpO2 * SP02 + (1.0 - fSpO2) * SpO2;//low pass filter
      break;
    }
    particleSensor.nextSample(); //We're finished with this sample so move to next sample
  }
}