/******************************************************************
  %%Sistema doble masa resorte armotiguador no lineal

 ******************************************************************/

#include <math.h>

/******************************************************************
   Configuración de la Red Modelo inverso
 ******************************************************************/
const int PatternCount_inv = 1;//todos los datos
const int HiddenNodes_inv = 10; //cuantas neuronos en capa oculta
const int InputNodes_inv = 5; //Nodos de entrada
const int OutputNodes_inv = 1;

float randNumber; //uniform random number

//INVERSO variables para hacer el regresos
float Y_K_1 = 0;
float Y_K_2 = 0;
float Y_K = 0;

float U_K_1 = 0;
float U_K_2 = 0;

// Estos valores son necesarios para la normalización de los datos cuando entran y salen de la red
// Rango de normalización. Esto es para normalizar entre -1 y 1 ya que se uso tanisg
const float YMin = -1;
const float YMax = 1;

//Modelo inverso [Y(k+1),Y(k-1),U(k-1),U(k-2)]
// Valores para normalizar la entrada de la red (estos valores se toman del sistema desde matlab)
//Y(K)
const float X1Miny = 0.1729;//valor minimo de y(k+1)
const float X1Maxy = 4.6735;//valor maximo de y(k+1)

const float X2Miny = -2.0535;//valor minimo de y(k)
const float X2Maxy = 3.6785;//valor maximo de y(k)

const float X3Miny = -2.0535;//valor minimo de y(k-1)
const float X3Maxy = 3.6785;//valor maximo de y(k-1)

//U(K)
const float X1Minu = -131.1569;//valor minimo de U(K-1)
const float X1Maxu = 132.4939;//valor maximo de U(K-1)

const float X2Minu = -131.1569;//valor minimo de U(K-2)
const float X2Maxu = 132.4939;//valor maximo de U(K-2)

//INVERSO  0.0833    2.3311
const float DMin_inv = -131.1569;
const float DMax_inv = 132.4939;

// Los pesos de la red fueron obtenidos en Matlab y se copiaron a este programa
// Pesos capa oculta
//INVERSO
const float HiddenWeights_inv[HiddenNodes_inv][InputNodes_inv + 1] = {

   {3.2980,   -5.4884,    2.1801,    0.5239,    0.0238,   -1.9200},
   {-1.4712,  -0.1714,   -0.3880,   -0.6554,    0.3525,   -2.5310},
   {-0.2870,   3.8306,   -1.5731,    0.3548,   -0.2270,    2.9957},
   {6.0335,   -2.1841,   -1.0740,   -2.2539,   -0.4834,    0.0653},
   {15.4308,  -21.7453,   7.9501,   -3.1388,   -0.2638,   -0.6005},
   {-7.2915,   4.8594,   -0.2556,    2.2037,    0.5241,   -0.1411},
   {15.1106,  -17.1150,   3.8295,   -0.1347,   -0.5587,    0.5531},
   {-7.7633,   17.5524,  -8.0776,   -1.0186,   -0.7979,    0.2151},
   {5.9692,   -2.2530,    2.2977,   -0.2733,    2.2213,    6.0304},
   {6.9265,    6.6665,   -6.6232 ,  -2.4427,   -1.0936,    4.6545},

};

// Pesos capa de salida
const float OutputWeights_inv[OutputNodes_inv][HiddenNodes_inv + 1]  =
{
  {5.1963,    5.8311,   20.6740,   -7.0137,   -0.5956,   -8.4903,    0.2455,   -1.3666,   -0.1094,   -0.1779,   -9.7719}
};

//int i_inv, j_inv, p_inv, q_inv, r_inv;
float Accum_inv; //Neta
float Hidden_inv[HiddenNodes_inv]; //vector 6 posiciones
float Output_inv[OutputNodes_inv]; //vector 1,1
float Input_inv[InputNodes_inv][PatternCount_inv]; //PatternCount {2,1} los datos siempre llegan ahi


/******************************************************************
   End Network Configuration Inverso
 ******************************************************************/

///////////////////////////////////////////////////////////MODELO ARX DE LA PLANTA

const int PatternCount_MP = 1; //Neuronas capa oculta ARX
const int HiddenNodes_MP = 10;
const int InputNodes_MP = 4;
const int OutputNodes_MP = 1;

//ARX
float Y_K_1MP = 0;
float Y_K_2MP = 0;
float U_K_1MP = 0;
float U_K_2MP = 0;

// Valores para normalizar la entrada de la red
const float Y_k_1Min_MP = -2.0535;
const float Y_k_1Max_MP = 3.6785;

const float Y_k_2Min_MP = -2.0535;
const float Y_k_2Max_MP = 3.6785;

const float U_k_1Min_MP = -131.1569;
const float U_k_1Max_MP = 132.4939;

const float U_k_2Min_MP = -131.1569;
const float U_k_2Max_MP = 132.4939;

//ARX Salida
const float DMin_MP = -131.1569;
const float DMax_MP = 132.4939;


const float HiddenWeights_MP[HiddenNodes_MP][InputNodes_MP + 1] = { // inputnodes matrix de bias pero como queremos todo en una sola matrix
   {-0.1373,   -3.3258,    2.4618,   -1.9290,   -3.9580},
   {-1.5658,    0.8810,    0.0772,    0.0567,    0.8701},
   {-1.0900,    0.3529,    0.2854,   -2.4170,    1.3458},
   {-2.5213,    1.3728,    0.3138,   -0.3766,   -0.0382},
   { 2.0142,   -2.5076,   -0.1101,   -1.6715,   -0.8786},
   {-3.3177,    1.7303,   -0.7952,    0.7200,   -1.2373},
   { 0.5498,   -0.1149,    0.0999,   -0.5660,    0.5971},
   {-0.7658,    0.8934,    0.2452,    1.0283,   -0.9029},
   { 1.9831,   -0.5060,    1.4879,   -1.0799,    2.7893},
   {-2.1202,    0.3244,   -2.0833,    1.2289,   -2.9278},

};

// Pesos capa de salida ARX
const float  OutputWeights_MP[OutputNodes_MP][HiddenNodes_MP + 1] = {
  {-0.0553,   -1.0698,    0.0446,   -0.2502,   -0.1383,   -0.2233,    0.5933,    0.3600,   -2.5840,   -0.9484,   1.9088}
};

//int i_MP, j_MP, p_MP, q_MP, r_MP;
float Accum_MP;
float Hidden_MP[HiddenNodes_MP];
float Output_MP[OutputNodes_MP];
float Input_MP[InputNodes_MP][PatternCount_MP];

//////////////////////////////////////////////////////////////// Acaba el modelo ARX de la planta

float a = -1;
double EsfControl;
double SalidaSis;
float Compara;

void setup() {
  //start serial connection
  Serial.begin(9600);
  randomSeed(analogRead(0)); //entrada randomica
}

void loop() {

  Serial.flush();
  if (a == -1) {
    Serial.println(0);
  }
  a = Serial.parseFloat();
  Compara = -SalidaSis + a; // Entrada a la red Inversa
  EsfControl = ModeloInverso(Compara, SalidaSis);
  SalidaSis = PlantaARX(EsfControl);
  delay(100);  //tao del sistema 50mili = 0.05
}

////////////////////Metodo del modelo inverso
float ModeloInverso(float comparacion, float salidaSys) {

  float Entrada_inv;
  float Salida_inv;
  float Samples = 0;


  Samples = Samples + 1;

  if (Samples == 1)
  {
    randNumber = random(25); //entre cero y 50
    Samples = Samples + 1;
  }

  if (Samples > 25) // sino pones 60
  {
    Samples = 0;
  }

  Entrada_inv = (randNumber / 10);

  // Normalización de la entrada que se usará en la red neuronal del movelo inverso
  Input_inv[0][0] = YMin + ((Y_K   - X1Miny) * ((YMax - YMin) / (X1Maxy - X1Miny))); //primer entrada Yk
  Input_inv[1][0] = YMin + ((Y_K_1 - X2Miny) * ((YMax - YMin) / (X2Maxy - X2Miny))); //Y-1 entrada
  Input_inv[2][0] = YMin + ((Y_K_2 - X3Miny) * ((YMax - YMin) / (X3Maxy - X3Miny))); //Y-2 entrada

  Input_inv[5][0] = YMin + ((U_K_1 - X1Minu) * ((YMax - YMin) / (X1Maxu - X1Minu)));
  Input_inv[6][0] = YMin + ((U_K_2 - X2Minu) * ((YMax - YMin) / (X2Maxu - X2Minu)));


  /******************************************************************
    Compute hidden layer activations Modelo inverso
  ******************************************************************/


  for ( int i = 0 ; i < HiddenNodes_inv ; i++ ) {
    Accum_inv = HiddenWeights_inv[i][InputNodes_inv] ; //bias
    for ( int j = 0 ; j < InputNodes_inv ; j++ ) {
      Accum_inv += HiddenWeights_inv[i][j] * Input_inv[j][0]; //bias + pesos x entradas
    }
    Hidden_inv[i] = (2.0 / (1.0 + exp(-2 * Accum_inv))) - 1.0; // funcion de activacion
  }

  /******************************************************************
    Compute output layer activations Modelo Inverso
  ******************************************************************/
  //   Serial.print("\n");    // prints a tab
  for ( int i = 0 ; i < OutputNodes_inv ; i++ ) {
    Accum_inv = OutputWeights_inv[i][HiddenNodes_inv] ;
    for ( int j = 0 ; j < HiddenNodes_inv ; j++ ) {
      Accum_inv +=  OutputWeights_inv[i][j] * Hidden_inv[j];
    }

    Output_inv[i] = Accum_inv; //Funcion de activación lineal por lo tanto salida es igual a la neta

  }

  Salida_inv = DMin_inv + ((Output_inv[0] - YMin) * ((DMax_inv - DMin_inv) / (YMax - YMin)));


  //Actualizacion de estados del modelo  inverso
  Y_K_2 = Y_K_1;
  Y_K_1 = salidaSys;
  Y_K = (comparacion); //Y_K la primera entrada de la red le llega la salida del filtro

  U_K_2 = U_K_1;
  U_K_1 = Salida_inv; //U es la accion de control de la red


  Serial.print(U_K_1); //Salida accion de control
  //Serial.print( Entrada_n1);
  //Serial.print(",");
  //Serial.println( Entrada); //Random

  return Salida_inv;

}
///////////////Fin metodo modelo inverso

//////////////Metodo del ARX para la planta

float PlantaARX(float Salida_INV) {
  float Entrada_MP;
  float Salida_MP;



  // Normalización de la entrada (-1,1) que se usará en la red neuronal para el modelo ARX
  Input_MP[0][0] = YMin + ((Y_K_1MP - Y_k_1Min_MP) * ((YMax - YMin) / (Y_k_1Max_MP - Y_k_1Min_MP))); //primer entrada Yk
  Input_MP[1][0] = YMin + ((Y_K_2MP - Y_k_2Min_MP) * ((YMax - YMin) / (Y_k_2Max_MP - Y_k_2Min_MP))); //Y-1 entrada

  Input_MP[5][0] = YMin + ((U_K_1MP - U_k_1Min_MP) * ((YMax - YMin) / (U_k_1Max_MP - U_k_1Min_MP)));
  Input_MP[6][0] = YMin + ((U_K_2MP - U_k_2Min_MP) * ((YMax - YMin) / (U_k_2Max_MP - U_k_2Max_MP)));

  /******************************************************************
    Compute hidden layer activations Modelo ARX Planta
  ******************************************************************/


  for ( int i = 0 ; i < HiddenNodes_MP ; i++ ) {
    Accum_MP = HiddenWeights_MP[i][InputNodes_MP] ; //bias
    for ( int j = 0 ; j < InputNodes_MP ; j++ ) {
      Accum_MP += HiddenWeights_MP[i][j] * Input_MP[j][0]; //bias + pesos x entradas
    }
    Hidden_MP[i] = (2.0 / (1.0 + exp(-2 * Accum_MP))) - 1.0; // funcion de activacion
  }

  /******************************************************************
    Compute output layer activations Modelo ARX planta
  ******************************************************************/
  //   Serial.print("\n");    // prints a tab
  for ( int i = 0 ; i < OutputNodes_MP ; i++ ) {
    Accum_MP = OutputWeights_MP[i][HiddenNodes_MP] ;
    for ( int j = 0 ; j < HiddenNodes_MP ; j++ ) {
      Accum_MP +=  OutputWeights_MP[i][j] * Hidden_MP[j];
    }

    Output_MP[i] = Accum_MP; //Funcion de activación lineal por lo tanto salida es igual a la neta

  }

  // La salida da la red esta nromalizada, para que quede en el rango original hay que desnormalizar
  // Desnormalización de la salida de la red neuronal
  Salida_MP = DMin_MP + ((Output_MP[0] - YMin) * ((DMax_MP - DMin_MP) / (YMax - YMin)));

  //Actualización de estados del modelo ARX de la planta
  Y_K_2MP = Y_K_1MP;
  Y_K_1MP = (Salida_MP); // Es la salida de la misma planta ARX

  U_K_2MP = U_K_1MP;
  U_K_1MP = Salida_INV; // Es la accion de control de la red del Inverso

  return Salida_MP;
  // Necesito enviar a arduino
}
