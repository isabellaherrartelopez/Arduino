#include <math.h>

/******************************************************************
   Configuración de la Red
 ******************************************************************/
const int PatternCount = 1;//todos los datos
const int HiddenNodes = 10; //cuantas neuronos en capa oculta
const int InputNodes = 5; //Nodos de entrada
const int OutputNodes = 1;

float Samples=0;
float randNumber; //uniform random number

//INVERSO

float r_k = 0;
float y_k = 0;
float y_k_1 = 0;

float u_k_1 = 0;
float u_k_2 = 0;



// Estos valores son necesarios para la normalización de los datos cuando entran y salen de la red
// Rango de normalización. Esto es para normalizar entre -1 y 1 ya que se uso tanisg
const float YMin = -1;
const float YMax = 1;

//Modelo inverso [Y(k+1),Y(k-1),U(k-1),U(k-2)]
// Valores para normalizar la entrada de la red (estos valores se toman del sistema desde matlab)

//Y(K)
const float X1Miny = -0.5024;//valor minimo de y(k+1)
const float X1Maxy = 4.9306;//valor maximo de y(k+1)

const float X2Miny = -0.5024;//valor minimo de y(k)
const float X2Maxy = 4.9306;//valor maximo de y(k)

const float X3Miny = -0.5024;//valor minimo de y(k-1)
const float X3Maxy = 4.9306;//valor maximo de y(k-1)

//U(K)
const float X1Minu = 0.0385;//valor minimo de U(K-1)
const float X1Maxu = 4.6735;//valor maximo de U(K-1)

const float X2Minu = 0.0385;//valor minimo de U(K-2)
const float X2Maxu = 4.6735;//valor maximo de U(K-2)



// Valores para Desnormalizar la salida de la red

//INVERSO  0.0385    4.6735
const float DMin = 0.0385;
const float DMax = 4.6735;

// Los pesos de la red fueron obtenidos en Matlab y se copiaron a este programa
// Pesos capa oculta
//INVERSO
const float HiddenWeights[HiddenNodes][InputNodes + 1]={
  { 1.1946,   -1.5833,    0.4467,    0.7987,   -0.1610,   -2.5103},
  { 0.7116,    0.8773,   -1.4080,   -0.8950,   -0.3300,   -1.7935},
  {-1.1726,    0.9465,    0.7620,   -0.8261,    0.7989,    1.6780},
  { 0.8767,    0.2290,    1.4767,   -1.4149,    0.8700,   -0.5935},
  {-3.8180,    6.7001,   -3.0297,    0.0702,    0.0060,   -0.1437},
  {-1.1262,    0.6241,    0.3369,    0.2080,    0.0485,    0.1029},
  { 3.4982,   -6.2186,    2.8493,   -0.0634,    0.0033,   -0.2089},
  {-1.2924,    2.8235,   -1.5285,   -0.0702,   -0.0149,   -0.0649},
  { 1.2973,    0.7788,    0.3750,    1.5115,    0.7538,    1.1373},
  { 0.8741,    0.9731,   -0.9377,   -0.8934,   -0.0129,    2.5748},
  
  };

// Pesos capa de salida
const float OutputWeights[OutputNodes][HiddenNodes+1]  = 
{
  {0.0765,   -0.0061,    0.0001,   -0.0000,   -8.5708,   -0.4087,    7.8828, -3.0472,   -0.0000,   -0.0020,    0.1970}
};  

/******************************************************************
 * End Network Configuration
 ******************************************************************/


int i, j, p, q, r;
float Accum; //Neta
float Hidden[HiddenNodes]; 
float Output[OutputNodes]; 
float Input[InputNodes][PatternCount]; //PatternCount {5,1}
 

/******************** Filtro *************/
//y(k) = 1.81y(k-1) - 0.8187y(k-2) + 0.004679u(k-1) + 0.004377u(k-2) 
float a1 = 1.81;  
float a2 = 0.8187; 
float b1 = 0.004679; 
float b2 = 0.004377;
float y_filtro_k = 0;
float y_filtro_k_1 = 0;
float y_filtro_k_2 = 0;
float u_filtro_k_1 = 0;
float u_filtro_k_2 = 0;


float Entrada;
float Salida;
float Tiempo;

void setup(){
  //start serial connection
  Serial.begin(9600);
  randomSeed(analogRead(0)); //entrada randomica 

}

void loop(){
/*
if (Serial.peek() != 0) {
    Serial.print(Salida);
    Serial.print(",");
    //Serial.print("  Entrada: ");
    Serial.println( Entrada);
    Serial.flush();
    y_k = Serial.parseFloat();
  }
*/  
  



Samples=Samples+1;

if (Samples==1)
{
  randNumber = random(0,50);
  Samples=Samples+1;
}

if (Samples>60)
{
  Samples=0;
}

Entrada=(randNumber/10);

y_filtro_k = a1 * y_filtro_k_1 - a2 * y_filtro_k_2 + b1 * u_filtro_k_1 + b2 * u_filtro_k_2;
//y_filtro_k = Entrada;


u_filtro_k_2 = u_filtro_k_1;
u_filtro_k_1 = Entrada;

y_filtro_k_2 = y_filtro_k_1;
y_filtro_k_1 = y_filtro_k;







// Normalización de la entrada que se usará en la red neuronal
Input[0][0]=YMin+ ((r_k-X1Miny)*((YMax-YMin)/(X1Maxy-X1Miny))); //  y(k+1) = r(k)
Input[1][0]=YMin+ ((y_k-X2Miny)*((YMax-YMin)/(X2Maxy-X2Miny))); //  y(k) 
Input[2][0]=YMin+ ((y_k_1-X3Miny)*((YMax-YMin)/(X3Maxy-X3Miny)));// y(k-1) 
Input[3][0]=YMin+ ((u_k_1-X1Minu)*((YMax-YMin)/(X1Maxu-X1Minu)));// u(k-1)
Input[4][0]=YMin+ ((u_k_2-X2Minu)*((YMax-YMin)/(X2Maxu-X2Minu)));// u(k-2)

/******************************************************************
* Compute hidden layer activations
******************************************************************/


    for( i = 0 ; i < HiddenNodes ; i++ ) {    
      Accum = HiddenWeights[i][InputNodes] ; //bias 
      for( j = 0 ; j < InputNodes ; j++ ) {
        Accum += HiddenWeights[i][j]*Input[j][0]; //bias + pesos x entradas
      }
      Hidden[i] = (2.0/(1.0 + exp(-2*Accum)))-1.0; // funcion de activacion
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

   Output[i] = Accum; //Funcion de activación lineal por lo tanto salida es igual a la neta

    }

// La salida da la red esta nromalizada, para que quede en el rango original hay que desnormalizar
// Desnormalización de la salida de la red neuronal
Salida=DMin+ ((Output[0]-YMin)*((DMax-DMin)/(YMax-YMin)));




y_k_1 = y_k;
y_k = r_k;
r_k = y_filtro_k; //Y_K la primera entrada de la red le llega la salida del filtro
//r_k = Entrada;

u_k_2=u_k_1;
u_k_1=Salida; //U es la accion de control de la red 

/*
// se lee y(k) de la planta
if (Serial.available()==0){
y_k = Serial.parseFloat();  
}
*/

Serial.print(u_k_1); //Salida accion de control
//Serial.print( Entrada_n1); 
Serial.print(","); 
Serial.println(Entrada); //Random 

delay(50);


}
