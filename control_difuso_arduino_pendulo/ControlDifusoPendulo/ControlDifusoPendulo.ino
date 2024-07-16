/*Implementacion de un controlador difuzo tipo sugeno.
 * La planta se tambien se encuentra implementada
 * Planta: pendulo invertido
 */

// Librería con las funciones para el sistema difuso
#include "sfun_fuzzyutilsug.h"
//Se definen las variables que se usarán en el programa
// Contador de muestras
float Samples=0;
// Salida de la planta Y(K)se hace igual a la condición inicial
float Y_K=0.2;

// Entrada de la planta U(K)
float U_K=0;
// Estado X1(K+1)
float X_1K_1;
// Estado X2(K+1)
float X_2K_1;
// Estado X3(K+1)
float X_3K_1;
// Estado X1(K)
float X_1K=0.2;
// Estado X2(K)
float X_2K=0;
// Estado X3(K)
float X_3K=0;

// Funciones con las derivadas del sistema
float h=0.001;
float sistemader1(float u,float x1,float x2,float x3);
float sistemader2(float u,float x1,float x2,float x3);
float sistemader3(float u,float x1,float x2,float x3);



float randNumber;
// Referencia
float R_K;
// Error y Derivada de la variable
float E, Edot;
float E_K_1=0;
// Tiempo Muestreo es igual a 10*h
float Ts=0.01;

// Valor de la perturbación
float Per=0;

// Salida del sistema difuso
float U_Fuzzy=0;

void setup() {
  // Se inicializa la comunicación serial
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void loop() {
 // Se crea la variable tipo fuszzy_system
  FUZ_SYS fuzzy_system; 
// Variables del método numérico
int i;
float k_11;
float k_12;
float k_13;

float k_21;
float k_22;
float k_23;

float k_31;
float k_32;
float k_33;

float k_41;
float k_42;
float k_43;



// Contador de muestras para saber cuando se va a cambiar la entrada al sistema
  Samples=Samples+1;


// Se activa la perturbación en la muestra 250
if (Samples>250)
{
  //Samples=0;
  Per=0.05;
}

// Se inactiva la perturbación en la muestra 400
if (Samples>400)
{
  //Samples=0;
  Per=0.0;
}
// La referencia es cero (radianes)
R_K=0;
// Método numérico para simular la dinámica no lineal del péndulo
//----------------------
for( i = 0 ; i < 10 ; i++ ) {
    k_11=h*sistemader1(U_K,X_1K,X_2K,X_3K);
    k_12=h*sistemader2(U_K,X_1K,X_2K,X_3K);
    k_13=h*sistemader3(U_K,X_1K,X_2K,X_3K);    
    
    k_21=h*sistemader1(U_K,X_1K+0.5*k_11,X_2K+0.5*k_12,X_3K+0.5*k_13);
    k_22=h*sistemader2(U_K,X_1K+0.5*k_11,X_2K+0.5*k_12,X_3K+0.5*k_13);
    k_23=h*sistemader3(U_K,X_1K+0.5*k_11,X_2K+0.5*k_12,X_3K+0.5*k_13);  
    
    k_31=h*sistemader1(U_K,(X_1K+0.5*k_21),(X_2K+0.5*k_22),(X_3K+0.5*k_23));
    k_32=h*sistemader2(U_K,(X_1K+0.5*k_21),(X_2K+0.5*k_22),(X_3K+0.5*k_23));
    k_33=h*sistemader2(U_K,(X_1K+0.5*k_21),(X_2K+0.5*k_22),(X_3K+0.5*k_23));
        
    k_41=h*sistemader1(U_K,(X_1K+k_31),(X_2K+k_32),(X_3K+k_33));
    k_42=h*sistemader2(U_K,(X_1K+k_31),(X_2K+k_32),(X_3K+k_33));
    k_43=h*sistemader2(U_K,(X_1K+k_31),(X_2K+k_32),(X_3K+k_33));

    X_1K_1=X_1K + (1.0/6)*(k_11+2*k_21+2*k_31+k_41);  
    X_2K_1=X_2K + (1.0/6)*(k_12+2*k_22+2*k_32+k_42);  
    X_3K_1=X_3K + (1.0/6)*(k_13+2*k_23+2*k_33+k_43);

    //Se actualizan el valor de los estados para la siguiente iteración
    
  X_1K=X_1K_1;
  X_2K=X_2K_1;
  X_3K=X_3K_1;
}


//-------------------
// Salida es igual al primer estado mas la perturbación
Y_K=X_1K+Per;

// Calculo del error
E=R_K-Y_K;

// Calculo derivada del error
Edot=(E-E_K_1)/Ts;
// Actualización de variables para el cálculo de la derivada del error
E_K_1=E;

// Calculo de la salida del controlador difuso. U_K es la acción de control
fuzzy_init(&fuzzy_system);

U_Fuzzy = fuzzy_control(E,0.5*Edot,&fuzzy_system);
U_K=1.0*U_Fuzzy ;

fuzzy_free(&fuzzy_system);
     

//Se envia vía serial la salida del sistema y la entrada
Serial.print(" Salida (x10): "); 
Serial.print( 10*Y_K);    
Serial.print(" Voltaje: "); 
Serial.print( 1.0*U_K);     
Serial.print(" Referencia: "); 
Serial.println( R_K);
//Retardo o tiempo en el que el ciclo se vuelve a repetir
delay(50);
}
// Funciones que representan las ecuaciones diferenciales del sistema
float sistemader1(float u,float x1,float x2,float x3){
float derX1;
derX1=x2;
return derX1;
}

float sistemader2(float u,float x1,float x2,float x3){
float derX2;
derX2=((9.8*sin(x1)+cos(x1))*(2.0/3)*(-x3-0.25*x2*x2*sin(x1)))/(0.5*((4.0/3)-(1.0/3)*cos(x1)*cos(x1)));
return derX2;
}

float sistemader3(float u,float x1,float x2,float x3){
float derX3;
derX3=-100*x3+100*u;
return derX3;
}

     

             
  
    
