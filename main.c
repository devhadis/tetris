#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Endereço I2C padrão 0x27 para LCD 2004
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Mapeamento de Pinos
const byte BTN_L = 2, BTN_R = 3, BTN_ROT = 4, BTN_D = 5;
const byte PIN_BUZZER = 6;

// Lógica do Jogo
const byte BOARD_W = 10, BOARD_H = 8;
byte board[BOARD_H][BOARD_W] = {0};
int pX, pY, pType, pRot;
unsigned long lastTick = 0;
int score = 0;
bool emJogo = false;


const uint16_t SHAPES[7][4] = {
  {0x0F00, 0x2222, 0x0F00, 0x2222}, // I
  {0x4460, 0x0E80, 0xC440, 0x2E00}, // L
  {0x44C0, 0x8E00, 0x6440, 0x0E20}, // J
  {0x0660, 0x0660, 0x0660, 0x0660}, // O
  {0x06C0, 0x8C40, 0x06C0, 0x8C40}, // S
  {0x0E40, 0x4C40, 0x4E00, 0x4640}, // T
  {0x0C60, 0x4C80, 0x0C60, 0x4C80}  // Z
};


byte charTop[8] = {0x1F, 0x15, 0x11, 0x1F, 0, 0, 0, 0};
byte charBot[8] = {0, 0, 0, 0, 0x1F, 0x15, 0x11, 0x1F};
byte charBoth[8] = {0x1F, 0x15, 0x11, 0x1F, 0x1F, 0x15, 0x11, 0x1F};

void setup() {
  Wire.begin();
  lcd.init();
  lcd.backlight();
  pinMode(BTN_L, INPUT_PULLUP); pinMode(BTN_R, INPUT_PULLUP);
  pinMode(BTN_ROT, INPUT_PULLUP); pinMode(BTN_D, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
  lcd.createChar(1, charTop); lcd.createChar(2, charBot); lcd.createChar(3, charBoth);
  randomSeed(analogRead(0));
  menu();
}

void menu() {
  lcd.clear();
  lcd.setCursor(4, 1); lcd.print("TETRIS NANO");
  lcd.setCursor(1, 3); lcd.print("PRESS ROT p/ START");
  tone(PIN_BUZZER, 800, 100);
}

void spawn() { pX = 3; pY = 0; pRot = 0; pType = random(7); }

bool colide(int tx, int ty, int tr) {
  for (int i = 0; i < 16; i++) {
    if ((SHAPES[pType][tr] >> (15 - i)) & 1) {
      int x = tx + i % 4, y = ty + i / 4;
      if (x < 0 || x >= BOARD_W || y >= BOARD_H || (y >= 0 && board[y][x])) return true;
    }
  }
  return false;
}

void render() {
  byte view[BOARD_H][BOARD_W];
  memcpy(view, board, sizeof(board));
  for (int i = 0; i < 16; i++) {
    if ((SHAPES[pType][pRot] >> (15 - i)) & 1) {
      int x = pX + i % 4, y = pY + i / 4;
      if (x >= 0 && x < BOARD_W && y >= 0 && y < BOARD_H) view[y][x] = 1;
    }
  }
  for (int r = 0; r < 4; r++) {
    lcd.setCursor(0, r);
    for (int c = 0; c < BOARD_W; c++) {
      byte t = view[r * 2][c], b = view[r * 2 + 1][c];
      if (t && b) lcd.write(3); else if (t) lcd.write(1); else if (b) lcd.write(2); else lcd.print(" ");
    }
    lcd.print("|");
  }
}

void loop() {
  if (!emJogo) {
    if (digitalRead(BTN_ROT) == LOW) {
      emJogo = true; score = 0; memset(board, 0, sizeof(board));
      lcd.clear(); lcd.setCursor(12, 1); lcd.print("PTS:");
      spawn(); render(); delay(300);
    }
    return;
  }

  if (digitalRead(BTN_L) == LOW && !colide(pX - 1, pY, pRot)) { pX--; render(); delay(120); }
  if (digitalRead(BTN_R) == LOW && !colide(pX + 1, pY, pRot)) { pX++; render(); delay(120); }
  if (digitalRead(BTN_ROT) == LOW) {
    int r = (pRot + 1) % 4;
    if (!colide(pX, pY, r)) { pRot = r; tone(PIN_BUZZER, 1200, 15); render(); delay(200); }
  }

  if (millis() - lastTick > (digitalRead(BTN_D) == LOW ? 60 : 700)) {
    if (!colide(pX, pY + 1, pRot)) { pY++; }
    else {
      for (int i = 0; i < 16; i++) {
        if ((SHAPES[pType][pRot] >> (15 - i)) & 1) {
          int x = pX + i % 4, y = pY + i / 4;
          if (y >= 0) board[y][x] = 1;
        }
      }
      for (int y = BOARD_H - 1; y >= 0; y--) {
        bool f = true; for (int x = 0; x < BOARD_W; x++) if (!board[y][x]) f = false;
        if (f) {
          score += 100; tone(PIN_BUZZER, 1800, 80);
          for (int ty = y; ty > 0; ty--) for (int tx = 0; tx < BOARD_W; tx++) board[ty][tx] = board[ty - 1][tx];
          for (int tx = 0; tx < BOARD_W; tx++) board[0][tx] = 0;
          y++; lcd.setCursor(16, 1); lcd.print(score);
        }
      }
      spawn();
      if (colide(pX, pY, pRot)) {
        lcd.clear(); lcd.setCursor(5, 1); lcd.print("FIM DE JOGO");
        tone(PIN_BUZZER, 300, 600); delay(3000); emJogo = false; menu();
      }
    }
    lastTick = millis(); render();
  }
}