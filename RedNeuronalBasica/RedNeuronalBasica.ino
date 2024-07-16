/******************************************************************
 * Programa que genera reproduce una función lógica AND usando una 
 * red neuronal monocapa
 * Autor:
 *      Jesús Alfonso López 
 *      jalopez@uao.edu.co
 ******************************************************************/

void setup(){
  //start serial connection
  Serial.begin(9600);
  //configure pin2 and pin3 as an input and enable the internal pull-up resistor
  pinMode(2, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(13, OUTPUT); 
  }

void loop(){
  float Neta;
  float W1;
  float W2;
  float B;
  int Salida;
  int X1Val = digitalRead(2);
  int X2Val = digitalRead(4);
//Definición de los pesos de la red  
W1=1.0 ;
W2=1.0;
B=-1.5;
//Cálculo de la neta
Neta=X1Val*W1+X2Val*W2+B;
//Cálculo de la neta con función de activación escalón
//El led del pin 13 de prede o apaga dependiendo de la salida que genere la red
if (Neta>=0) 
{
  Salida=1;
  digitalWrite(13, HIGH);
}
else
{
  Salida=0; 
  digitalWrite(13, LOW); 
}
   Serial.print(X1Val);       // print as an ASCII-encoded decimal - same as "DEC"
   Serial.print("\t");    // prints a tab
   Serial.print(X2Val);       // print as an ASCII-encoded decimal - same as "DEC"
   Serial.print("\t");    // prints a tab
   Serial.print(Neta);       // print as an ASCII-encoded decimal - same as "DEC"
   Serial.print("\t");    // prints a tab
   Serial.print(Salida);       // print as an ASCII-encoded decimal - same as "DEC"
   Serial.print("\t");    // prints a tab
   Serial.print("\n");    // prints a tab
   delay(1000);  

}
