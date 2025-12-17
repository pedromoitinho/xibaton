// --- PINOS DOS MOTORES ---
const int motorEsqA = 11; 
const int motorEsqB = 12;
const int motorDirA = 2;
const int motorDirB = 3;

// --- PINOS DOS SENSORES ULTRASSÔNICOS (OPONENTE) ---
const int trigFrente = 4; const int echoFrente = 5;

// --- PINOS DOS SENSORES INFRAVERMELHOS (BORDA DA ARENA) ---
const int irCentro = A2;
const int irEsq = A3;
const int irDir = A4;

// Ajuste esse valor conforme a cor da sua arena (0 a 1023)
// No Tinkercad, a linha branca costuma dar valores baixos.
const int limiteLinha = 500; 

void setup() {
  pinMode(motorEsqA, OUTPUT); pinMode(motorEsqB, OUTPUT);
  pinMode(motorDirA, OUTPUT); pinMode(motorDirB, OUTPUT);
  
  pinMode(trigFrente, OUTPUT); pinMode(echoFrente, INPUT);
  
  // Sensores IR são de entrada
  pinMode(irCentro, INPUT);
  pinMode(irEsq, INPUT);
  pinMode(irDir, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  // 1. VERIFICA SE ESTÁ NA BORDA (PRIORIDADE MÁXIMA)
  if (analogRead(irCentro) < limiteLinha || analogRead(irEsq) < limiteLinha || analogRead(irDir) < limiteLinha) {
    recuarEDesviar();
  } 
  // 2. SE NÃO ESTIVER CAINDO, PROCURA O OPONENTE
  else {
    long dist = lerDistancia(trigFrente, echoFrente);
    
    if (dist < 50) { // Oponente detectado a menos de 50cm
      frente(); // ATACAR!
    } else {
      girarProcurando(); // Fica girando para achar o alvo
    }
  }
  delay(20);
}

// --- FUNÇÕES DE LÓGICA ---

void recuarEDesviar() {
  tras();
  delay(800);
  girarDireita();
  delay(500);
}

long lerDistancia(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  return pulseIn(echo, HIGH) * 0.034 / 2;
}

// --- MOVIMENTOS BÁSICOS ---

void frente() {
  digitalWrite(motorEsqA, HIGH); digitalWrite(motorEsqB, LOW);
  digitalWrite(motorDirA, HIGH); digitalWrite(motorDirB, LOW);
}

void tras() {
  digitalWrite(motorEsqA, LOW); digitalWrite(motorEsqB, HIGH);
  digitalWrite(motorDirA, LOW); digitalWrite(motorDirB, HIGH);
}

void girarDireita() {
  digitalWrite(motorEsqA, HIGH); digitalWrite(motorEsqB, LOW);
  digitalWrite(motorDirA, LOW);  digitalWrite(motorDirB, HIGH);
}

void girarProcurando() {
  // Gira devagar para encontrar o oponente
  digitalWrite(motorEsqA, HIGH); digitalWrite(motorEsqB, LOW);
  digitalWrite(motorDirA, LOW);  digitalWrite(motorDirB, LOW);
}

void parar() {
  digitalWrite(motorEsqA, LOW); digitalWrite(motorEsqB, LOW);
  digitalWrite(motorDirA, LOW); digitalWrite(motorDirB, LOW);
}