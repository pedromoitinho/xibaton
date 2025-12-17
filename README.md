# 🤖 Projeto Robô Sumô Autônomo

Este repositório contém o código-fonte e a documentação técnica para um robô de combate sumô baseado em Arduino. O projeto foi desenhado para detectar a borda da arena (Dohyo) e atacar oponentes automaticamente.

## 📋 Funcionalidades
- **Detecção de Oponente:** Usa sensor ultrassônico para medir distância.
- **Evasão de Borda:** 3 sensores infravermelhos monitoram a linha branca.
- **Estratégia de Busca:** Gira automaticamente para localizar alvos.

---

# 🔧 Configuração de Hardware
Pinagem Resumida
Motores: Pinos 11, 12 (Esq) e 2, 3 (Dir).

Ultrassônico: Trig 4, Echo 5.

Infravermelhos: A2 (Centro), A3 (Esq), A4 (Dir).

Alimentação Recomendada
Arduino: Bateria 9V ou Power Bank.

Motores (Ponte H L293D): Bateria externa de 7.4V a 9V (compartilhando o GND com o Arduino).
