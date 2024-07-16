/******************************************************************
 * ArduinoANN - An artificial neural network for the Arduino
 * All basic settings can be controlled via the Network Configuration
 * section.
 * See robotics.hobbizine.com/arduinoann.html for details.
 ******************************************************************/

#include <math.h>

/******************************************************************
 * Network Configuration - customized per network 
 ******************************************************************/

const int PatternCount = 1;  //todos los datos
const int HiddenNodes = 10;
const int InputNodes = 3;
const int OutputNodes = 2;

float Samples = 0;
float randNumber;
float X1_K_1 = 0;
float X2_K_1 = 0;
float U_K_1 = 0;
// Estos valores son necesarios para la normalización de los datos cuando entran y salen de la red
// Rango de normalización
const float YMin = -1;
const float YMax = 1;

// Valores para normalizar la entrada de la red (estos valores se toman del sistema desde matlab)
const float X1Min = 0;       //valor minimo de X1
const float X1Max = 3.4027;  //valor maximo de X1
const float X2Min = -2.2610;  //valor minimo de X2
const float X2Max = 2.1714;  //valor maximo de X2
const float X3Min = 1.0693;  //valor minimo de U
const float X3Max = 9.4122;  //valor maximo de U
// Valores para normalizar la salida de la red

const float DMin = 0;  //valor minimo de la salida del sistema
const float DMax = 2;  //valor maximo de la salida del sistema

// Los pesos de la red fueron obtenidos en Matlab y se copiaron a este programa
// Pesos capa oculta
// HiddenWeights[10][3+1] = [10][4]
const float HiddenWeights[HiddenNodes][InputNodes + 1] = {
   {2.1592, 0.1387, 0.0102, -2.7381},    
   {0.5371, 1.1667, -0.4585, -1.6080},
   {-0.1181, 0.2049, 0.1106, 0.5076},
   {0.9216, 0.0573, 0.0045, -0.8896},
   {0.3317, 0.0190, 0.0015, -0.0030},
   {-0.0996, -0.2257, 0.0782, -0.1294},
   {-0.2013, 0.3629, 0.1886, -0.7981},
   {0.3107, 2.4219, -1.5935, 1.4494},
   {0.9605, 0.0513, 0.0056, 1.3471},
   {2.1828, 1.2368, -1.3111, 3.5762}

};

// Pesos capa de salida
// OutputWeights[2][10+1] = [2][11]
const float OutputWeights[OutputNodes][HiddenNodes + 1] = {
  { 0.035, 0.0036, 0.1562, 0.0894, 2.7260, -0.1461, 0.0519, -0.0001, 0.2188, 0.0412, -0.1770},
  { -0.1635, 0.0380, 2.5081, -0.5910, 0.7024, -1.5673, 0.8245, -0.0004, 0.0139, -0.0887, -1.1475}
};
 
/******************************************************************
 * End Network Configuration
 ******************************************************************/


int i, j, p, q, r;
float Accum;
float Hidden[HiddenNodes];
float Output[OutputNodes];
float Input[InputNodes][PatternCount];

void setup() {
  //start serial connection
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void loop() {

  float Entrada;
  float Salida_1;
  float Salida_2;
  float Tiempo;

  //Tiempo=millis();
  //Entrada=(fmod(Tiempo,6283))/1000;

  Samples = Samples + 1;

  if (Samples == 1) {
    randNumber = random(50);
    Samples = Samples + 1;
  }

  if (Samples > 60) {
    Samples = 0;
  }

  //Tiempo=millis();
  //Entrada=(fmod(Tiempo,6283))/1000;
  Entrada = (randNumber / 10);

  // Normalización de la entrada que se usará en la red neuronal
  Input[0][0] = YMin + ((X1_K_1 - X1Min) * ((YMax - YMin) / (X1Max - X1Min)));
  Input[1][0] = YMin + ((X2_K_1 - X2Min) * ((YMax - YMin) / (X2Max - X2Min)));
  Input[2][0] = YMin + ((U_K_1 - X3Min) * ((YMax - YMin) / (X3Max - X3Min)));

  /******************************************************************
* Compute hidden layer activations
******************************************************************/


  for (i = 0; i < HiddenNodes; i++) {
    Accum = HiddenWeights[i][InputNodes];  // bias
    for (j = 0; j < InputNodes; j++) {
      Accum += HiddenWeights[i][j] * Input[j][0];  // neta
    }
    Hidden[i] = (2.0 / (1.0 + exp(-2 * Accum))) - 1.0;  // tangente sigmodial
  }



  /******************************************************************
* Compute output layer activations
******************************************************************/
  //   Serial.print("\n");    // prints a tab
  for (i = 0; i < OutputNodes; i++) {
    Accum = OutputWeights[i][HiddenNodes];
    for (j = 0; j < HiddenNodes; j++) {
      Accum += OutputWeights[i][j] * Hidden[j];
    }

    Output[i] = Accum;
  }

  // La salida da la red esta nromalizada, para que quede en el rango original hay que desnormalizar
  // Desnormalización de la salida de la red neuronal
  Salida_1 = DMin + ((Output[0] - YMin) * ((DMax - DMin) / (YMax - YMin)));
  Salida_2 = DMin + ((Output[1] - YMin) * ((DMax - DMin) / (YMax - YMin)));

  X1_K_1 = Salida_1;
  X2_K_1 = Salida_2;
  U_K_1 = Entrada;

  Serial.println("Salida1\tSalida2\tEntrada");
  
  Serial.print(Salida_1); // print as an ASCII-encoded decimal - same as "DEC"
  Serial.print(",");
  Serial.print(Salida_2);  
  Serial.print(",");
  Serial.println(Entrada);

  //Serial.print(Salida_1);
  //Serial.print(Salida_2);  
  //Serial.println(Entrada);

  delay(50);
}
