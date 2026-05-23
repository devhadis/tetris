# 🕹️ Tetris Nano (Sistema Smart-Flow)

Este projeto consiste na adaptação e otimização do clássico jogo Tetris para rodar no microcontrolador **Arduino Nano (ATmega328P)** utilizando um **Display LCD 2004 I2C**. O sistema foi projetado sob severas restrições de hardware, utilizando apenas **2KB de memória RAM**, aplicando técnicas de bitmask e renderização fracionária de caracteres.

---

## 📐 Arquitetura do Sistema e Pinagem

Abaixo está o mapeamento completo das conexões entre o Arduino Nano, o Display LCD, os botões de controle e o buzzer.

### Tabela de Conexões (Pin Mapping)

| Componente | Pino do Componente | Pino no Arduino Nano | Função | Tipo de Sinal |
| :--- | :--- | :--- | :--- | :--- |
| **LCD 2004 I2C** | GND | GND | Aterramento do Display | Energia |
| **LCD 2004 I2C** | VCC | 5V | Alimentação +5V | Energia |
| **LCD 2004 I2C** | SDA | **A4** | Barramento de Dados I2C | Digital (Fixo) |
| **LCD 2004 I2C** | SCL | **A5** | Barramento de Clock I2C | Digital (Fixo) |
| **Botão L** | Pino de Sinal | **D2** | Mover Peça para Esquerda | Digital (Input Pullup) |
| **Botão R** | Pino de Sinal | **D3** | Mover Peça para Direita | Digital (Input Pullup) |
| **Botão ROT** | Pino de Sinal | **D4** | Rotacionar Peça / START | Digital (Input Pullup) |
| **Botão DOWN** | Pino de Sinal | **D5** | Queda Rápida (Soft Drop) | Digital (Input Pullup) |
| **Buzzer** | Pino Positivo (+) | **D6** | Efeitos Sonoros do Jogo | Saída PWM (Tone) |

> ⚠️ **Nota Importante:** Todos os botões utilizam a lógica `INPUT_PULLUP` interna do Arduino Nano. Isso significa que o outro terminal de cada um dos botões deve ser conectado diretamente ao pino **GND** comum do circuito.

---

## 🛠️ Como Aplicar e Executar o Projeto

### 1. Clonar no Simulador Wokwi
Se preferir testar o ambiente virtualmente antes de ir para a bancada física:
1. Acesse o [Wokwi](https://wokwi.com/projects/464839653682657281).
2. Selecione a placa **Arduino Nano**.
3. Na aba `diagram.json`, cole o código estruturado de conexões.
4. Na aba principal do código (`.ino`), cole o algoritmo otimizado do Tetris.

### 2. Montagem Física (Bancada)
1. Encaixe o **Arduino Nano** centralizado na sua protoboard (Breadboard).
2. Conecte as linhas laterais da protoboard ao pino **5V** e **GND** do Nano para criar os barramentos de alimentação.
3. Faça a fiação do **LCD 2004** utilizando quatro jumpers machoxfêmea conectados diretamente aos pinos **A4** e **A5** do Nano.
4. Posicione os 4 botões táteis e faça a ligação de um dos lados no GND comum e o outro lado nos respectivos pinos digitais (**D2, D3, D4, D5**).
5. Instale o Buzzer com o polo positivo no pino **D6** e o negativo no GND.

### 3. Compilação e Upload (Arduino IDE)
1. Abra a **Arduino IDE**.
2. Vá em `Ferramentas` -> `Gerenciador de Bibliotecas` e instale a biblioteca **LiquidCrystal_I2C** (por Frank de Brabander).
3. Conecte o seu Arduino Nano ao computador via cabo Mini-USB.
4. Em `Ferramentas` -> `Placa`, selecione **Arduino Nano**.
5. Se o seu Nano for uma versão chinesa/comum, lembre-se de alterar o processador para **ATmega328P (Old Bootloader)** em `Ferramentas` -> `Processador`.
6. Selecione a porta COM correspondente e clique em **Carregar (Upload)**.

---

## 🧠 Detalhes de Engenharia e Otimização

* **Renderização Fracionária:** Como o display LCD possui apenas 4 linhas horizontais, o código divide cada linha de caractere ao meio utilizando caracteres customizados (`lcd.createChar`). Isso permite criar uma matriz lógica de **8 linhas de altura por 10 colunas de largura**, simulando um display gráfico dentro de uma tela de texto comum.
* **Armazenamento em Bitmask:** As formas geométricas das peças (Tetraminós) são salvas em formato hexadecimal de 16 bits (`uint16_t`), ocupando pouquíssimo espaço na memória flash e poupando a memória RAM dinâmica.

---
Projetado por **Pedro Henrique** | Computação Engenharia - Estácio
