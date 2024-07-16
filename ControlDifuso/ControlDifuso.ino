// Librería con las funciones para el sistema difuso
#include "sfun_fuzzyutilsug.h"
//Se definen las variables que se usarán en el programa
// Contador de muestras
float Samples=0;
// Salida de la planta Y(K)
float Y_K;
// Salida de la planta Y(K-1)
float Y_K_1=0;
// Entrada de la planta U(K)
float U_K;
// Entrada de la planta U(K-1)
float U_K_1=0;

float randNumber;
// Referencia
float R_K;
// Error y Derivada del Error
float E, Ydot;
// Tiempo Muestreo
float Ts=1.5;

// Salida del sistema difuso
float U_Fuzzy_1=0;
float U_Fuzzy=0;

void setup() {
  // Se inicializa la comunicación serial
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void loop() {
 // Se crea la variable tipo fuszzy_system
  FUZ_SYS fuzzy_system; 
  //Ciclo principal donde se calcula la actualización de la salida del sistema usando la ecuación en diferencia

// Contador de muestras para saber cuando se va a cambiar la entrada al sistema
  Samples=Samples+1;

// Si el contador de muestras es 1 (uno) se modifica la entrada al sistema generando un número randómico
if (Samples==1)
{
  //Se genera un número randómico entre 0 y 50
  randNumber = random(50);
  Samples=Samples+1;
}

// Si el contador de muestras supera un valor se resetea el contador para que se vuelva a generar una nueva entrada
if (Samples>150)
{
  Samples=0;
}

// Se escala el numero randómico para que quede en el rango entre 0 y 5 y se usa como refeencia
R_K=(randNumber/10);

// Ecuación en diferencia para calcular la salida del sistema
Y_K=0.9048*Y_K_1+0.04758*U_K_1;//leerlo de matlab via serial
E=R_K-Y_K;
Ydot=(Y_K-Y_K_1)/Ts;

fuzzy_init(&fuzzy_system);

U_Fuzzy = fuzzy_control(E,0.1*Ydot,&fuzzy_system); //este comando es similar al evalfis de matlab
U_K=U_K_1+0.5*(U_Fuzzy+U_Fuzzy_1)*Ts;
U_K=1.0*U_K;//enviar a matlab via serial.

fuzzy_free(&fuzzy_system);
     
//Se actualizan el valor de las variables para la siguiente iteración
  Y_K_1=Y_K;
  U_K_1=U_K;
  U_Fuzzy_1= U_Fuzzy;

//Se manda vía serial la salida del sistema y la entrada
//Serial.print(R_K);
//Serial.print(",");
Serial.println(U_K);
//Retardo o tiempo en el que el ciclo se vuelve a repetir
delay(50);
}
