/* ************************************************
 *  Configuracion del sistema en espacio estados
 ***************************************************/
//Se definen las variables que se usarán en el programa

// Estado X1(K+1)
float X_1K_1;
// Estado X2(K+1)
float X_2K_1;
// Estado X1(K)
float X_1K=0;
// Estado X2(K)
float X_2K=0;
float randNumber1;
float randNumber2;

float h=0.02;

float sistemader1(float x1,float x2);
float sistemader2(float x1,float x2);

//Entrada
float u=0;
float Salida;




#include <math.h>

/******************************************************************
 * Configuración de la Red
 ******************************************************************/

const int NeuronasOculta = 10;
const int NeuronasEntrada = 5;
const int NeuronasSalida = 1;

float Samples=0;
float randNumber;

float R_K=0;
float Y_K=0;
float Y_K_1=0;
float U_K_1=0;
float U_K_2=0;

// Estos valores son necesarios para la normalización de los datos cuando entran y salen de la red
// Rango de normalización
const float YMin = -1;
const float YMax = 1;

// Valores para normalizar la entrada de la red
const float X1Min = -0.6050;
const float X1Max = 2.6225;
const float X2Min = -0.6050;
const float X2Max = 2.6225;
const float X3Min = -0.6050;
const float X3Max = 2.6225;
const float X4Min = 0.0385; 
const float X4Max = 4.6735;
const float X5Min = 0.0385;
const float X5Max = 4.6735;

// Valores para normalizar la salida de la red
const float DMin =0.0385;
const float DMax =4.6735;
  
// Los pesos de la red fueron obtenidos en Matlab y se copiaron a este programa
// Pesos capa oculta
const float PesosOculta[NeuronasOculta][NeuronasEntrada+1]= {
 {1.2140,-1.5404, 0.4160, 0.8532,-0.1002,-2.5851},
 {1.1462, 1.2162,-1.0820,-0.6469,-0.0575,-1.9699},
 {-0.7196, 1.2157, 1.0379,-0.4815, 0.8454, 1.4531},
 {0.3407,-0.1203, 1.0116,-1.5865, 0.7201,-1.0308},
 {-2.6363, 4.4118,-2.0582, 0.1385, 0.0020,-0.0664},
 {-0.8906, 0.5049, 0.0145, 0.6223, 0.0913, 0.0258},
 {2.1418,-3.7631, 1.8005,-0.0629, 0.0048,-0.2625},
 {-1.5009, 2.7904,-1.3556,-0.0562, 0.0020,-0.1353},
 {1.2825, 0.7437, 0.3640, 1.4597, 0.6541, 1.2985},
 {0.8578, 0.9743,-0.9285,-0.9634, 0.0578, 2.6210}
};



// Pesos capa de salida
const float PesosSalida[NeuronasSalida][NeuronasOculta+1] = {
 {0.0888,    0.0100,    0.0008,    0.0009,   -5.9896,   -0.0314,    4.7208,   -3.2831,   -0.0001,   -0.1322,    0.3188
}
};  

int i, j, p, q, r;
float Neta;
float CapaOculta[NeuronasOculta];
float CapaSalida[NeuronasSalida];
float CapaEntrada[NeuronasEntrada];


/******************************************************
 * Filtro
 *****************************************************/
float b1 = 0.09516;  
float a1 = 0.9048; 
float SalidaFiltro_K = 0; 
float SalidaFiltro_K_1 = 0;
float Entrada_K_1 = 0;

void setup(){
  //start serial connection
  Serial.begin(9600);
  randomSeed(analogRead(0));

}

void loop(){
  
  float Entrada;

  float Tiempo;

Samples=Samples+1;

if (Samples==1)
{
  randNumber = random(25);
  Samples=Samples+1;
}

if (Samples>25)
{
  Samples=0;
}



Entrada=(randNumber/10);

Entrada_K_1=Entrada;
SalidaFiltro_K=Entrada_K_1*b1+SalidaFiltro_K_1*a1;
SalidaFiltro_K_1=SalidaFiltro_K;


// Normalización de las entradas que se usará en la red neuronal
CapaEntrada[0]=YMin+ ((R_K-X1Min)*((YMax-YMin)/(X1Max-X1Min)));
CapaEntrada[1]=YMin+ ((Y_K-X2Min)*((YMax-YMin)/(X2Max-X2Min)));
CapaEntrada[2]=YMin+ ((Y_K_1-X2Min)*((YMax-YMin)/(X3Max-X3Min)));
CapaEntrada[3]=YMin+ ((U_K_1-X2Min)*((YMax-YMin)/(X4Max-X4Min)));
CapaEntrada[4]=YMin+ ((U_K_2-X2Min)*((YMax-YMin)/(X5Max-X5Min)));
/******************************************************************
* Calculo de la salida de la capa oculta
******************************************************************/

    for( i = 0 ; i < NeuronasOculta ; i++ ) {    
      Neta = PesosOculta[i][NeuronasEntrada] ;
      for( j = 0 ; j < NeuronasEntrada ; j++ ) {
        Neta += PesosOculta[i][j]*CapaEntrada[j];
      }
      CapaOculta[i] = (2.0/(1.0 + exp(-2*Neta)))-1.0; 
    }

/******************************************************************
* Calculo de la salida de la red
******************************************************************/
    for( i = 0 ; i < NeuronasSalida ; i++ ) {    
      Neta = PesosSalida[i][NeuronasOculta] ;
      for( j = 0 ; j < NeuronasOculta ; j++ ) {
        Neta +=  PesosSalida[i][j]*CapaOculta[j];
      }
    CapaSalida[i] = Neta; 
    }


// La salida da la red esta nromalizada, para que quede en el rango original hay que desnormalizar
// Desnormalización de la salida de la red neuronal
Salida=DMin+ ((CapaSalida[0]-YMin)*((DMax-DMin)/(YMax-YMin)));


/*****************************************************
 * Solución númerica de las ecuaciones de estado
 *****************************************************/
float k_11;
float k_12;
float k_21;
float k_22;
float k_31;
float k_32;
float k_41;
float k_42;
             
    k_11=h*sistemader1(X_1K,X_2K);
    k_12=h*sistemader2(X_1K,X_2K);
    
    k_21=h*sistemader1(X_1K+0.5*k_11,X_2K+0.5*k_12);
    k_22=h*sistemader2(X_1K+0.5*k_11,X_2K+0.5*k_12);
    
    k_31=h*sistemader1((X_1K+0.5*k_21),(X_2K+0.5*k_22));
    k_32=h*sistemader2((X_1K+0.5*k_21),(X_2K+0.5*k_22));
    
    k_41=h*sistemader1((X_1K+k_31),(X_2K+k_32));
    k_42=h*sistemader2((X_1K+k_31),(X_2K+k_32));

    X_1K_1=X_1K + (0.1667)*(k_11+2*k_21+2*k_31+k_41);  
    X_2K_1=X_2K + (0.1667)*(k_12+2*k_22+2*k_32+k_42);  





U_K_2=U_K_1;
U_K_1=Salida;

Y_K_1=Y_K;
Y_K=X_2K_1;

R_K=SalidaFiltro_K;

//Se actualizan el valor de los estados para la siguiente iteración
  X_1K=X_1K_1;
  X_2K=X_2K_1;





//Serial.print("Salida: "); 
Serial.print(X_2K_1);
Serial.print(",");      
//Serial.print("  Entrada: ");
Serial.print(SalidaFiltro_K); 
Serial.print(","); 
Serial.println( Entrada);

delay(50);  
}


// Funciones que representan las ecuaciones diferenciales del sistema
float sistemader1(float x1,float x2){
float derX1;
derX1=-2*x1-10*x2+5*Salida;
return derX1;
}

float sistemader2(float x1,float x2){
float derX2;
derX2=x1;
return derX2;
}
