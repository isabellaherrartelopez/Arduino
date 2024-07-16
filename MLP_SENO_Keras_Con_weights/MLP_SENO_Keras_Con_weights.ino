/******************************************************************
 * Programa que genera una onda senoidal usando una red neuronal
 * MLP superficial
 * Autor:
 *      Jesús Alfonso López 
 *      jalopez@uao.edu.co
 ******************************************************************/

#include <math.h>

/******************************************************************
 * Definición estructura de la red
 ******************************************************************/

const int HiddenNodes = 10;
const int InputNodes = 1;
const int OutputNodes = 1;

// Los pesos de la red fueron obtenidos en TensorFlow y se copiaron a este programa
// Pesos capa oculta
//HiddenWeights[2][10]
const float HiddenWeights[InputNodes+1][HiddenNodes]= {
{ 0.7842761,   0.19795926, -0.8510847,  -0.7469934,  -0.5711364,  -0.7873737, -0.84794205,  0.31743935,  0.250596,   -0.6681011 },
{-2.1813004,  -0.6895105,  -0.45699698 , 3.0326538,   1.1498396,   1.0462041,  0.44000843, -1.7400978,  -1.1975504,   1.7032444 }
    };

// Pesos capa de salida
//OutputWeights[11][1]
const float OutputWeights[HiddenNodes+1][OutputNodes]  = {
 {-0.7218174 },
 { 1.9207445 },
 {-0.76334596},
 { 1.7340049 },
 { 0.9664291 },
 {-0.36953878},
 {-0.34595907},
 { 1.6919754 },
 { 1.8190591 },
 { 0.7215095 },
 {0.4614294 }
     }; 

int i, j, p, q, r;
float Accum;
float Hidden[HiddenNodes];
float Output[OutputNodes];
float Input[InputNodes];
 

void setup(){
  //start serial connection
  Serial.begin(9600);
}

void loop(){
  
  float Entrada;
  float Salida;
  float Tiempo;

Tiempo=millis();
//Artificio para generar una entrada entre 0 y 2*pi
Entrada=(fmod(Tiempo,6283))/1000; 

Input[0]=Entrada;
/******************************************************************
* Cálculo de la salida de capa oculta
******************************************************************/
    for( i = 0 ; i < HiddenNodes ; i++ ) {    
      Accum = HiddenWeights[InputNodes][i] ;
      for( j = 0 ; j < InputNodes ; j++ ) {
        Accum += HiddenWeights[j][i]*Input[j];
      }
      // función de activación tangente hiperbólica
        Hidden[i] = (exp(Accum)-exp(-Accum))/(exp(Accum)+exp(-Accum));
    }

/******************************************************************
* Cálculo de la salida de capa de salida
******************************************************************/

    for( i = 0 ; i < OutputNodes ; i++ ) {    
      Accum = OutputWeights[HiddenNodes][i] ;
      for( j = 0 ; j < HiddenNodes ; j++ ) {
        Accum +=  OutputWeights[j][i]*Hidden[j];
      }
      // función de activación lineal
        Output[i] = Accum; 
    }

Salida=Output[0];

Serial.print("SalidaDes:"); 
Serial.println(Salida);      

delay(50);  

}
