#include <Servo.h>

Servo servo;
int servoPin = 8;
int trigger = 10;
int echo = 9;
float distanciaCM = 0;

int pos = 0;
bool portaAberta = false;
unsigned long tempoAberto = 0;
const unsigned long intervaloAberta = 3000; // 3 segundos

void setup() {
  Serial.begin(9600);
  servo.attach(servoPin);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  servo.write(0); // Inicializa a posição do servo
}

void loop() {
  // Enviar um pulso para o sensor ultrassônico
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);

  // Ler o tempo do pulso de volta
  long duration = pulseIn(echo, HIGH);

  // Converter o tempo para distância em centímetros
  distanciaCM = duration / 29.0 / 2.0;

  // Mostrar a distância no Serial Monitor
  Serial.print("Distância: ");
  Serial.print(distanciaCM);
  Serial.println(" cm");

  if (distanciaCM < 50) {
    if (!portaAberta) {
      // Abrir a porta se não estiver já aberta
      abrirPorta();
      portaAberta = true;
      tempoAberto = millis(); // Marca o tempo atual
    }
  } else {
    if (portaAberta && millis() - tempoAberto >= intervaloAberta) {
      // Fechar a porta após 3 segundos se estiver aberta
      fecharPorta();
      portaAberta = false;
    }
  }

  // Pequeno atraso para estabilização
  delay(100);
}

void abrirPorta() {
  for (pos = 0; pos <= 90; pos++) {
    servo.write(pos);
    delay(15); // Ajustar a velocidade de movimento do servo
  }
}

void fecharPorta() {
  for (pos = 90; pos >= 0; pos--) {
    servo.write(pos);
    delay(15); // Ajustar a velocidade de movimento do servo
  }
}