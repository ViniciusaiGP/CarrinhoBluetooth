#include <Servo.h>
#include <Keyboard.h>
#include <AFMotor.h>
#include <SoftwareSerial.h>

#define pinSH_CP 4
#define pinST_CP 12
#define pinDS 8
#define pinEnable 7
#define pinMotor1PWM 11
#define pinMotor2PWM 3
#define pinMotor3PWM 5
#define pinMotor4PWM 6
#define pinServo1 10
#define pinServo2 9
#define qtdeCI 1
#define bitMotor1A 2
#define bitMotor1B 3
#define bitMotor2A 1
#define bitMotor2B 4
#define bitMotor3A 5
#define bitMotor3B 7
#define bitMotor4A 0
#define bitMotor4B 6

Servo myservo;

void ci74HC595Write(byte pino, bool estado);
int m = 0;

Servo servo2;
Servo servo1;

SoftwareSerial bluetooht(0, 1); //rx,tx do arduino
String command = "";

void setup() {
  pinMode(pinSH_CP, OUTPUT);
  pinMode(pinST_CP, OUTPUT);
  pinMode(pinEnable, OUTPUT);
  pinMode(pinDS, OUTPUT);
  pinMode(pinMotor2PWM, OUTPUT);
  digitalWrite(pinEnable, LOW);
  servo2.attach(pinServo2);
  servo1.attach(pinServo1);
  Serial.begin(9600);
  servo2.write(5);
  servo1.write(5);

}
//char cha;
void loop() {
  if (Serial.available() > 0) {
    char key;
    key = Serial.read();
    delay(2);

    
    uint8_t i;

    //key = Serial.read();

    Serial.println(key);

    if ((m > 0) || (m < 60)) {
      if (key == 'D') {
        myservo.write(m);
        if (m < 60) {
          m = m + 5;//mudo aq
          servo2.write(m);
          servo1.write(m);
        }
        Serial.print(m);
      }
      if (key == 'A') {
        if (m > 0) {
          m = m - 5;//mudou aq
          servo2.write(m);
          servo1.write(m);
        }
        myservo.write(m);
        Serial.print(m);
      }
    }
    if (key == 'P') { //para o motor
      Serial.println("Motores A=LOW B=LOW");
      ci74HC595Write(bitMotor2A, LOW);
      ci74HC595Write(bitMotor2B, LOW);
      ci74HC595Write(bitMotor3A, LOW);
      ci74HC595Write(bitMotor3B, LOW);
      delayPWM(2000);
    }
    if (key == 'W') {
      Serial.println("Motores A=HIGH B=LOW");
      ci74HC595Write(bitMotor2A, HIGH);
      ci74HC595Write(bitMotor3A, HIGH);
      ci74HC595Write(bitMotor2B, LOW);
      ci74HC595Write(bitMotor3B, LOW);
      delayPWM(key);
    }
    if (key == 'S') { //esquerda
      Serial.println("Motor1 A=LOW B=HIGH");
      ci74HC595Write(bitMotor2A, LOW);
      ci74HC595Write(bitMotor3A, LOW);
      ci74HC595Write(bitMotor2B, HIGH);
      ci74HC595Write(bitMotor3B, HIGH);
      delayPWM(key);
    }
  }
}
void ci74HC595Write(byte pino, bool estado) {
  static byte ciBuffer[qtdeCI];
  bitWrite(ciBuffer[pino / 8], pino % 8, estado);
  digitalWrite(pinST_CP, LOW); //Inicia a Transmissão
  digitalWrite(pinDS, LOW); //Apaga Tudo para Preparar Transmissão
  digitalWrite(pinSH_CP, LOW);
  for (int nC = qtdeCI - 1; nC >= 0; nC--) {
    for (int nB = 7; nB >= 0; nB--) {
      digitalWrite(pinSH_CP, LOW); //Baixa o Clock
      digitalWrite(pinDS, bitRead(ciBuffer[nC], nB) ); //Escreve o BIT
      digitalWrite(pinSH_CP, HIGH); //Eleva o Clock
      digitalWrite(pinDS, LOW); //Baixa o Data para Previnir Vazamento
    }
  }
  digitalWrite(pinST_CP, HIGH); //Finaliza a Transmissão
}
void delayPWM(unsigned long tempo) {
  unsigned long inicio = millis();
  byte valA0;
  while ( (millis() - inicio) < tempo ) {
    valA0 = map(analogRead(A0), 100, 1023, 100, 255);
    analogWrite(pinMotor2PWM, valA0);
  }
}
