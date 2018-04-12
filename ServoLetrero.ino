/*Control de 6 pantallas 7 segmentos controladas con  42 servomotores sg90 y 6 driver PCA9685.
  Autor: Javier Vargas. El Hormiguero 12/04/2018.
  https://creativecommons.org/licenses/by/4.0/
*/

//#define Debug_Servos //Ajuste de la señal PWM de los servos
//#define Debug_Offset //Ajuste del offset de los servos
#define Accion //Accion de programa

/*   NUMERACION LOCAL DE LOS SERVOMOTORES (DE CADA PANEL)        
             __0
         5/ __6 /1
        4/ __3 /2
*/

boolean Inv[] = {0, 0, 0, 1, 1, 1, 0}; //Servomotores que tienen el ángulo inveritdo (paneles 0, 1, 2)
boolean Inv2[] = {0, 1, 0, 1, 0, 1, 0}; //Servomotores que tienen el ángulo inveritdo (paneles 3, 4, 5)

//Servo angulo
#define Amplitud 60
#define Angulo 90
#define SERVOMIN  100 // this is the 'minimum' pulse length count (out of 4096) //120
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096) //570
int Offset[42]; //Vector de offset de los servos (modificar en setup)
int OffsetAmp[42]; //Vector de offset de amplitud (modificar en setup)


#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm0 = Adafruit_PWMServoDriver(0x40); //Sin jumper
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x41); //Jumper A0
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x42); //Jumper A1
Adafruit_PWMServoDriver pwm3 = Adafruit_PWMServoDriver(0x43); //Jumper A1 y A2
Adafruit_PWMServoDriver pwm4 = Adafruit_PWMServoDriver(0x44); //Jumper A3
Adafruit_PWMServoDriver pwm5 = Adafruit_PWMServoDriver(0x45); //Jumper A3 y A0

boolean sVect[7];
int estado[42];

void setup() {
  Serial.begin(115200);

  //Offset panel 0
  Offset[0] = -71;
  OffsetAmp[0] = -5;
  Offset[1] = -56;
  OffsetAmp[1] = 3;
  Offset[2] = -60;
  OffsetAmp[2] = 40;
  Offset[3] = -43;
  OffsetAmp[3] = 37;
  Offset[4] = -35;
  OffsetAmp[4] = 30;
  Offset[5] = -22;
  OffsetAmp[5] = 22;
  Offset[6] = -63;
  OffsetAmp[6] = 30;

  //Offset panel 1
  Offset[7] = -80; //0
  OffsetAmp[7] = 20;
  Offset[8] = -40; //1
  OffsetAmp[8] = 15;
  Offset[9] = -58; //2
  OffsetAmp[9] = 25;
  Offset[10] = -18; //3
  OffsetAmp[10] = 5;
  Offset[11] = -39; //4
  OffsetAmp[11] = 0;
  Offset[12] = 2; //5
  OffsetAmp[12] = 10;
  Offset[13] = -68; //6
  OffsetAmp[13] = 20;

  //Offset panel 2
  Offset[14] = -58; //0
  OffsetAmp[14] = 5;
  Offset[15] = -50; //1
  OffsetAmp[15] = 18;
  Offset[16] = -72; //2
  OffsetAmp[16] = 5;
  Offset[17] = -5; //3
  OffsetAmp[17] = 26;
  Offset[18] = -45; //4
  OffsetAmp[18] = 15;
  Offset[19] = -50; //5
  OffsetAmp[19] = 24;
  Offset[20] = -45; //6
  OffsetAmp[20] = 25;

  //Offset panel 3
  Offset[21] = -50; //0
  OffsetAmp[21] = 18;
  Offset[22] = -55; //1
  OffsetAmp[22] = 28;
  Offset[23] = -54; //2
  OffsetAmp[23] = 25;
  Offset[24] = -90; //3
  OffsetAmp[24] = 33;
  Offset[25] = -50; //4
  OffsetAmp[25] = 20;
  Offset[26] = -50; //5
  OffsetAmp[26] = 24;
  Offset[27] = -55; //6
  OffsetAmp[27] = 25;

  //Offset panel 4
  Offset[28] = -50; //0
  OffsetAmp[28] = 20; //0
  Offset[29] = -25; //1
  OffsetAmp[29] = 20; //1
  Offset[30] = -33; //2
  OffsetAmp[30] = 25; //2
  Offset[31] = -45; //3
  OffsetAmp[31] = 20; //3
  Offset[32] = -70; //4
  OffsetAmp[32] = 20; //4
  Offset[33] = -40; //5
  OffsetAmp[33] = 20; //5
  Offset[34] = -70; //6
  OffsetAmp[34] = 35; //6

  //Offset panel 5
  Offset[35] = -60; //0
  OffsetAmp[35] = 10; //0
  Offset[36] = 21; //1
  OffsetAmp[36] = -5; //1
  Offset[37] = -88; //2
  OffsetAmp[37] = -5; //2
  Offset[38] = -15; //3
  OffsetAmp[38] = 25; //3
  Offset[39] = -80; //4
  OffsetAmp[39] = 0; //4
  Offset[40] = -5; //5
  OffsetAmp[40] = 15; //5
  Offset[41] = -55; //6
  OffsetAmp[41] = 5; //6

  pwm0.begin();
  pwm1.begin();
  pwm2.begin();
  pwm3.begin();
  pwm4.begin();
  pwm5.begin();
  pwm0.setPWMFreq(60);
  pwm1.setPWMFreq(60);
  pwm2.setPWMFreq(60);
  pwm3.setPWMFreq(60);
  pwm4.setPWMFreq(60);
  pwm5.setPWMFreq(60);
  delay(10);
  //Inicio de todo apagado
  Clear(0);
  delay(2000);
}
///////////////////

void loop() {
#ifdef Debug_Offset
  Clear(0);
  Serial.println("null");
  delay(5000);
  All(0);
  Serial.println("full");
  delay(5000);
#endif

#ifdef Debug_Servos
  for (int i = SERVOMIN; i <= SERVOMAX; i += 10) {
    pwm0.setPWM(0, 0, i);
    Serial.println(i);
    delay(200);
  }
#endif

#ifdef Accion
  Efecto1(200);
  Efecto1(150);
  Efecto1(100);
  Efecto1(80);
  Efecto1(80);
  Efecto2(100);
  Efecto3(100);
  Imprimir(0, "5", 0);
  delay(500);
  Efecto3(100);
  Imprimir(0, "4", 0);
  delay(500);
  Efecto3(100);
  Imprimir(0, "3", 0);
  delay(500);
  Efecto3(100);
  Imprimir(0, "2", 0);
  delay(500);
  Efecto3(100);
  Imprimir(0, "1", 0);
  delay(500);
  Efecto3(100);
  Imprimir(0, "0", 0);
  delay(1000);
  All(100);
  delay(500);
  Imprimir(5, "j", 100);
  Imprimir(4, "o", 100);
  Imprimir(3, "r", 100);
  Imprimir(2, "g", 100);
  Imprimir(1, "e", 100);
  Imprimir(0, "null", 100);
  delay(3000);
  Clear(50);
  Barrido("lorenzo", 1000);
  delay(1000000);
#endif
}

//////////////////

//Efecto 1: Rodea los 6 letreros con los segmentos de alrededor
void Efecto1(int d) {
  Imprimir(0, "s0", 0);
  delay(d);
  Imprimir(0, "null", 0);
  Imprimir(1, "s0", 0);
  delay(d);
  Imprimir(1, "null", 0);
  Imprimir(2, "s0", 0);
  delay(d);
  Imprimir(2, "null", 0);
  Imprimir(3, "s0", 0);
  delay(d);
  Imprimir(3, "null", 0);
  Imprimir(4, "s0", 0);
  delay(d);
  Imprimir(4, "null", 0);
  Imprimir(5, "s0", 0);
  delay(d);
  Imprimir(5, "s5", 0);
  delay(d);
  Imprimir(5, "s4", 0);
  delay(d);
  Imprimir(5, "s3", 0);
  delay(d);
  Imprimir(5, "null", 0);
  Imprimir(4, "s3", 0);
  delay(d);
  Imprimir(4, "null", 0);
  Imprimir(3, "s3", 0);
  delay(d);
  Imprimir(3, "null", 0);
  Imprimir(2, "s3", 0);
  delay(d);
  Imprimir(2, "null", 0);
  Imprimir(1, "s3", 0);
  delay(d);
  Imprimir(1, "null", 0);
  Imprimir(0, "s3", 0);
  delay(d);
  Imprimir(0, "s2", 0);
  delay(d);
  Imprimir(0, "s1", 0);
  delay(d);
  Imprimir(0, "null", 0);
}

//Efecto 2: Zigzag desde el panel 0 al panel 5
void Efecto2(int d) {
  Imprimir(0, "s0", 0);
  delay(d);
  Imprimir(0, "s5", 0);
  delay(d);
  Imprimir(0, "s4", 0);
  delay(d);
  Imprimir(0, "null", 0);
  Imprimir(1, "s3", 0);
  delay(d);
  Imprimir(1, "s4", 0);
  delay(d);
  Imprimir(1, "s5", 0);
  delay(d);
  Imprimir(1, "null", 0);
  Imprimir(2, "s0", 0);
  delay(d);
  Imprimir(2, "s5", 0);
  delay(d);
  Imprimir(2, "s4", 0);
  delay(d);
  Imprimir(2, "null", 0);
  Imprimir(3, "s3", 0);
  delay(d);
  Imprimir(3, "s4", 0);
  delay(d);
  Imprimir(3, "s5", 0);
  delay(d);
  Imprimir(3, "null", 0);
  Imprimir(4, "s0", 0);
  delay(d);
  Imprimir(4, "s5", 0);
  delay(d);
  Imprimir(4, "s4", 0);
  delay(d);
  Imprimir(4, "null", 0);
  Imprimir(5, "s3", 0);
  delay(d);
  Imprimir(5, "s4", 0);
  delay(d);
  Imprimir(5, "null", 0);
}

//Efecto 3: Linea desde el letrero 5 al 1
void Efecto3(int d) {
  Imprimir(5, "s6", 0);
  delay(d);
  Imprimir(5, "null", 0);
  Imprimir(4, "s6", 0);
  delay(d);
  Imprimir(4, "null", 0);
  Imprimir(3, "s6", 0);
  delay(d);
  Imprimir(3, "null", 0);
  Imprimir(2, "s6", 0);
  delay(d);
  Imprimir(2, "null", 0);
  Imprimir(1, "s6", 0);
  delay(d);
  Imprimir(1, "null", 0);
}

void Barrido(String t, int d) { //Pasa un texto por los paneles que se desplaza de izquierda a derecha
  Clear(0);
  int longitud = t.length();
  //Recorrideo a lo largo de los paneles
  for (int panel = 0; panel < 7 + longitud; panel++) {
    boolean disp[] = {0, 0, 0, 0, 0, 0};
    //Recorrido a lo largo de las letras
    for (int letra = 0; letra < longitud; letra++) {
      String txt = String(t[letra]);
      int pos = -letra + panel;
      if (pos >= 0 && pos < 6) {
        Imprimir(pos, txt, 0);
        disp[pos] = 1;
      }
    }
    //Borrado de los paneles no utilizados;
    for (int i = 0; i < 6; i++) {
      if (disp[i] == 0) {
        Imprimir(i, "null", 0);
      }
    }
    delay(d);
  }
  Clear(0);
}

void Clear(int d) { //Todos OFF
  Imprimir(0, "null", d);
  Imprimir(1, "null", d);
  Imprimir(2, "null", d);
  Imprimir(3, "null", d);
  Imprimir(4, "null", d);
  Imprimir(5, "null", d);
}

void All(int d) { //Todos ON
  Imprimir(0, "full", d);
  Imprimir(1, "full", d);
  Imprimir(2, "full", d);
  Imprimir(3, "full", d);
  Imprimir(4, "full", d);
  Imprimir(5, "full", d);
}

void Imprimir(byte n, String c, int d) {

  if (c == "s0") vector(1, 0, 0, 0, 0, 0, 0);
  if (c == "s1") vector(0, 1, 0, 0, 0, 0, 0);
  if (c == "s2") vector(0, 0, 1, 0, 0, 0, 0);
  if (c == "s3") vector(0, 0, 0, 1, 0, 0, 0);
  if (c == "s4") vector(0, 0, 0, 0, 1, 0, 0);
  if (c == "s5") vector(0, 0, 0, 0, 0, 1, 0);
  if (c == "s6") vector(0, 0, 0, 0, 0, 0, 1);
  if (c == "null") vector(0, 0, 0, 0, 0, 0, 0);
  if (c == "full") vector(1, 1, 1, 1, 1, 1, 1);

  if (c == "0") vector(1, 1, 1, 1, 1, 1, 0);
  if (c == "1") vector(0, 1, 1, 0, 0, 0, 0);
  if (c == "2") vector(1, 1, 0, 1, 1, 0, 1);
  if (c == "3") vector(1, 1, 1, 1, 0, 0, 1);
  if (c == "4") vector(0, 1, 1, 0, 0, 1, 1);
  if (c == "5") vector(1, 0, 1, 1, 0, 1, 1);

  if (c == "j") vector(0, 1, 1, 1, 0, 0, 0);
  if (c == "o") vector(1, 1, 1, 1, 1, 1, 0);
  if (c == "r") vector(1, 0, 0, 0, 1, 1, 0);
  if (c == "g") vector(1, 0, 1, 1, 1, 1, 1);
  if (c == "e") vector(1, 0, 0, 1, 1, 1, 1);
  if (c == "l") vector(0, 0, 0, 1, 1, 1, 0);
  if (c == "n") vector(1, 1, 1, 0, 1, 1, 0);
  if (c == "z") vector(1, 1, 0, 1, 1, 0, 1);
  if (c == "c") vector(1, 0, 0, 1, 1, 1, 0);
  if (c == "a") vector(1, 1, 1, 0, 1, 1, 1);
  if (c == "s") vector(1, 0, 1, 1, 0, 1, 1);
  if (c == "i") vector(0, 1, 1, 0, 0, 0, 0);

  ImprimirServos(n, sVect, d);
}

//Coloca los servos on / off (vector s) del 7 segmentos número n, esperando un cierto tiempo d en ms entre cada movimiento
void ImprimirServos(byte n, boolean s[7], int d) {
  for (int i = 0; i < 7; i++) {
    int servo = n * 7 + i; //Numero de servo que corresponde numerados del 0 al 41
    if (s[i]) { //SERVO ACTIVADO
      if (servo < 21) { //Panel 0,1,2
        if (!Inv[i]) SetServo(servo, Angulo + (Amplitud + OffsetAmp[servo]) + Offset[servo]); //Servo activado no invertido
        else SetServo(servo, Angulo - (Amplitud + OffsetAmp[servo]) - Offset[servo]); //Servo activado invertido
      }
      else { //Panel 3,4,5
        if (!Inv2[i]) SetServo(servo, Angulo + (Amplitud + OffsetAmp[servo]) + Offset[servo]); //Servo activado no invertido
        else SetServo(servo, Angulo - (Amplitud + OffsetAmp[servo]) - Offset[servo]); //Servo activado invertido
      }

    }
    else { //SERVO DESACTIVADO
      if (servo < 21) { //Panel 0,1,2
        if (!Inv[i]) SetServo(servo, Angulo + Offset[servo]); //Servo desactivado no invertido
        else SetServo(servo, Angulo - Offset[servo]); //Servo desactivado invertido
      }
      else { //Panel 3,4,5
        if (!Inv2[i]) SetServo(servo, Angulo + Offset[servo]); //Servo desactivado no invertido
        else SetServo(servo, Angulo - Offset[servo]); //Servo desactivado invertido
      }
    }
    //Espera si el servo ha cambiado de posicion
    if (estado[servo] != s[i]) {
      estado[servo] = s[i];
      delay(d);
    }
  }
}

//Servo 0 - 180º repartidos en 3 drivers PCA9685
void SetServo(byte servo, int angulo) {
  //  angulo = constrain(angulo, 0, 180);
  if (servo < 7) pwm0.setPWM(servo, 0, map(angulo, 0, 180, SERVOMIN, SERVOMAX));
  if (servo >= 7 && servo < 14) pwm1.setPWM(servo - 7, 0, map(angulo, 0, 180, SERVOMIN, SERVOMAX));
  if (servo >= 14 && servo < 21) pwm2.setPWM(servo - 14, 0, map(angulo, 0, 180, SERVOMIN, SERVOMAX));
  if (servo >= 21 && servo < 28) pwm3.setPWM(servo - 21, 0, map(angulo, 0, 180, SERVOMIN, SERVOMAX));
  if (servo >= 28 && servo < 35) pwm4.setPWM(servo - 28, 0, map(angulo, 0, 180, SERVOMIN, SERVOMAX));
  if (servo >= 35 && servo < 42) pwm5.setPWM(servo - 35, 0, map(angulo, 0, 180, SERVOMIN, SERVOMAX));
}

//Asigna valores al vector sVect de 7 posiciones
void vector(boolean p0, boolean p1, boolean p2, boolean p3, boolean p4, boolean p5, boolean p6) {
  sVect[0] = p0;
  sVect[1] = p1;
  sVect[2] = p2;
  sVect[3] = p3;
  sVect[4] = p4;
  sVect[5] = p5;
  sVect[6] = p6;
}



