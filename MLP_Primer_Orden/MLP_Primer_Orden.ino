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

const int PatternCount = 1;//todos los datos
const int HiddenNodes = 10;
const int InputNodes = 2;
const int OutputNodes = 1;

float Samples=0;
float randNumber;
float Y_K_1=0;
float U_K_1=0;
// Estos valores son necesarios para la normalización de los datos cuando entran y salen de la red
// Rango de normalización
const float YMin = -1;
const float YMax = 1;


// Valores para normalizar la entrada de la red (estos valores se toman del sistema desde matlab)
const float X1Min = 0;//valor minimo de Y
const float X1Max = 2.3324;//valor maximo de Y
const float X2Min = 0.0385;//valor minimo de U
const float X2Max = 4.6735;//valor maximo de U
// Valores para normalizar la salida de la red
     
const float DMin = 0.0304 ;//valor minimo de la salida del sistema
const float DMax = 2.3324;//valor maximo de la salida del sistema

// Los pesos de la red fueron obtenidos en Matlab y se copiaron a este programa
// Pesos capa oculta
const float HiddenWeights[HiddenNodes][InputNodes+1]= {
    {    2.9656,   -3.0779   -4.5392},
    {    3.2358,    3.0299,   -3.4406},
    {   -2.8676,    3.3185,    2.6371},
    {    0.2000,    0.0210,   -0.2081},
    {    2.4334,    3.6194,   -1.0010},
    {   -3.1351,   -2.9604,   -1.1380},
    {   -1.0968,   -0.1143,   -0.8778},
    {    0.5309,    4.4285,    2.2375},
    {    2.4988,    0.2690,    3.0061},
    {    3.0463,    2.7814,    4.7591}

};

// {   3.9399,    0.3609,   -3.9227},
// {   3.0921,   -3.9409,   -1.9571},
// {  -2.8467,   -3.6670,    2.1591},
// {  -2.0853,   -0.2878,    1.0843},
// {  -0.4987,    4.2863,    0.6042},
// {   2.8407,   -3.4766,    0.5117},
// {   0.4743,    0.0417,    0.3156},
// {   2.9659,   -3.2387,    2.4841},
// {  -2.4386,    3.4401,   -3.6436},
// {   2.3125,   -3.2919,    4.8585}

//  { -5.6372,    5.3821},
//  { 10.4686,   -7.6237},
//  {  5.1070,   -3.2261},
//  {  4.4430,   -1.4989},
//  {  4.3611,   -0.1144},
//  {  8.6242,    0.7900},
//  { -4.1019,   -1.3459},
//  {  4.4553,    2.9074},
//  { -9.3769,   -7.1024},
//  {  5.4661,    5.4731}
// 
//    
  
//{   12.1008 , -12.2479},
//{   -2.0963 ,   1.9875},
//{   -6.6196 ,   4.2421},
//{   -6.2267 ,   2.1509},
//{    2.1891 ,  -0.2349},
//{    2.9252 ,   0.5600},
//{    6.3450 ,   2.2821},
//{   -2.9795 ,  -2.3524},
//{    5.3267 ,   5.0348},
//{   8.6169  ,  9.1310} 
 
    

// Pesos capa de salida
const float OutputWeights[OutputNodes][HiddenNodes+1]  = {
{0.0001,   -0.0003,   0.0001,   4.5464,   -0.0001,    0.0002,   -0.0817,   -0.0001,    0.0296,   -0.0002, 0.8333}
};  

//{0.0814,   -0.0047,    0.0037,   -0.1021,   -0.0001,   -0.0035, 1.9606,   -0.0096,   -0.0671,   -0.5309,    0.0177}
//   {-1.2589,   -0.0347,   -1.1682,    1.5808,   -2.3183,   -0.0512,  -3.0535,   -2.6312,    0.0598,    1.4445,    0.3498}
//};
 // {0.0046,   -1.5960,   -0.0046,    0.0045,   -1.3084, -0.3446,   -0.0055,   -0.7463,    0.1888,    0.1186, 0.3803 }
 //   }; 

/******************************************************************
 * End Network Configuration
 ******************************************************************/


int i, j, p, q, r;
float Accum;
float Hidden[HiddenNodes];
float Output[OutputNodes];
float Input[InputNodes][PatternCount];
 

void setup(){
  //start serial connection
  Serial.begin(9600);
  randomSeed(analogRead(0));

}

void loop(){
  
  float Entrada;
  float Salida;
  float Tiempo;

//Tiempo=millis();
//Entrada=(fmod(Tiempo,6283))/1000; 

Samples=Samples+1;

if (Samples==1)
{
  randNumber = random(50);
  Samples=Samples+1;
}

if (Samples>60)
{
  Samples=0;
}

//Tiempo=millis();
//Entrada=(fmod(Tiempo,6283))/1000; 
Entrada=(randNumber/10);

// Normalización de la entrada que se usará en la red neuronal
Input[0][0]=YMin+ ((Y_K_1-X1Min)*((YMax-YMin)/(X1Max-X1Min)));
Input[1][0]=YMin+ ((U_K_1-X2Min)*((YMax-YMin)/(X2Max-X2Min)));

/******************************************************************
* Compute hidden layer activations
******************************************************************/


    for( i = 0 ; i < HiddenNodes ; i++ ) {    
      Accum = HiddenWeights[i][InputNodes] ;
      for( j = 0 ; j < InputNodes ; j++ ) {
        Accum += HiddenWeights[i][j]*Input[j][0];
      }
      Hidden[i] = (2.0/(1.0 + exp(-2*Accum)))-1.0; 
    }



/******************************************************************
* Compute output layer activations
******************************************************************/
 //   Serial.print("\n");    // prints a tab 
    for( i = 0 ; i < OutputNodes ; i++ ) {    
      Accum = OutputWeights[i][HiddenNodes] ;
      for( j = 0 ; j < HiddenNodes ; j++ ) {
        Accum +=  OutputWeights[i][j]*Hidden[j];
      }

   Output[i] = Accum; 

    }

// La salida da la red esta nromalizada, para que quede en el rango original hay que desnormalizar
// Desnormalización de la salida de la red neuronal
Salida=DMin+ ((Output[0]-YMin)*((DMax-DMin)/(YMax-YMin)));

Y_K_1=Salida;
U_K_1=Entrada;

Serial.print("Salida: "); 
Serial.print( Salida);       // print as an ASCII-encoded decimal - same as "DEC"
Serial.print("  Entrada: "); 
Serial.println( Entrada);

delay(50);  


}
