// Variables asociadas a las los cinco LEDs que se van a controlar
int led_1 = 4;
int led_2 = 5;
int led_3 = 6;
int led_4 = 7;
int led_5 = 8;

char valor; // Variable para indicar que llega una orden
void setup() {
  // put your setup code here, to run once:
  pinMode(led_1,OUTPUT);
  pinMode(led_2,OUTPUT);
  pinMode(led_3,OUTPUT);
  pinMode(led_4,OUTPUT);
  pinMode(led_5,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) // Si el puerto serie Bluetooth esta disponible
  {
    valor = Serial.read(); // Lee el dato entrante en la via Bluetooth
    Serial.print("Número: ");
    Serial.println(valor);

    if (valor == 'A') // Si el dato que llega es una A
    {
      digitalWrite(led_1,HIGH); //Enciende el LED 1
      digitalWrite(led_2,LOW); //Enciende el LED 2
      digitalWrite(led_3,LOW); //Enciende el LED 3
      digitalWrite(led_4,LOW); //Enciende el LED 4
      digitalWrite(led_5,LOW); //Enciende el LED 5
    }
    if (valor == 'B') // Si el dato que llega es una B
    {
      digitalWrite(led_1,HIGH); //Enciende el LED 1
      digitalWrite(led_2,HIGH); //Enciende el LED 2
      digitalWrite(led_3,LOW); //Enciende el LED 3
      digitalWrite(led_4,LOW); //Enciende el LED 4
      digitalWrite(led_5,LOW); //Enciende el LED 5
    }
    if (valor == 'C') // Si el dato que llega es una C
    {
      digitalWrite(led_1,HIGH); //Enciende el LED 1
      digitalWrite(led_2,HIGH); //Enciende el LED 2
      digitalWrite(led_3,HIGH); //Enciende el LED 3
      digitalWrite(led_4,LOW); //Enciende el LED 4
      digitalWrite(led_5,LOW); //Enciende el LED 5
    }
    if (valor == 'D') // Si el dato que llega es una D
    {
      digitalWrite(led_1,HIGH); //Enciende el LED 1
      digitalWrite(led_2,HIGH); //Enciende el LED 2
      digitalWrite(led_3,HIGH); //Enciende el LED 3
      digitalWrite(led_4,HIGH); //Enciende el LED 4
      digitalWrite(led_5,LOW); //Enciende el LED 5
    }
    if (valor == 'E') // Si el dato que llega es una E
    {
      digitalWrite(led_1,HIGH); //Enciende el LED 1
      digitalWrite(led_2,HIGH); //Enciende el LED 2
      digitalWrite(led_3,HIGH); //Enciende el LED 3
      digitalWrite(led_4,HIGH); //Enciende el LED 4
      digitalWrite(led_5,HIGH); //Enciende el LED 5
    }
    if (valor == 'F') // Si el dato que llega es una E
    {
      digitalWrite(led_1,LOW); //Enciende el LED 1
      digitalWrite(led_2,LOW); //Enciende el LED 2
      digitalWrite(led_3,LOW); //Enciende el LED 3
      digitalWrite(led_4,LOW); //Enciende el LED 4
      digitalWrite(led_5,LOW); //Enciende el LED 5
    }
  }

}
