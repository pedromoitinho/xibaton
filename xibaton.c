// ================= MOTORES (L298N) =================
const int motorEsqA = 11;
const int motorEsqB = 12;
const int motorDirA = 2;
const int motorDirB = 3;

// ================= ULTRASSÔNICOS (HC-SR04) =================
const int trigEsq    = 2;  const int echoEsq    = 3;
const int trigFrente = 4;  const int echoFrente = 5;
const int trigDir    = 6;  const int echoDir    = 7;

// ================= IR HW-511 (AO) =================
const int irCentro = A2;
const int irEsq    = A3;
const int irDir    = A4;

const int limiteLinha = 500;                 // ajuste com base na sua arena
const unsigned long echoTimeout = 30000UL;   // evita travar no pulseIn

void setup() {
  pinMode(motorEsqA, OUTPUT); pinMode(motorEsqB, OUTPUT);
  pinMode(motorDirA, OUTPUT); pinMode(motorDirB, OUTPUT);

  pinMode(trigEsq, OUTPUT);    pinMode(echoEsq, INPUT);
  pinMode(trigFrente, OUTPUT); pinMode(echoFrente, INPUT);
  pinMode(trigDir, OUTPUT);    pinMode(echoDir, INPUT);

  pinMode(irCentro, INPUT);
  pinMode(irEsq, INPUT);
  pinMode(irDir, INPUT);

  Serial.begin(9600);
}

void loop() {
  int vC = analogRead(irCentro);
  int vL = analogRead(irEsq);
  int vR = analogRead(irDir);

  bool bordaCentro = (vC < limiteLinha);
  bool bordaEsq    = (vL < limiteLinha);
  bool bordaDir    = (vR < limiteLinha);

  // 1) BORDA (prioridade máxima)
  if (bordaCentro || bordaEsq || bordaDir) {
    recuarEDesviar(bordaEsq, bordaDir, bordaCentro);
  } else {
    // 2) PROCURA/ATAQUE (3 ultrassônicos)
    long dF = lerDistancia(trigFrente, echoFrente);
    long dLft = lerDistancia(trigEsq, echoEsq);
    long dRgt = lerDistancia(trigDir, echoDir);

    if (dF != -1 && dF < 50) {
      frente();
    } else if (dLft != -1 && dLft < 50) {
      girarEsquerda();
    } else if (dRgt != -1 && dRgt < 50) {
      girarDireita();
    } else {
      girarProcurando();
    }
  }

  delay(20);
}

// ================= LÓGICA DE BORDA =================
void recuarEDesviar(bool bordaEsq, bool bordaDir, bool bordaCentro) {
  tras();
  delay(700);
  parar();
  delay(50);

  // Se pegou borda do lado esquerdo, vira para a direita (e vice-versa)
  if (bordaEsq && !bordaDir) {
    girarDireita();
    delay(500);
  } else if (bordaDir && !bordaEsq) {
    girarEsquerda();
    delay(500);
  } else {
    // centro ou ambos: escolha um lado padrão
    girarDireita();
    delay(550);
  }

  parar();
  delay(50);
}

// ================= ULTRASSÔNICO =================
long lerDistancia(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  unsigned long dur = pulseIn(echo, HIGH, echoTimeout);
  if (dur == 0) return -1;

  return (long)(dur * 0.034 / 2.0); // cm
}

// ================= MOVIMENTOS (ON/OFF) =================
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

void girarEsquerda() {
  digitalWrite(motorEsqA, LOW);  digitalWrite(motorEsqB, HIGH);
  digitalWrite(motorDirA, HIGH); digitalWrite(motorDirB, LOW);
}

void girarProcurando() {
  // giro contínuo (busca agressiva)
  girarDireita();
}

void parar() {
  digitalWrite(motorEsqA, LOW); digitalWrite(motorEsqB, LOW);
  digitalWrite(motorDirA, LOW); digitalWrite(motorDirB, LOW);
}
