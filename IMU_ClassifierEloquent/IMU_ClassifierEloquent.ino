
#include <Arduino_LSM9DS1.h>

#include "model.h"

const float accelerationThreshold = 2.5; // threshold of significant in G's
const int numSamples = 119;
float input[numSamples*6];
float output[3];
int samplesRead = numSamples;

// array to map gesture index to a name
const char* GESTURES[] = {
  "Arriba",
  "Flexion",
  "Punho"
};

#define NUM_GESTURES (sizeof(GESTURES) / sizeof(GESTURES[0]))

void setup() {
      while (!MovimientoNN.begin()) {
        Serial.print("Error in NN initialization: ");
        Serial.println(MovimientoNN.getErrorMessage());
      }
  
  Serial.begin(9600);
  while (!Serial);

  // initialize the IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  // print out the samples rates of the IMUs
  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");

  Serial.println();

}

void loop() {
  float aX, aY, aZ, gX, gY, gZ;

  // wait for significant motion
  while (samplesRead == numSamples) {
    if (IMU.accelerationAvailable()) {
      // read the acceleration data
      IMU.readAcceleration(aX, aY, aZ);

      // sum up the absolutes
      float aSum = fabs(aX) + fabs(aY) + fabs(aZ);

      // check if it's above the threshold
      if (aSum >= accelerationThreshold) {
        // reset the sample read count
        samplesRead = 0;
        break;
      }
    }
  }

  // check if the all the required samples have been read since
  // the last time the significant motion was detected
  while (samplesRead < numSamples) {
    // check if new acceleration AND gyroscope data is available
    if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
      // read the acceleration and gyroscope data
      IMU.readAcceleration(aX, aY, aZ);
      IMU.readGyroscope(gX, gY, gZ);

      // normalize the IMU data between 0 to 1 and store in the model's
      // input tensor


      input[samplesRead * 6 + 0] = (aX + 4.0) / 8.0;
      input[samplesRead * 6 + 1] = (aY + 4.0) / 8.0;
      input[samplesRead * 6 + 2] = (aZ + 4.0) / 8.0;
      input[samplesRead * 6 + 3] = (gX + 2000.0) / 4000.0;
      input[samplesRead * 6 + 4] = (gY + 2000.0) / 4000.0;
      input[samplesRead * 6 + 5] = (gZ + 2000.0) / 4000.0;

      samplesRead++;

      if (samplesRead == numSamples) {

       MovimientoNN.predictClass(input);


    Serial.print(GESTURES[0]);
    Serial.print(": ");
    Serial.println(MovimientoNN.getScoreAt(0));
    Serial.print(GESTURES[1]);
    Serial.print(": ");
    Serial.println(MovimientoNN.getScoreAt(1));    
    Serial.print(GESTURES[2]);
    Serial.print(": ");
    Serial.println(MovimientoNN.getScoreAt(2));

       
      }
    }
  }
}
