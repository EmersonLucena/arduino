/* 
*  Universidade Federal de Campina Grande
*  Introdução à Computação 2016.1
*  Grupo:
*    Emerson Leonardo Lucena
*  
*  RadArduino
*  Descrição: Um alvo é posicionado aleatoriamente no plano.
*  Seu objetivo é mover-se até ele.
*  Porém a única maneira de se orientar é através de um radar:
*  um LED que pisca com frequência proporcional à proximidade do alvo.
*  Boa caçada!
*/

#include <math.h>

//Mover para: cima - 7; esquerda - 6; baixo - 5; direita - 4
const int Button[] = {7, 6, 5, 4};
const int Led = 8;

//Estado de cada botão: 0 - desligado, 1 - ligado
int estados[] = {0, 0, 0, 0};

//Mudança de posição nas coordenadas X e Y ao se mover para as quatro direçoes
int dX[] = {0, -1, 0, 1};
int dY[] = {1, 0, -1, 0};

int X, Y, Xt, Yt, D;
int LedOn,LedOff,LedState;

//Lê os quatro botões. Retorna o indice do botão pressionado, -1 caso contrário
int ReadButtons() {
  int resp = -1;
  
  for(int i=0; i<4; i++) {
    int C = digitalRead(Button[i]);
    if(C == 1 && estados[i] == 0) {
      estados[i] = 1;
      resp = i;
      break;
    }

    else if(C == 0 && estados[i] == 1) {
      estados[i] = 0;
    }
  }

  return resp;
}

//Define o momento em que o LED será ligado ou desligado
void CheckLeds() {
  int t = millis();
  if(LedState == 0 && t >= LedOn) {
    digitalWrite(Led, HIGH);
    LedState = 1;
    LedOff = t + 100;
  }

  if(LedState == 1 && t >= LedOff) {
    digitalWrite(Led, LOW);
    LedState = 0;
    LedOn = t + 200 * D;
  }
}

//Define a distância atual da desejada para calcular frequência que o LED pisca
int dist(int x1, int x2, int y1, int y2) {
  return abs(x1 - x2) + abs(y1 - y2);
}

void EndGame() {
  Serial.println("Voce ganhou!\n Aperte qualquer botao para jogar novamente");

  digitalWrite(Led, HIGH);
  while(ReadButtons() == -1);
  
  srand(millis());
  Xt = (rand() % 7) - 3;
  Yt = (rand() % 7) - 3;
  X = 0, Y = 0;
  D = dist(X,Xt,Y,Yt);

  digitalWrite(Led, LOW);
}

void setup() {
  Serial.begin(9600);

  pinMode(Led, OUTPUT);
  for(int i=0; i<4; i++)
    pinMode(Button[i], OUTPUT);

  //O programa aguarda o usuário apertar algum botão
  digitalWrite(Led, HIGH);
  while(ReadButtons() == -1);

  //As coordenadas desejadas são escolhidas aleatoriamente no intervalo [-3, 3]
  srand(millis());

  LedState = LedOn = LedOff = 0;
  X = 0, Y = 0;
  Xt = (rand() % 7) - 3;
  Yt = (rand() % 7) - 3;
  D = dist(X,Xt,Y,Yt);
  
  digitalWrite(Led, LOW);
}

void loop() {
  if(D == 0)
    EndGame();

  CheckLeds();
  int b = ReadButtons();
  
  if(b != -1) {
    X += dX[b];
    Y += dY[b];
    D = dist(X,Xt,Y,Yt);
    
    Serial.print(X);
    Serial.print(" ");
    Serial.println(Y);
    Serial.println(D);
  }
}
