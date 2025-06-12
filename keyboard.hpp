#include <Arduino.h>
#include <M5StickCPlus2.h>
#define BTN_C_PIN 35

void drawKeyboard(String str_label, uint8_t font_size);

String inputText = "";
bool upper = false;
int row = 0;
int col = 0;

const char* keys[][11] = {
  //{"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-"},
  {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"},
  {"q", "w", "e", "r", "t", "y", "u", "i", "o", "p"},
  {"a", "s", "d", "f", "g", "h", "j", "k", "l", ";"},
  {"z", "x", "c", "v", "b", "n", "m", ",", ".", "/"},
  {"BCK", "LST", "CAP", " ", "DEL", "RST", "OK"}
};

//int keyLengths[] = {11, 10, 9, 7, 3};
int keyLengths[] = {10, 10, 10, 10, 7};
//const char* shiftedNums[] = {"!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "_"};
const char* shiftedNums[] = {"!", "@", "#", "$", "%", "^", "&", "*", "(", ")"};
const char* shiftedRow4[] = {"<", ">", "?"};
const char* shiftedRow3[] = {":"};

unsigned long btnCTimer = 0;
bool btnCHolding = false;

