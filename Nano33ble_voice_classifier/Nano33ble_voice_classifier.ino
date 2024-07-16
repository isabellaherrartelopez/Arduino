/*
 * Clasificador de voz para Arduino Nano 33 BLE Sense por Alan Wang
 */

#include <math.h>
#include <PDM.h>
#include <EloquentTinyML.h>
#include "audio_classification_model.h"  // Archivo de modelo.h

#define PDM_SOUND_GAIN     255   // Ganancia de sonido del micrófono PDM
#define PDM_BUFFER_SIZE    256   // Tamaño del búfer del micrófono PDM

#define SAMPLE_THRESHOLD   900   // Umbral RMS para activar el muestreo
#define FEATURE_SIZE       32    // Tamaño de muestreo de una instancia de voz
#define SAMPLE_DELAY       20    // Tiempo de retardo (ms) entre muestras

#define NUMBER_OF_LABELS   6     // Número de etiquetas de voz

const String words[NUMBER_OF_LABELS] = {"uno", "dos", "tres", "cuatro", "cinco", "fondo"};  // Palabras para cada etiqueta

float feature_data[FEATURE_SIZE];
volatile float rms;
bool voice_detected;


// Función de devolución de llamada para el micrófono PDM
void onPDMdata() {
  rms = -1;
  short sample_buffer[PDM_BUFFER_SIZE];
  int bytes_available = PDM.available();
  PDM.read(sample_buffer, bytes_available);

  // Calcula el RMS (raíz cuadrada de la media) a partir del búfer de muestras
  unsigned int sum = 0;
  for (unsigned short i = 0; i < (bytes_available / 2); i++) {
    sum += pow(sample_buffer[i], 2);
  }
  rms = sqrt(float(sum) / (float(bytes_available) / 2.0));
}

void setup() {
  Serial.begin(115200);
  while (!Serial);

  PDM.onReceive(onPDMdata);
  PDM.setBufferSize(PDM_BUFFER_SIZE);
  PDM.setGain(PDM_SOUND_GAIN);

  if (!PDM.begin(1, 16000)) {  // Inicia el micrófono PDM y el muestreo a 16 KHz
    Serial.println("¡Error al iniciar PDM!");
    while (1);
  }

  pinMode(LED_BUILTIN, OUTPUT);

  // Espera 1 segundo para evitar la lectura inicial de PDM
  delay(900);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("=== Inicio del clasificador ===\n");
}

void loop() {
  // Espera hasta que se active el muestreo
  while (rms < SAMPLE_THRESHOLD);

  digitalWrite(LED_BUILTIN, HIGH);
  for (int i = 0; i < FEATURE_SIZE; i++) {  // Muestreo
    while (rms < 0);
    feature_data[i] = rms;
    delay(SAMPLE_DELAY);
  }
  digitalWrite(LED_BUILTIN, LOW);

  // Predice la voz y coloca los resultados (probabilidad) para cada etiqueta en el arreglo
  float prediction[NUMBER_OF_LABELS];
  //tf_model.predict(feature_data, prediction);  // Utiliza la función tf_model.predict()

  // Imprime los resultados de la predicción
  Serial.println("Prediciendo la palabra:");
  for (int i = 0; i < NUMBER_OF_LABELS; i++) {
    Serial.print("Etiqueta ");
    Serial.print(i);
    Serial.print(" = ");
    Serial.println(prediction[i]);
  }

  voice_detected = false;
  for (int i = 0; i < NUMBER_OF_LABELS; i++) {
    if (prediction[i] >= 0.6) {  // Umbral de probabilidad de predicción para las etiquetas
      Serial.print("Palabra detectada: ");
      Serial.println(words[i]);
      Serial.println("");
      voice_detected = true;
    }
  }
  if (!voice_detected) {
    Serial.println("Palabra no reconocida\n");
  }

  // Espera 1 segundo después de un muestreo/predicción
  delay(900);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
}
