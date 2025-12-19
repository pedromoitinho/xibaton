// ================== MOTORES (L298N) ==================
#define IN1 9
#define IN2 10
#define IN3 11
#define IN4 12

// ===== VELOCIDADES (REDUZIDAS E ESTÁVEIS) =====
int velAtaque = 150;
int velBusca  = 120;
int velRe     = 130;
int velGiro   = 110;

// ================= ULTRASSÔNICO =====================
#define TRIG 4
#define ECHO 5

// ================= SENSORES DE LINHA =================
#define LINHA_F A2   // Frontal
#define LINHA_E A3   // Esquerdo
#define LINHA_D A4   // Direito

// ================= SETUP =============================
void setup() {

  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(LINHA_F, INPUT);
  pinMode(LINHA_E, INPUT);
  pinMode(LINHA_D, INPUT);

  Serial.begin(9600);
}

// ================= MOVIMENTOS ========================
void frente(int v) {
  analogWrite(IN1, 0);
  analogWrite(IN2, v);
  analogWrite(IN3, 0);
  analogWrite(IN4, v);
}

void tras(int v) {
  analogWrite(IN1, v);
  analogWrite(IN2, 0);
  analogWrite(IN3, v);
  analogWrite(IN4, 0);
}

void direita(int v) {
  analogWrite(IN1, 0);
  analogWrite(IN2, v);
  analogWrite(IN3, v);
  analogWrite(IN4, 0);
}

void esquerda(int v) {
  analogWrite(IN1, v);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, v);
}

void parar() {
  analogWrite(IN1, 0); analogWrite(IN2, 0);
  analogWrite(IN3, 0); analogWrite(IN4, 0);
}

// ================= DISTÂNCIA =========================
long distanciaCM() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long tempo = pulseIn(ECHO, HIGH, 25000);
  if (tempo == 0) return 0;
  return tempo / 58;
}

// ================= LOOP PRINCIPAL ====================
void loop() {

  int f = digitalRead(LINHA_F);
  int e = digitalRead(LINHA_E);
  int d = digitalRead(LINHA_D);
  long dist = distanciaCM();

  // -------- PRIORIDADE 1: BORDA --------
  if (f == 0) {
    tras(velRe);
    delay(300);
    direita(velGiro);
    delay(250);
  }
  else if (e == 0) {
    tras(velRe);
    delay(250);
    direita(velGiro);
    delay(200);
  }
  else if (d == 0) {
    tras(velRe);
    delay(250);
    esquerda(velGiro);
    delay(200);
  }

  // -------- PRIORIDADE 2: ATAQUE --------
  else if (dist > 0 && dist <= 25) {
    frente(velAtaque);
  }

  // -------- PRIORIDADE 3: AJUSTE --------
  else if (dist > 25 && dist <= 45) {
    direita(velBusca);
  }

  // -------- PRIORIDADE 4: BUSCA --------
  else {
    direita(velBusca);
    delay(80);   // giro suave
  }
}
