/******************************************************************
 * Programa que genera reproduce una función lógica XOR usando una 
 * red neuronal superficial
 * Autor:
 *      Jesús Alfonso López 
 *      jalopez@uao.edu.co
 ******************************************************************/

  //Definición de los pesos de la red  

//[[ 4.4491844 -3.2876225  4.363248 ]
// [-3.1173658  2.8611434 -3.1131463]]
//[-2.7249172 -1.1544945 -2.6519682]
#include <math.h>

  float W11Co=4.4491844;
  float W21Co=-3.1173658;
  float B1Co=-2.7249172;
  float Neta1Co;
  float Salida1Co;
  
  float W12Co=-3.2876225;
  float W22Co=2.8611434;
  float B2Co=-1.1544945;
  float Neta2Co;
  float Salida2Co;


  float W13Co=4.363248;
  float W23Co=-3.1131463;
  float B3Co=-2.6519682;
  float Neta3Co;
  float Salida3Co;

//[[3.1632652]
// [5.907836 ]
// [2.80647  ]]
//[5.3300915]

  float W11Cs=3.1632652;
  float W21Cs=5.907836;
  float W31Cs=2.80647;
  float B1Cs=5.3300915;
  float Neta1Cs;
  float Salida1Cs;

  int Salida;
  
void setup(){
  //start serial connection
  Serial.begin(9600);
  //configure pin2 and pin3 as an input and enable the internal pull-up resistor
  pinMode(2, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(13, OUTPUT); 
  }

void loop(){
 

  int X1Val = digitalRead(2);
  int X2Val = digitalRead(4);

//Cálculo de la neta 1 CO
Neta1Co=X1Val*W11Co+X2Val*W21Co+B1Co;
//Cálculo de la neta con función de activación Tanh
Salida1Co = ((exp(Neta1Co) - exp(-Neta1Co))/(exp(Neta1Co) + exp(-Neta1Co)));

//Cálculo de la neta 2 CO
Neta2Co=X1Val*W12Co+X2Val*W22Co+B2Co;
//Cálculo de la neta con función de activación Tanh
Salida2Co = ((exp(Neta2Co) - exp(-Neta2Co))/(exp(Neta2Co) + exp(-Neta2Co)));

//Cálculo de la neta 3 CO
Neta3Co=X1Val*W13Co+X2Val*W23Co+B3Co;
//Cálculo de la neta con función de activación Tanh
Salida3Co = ((exp(Neta3Co) - exp(-Neta3Co))/(exp(Neta3Co) + exp(-Neta3Co)));


//Cálculo de la neta 1 Cs
Neta1Cs=Salida1Co*W11Cs+Salida2Co*W21Cs+Salida3Co*W31Cs+B1Cs;
//Cálculo de la neta con función de activación Tanh
Salida1Cs = 1 / (1 + exp(-Neta1Cs));


////El led del pin 13 de prede o apaga dependiendo de la salida que genere la red
if (Neta1Cs>=0) 
{
 // Salida=1;
  digitalWrite(13, HIGH);
}
else
{
 // Salida=0; 
  digitalWrite(13, LOW); 
}
   Serial.print(X1Val);       // print as an ASCII-encoded decimal - same as "DEC"
   Serial.print("\t");    // prints a tab
   Serial.print(X2Val);       // print as an ASCII-encoded decimal - same as "DEC"
   Serial.print("\t");    // prints a tab
//   Serial.print(Neta);       // print as an ASCII-encoded decimal - same as "DEC"
//   Serial.print("\t");    // prints a tab
   Serial.print(Salida1Cs);       // print as an ASCII-encoded decimal - same as "DEC"
   Serial.print("\t");    // prints a tab
   Serial.print("\n");    // prints a tab
   delay(1000);  

}
