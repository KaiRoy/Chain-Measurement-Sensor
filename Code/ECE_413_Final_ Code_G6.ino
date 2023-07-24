#include <phyphoxBle.h>
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

MAX30105 sensor;

double avered = 0; double aveir = 0;
double sumirrms = 0;
double sumredrms = 0;
int i = 0;
int Num = 100;//calculate SpO2 by this sampling interval

double ESpO2 = 95.0;//initial value of estimated SpO2
double FSpO2 = 0.7; //filter factor for estimated SpO2
double frate = 0.95; //low pass filter for IR/red LED value to eliminate AC component
#define TIMETOBOOT 3000 // wait for this time(msec) to output SpO2
#define SCALE 88.0 //adjust to display heart beat and SpO2 in the same scale
#define SAMPLING 5 //if you want to see heart beat more precisely , set SAMPLING to 1
#define FINGER_ON 30000 // if red signal is lower than this , it indicates your finger is not on the sensor
#define MINIMUM_SPO2 80.0

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred
float beatsPerMinute;
int beatAvg;

void pyphox_init();
void spo2_hr_init();

#define USEFIFO
void setup() {
  Serial.begin(115200);
  PhyphoxBLE::start("ECE 413 - Heart");

  pyphox_init();

  // Initialize sensor
  while (!sensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30102 was not found. Please check wiring/power/solder jumper at MH-ET LIVE MAX30102 board. ");
    //while (1);
  }

  spo2_hr_init();
}

void loop() {

  uint32_t ir, red , green;
  double fred, fir;
  float floatred, floatir, floatspo2;
  double SpO2 = 0; //raw SpO2 before low pass filtered

  #ifdef USEFIFO
    sensor.check(); //Check the sensor, read up to 3 samples

    while (sensor.available()) {//do we have new data
      #ifdef MAX30105
        red = sensor.getFIFORed(); //Sparkfun's MAX30105
        ir = sensor.getFIFOIR();  //Sparkfun's MAX30105
      #else
        red = sensor.getFIFOIR(); //why getFOFOIR output Red data by MAX30102 on MH-ET LIVE breakout board
        ir = sensor.getFIFORed(); //why getFIFORed output IR data by MAX30102 on MH-ET LIVE breakout board
      #endif

      if (checkForBeat(ir) == true) {
        //We sensed a beat!
        long delta = millis() - lastBeat;
        lastBeat = millis();

        beatsPerMinute = 60 / (delta / 1000.0);

        if (beatsPerMinute < 255 && beatsPerMinute > 20)
        {
          rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
          rateSpot %= RATE_SIZE; //Wrap variable

          //Take average of readings
          beatAvg = 0;
          for (byte x = 0 ; x < RATE_SIZE ; x++)
            beatAvg += rates[x];
          beatAvg /= RATE_SIZE;
        }
      }

      i++;
      fred = (double)red;
      fir = (double)ir;
      floatred = (float)red;
      floatir = (float)ir;
      avered = avered * frate + (double)red * (1.0 - frate);//average red level by low pass filter
      aveir = aveir * frate + (double)ir * (1.0 - frate); //average IR level by low pass filter
      sumredrms += (fred - avered) * (fred - avered); //square sum of alternate component of red level
      sumirrms += (fir - aveir) * (fir - aveir);//square sum of alternate component of IR level
      if ((i % SAMPLING) == 0) {//slow down graph plotting speed for arduino Serial plotter by thin out
        if ( millis() > TIMETOBOOT) {
          float ir_forGraph = (2.0 * fir - aveir) / aveir * SCALE;
          float red_forGraph = (2.0 * fred - avered) / avered * SCALE;
          //trancation for Serial plotter's autoscaling
          if ( ir_forGraph > 100.0) ir_forGraph = 100.0;
          if ( ir_forGraph < 80.0) ir_forGraph = 80.0;
          if ( red_forGraph > 100.0 ) red_forGraph = 100.0;
          if ( red_forGraph < 80.0 ) red_forGraph = 80.0;
          //        Serial.print(red); Serial.print(","); Serial.print(ir);Serial.print(".");
          if (ir < FINGER_ON) ESpO2 = MINIMUM_SPO2; //indicator for finger detached
          float temperature = sensor.readTemperatureF();
          // Blynk.run();
          // Blynk.virtualWrite(V4,ESpO2 );
          Serial.print("Red:");
          Serial.print(red);
          Serial.print(",");
          Serial.print("IR:");
          Serial.print(ir);
          Serial.print(",");
          Serial.print("BPM:");
          Serial.print(beatsPerMinute);
          Serial.print(",");
          Serial.print("SPO2:");
          Serial.println(ESpO2);
          floatspo2 = (float)ESpO2;
          PhyphoxBLE::write(floatred, floatir, beatsPerMinute, floatspo2);
        }
      }
      if ((i % Num) == 0) {
        double R = (sqrt(sumredrms) / avered) / (sqrt(sumirrms) / aveir);
        // Serial.println(R);
        SpO2 = -23.3 * (R - 0.4) + 100; //http://ww1.microchip.com/downloads/jp/AppNotes/00001525B_JP.pdf
        ESpO2 = FSpO2 * ESpO2 + (1.0 - FSpO2) * SpO2;//low pass filter
        //  Serial.print(SpO2);Serial.print(",");Serial.println(ESpO2);
        sumredrms = 0.0; sumirrms = 0.0; i = 0;
        break;
      }
      sensor.nextSample(); //We're finished with this sample so move to next sample
      //Serial.println(SpO2);
    }
  #endif
}

void spo2_hr_init() {
  //Setup to sense a nice looking saw tooth on the plotter
  byte ledBrightness = 0x7F; //Options: 0=Off to 255=50mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  //Options: 1 = IR only, 2 = Red + IR on MH-ET LIVE MAX30102 board
  int sampleRate = 200; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 16384; //Options: 2048, 4096, 8192, 16384
  // Set up the wanted parameters
  sensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings

  sensor.enableDIETEMPRDY();
}

void pyphox_init() {
  //Experiment Setup
  PhyphoxBleExperiment Heart;
  Heart.setTitle("MAX30105 Sensor Data");
  Heart.setCategory("Arduino Experiments");
  Heart.setDescription("The MAX30105 sensor sends raw data to the Arduino which can be used to determine things such has Heart Rate and Blood Oxygen Levels");

  //Creat View
  PhyphoxBleExperiment::View defaultView;
  defaultView.setLabel("Default View");             //Create a "view"

  PhyphoxBleExperiment::View rawView;
  rawView.setLabel("Raw Data View");                //Create a "view"

  //Graphs Setup
  PhyphoxBleExperiment::Graph firstGraph;
  firstGraph.setLabel("Red Value over Time");
  firstGraph.setUnitX("");
  firstGraph.setUnitY("");
  firstGraph.setLabelX("time");
  firstGraph.setLabelY("Red Value");
  
  firstGraph.setChannel(0, 1);

  //Graphs Setup
  PhyphoxBleExperiment::Graph secondGraph;
  secondGraph.setLabel("IR Value over Time");
  secondGraph.setUnitX("");
  secondGraph.setUnitY("");
  secondGraph.setLabelX("time");
  secondGraph.setLabelY("IR Value");

  secondGraph.setChannel(0, 2);

  //Graphs Setup
  PhyphoxBleExperiment::Graph thirdGraph;
  thirdGraph.setLabel("Heart Rate over Time");
  thirdGraph.setUnitX("");
  thirdGraph.setUnitY("BPM");
  thirdGraph.setLabelX("time");
  thirdGraph.setLabelY("Heart Rate");
  
  thirdGraph.setChannel(0, 3);

  PhyphoxBleExperiment::Graph fourthGraph;
  fourthGraph.setLabel("SPO2 over Time");
  fourthGraph.setUnitX("");
  fourthGraph.setUnitY("");
  fourthGraph.setLabelX("time");
  fourthGraph.setLabelY("SPO2");
  
  fourthGraph.setChannel(0, 4);

  //Export
  PhyphoxBleExperiment::ExportSet mySet;       //Provides exporting the data to excel etc.
  mySet.setLabel("mySet");

  PhyphoxBleExperiment::ExportData myData1;
  myData1.setLabel("Red Value");
  myData1.setDatachannel(1);

  PhyphoxBleExperiment::ExportData myData2;
  myData2.setLabel("IR Value");
  myData2.setDatachannel(2);

  PhyphoxBleExperiment::ExportData myData3;
  myData3.setLabel("Heart Rate");
  myData3.setDatachannel(3);

  PhyphoxBleExperiment::ExportData myData4;
  myData4.setLabel("SPO2");
  myData4.setDatachannel(4);

  //Attach to Experiment
  rawView.addElement(firstGraph);               //attach graph to view
  rawView.addElement(secondGraph);              //attach second graph to view
  defaultView.addElement(thirdGraph);           //attach second graph to view
  defaultView.addElement(fourthGraph);
  Heart.addView(rawView);
  Heart.addView(defaultView);                   //attach view to experiment
  mySet.addElement(myData1);                    //attach data to exportSet
  mySet.addElement(myData2);                    //attach data to exportSet
  mySet.addElement(myData3);                    //attach data to exportSet
  mySet.addElement(myData4);                    //attach data to exportSet
  Heart.addExportSet(mySet);                    //attach exportSet to experiment
  PhyphoxBLE::addExperiment(Heart);             //attach experiment to server
}
