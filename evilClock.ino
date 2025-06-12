// Most of the code that comes from Volos Projects
// Thanks to @VolosProjects for the source code and sprite configuation and built the UI of the M5StickC Plus2!
// Volos YT: https://www.youtube.com/@VolosProjects/
//
// Source code: https://github.com/VolosR/evilClock/

//#include <Wire.h>
#include "M5StickCPlus2.h"
#include "WORLD_IR_CODES.h"
#include "read_from_file.hpp"
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <ESP32Time.h>
#include "readable_wificode.hpp"
#include <EEPROM.h>
#include "esp32-hal-timer.h"
#include "keyboard.hpp"
#include "icons.h"
// #include <HardwareSerial.h>
#include "FreeSansBold12pt7b.h"
#include <BleKeyboard.h>
#include <DuckyEngine.h>
// #include <SD.h>

//#include "play_gif.hpp"
//#include "boot_gif.hpp"

//#include <UniversalTelegramBot.h>
#include "driver/timer.h"
#include <WebServer.h>
#include <Update.h>
#include "soc/soc_caps.h"
#include "pitches.h"
#include <BluetoothSerial.h>
#include <HTTPClient.h>
#include <WiFi.h>
//#include <SD.h>
//#include <SPI.h>
#include <ArduinoJson.h>
#include <time.h>
#include <FS.h>
#include <LITTLEFS.h>
//#include "readfile.hpp"
//#include <LittleFS.h>
//#include <esp_wifi.h>
#include <TinyGPSPlus.h>
#include "telegram.hpp"
//#include <BleKeyboard.h>
//#include <vector>
#include "applejuice.hpp"
#include "lora_audio_uart.hpp"
BleKeyboard bleKeyboard("M5StickC+2", "M5Stack", 100);
DuckyEngine macro(bleKeyboard);
//#ifndef BOT_TOKEN
UniversalTelegramBot bot(BOT_TOKEN, scr_cli);

WebServer server(80);
BluetoothSerial SerialBT;
TinyGPSPlus gps;
HardwareSerial GPSRaw(1);      // Use UART1
HardwareSerial LoRaSerial(2);  // Use UART2
//#include "timer.h"
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);
#include "Noto.h"
#include "smallFont.h"
#include "middleFont.h"
#include "bigFont.h"
#include "secFont.h"

void xmitCodeElement(uint16_t ontime, uint16_t offtime, uint8_t PWM_code);
void delay_ten_us(uint16_t us);
uint8_t read_bits(uint8_t count);
void displayMenu();
void selectKey();
void playSong();
void moveHorizontal(int dir);
void moveVertical(int dir);
uint16_t rawData[300];
float imu_temp;

unsigned long delay_time = 0;
unsigned long _time_d = 0;
String gps_lon, gps_lat;
String payload;  // global var
String old_payload;
String fullday;  // full day with the format "yyyy-mm-ddThh:mm"
String timezone, full_timezone;
String last_ssid = "";
int w_code;
int httpCode;

uint8_t DEVICE_ID = 0x01;

uint8_t ssid_cnt = 0;
uint8_t max_song = 1;
uint8_t song_cnt = 0;

#define putstring_nl(s) Serial.println(s)
#define putstring(s) Serial.print(s)
#define putnum_ud(n) Serial.print(n, DEC)
#define putnum_uh(n) Serial.print(n, HEX)


#define CS_PIN 26
#define BT_ID "YOUR_BT_ID"
#define WIFI_SSID BT_ID
#define WIFI_PWD "YOUR_WIFI_PWD"
#define LONG_PRESS_TIME 500  // long press time (in milliseconds)
//#define SCREEN_WIDTH 240
//#define SCREEN_HEIGHT 135
const int SCREEN_WIDTH = 240;
const int SCREEN_HEIGHT = 135;
#define IRLED 19
#define IRLED_2 26
static const unsigned char PROGMEM image_bolt_bits[] = { 0x00, 0x30, 0x00, 0x70, 0x00, 0xf0, 0x01, 0xe0, 0x03, 0xe0, 0x07, 0xe0, 0x0f, 0xfc, 0x1f, 0xfc, 0x3f, 0xf8, 0x3f, 0xf0, 0x07, 0xe0, 0x07, 0xc0, 0x07, 0x80, 0x0f, 0x00, 0x0e, 0x00, 0x0c, 0x00 };
static const unsigned char PROGMEM image_download_1_bits[] = { 0x01, 0xf0, 0x00, 0x06, 0x0c, 0x00, 0x18, 0x03, 0x00, 0x21, 0xf0, 0x80, 0x46, 0x0c, 0x40, 0x88, 0x02, 0x20, 0x10, 0xe1, 0x00, 0x23, 0x18, 0x80, 0x04, 0x04, 0x00, 0x08, 0x42, 0x00, 0x01, 0xb0, 0x00, 0x02, 0x08, 0x00, 0x00, 0x40, 0x00, 0x00, 0xa0, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00 };

static const unsigned char PROGMEM image_download_2_bits[] = { 0x01, 0x00, 0x02, 0x80, 0x02, 0x40, 0x22, 0x20, 0x12, 0x20, 0x0a, 0x40, 0x06, 0x80, 0x03, 0x00, 0x06, 0x80, 0x0a, 0x40, 0x12, 0x20, 0x22, 0x20, 0x02, 0x40, 0x02, 0x80, 0x01, 0x00, 0x00, 0x00 };

static const unsigned char PROGMEM image_download_bits[] = { 0x0f, 0x80, 0x30, 0x60, 0x40, 0x10, 0x47, 0x10, 0x88, 0x88, 0x90, 0x48, 0x90, 0x48, 0x50, 0x50, 0x48, 0x90, 0x27, 0x20, 0x20, 0x20, 0x10, 0x40, 0x08, 0x80, 0x05, 0x00, 0x07, 0x00, 0x02, 0x00 };

//weather bits
static const unsigned char PROGMEM image_download_3_bits[] = { 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x08, 0x20, 0x00, 0x10, 0x10, 0x00, 0x30, 0x08, 0x00, 0x40, 0x0e, 0x00, 0x80, 0x01, 0x00, 0x80, 0x00, 0x80, 0x40, 0x00, 0x80, 0x3f, 0xff, 0x00, 0x01, 0x10, 0x00, 0x22, 0x22, 0x00, 0x44, 0x84, 0x00, 0x91, 0x28, 0x00, 0x22, 0x40, 0x00, 0x00, 0x80, 0x00 };
static const unsigned char PROGMEM image_sun_bits[] = { 0x01, 0x00, 0x21, 0x08, 0x10, 0x10, 0x03, 0x80, 0x8c, 0x62, 0x48, 0x24, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x48, 0x24, 0x8c, 0x62, 0x03, 0x80, 0x10, 0x10, 0x21, 0x08, 0x01, 0x00, 0x00, 0x00 };
static const unsigned char PROGMEM image_download_4_bits[] = { 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x08, 0x20, 0x00, 0x10, 0x10, 0x00, 0x30, 0x08, 0x00, 0x40, 0x0e, 0x00, 0x80, 0x01, 0x00, 0x80, 0x00, 0x80, 0x40, 0x00, 0x80, 0x3f, 0xff, 0x00, 0x00, 0x00, 0x00, 0x09, 0x4a, 0x00, 0x20, 0x00, 0x00, 0x08, 0x90, 0x00, 0x22, 0x44, 0x00, 0x00, 0x00, 0x00 };

static const unsigned char PROGMEM image_download_5_bits[] = { 0x00, 0x20, 0x00, 0x02, 0x02, 0x00, 0x00, 0x70, 0x00, 0x01, 0x8c, 0x00, 0x09, 0x04, 0x80, 0x02, 0x02, 0x00, 0x02, 0x02, 0x00, 0x07, 0x82, 0x00, 0x08, 0x44, 0x80, 0x10, 0x2c, 0x00, 0x30, 0x30, 0x00, 0x60, 0x1e, 0x00, 0x80, 0x03, 0x00, 0x80, 0x01, 0x00, 0x80, 0x01, 0x00, 0x7f, 0xfe, 0x00 };

static const unsigned char PROGMEM image_download_6_bits[] = { 0x01, 0x00, 0x13, 0x90, 0x31, 0x18, 0x73, 0x9c, 0x09, 0x20, 0x05, 0x40, 0x53, 0x94, 0xfe, 0xfe, 0x53, 0x94, 0x05, 0x40, 0x09, 0x20, 0x73, 0x9c, 0x31, 0x18, 0x13, 0x90, 0x01, 0x00, 0x00, 0x00 };

static const unsigned char PROGMEM image_download_7_bits[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x03, 0x88, 0x04, 0x44, 0x04, 0x44, 0x00, 0x44, 0x00, 0x88, 0xff, 0x32, 0x00, 0x00, 0xad, 0x82, 0x00, 0x60, 0x00, 0x10, 0x00, 0x10, 0x01, 0x20, 0x00, 0xc0 };

//static const unsigned char PROGMEM image_download_bits[] = { 0x0f, 0x80, 0x30, 0x60, 0x40, 0x10, 0x47, 0x10, 0x88, 0x88, 0x90, 0x48, 0x90, 0x48, 0x50, 0x50, 0x48, 0x90, 0x27, 0x20, 0x20, 0x20, 0x10, 0x40, 0x08, 0x80, 0x05, 0x00, 0x07, 0x00, 0x02, 0x00 };

static const unsigned char PROGMEM image_weather_cloud_lightning_bolt_bits[] = { 0x00, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x08, 0x20, 0x00, 0x10, 0x10, 0x00, 0x30, 0x08, 0x00, 0x40, 0x0e, 0x00, 0x80, 0x81, 0x00, 0x81, 0x00, 0x80, 0x43, 0x00, 0x80, 0x26, 0x3f, 0x00, 0x0f, 0x80, 0x00, 0x01, 0x80, 0x00, 0x03, 0x00, 0x00, 0x02, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00 };

const int Layer_11_color = 0x4208;

#define MAX_WAIT_TIME 65535  //tens of us (ie: 655.350ms)

IRsend irsend(IRLED);  // Set the GPIO to be used to sending the message.
IRsend irsend_2(IRLED_2);

extern const IrCode* const NApowerCodes[];
extern const IrCode* const EUpowerCodes[];
extern uint8_t num_NAcodes, num_EUcodes;
const char* version = "1.3.0";
bool is_beta = true;
const char* bullet = "|";
const char* dow_str[] PROGMEM = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };

bool inGPS = false;
uint8_t bitsleft_r = 0;
uint8_t _count = 0;
uint8_t _w_count = 0;
unsigned long _delay = 0;
unsigned long _w_c_delay = 0;
unsigned long _max_cnt = 1;
uint8_t bits_r = 0;
uint8_t code_ptr;
volatile const IrCode* powerCode;
bool isInMenu = false;
bool isSetup = false;
String selected_wifi;
String temp, weather, name, country, lon_lat, weather_time, humid, clouds, pressure;
//int tempo = 114;

// change this to whichever pin you want to use
//const int buzzer = 2; // Buzzer pin of the M5StickC Plus2

// Change to 0.5 for a slower version of the song, 1.25 for a faster version
const float songSpeed = 0.75;
//std::vector<int> notes = {
int16_t notes[] PROGMEM = {
  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
  NOTE_A4, NOTE_G4, NOTE_A4, 0,

  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
  NOTE_A4, NOTE_G4, NOTE_A4, 0,

  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
  NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, 0,
  NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, 0,
  NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, 0,

  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
  NOTE_D5, NOTE_E5, NOTE_A4, 0,
  NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, 0,
  NOTE_C5, NOTE_A4, NOTE_B4, 0,

  NOTE_A4, NOTE_A4,
  //Repeat of first part
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
  NOTE_A4, NOTE_G4, NOTE_A4, 0,

  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
  NOTE_A4, NOTE_G4, NOTE_A4, 0,

  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
  NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, 0,
  NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, 0,
  NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, 0,

  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
  NOTE_D5, NOTE_E5, NOTE_A4, 0,
  NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, 0,
  NOTE_C5, NOTE_A4, NOTE_B4, 0,
  //End of Repeat

  NOTE_E5, 0, 0, NOTE_F5, 0, 0,
  NOTE_E5, NOTE_E5, 0, NOTE_G5, 0, NOTE_E5, NOTE_D5, 0, 0,
  NOTE_D5, 0, 0, NOTE_C5, 0, 0,
  NOTE_B4, NOTE_C5, 0, NOTE_B4, 0, NOTE_A4,

  NOTE_E5, 0, 0, NOTE_F5, 0, 0,
  NOTE_E5, NOTE_E5, 0, NOTE_G5, 0, NOTE_E5, NOTE_D5, 0, 0,
  NOTE_D5, 0, 0, NOTE_C5, 0, 0,
  NOTE_B4, NOTE_C5, 0, NOTE_B4, 0, NOTE_A4
};

//std::vector<int> durations = {
int durations[] PROGMEM = {
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125,

  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125,

  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 125, 250, 125,

  125, 125, 250, 125, 125,
  250, 125, 250, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 375,

  250, 125,
  //Rpeat of First Part
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125,

  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125,

  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 125, 250, 125,

  125, 125, 250, 125, 125,
  250, 125, 250, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 375,
  //End of Repeat

  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 125, 125, 125, 375,
  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 500,

  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 125, 125, 125, 375,
  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 500
};


ESP32Time rtc(0);
#define EEPROM_SIZE 512
#define MAX_LEN 32
#define CRED_SIZE 64
#define MAX_NETWORKS 10

#define SSID_ADDR(n) (n * CRED_SIZE + 5)
#define PASS_ADDR(n) (SSID_ADDR(n) + MAX_LEN)

//extern const GFXfont FreeSansBold12pt7b;

//colors
unsigned short grays[12];
unsigned long lastPressTime = millis();
#define blue 0x1314
#define blue2 0x022F
unsigned short tmpColor = 0;
unsigned long lastLoad = 0;
uint16_t ssid_index = 0;

double rad = 0.01745;
float x[10];
float y[10];
float px[10];
float py[10];

float x2[10];
float y2[10];
float px2[10];
float py2[10];

float x3[61];
float y3[61];
float px3[61];
float py3[61];

int r = 8;
int sx = 120;
int sy = 84;
bool charge;
int r2 = 16;
int sx2 = 23;
int sy2 = 178;

int r3 = 21;
int sx3 = 65;
int sy3 = 98;

int poz = 0;
uint8_t max_count = 2;
//time variables
String h, m, s;
uint16_t day, month;
String months[12] PROGMEM = { "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC" };
String day_of_week[7] PROGMEM = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };
//settime variables
bool setTimeDate = false;
int setData[8];  //setHour,setMin,setSec,setDate,setMonth,setYear; SET REGION , SET BEEPER;
String setDataLbl[8] PROGMEM = { "HOUR", "MIN", "SEC", "DATE", "MON", "YEAR", "REGION", "SOUND" };
int setMin[8] PROGMEM = { 0, 0, 0, 1, 1, 24, 0, 0 };
int setMax[8] PROGMEM = { 24, 60, 60, 32, 13, 100, 2, 2 };
int setPosX[8] PROGMEM = { 10, 50, 91, 10, 50, 91, 8, 8 };
int setPosY[8] PROGMEM = { 54, 54, 54, 124, 124, 124, 172, 192 };
int chosen = 0;


//brightness and battery
int brightnes[6] PROGMEM = { 16, 32, 48, 64, 96, 180 };
uint8_t b = 0;
int vol;
int volE;
int bat;

//region and buzze and ir
int Myregion = 0;
int buzzer = 0;
bool sendIR = false;

//stopwatch variable
int fase = 0;  //0=0 1=start 2=stop
String minn = "00";
String secc = "00";
String mill = "00";

//sleep variables
int sleepTime = 10;
int ts, tts = 0;
bool slp = false;
String dow;

#define IP5306_ADDR 0x75
#define IP5306_REG_CHG_STATUS 0x78

typedef enum {
  NOT_CHARGING = 0,
  CHARGING,
  UNKNOWN_CHARGE
} charge_t;
/*
charge_t getChargeStatus() {
  Wire.beginTransmission(IP5306_ADDR);
  Wire.write(IP5306_REG_CHG_STATUS);
  if (Wire.endTransmission(false) != 0) {
    return UNKNOWN_CHARGE;
  }

  Wire.requestFrom(IP5306_ADDR, 1);
  if (Wire.available()) {
    uint8_t val = Wire.read();
    return (val & 0x40) ? CHARGING : NOT_CHARGING;  // Bit 6: Charging
  }

  return UNKNOWN_CHARGE;
}
*/
void handleRoot() {
  server.send(200, "text/html", upload_html);
}

void handleUpdateUpload() {
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    //M5.Lcd.println("OTA Start");
    if (!Update.begin()) {
      Update.printError(Serial);
    }
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    Update.write(upload.buf, upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    if (Update.end(true)) {
      //M5.Lcd.println("OTA Success");
      server.send(200, "text/plain", "Update complete. Rebooting...");
      delay(1000);
      ESP.restart();
    } else {
      Update.printError(Serial);
      server.send(500, "text/plain", "Update Failed!");
    }
  }
}
String default_html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>M5StickC Plus2 Main Menu</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      background-color: #f2f2f2;
      padding: 20px;
    }
    h1 {
      color: #333;
    }
    .menu {
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 20px;
      margin-top: 30px;
    }
    .menu a {
      display: block;
      padding: 20px;
      background-color: #4CAF50;
      color: white;
      text-decoration: none;
      border-radius: 10px;
      font-size: 20px;
      transition: background-color 0.3s ease;
    }
    .menu a:hover {
      background-color: #45a049;
    }
  </style>
</head>
<body>
  <h1>M5StickC Plus2 Home</h1>
  <div class="menu">
    <a href="/upload">Upload file (/upload)</a>
    <br>
    <a href="/file">LittleFS files (/file)</a>
  </div>
</body>
</html>
)rawliteral";
void handleUpload() {
  HTTPUpload& upload = server.upload();
  static File uploadFile;
  static String uploadPath = "/";

  if (upload.status == UPLOAD_FILE_START) {
    uploadPath = "/";
    if (server.hasArg("path")) uploadPath = server.arg("path");
    if (!uploadPath.endsWith("/")) uploadPath += "/";

    String filename = uploadPath + upload.filename;
    if (LITTLEFS.exists(filename)) LITTLEFS.remove(filename);
    uploadFile = LITTLEFS.open(filename, FILE_WRITE);
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (uploadFile) uploadFile.write(upload.buf, upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    if (uploadFile) uploadFile.close();
  }
}
void handleUploadForm() {
  String redirectPath = "/";
  if (server.hasArg("path")) redirectPath = server.arg("path");
  server.sendHeader("Location", "/file?dir=" + redirectPath, true);
  server.send(303);
}
void handleFileBrowser() {
  String path = server.arg("dir");
  if (path == "") path = "/";

  File root = LITTLEFS.open(path, "r");
  if (!root || !root.isDirectory()) {
    server.send(404, "text/plain", "Directory not found");
    return;
  }

  String html = R"rawliteral(
    <!DOCTYPE html><html><head><meta charset="UTF-8">
    <title>File Browser</title>
    <style>
      body { font-family: sans-serif; background: #f4f5f7; padding: 20px; }
      table { width: 100%; border-collapse: collapse; margin-top: 10px; }
      th, td { padding: 10px; text-align: left; }
      th { background-color: #f0eeee; }
      tr:nth-child(even) { background-color: #f9f9f9; }
      a { text-decoration: none; color: #0077cc; }
      .folder { font-weight: bold; }
      .upload { margin-top: 20px; }
    </style>
    </head><body>
  )rawliteral";

  html += "<h2>📁 Listing: " + path + "</h2>";
  html += "<table><tr><th>Name</th><th>Type</th><th>Action</th></tr>";

  // Nút trở về thư mục trước
  if (path != "/") {
    int lastSlash = path.lastIndexOf('/');
    String parent = path.substring(0, lastSlash);
    if (parent == "") parent = "/";
    html += "<tr><td colspan='3'><a href='/file?dir=" + parent + "'>🔙 ..</a></td></tr>";
  }

  File file = root.openNextFile();
  while (file) {
    String fullPath = String(file.path());
    if (!fullPath.startsWith(path)) {
      file = root.openNextFile();
      continue;
    }

    String name = fullPath.substring(path.length());
    if (name.startsWith("/")) name = name.substring(1);
    if (name == "") {
      file = root.openNextFile();
      continue;
    }

    // Nếu là thư mục, tạo link vào sâu hơn
    if (file.isDirectory()) {
      html += "<tr><td class='folder'><a href='/file?dir=" + fullPath + "'>📁 " + name + "</a></td><td>Directory</td><td></td></tr>";
    } else {
      html += "<tr><td><a href='/open_file?path=" + fullPath + "'>📄 " + name + "</a></td><td>File</td><td><a href='/delete_file?dir=" + fullPath + "' onclick=\"return confirm('Delete file?')\">🗑️ Delete</a></td></tr>";
    }

    file = root.openNextFile();
  }

  html += "</table>";

  html += R"rawliteral(
    <div class='upload'>
      <h3>📤 Upload New File</h3>
      <form method='POST' action='/upload_file?path=)rawliteral"
          + path + R"rawliteral(' enctype='multipart/form-data'>
        <input type='file' name='upload'><input type='hidden' name='path' value=')rawliteral"
          + path + R"rawliteral('>
        <input type='submit' value='Upload'>
      </form>
    </div>
  )rawliteral";

  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setupWebServer() {
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", default_html);
  });
  server.on("/upload", HTTP_GET, handleRoot);
  server.on(
    "/update", HTTP_POST, []() {
      server.send(200, "text/plain", Update.hasError() ? "FAIL" : "OK");
    },
    handleUpdateUpload);
  // server.on("/file", HTTP_GET, []() {
  //   // String path = "/";
  //   // if (server.hasArg("dir")) {
  //   //   path = server.arg("dir");
  //   //   if (!path.startsWith("/")) path = "/" + path;
  //   // }
  //   // String html = generateFileListHTML(path);
  //   // server.send(200, "text/html", html);

  // });
  server.on("/file", HTTP_GET, handleFileBrowser);
  server.on("/delete_file", HTTP_GET, [](void) {
    String path = server.arg("dir");
    if (LITTLEFS.remove(path)) {
      server.sendHeader("Location", "/file?dir=" + String(path).substring(0, path.lastIndexOf('/')));
      server.send(303);
    } else {
      server.send(500, "text/plain", "Delete failed");
    }
  });
  server.on("/open_file", HTTP_GET, handleFileRequest);
  server.on("/upload_file", HTTP_POST, handleUploadForm, handleUpload);
  server.begin();
}
void handleFileRequest() {
  String path = server.arg("path");
  if (!LITTLEFS.exists(path)) {
    server.send(404, "text/plain", "File not found");
    return;
  }

  File file = LITTLEFS.open(path, "r");
  if (!file || file.isDirectory()) {
    server.send(400, "text/plain", "Invalid file");
    return;
  }

  // Simple content type detection
  String contentType = "text/plain";
  if (path.endsWith(".html")) contentType = "text/html";
  else if (path.endsWith(".css")) contentType = "text/css";
  else if (path.endsWith(".js")) contentType = "application/javascript";
  else if (path.endsWith(".json")) contentType = "application/json";
  else if (path.endsWith(".png")) contentType = "image/png";
  else if (path.endsWith(".jpg") || path.endsWith(".jpeg")) contentType = "image/jpeg";
  else if (path.endsWith(".gif")) contentType = "image/gif";
  else if (path.endsWith(".ico")) contentType = "image/x-icon";
  else if (path.endsWith(".pdf")) contentType = "application/pdf";
  else if (path.endsWith(".zip")) contentType = "application/zip";

  server.streamFile(file, contentType);
  file.close();
}

// we cant read more than 8 bits at a time so dont try!
uint8_t read_bits(uint8_t count) {
  uint8_t i;
  uint8_t tmp = 0;

  // we need to read back count bytes
  for (i = 0; i < count; i++) {
    // check if the 8-bit buffer we have has run out
    if (bitsleft_r == 0) {
      // in which case we read a new byte in
      bits_r = powerCode->codes[code_ptr++];
      DEBUGP(putstring("\n\rGet byte: ");
             putnum_uh(bits_r););
      // and reset the buffer size (8 bites in a byte)
      bitsleft_r = 8;
    }
    // remove one bit
    bitsleft_r--;
    // and shift it off of the end of 'bits_r'
    tmp |= (((bits_r >> (bitsleft_r)) & 1) << (count - 1 - i));
  }
  // return the selected bits in the LSB part of tmp
  return tmp;
}

int getDayOfWeek(int year, int month, int day) {
  if (month < 3) {
    month += 12;
    year -= 1;
  }
  int K = year % 100;
  int J = year / 100;
  int h = (day + 13 * (month + 1) / 5 + K + K / 4 + J / 4 + 5 * J) % 7;
  return (h + 6) % 7;  // Make Sunday = 0
}

String readStringFromEEPROM(int addr) {
  char data[MAX_LEN];
  for (int i = 0; i < MAX_LEN; i++) {
    data[i] = EEPROM.read(addr + i);
    if (data[i] == 0) break;
  }
  return String(data);
}
bool findCredentialBySSID(const String& targetSSID, String& passwordOut, int& indexOut) {
  String ssid, password;

  for (int i = 0; i < MAX_NETWORKS; i++) {
    int base = i * CRED_SIZE;
    char ssidBuf[MAX_LEN + 1] = { 0 };
    char passBuf[MAX_LEN + 1] = { 0 };

    for (int j = 0; j < MAX_LEN; j++) {
      ssidBuf[j] = EEPROM.read(base + j);
      passBuf[j] = EEPROM.read(base + MAX_LEN + j);
    }

    ssid = String(ssidBuf);
    password = String(passBuf);

    if (ssid == targetSSID) {
      passwordOut = password;
      indexOut = i;
      return true;
    }
  }

  return false;
}

void clearCredential(int index) {
  if (index < 0 || index >= MAX_NETWORKS) return;
  int base = index * CRED_SIZE;
  for (int i = 0; i < CRED_SIZE; i++) {
    EEPROM.write(base + i, 0);
  }
  EEPROM.commit();
  Serial.printf("Cleared credential at slot %d\n", index);
}
void saveToEEPROM(int addr, const String& data) {
  for (int i = 0; i < MAX_LEN; i++) {
    EEPROM.write(addr + i, i < data.length() ? data[i] : 0);
  }
}
void saveCredential(int index, const String& ssid, const String& password) {
  if (index >= MAX_NETWORKS) return;
  saveToEEPROM(SSID_ADDR(index), ssid);
  saveToEEPROM(PASS_ADDR(index), password);
  EEPROM.commit();
}



#define BUTTON_PRESSED LOW
#define BUTTON_RELEASED HIGH

uint16_t ontime, offtime;
uint8_t i, num_codes;
uint8_t region;
bool isActivateBT = false;
bool isWiFistarted = false;
bool isAPstarted = false;
uint8_t dow_int;

void setup()  // ##############  SETUP ##################
{
  Serial.begin(115200);
  //fs::File read_cfg = LITTLEFS.open("/config/config.txt", "r");
  pinMode(35, INPUT_PULLUP);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  auto cfg = M5.config();
  //StickCP2.begin(cfg);
  M5.begin(cfg);
  //StickCP2.Rtc.setDateTime( { { 2024, 2, 17 }, { 8, 20, 0 } } );

  irsend.begin();
  irsend_2.begin();  // for external ir led
  // Wire.begin();
  pinMode(LED, OUTPUT);
  pinMode(TRIGGER, INPUT_PULLUP);
  //#ifdef __TinyGPSPlus_h
  //HardwareSerial gps(1, 3);
  //#endif
  WiFi.mode(WIFI_AP_STA);
  WiFi.setScanMethod(WIFI_ALL_CHANNEL_SCAN);
  WiFi.setAutoReconnect(true);
  if (!LITTLEFS.begin()) {
    Serial.printf("[%lu] E: LittleFS mounted unsuccessfully\n", millis());
  }
  //if (!GPSRaw.begin(9600, SERIAL_8N1, 33, 32)) {
  //Serial.printf("[%lu] [GPS] E: Failed to initialize GPS module.\n", millis());
  // }

  // if (!SD.begin(CS_PIN)) {
  //   Serial.printf("[%lu] E: Failed to initialize SD card\n", millis());
  // }
  delay_ten_us(5000);

  // Debug output: indicate how big our database is
  DEBUGP(putstring(F("\n\rNA Codesize: "));
         putnum_ud(num_NAcodes););
  DEBUGP(putstring(F("\n\rEU Codesize: "));
         putnum_ud(num_EUcodes););

  rtc.setTime(0, 0, 0, 1, 1, 2023, 0);
  EEPROM.begin(EEPROM_SIZE);
  b = EEPROM.read(0);
  if (b > 7) b = 2;

  ssid_cnt = 0;
  ssid_cnt = EEPROM.read(3);
  if (ssid_cnt > MAX_NETWORKS) ssid_cnt = 0;

  Myregion = EEPROM.read(1);
  if (Myregion > 1) Myregion = 0;

  buzzer = EEPROM.read(2);
  if (buzzer > 1) buzzer = 0;

  StickCP2.Display.setBrightness((StickCP2.Power.getBatteryLevel() <= 15) ? 8 : brightnes[b]);

  sprite.createSprite(135, 240);
  sprite.setSwapBytes(true);

  for (int i = 0; i < 8; i++) {
    x[i] = ((r - 1) * cos(rad * (i * 45))) + sx;
    y[i] = ((r - 1) * sin(rad * (i * 45))) + sy;
    px[i] = (r * cos(rad * (i * 45))) + sx;
    py[i] = (r * sin(rad * (i * 45))) + sy;
  }

  for (int i = 0; i < 10; i++) {
    x2[i] = ((r2 - 3) * cos(rad * (i * 36))) + sx2;
    y2[i] = ((r2 - 3) * sin(rad * (i * 36))) + sy2;
    px2[i] = (r2 * cos(rad * (i * 36))) + sx2;
    py2[i] = (r2 * sin(rad * (i * 36))) + sy2;
  }

  int an = 270;
  for (int i = 0; i < 60; i++) {
    x3[i] = ((r3 - 6) * cos(rad * an)) + sx3;
    y3[i] = ((r3 - 6) * sin(rad * an)) + sy3;
    px3[i] = (r3 * cos(rad * an)) + sx3;
    py3[i] = (r3 * sin(rad * an)) + sy3;
    an = an + 6;
    if (an >= 360) an = 0;
  }

  int co = 216;
  for (int i = 0; i < 12; i++) {
    grays[i] = tft.color565(co, co, co);
    co = co - 20;
  }
  /*
  sprite.fillSprite(TFT_BLACK);
  sprite.setTextColor(TFT_WHITE);
  sprite.setTextDatum(4);
  sprite.drawString("[" + String(version) + "]", SCREEN_HEIGHT - 6 * String("[" + String(version) + "]").length(), SCREEN_WIDTH / 2);
  */
  StickCP2.Display.fillScreen(BLACK);
  StickCP2.Display.setTextDatum(0);
  StickCP2.Display.setTextSize(2);
  StickCP2.Display.setRotation(3);
  StickCP2.Display.setTextColor(TFT_GREEN, TFT_BLACK);
  StickCP2.Display.setCursor((SCREEN_WIDTH - 6 * 2 * strlen("M5-UI")) / 2, SCREEN_HEIGHT / 2 - 20);
  StickCP2.Display.println(F("M5-UI"));
  StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLACK);
  if (is_beta) {
    StickCP2.Display.setCursor((SCREEN_WIDTH - 6 * 2 * String("[v" + String(version) + "]").length()) / 2, SCREEN_HEIGHT / 2);
    StickCP2.Display.println("[v" + String(version) + "]");
    StickCP2.Display.setTextSize(1);
    StickCP2.Display.setCursor((SCREEN_WIDTH - 6 * strlen("beta")) / 2, StickCP2.Display.getCursorY());
    StickCP2.Display.println("beta");
#define IS_BETA true
  } else {
    StickCP2.Display.setCursor((SCREEN_WIDTH - 6 * 2 * String("[v" + String(version) + "]").length()) / 2, SCREEN_HEIGHT / 2);
    StickCP2.Display.println("[v" + String(version) + "]");
#define IS_BETA false
  }
  //Serial.println(F("Beta version"));
  delay(1000);
  if (digitalRead(39) != 0) {
    StickCP2.Display.fillScreen(BLACK);
    File boot = LITTLEFS.open("/boot.jpg", "r");
    if (!boot || boot.isDirectory()) {
      Serial.println(F("[BOOT FILE] E: cannot open boot.jpg as file"));
      StickCP2.Display.println(F("[BOOT FILE] E: cannot open boot.jpg as file"));
    } else {
      size_t jpg_size = boot.size();
      uint8_t* jpg_buf = (uint8_t*)malloc(jpg_size);
      if (!jpg_buf) {
        Serial.println(F("[BOOT FILE] E: cannot get the buf of boot.jpg"));
        StickCP2.Display.println(F("[BOOT FILE] E: cannot get the buf of boot.jpg"));
        unsigned long _not_press = millis();

        while (digitalRead(37) != 0 || digitalRead(39) != 0 || (millis() - _not_press < 5000)) {
          if (digitalRead(37) == 0 || digitalRead(39) == 0) {
            _not_press = millis();
            break;
          }
          if (millis() - _not_press >= 5000) break;
          yield();
        }
        boot.close();
      } else {
        boot.read(jpg_buf, jpg_size);
        boot.close();
        StickCP2.Display.drawJpg(jpg_buf, jpg_size, 0, 0);
        free(jpg_buf);
        unsigned long _not_press = millis();

        while (digitalRead(37) != 0 || digitalRead(39) != 0 || (millis() - _not_press < 5000)) {
          if (digitalRead(37) == 0 || digitalRead(39) == 0) {
            _not_press = millis();
            break;
          }
          if (millis() - _not_press >= 5000) break;
          yield();
        }
        delay(500);
      }
    }
  }

  //delay(5000);

  /*
  gif.begin();  // Khởi tạo palette RGB565

  if (gif.openFLASH((uint8_t*)f__evilClock_main_evilClock_data_boot_gif, f__evilClock_main_evilClock_data_boot_gif_len, GIFDraw)) {
    StickCP2.Display.fillScreen(BLACK);

    int delayMs;
    while (gif.playFrame(true, &delayMs, NULL)) {
      delay(delayMs);
    }
    gif.close();
  } else {
    Serial.println(F("Cannot open GIF file"));
  }*/
  StickCP2.Display.fillScreen(BLACK);
  // display message when the battery is low (equal or below 15)
  if (StickCP2.Power.getBatteryLevel() <= 15) {
    StickCP2.Display.setTextDatum(0);
    StickCP2.Display.setTextSize(2);
    StickCP2.Display.setRotation(3);
    StickCP2.Display.drawRoundRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 3, WHITE);
    StickCP2.Display.setTextColor(WHITE, BLACK);
    StickCP2.Display.setCursor((SCREEN_WIDTH - 6 * 2 * strlen("Your battery is low")) / 2, 10);
    StickCP2.Display.println(F("Your battery is low"));

    StickCP2.Display.setTextSize(1);
    StickCP2.Display.setCursor((SCREEN_WIDTH - 6 * strlen("Charging is recommended")) / 2, 30);
    StickCP2.Display.println(F("Charging is recommended"));
    delay(3000);
    StickCP2.Display.fillScreen(BLACK);
  }
  StickCP2.Display.setRotation(0);
}

void sendAllCodes() {
  bool endingEarly = false;  //will be set to true if the user presses the button during code-sending

  // determine region from REGIONSWITCH: 1 = NA, 0 = EU (defined in main.h)

  if (Myregion == 0) {
    region = EU;
    num_codes = num_EUcodes;
  }

  if (Myregion == 1) {
    region = NA;
    num_codes = num_NAcodes;
  }


  // for every POWER code in our collection
  for (i = 0; i < num_codes; i++) {

    // print out the code # we are about to transmit
    DEBUGP(putstring(F("\n\r\n\rCode #: "));
           putnum_ud(i));

    // point to next POWER code, from the right database
    if (region == NA) {
      powerCode = NApowerCodes[i];
    } else {
      powerCode = EUpowerCodes[i];
    }

    // Read the carrier frequency from the first byte of code structure
    const uint8_t freq = powerCode->timer_val;
    // set OCR for Timer1 to output this POWER code's carrier frequency

    // Print out the frequency of the carrier and the PWM settings
    DEBUGP(putstring(F("\n\rFrequency: "));
           putnum_ud(freq););

    DEBUGP(uint16_t x = (freq + 1) * 2;
           putstring(F("\n\rFreq: "));
           putnum_ud(F_CPU / x););

    // Get the number of pairs, the second byte from the code struct
    const uint8_t numpairs = powerCode->numpairs;
    DEBUGP(putstring(F("\n\rOn/off pairs: "));
           putnum_ud(numpairs));

    // Get the number of bits we use to index into the timer table
    // This is the third byte of the structure
    const uint8_t bitcompression = powerCode->bitcompression;
    DEBUGP(putstring(F("\n\rCompression: "));
           putnum_ud(bitcompression);
           putstring(F("\n\r")));

    // For EACH pair in this code....
    code_ptr = 0;
    for (uint8_t k = 0; k < numpairs; k++) {
      uint16_t ti;

      // Read the next 'n' bits as indicated by the compression variable
      // The multiply by 4 because there are 2 timing numbers per pair
      // and each timing number is one word long, so 4 bytes total!
      ti = (read_bits(bitcompression)) * 2;

      // read the onTime and offTime from the program memory
      ontime = powerCode->times[ti];       // read word 1 - ontime
      offtime = powerCode->times[ti + 1];  // read word 2 - offtime

      DEBUGP(putstring(F("\n\rti = "));
             putnum_ud(ti >> 1);
             putstring(F("\tPair = "));
             putnum_ud(ontime));
      DEBUGP(putstring(F("\t"));
             putnum_ud(offtime));

      rawData[k * 2] = ontime * 10;
      rawData[(k * 2) + 1] = offtime * 10;
      yield();
    }

    // Send Code with library
    irsend.sendRaw(rawData, (numpairs * 2), freq);
    irsend_2.sendRaw(rawData, (numpairs * 2), freq);
    Serial.print(F("\n"));
    yield();
    //Flush remaining bits, so that next code starts
    //with a fresh set of 8 bits.
    bitsleft_r = 0;


    // delay 205 milliseconds before transmitting next POWER code
    delay_ten_us(20500);

    // if user is pushing (holding down) TRIGGER button, stop transmission early
    if (digitalRead(TRIGGER) == BUTTON_PRESSED) {
      while (digitalRead(TRIGGER) == BUTTON_PRESSED) {
        yield();
      }
      endingEarly = true;
      delay_ten_us(50000);  //500ms delay
      //quickflashLEDx(4);
      //pause for ~1.3 sec to give the user time to release the button so that the code sequence won't immediately start again.
      delay_ten_us(MAX_WAIT_TIME);  // wait 655.350ms
      delay_ten_us(MAX_WAIT_TIME);  // wait 655.350ms
      break;                        //exit the POWER code "for" loop
    }

  }  //end of POWER code for loop

  if (endingEarly == false) {
    //pause for ~1.3 sec, then flash the visible LED 8 times to indicate that we're done
    delay_ten_us(MAX_WAIT_TIME);  // wait 655.350ms
    delay_ten_us(MAX_WAIT_TIME);  // wait 655.350ms
  }

}  //end of sendAllCodes
//#include "FreeSansBold12pt7b.h"



void drawGPS() {
  sprite.fillSprite(TFT_BLACK);
  sprite.setTextDatum(0);
  sprite.setTextColor(0x16C7);
  sprite.setTextSize(1);
  //sprite.loadFont(FreeSansBold12pt7bBitmaps);
  sprite.loadFont(Noto);
  sprite.drawString(F("GPS"), 6, 5);

  // Layer 2
  sprite.fillRoundRect(5, 220, 34, 16, 3, 0x24BE);

  // download
  if (GPSRaw.available())
    sprite.drawBitmap(115, 7, image_download_bits, 13, 16, 0xFFFF);
  else
    sprite.drawBitmap(115, 7, image_download_bits, 13, 16, 0x4208);

  // Layer 4
  sprite.setTextColor(0xFFFF);
  sprite.unloadFont();

  sprite.setTextSize(1);
  sprite.drawString(F("Exit"), 10, 225);

  // download

  if (!WiFi.isConnected())
    sprite.drawBitmap(92, 7, image_download_1_bits, 19, 16, 0x4208);
  else {
    auto ip_addr = WiFi.localIP();
    String ip_str = ip_addr.toString();
    sprite.drawString(ip_str, 43, 225);
    sprite.drawBitmap(92, 7, image_download_1_bits, 19, 16, 0xFFFF);
  }

  // download
  if (isActivateBT)
    sprite.drawBitmap(76, 7, image_download_2_bits, 14, 16, 0xFFFF);
  else
    sprite.drawBitmap(76, 7, image_download_2_bits, 14, 16, 0x4208);

  // Layer 7
  sprite.drawRoundRect(5, 27, 125, 190, 5, 0x16C7);

  // Layer 8
  sprite.setTextColor(0xD3B);
  sprite.drawString(F("LON"), 11, 37);

  // Layer 8 copy
  sprite.drawString(F("LAT"), 73, 37);

  // Layer 8 copy
  sprite.drawString(F("SATELLITES"), 38, 81);

  // Layer 11
  sprite.fillRoundRect(11, 49, 52, 23, 5, Layer_11_color);

  // Layer 11 copy
  sprite.fillRoundRect(73, 49, 53, 23, 5, 0x4208);

  // Layer 11 copy
  sprite.fillRoundRect(50, 91, 32, 24, 5, 0x4208);

  // Layer 14

  // Layer 8 copy
  sprite.setTextColor(0xD3B);
  sprite.drawString(F("SPEED"), 11, 122);

  // Layer 16
  sprite.setTextColor(0xFFFF);


  // Layer 11 copy
  sprite.fillRoundRect(12, 132, 59, 23, 5, 0x4208);

  // Layer 14 copy


  // Layer 14 copy
  sprite.drawString(F("km/h"), 73, 140);

  // Layer 8 copy
  sprite.setTextColor(0xD3B);
  sprite.drawString(F("ALTITUDE"), 11, 159);

  // Layer 11 copy
  sprite.fillRoundRect(11, 169, 29, 25, 5, 0x4208);

  // Layer 14 copy
  sprite.setTextColor(0xFFFF);
  sprite.drawString(F("m"), 43, 177);

  // Layer 14 copy


  // Layer 8 copy
  sprite.setTextColor(0xD3B);
  sprite.drawString(F("DEG"), 76, 159);

  // Layer 11 copy
  sprite.fillRoundRect(76, 169, 38, 25, 5, 0x4208);

  // Layer 14 copy
  sprite.setTextColor(0xFFFF);
  sprite.drawString(F("o"), 118, 170);

  // Layer 14 copy
  //weather
  if (w_code == 45)
    sprite.drawBitmap(58, 6, image_download_7_bits, 15, 16, 0xFFFF);
  else if (w_code == 48)
    sprite.drawBitmap(58, 6, image_download_7_bits, 15, 16, 0xFFFF);
  // else if (w_code == 48)
  //   sprite.drawBitmap(58, 6, image_download_7_bits, 15, 16, 0xFFFF);
  else if (w_code == 71 || w_code == 73 || w_code == 75 || w_code == 77)
    sprite.drawBitmap(58, 6, image_download_6_bits, 15, 16, 0xFFFF);
  else if (w_code == 95 || w_code == 96 || w_code == 99)
    sprite.drawBitmap(58, 6, image_weather_cloud_lightning_bolt_bits, 15, 16, 0xFFFF);
  else if (w_code == 85 || w_code == 86)
    sprite.drawBitmap(58, 6, image_download_4_bits, 15, 16, 0xFFFF);
  else if (w_code == 61 || w_code == 63 || w_code == 65 || w_code == 66 || w_code == 67 || w_code == 80 || w_code == 81 || w_code == 82)
    sprite.drawBitmap(58, 6, image_download_3_bits, 15, 16, 0xFFFF);
  else if (w_code == 51 || w_code == 53 || w_code == 55 || w_code == 56 || w_code == 57)
    sprite.drawBitmap(58, 6, image_download_3_bits, 15, 16, 0xFFFF);
  else if (w_code == 2 || w_code == 3)
    sprite.drawBitmap(58, 6, image_download_5_bits, 15, 16, 0xFFFF);
  else if (w_code == 0 || w_code == 1)
    sprite.drawBitmap(58, 6, image_sun_bits, 15, 16, 0xFFFF);

  // sprite.fillRoundRect(51, 220, 80, 16, 3, 0x73AE);
  // sprite.drawString(F("Refresh"), 78, 224);
  // if (StickCP2.BtnA.wasPressed()) {
  //   sprite.drawRoundRect(51, 220, 80, 16, 3, 0xFFFF);
  // }
  if (digitalRead(35) == 0) {
    sprite.drawRoundRect(5, 220, 34, 16, 3, 0xFFFF);
  }

  /*
      if (millis() - _w_c_delay > (10000 / (_max_cnt + 1)) || _w_c_delay == 0) {
        _w_count++;
        if (_w_count > _max_cnt) {
          _w_count = 0;
        }
        _w_c_delay = millis();
      }*/
  if (GPSRaw.available()) {
    unsigned long start = millis();
    while (GPSRaw.available() && millis() - start < 100) {
      gps.encode(GPSRaw.read());
      delay(1);
    }
    if (gps.location.isUpdated()) {
      sprite.setTextColor(0xFFFF);
      sprite.drawString(String(gps.location.lng()), 16, 57);

      // Layer 14 copy
      sprite.drawString(String(gps.location.lat()), 79, 57);
      sprite.drawString(String(gps.satellites.value()), 64, 99);
      sprite.drawString(String(gps.altitude.meters()), 19, 178);
      sprite.drawString(String(gps.speed.kmph()), 19, 140);
      sprite.drawString(String(gps.course.deg()), 86, 178);
    } else {
      sprite.drawString(F("0"), 16, 57);
      sprite.drawString(F("0"), 79, 57);
      sprite.drawString(F("0"), 64, 99);
      sprite.drawString(F("0"), 19, 178);
      sprite.drawString(F("0"), 19, 140);
      sprite.drawString(F("0"), 86, 178);
    }
  } else {
    sprite.drawString(F("0"), 16, 57);
    sprite.drawString(F("0"), 79, 57);
    sprite.drawString(F("0"), 64, 99);
    sprite.drawString(F("0"), 19, 178);
    sprite.drawString(F("0"), 19, 140);
    sprite.drawString(F("0"), 86, 178);
  }
  //sprite.pushSprite(0, 0);
  StickCP2.Display.pushImage(0, 0, 135, 240, (uint16_t*)sprite.getPointer());
}

void drawMain() {

  sprite.fillSprite(TFT_BLACK);
  sprite.setTextDatum(4);

  sprite.fillSmoothRoundRect(6, 6, 98, 124, 2, grays[7], TFT_BLACK);
  sprite.fillSmoothRoundRect(8, 8, 94, 120, 2, TFT_BLACK, grays[7]);

  sprite.fillSmoothRoundRect(46, 158, 84, 75, 2, grays[7], TFT_BLACK);
  sprite.fillSmoothRoundRect(48, 160, 80, 71, 2, TFT_BLACK, grays[7]);
  if (!WiFi.isConnected()) {
    if (_count == 0)
      sprite.fillSmoothRoundRect(52, 209, 72, 18, 2, blue2, TFT_BLACK);
  }
  if ((poz == 1 && fase == 2) || (poz == 1 && fase != 2))
    sprite.fillSmoothRoundRect(6, 214, 34, 18, 2, TFT_ORANGE, TFT_BLACK);
  if (poz == 1 && fase != 2) {
    sprite.loadFont(Noto);
    sprite.setTextColor(TFT_BLACK, TFT_ORANGE);
    sprite.drawString(F("IMG"), 22, 225);
    sprite.unloadFont();
  }
  //seconds %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  sprite.loadFont(secFont);
  sprite.setTextColor(grays[5], TFT_BLACK);

  for (int i = 0; i < 60; i++)
    if (i % 5 == 0)
      sprite.fillSmoothCircle(px3[i], py3[i], 1, TFT_ORANGE, TFT_BLACK);
  sprite.drawWedgeLine(sx3, sy3, x3[s.toInt()], y3[s.toInt()], 4, 1, TFT_ORANGE, TFT_BLACK);
  sprite.fillSmoothCircle(sx3, sy3, 2, TFT_BLACK, TFT_ORANGE);

  sprite.unloadFont();
  //the line splits the time and the date ################################
  sprite.drawLine(10 - 2, 26, 94 + 8, 26, grays[7]);
  sprite.drawLine(10 - 2, 27, 94 + 8, 27, grays[7]);
  sprite.drawLine(48, 175 - 2, 55 + 48 + 1 + 25, 175 - 2, grays[7]);
  sprite.drawLine(48, 175 - 1, 55 + 48 + 1 + 25, 175 - 1, grays[7]);


  sprite.drawLine(10 - 2, 70, 94 + 8, 70, grays[7]);
  sprite.drawLine(10 - 2, 71, 94 + 8, 71, grays[7]);

  //date #########################################################
  sprite.loadFont(smallFont);
  sprite.setTextColor(0x35D7, TFT_BLACK);
  sprite.drawString(months[month - 1], 22, 84);
  sprite.fillRect(12, 94, 22, 30, grays[8]);
  sprite.setTextColor(TFT_ORANGE, grays[8]);
  sprite.drawString(String(day), 22, 110);
  sprite.setTextColor(grays[3], TFT_BLACK);
  sprite.drawString(String(sleepTime), sx2, sy2 + 2);
  sprite.unloadFont();


  //stopwatch ################

  if (!WiFi.isConnected()) {
    max_count = 2;
    fs::File file = LITTLEFS.open("/weather.json", "r");
    if (_count == 0) {
      sprite.loadFont(middleFont);
      sprite.setTextColor(TFT_BLACK, grays[4]);
      sprite.setTextColor(grays[2], TFT_BLACK);
      sprite.drawString(F(":"), 88 - 12, 186);
      sprite.drawString(F("."), 88 + 12, 186);
      sprite.drawString(secc, 88, 186);
      sprite.drawString(minn, 88 - 24, 186);
      sprite.drawString(mill, 88 + 24, 186);
      sprite.unloadFont();
    } else if (_count == 1) {
      if (file.available()) {
        if (payload != old_payload || payload == "") {
          while (file.available()) {
            payload += (char)file.read();
          }
          old_payload = payload;
          file.close();
        }

        DynamicJsonDocument doc(9000 * 10);
        DeserializationError error = deserializeJson(doc, payload);
        temp = doc["current"]["temperature_2m"].as<String>() + " C";
        //weather = doc["weather"][0]["description"].as<String>();
        full_timezone = doc["timezone"].as<String>();
        name = doc["current"]["rain"].as<String>() + "," + doc["current"]["showers"].as<String>() + "," + doc["current"]["snowfall"].as<String>() + "mm";
        country = doc["current"]["wind_speed_10m"].as<String>() + "km/h";  // replace with wind speed
        w_code = doc["current"]["weather_code"].as<int>();
        //Serial.println("Code: " + String(w_code) + " " + doc["current"]["weather_code"].as<String>() + "," + doc["current"]["weather_code"].as<int>());
        //Serial.println(w_code);
        //if (SerialBT.available()) SerialBT.println("Code: " + String(w_code) + " " + doc["current"]["weather_code"].as<String>());
        //switch (w_code) {
        //w_code = doc["current"]["weather_code"].as<int>();

        if (w_code == 0) weather = "clear sky";
        else if (w_code == 1) weather = "mainly clear";
        else if (w_code == 2) weather = "partly cloudy";
        else if (w_code == 3) weather = "overcast";
        else if (w_code == 45) weather = "fog";
        else if (w_code == 48) weather = "rime fog";
        else if (w_code == 51) weather = "light drizzle";
        else if (w_code == 53) weather = "moderate drizzle";
        else if (w_code == 55) weather = "heavy drizzle";
        else if (w_code == 56) weather = "light drizzle";
        else if (w_code == 57) weather = "heavy drizzle";
        else if (w_code == 61) weather = "slight rain";
        else if (w_code == 63) weather = "moderate rain";
        else if (w_code == 65) weather = "heavy rain";
        else if (w_code == 66) weather = "light rain";
        else if (w_code == 67) weather = "heavy rain";
        else if (w_code == 71) weather = "slight snowfall";
        else if (w_code == 73) weather = "moderate snowfall";
        else if (w_code == 75) weather = "heavy snowfall";
        else if (w_code == 77) weather = "snow grains";
        else if (w_code == 80) weather = "shower rain";
        else if (w_code == 81) weather = "moderate showers";
        else if (w_code == 82) weather = "violent showers";
        else if (w_code == 85) weather = "snow showers";
        else if (w_code == 86) weather = "heavy showers";
        else if (w_code == 95) weather = "thunderstorms";
        else if (w_code == 96) weather = "thunderstorm";
        else if (w_code == 99) weather = "thunderstorm";
        else weather = "Invalid";

        // Serial.println("Timezone: " + doc["timezone"].as<String>());
        // if (SerialBT.available()) SerialBT.println("Timezone: " + doc["timezone"].as<String>());
        lon_lat = doc["latitude"].as<String>() + "," + doc["longitude"].as<String>();
        timezone = doc["timezone_abbreviation"].as<String>();

        sprite.drawString(name, 88, 186 + 10 + 10);
        sprite.drawString(country, 88, 186 + 10);

        sprite.drawString(lon_lat, 88, 176 + 6);
        sprite.drawString(weather, 88, 186 + 20 + 10);
        sprite.drawString(temp, 88, 186 + 20 + 20);
      } else {
        sprite.drawString(F("No weather"), 88, 176 + 6);
        sprite.drawString(F("data"), 88, 176 + 6 + 6);
      }
    } else if (_count == 2) {
      if (file.available()) {
        if (payload != old_payload || payload == "") {
          while (file.available()) {
            payload += (char)file.read();
          }
          old_payload = payload;
          file.close();
        }

        DynamicJsonDocument doc(9000 * 10);
        DeserializationError error = deserializeJson(doc, payload);
        weather_time = doc["current"]["time"].as<String>();
        humid = "Humidity:" + doc["current"]["relative_humidity_2m"].as<String>() + "%";
        clouds = "Cloud:" + doc["current"]["cloud_cover"].as<String>() + "%";
        pressure = doc["current"]["surface_pressure"].as<String>() + "hPa";
        sprite.drawString(weather_time, 88, 186 + 10 + 10);
        sprite.drawString(humid, 88, 186 + 10);

        sprite.drawString(lon_lat, 88, 176 + 6);
        sprite.drawString(clouds, 88, 186 + 20 + 10);
        sprite.drawString(pressure, 88, 186 + 20 + 20);

      } else {
        sprite.drawString(F("No weather"), 88, 176 + 6);
        sprite.drawString(F("data"), 88, 176 + 6 + 6);
      }
    }
  }

  else {
    //lastLoad = millis();
    double lon, lat;
    if (SerialBT.available()) {
      if (WiFi.isConnected()) {
        String lon_lat = SerialBT.readStringUntil('\n');
        int cma_i = lon_lat.indexOf(",");
        if (cma_i != -1) {
          lon = lon_lat.substring(0, cma_i).toDouble();
          lat = lon_lat.substring(cma_i + 1).toDouble();
          SerialBT.printf("Received: %f, %f\n", lat, lon);
          Serial.printf("Received: %f, %f\n", lat, lon);
        } else {
          //default
          lon = 106.61384178324955;
          lat = 10.862385596190741;
        }
      }
    } else {
      //default if the bluetooth serial is not available
      if (WiFi.isConnected()) {
        if (Serial.available()) {

          String lon_lat = Serial.readStringUntil('\n');
          int8_t cma_i = lon_lat.indexOf(",");
          if (cma_i != -1) {
            lon = lon_lat.substring(0, cma_i).toDouble();
            lat = lon_lat.substring(cma_i + 1).toDouble();
            Serial.printf("Received: %f, %f\n", lat, lon);
          } else {
            lon = 106.61384178324955;
            lat = 10.862385596190741;
          }
        } else if (SerialBT.available()) {
          String lon_lat = SerialBT.readStringUntil('\n');
          int8_t cma_i = lon_lat.indexOf(",");
          if (cma_i != -1) {
            lon = lon_lat.substring(0, cma_i).toDouble();
            lat = lon_lat.substring(cma_i + 1).toDouble();
            Serial.printf("Received: %f, %f\n", lat, lon);
          } else {
            lon = 106.61384178324955;
            lat = 10.862385596190741;
          }
        } else {
          lon = 106.61384178324955;
          lat = 10.862385596190741;
        }
      }
    }
    HTTPClient http;
    //sleepTime=10;
    //slp=false;
    String _raw;
    if ((millis() - lastLoad >= 3 * 60 * 1000) || lastLoad == 0 || (Serial.available() || SerialBT.available())) {
      if (Serial.available()) {
        _raw = Serial.readStringUntil('\n');
        int _cma = _raw.indexOf(",");
        if (_cma != -1) {
          lon = _raw.substring(_cma + 1).toDouble();
          lat = _raw.substring(0, _cma).toDouble();
          Serial.printf("Received: %f, %f\n", lat, lon);
        } else {
          //default
          lon = 106.61384178324955;
          lat = 10.862385596190741;
        }
      } else if (SerialBT.available()) {
        _raw = SerialBT.readStringUntil('\n');
        int _cma = _raw.indexOf(",");
        if (_cma != -1) {
          lon = _raw.substring(_cma + 1).toDouble();
          lat = _raw.substring(0, _cma).toDouble();
          SerialBT.printf("Received: %f, %f\n", lat, lon);
        } else {
          //default
          lon = 106.61384178324955;
          lat = 10.862385596190741;
        }
      }
      sleepTime = 10;

      temp = weather = name = country = lon_lat = weather_time = humid = clouds = pressure = "";
      sprite.drawString(F("Fetching"), 88, 176 + 6);
      sprite.drawString(F("weather data"), 88, 176 + 6 + 10);
      Serial.println(F("Fetching weather API..."));
      if (SerialBT.available()) SerialBT.println(F("Fetching weather API..."));

      //sleepTime = 10;

      if (slp) {
        StickCP2.Display.wakeup();
        StickCP2.Display.setBrightness((StickCP2.Power.getBatteryLevel() <= 15) ? 8 : brightnes[b]);
        fase = 0;
        sleepTime = 10;
        slp = false;
      }
      sprite.loadFont(bigFont);
      sprite.setTextColor(grays[0], TFT_BLACK);
      if (_time_d == 0 || millis() - _time_d >= 1000) {
        //sprite.drawString(h + ":" + m, 52, 52);
        /*
    sprite.drawString(h + " " + m, 52, 52);
    auto cursor_x = sprite.getCursorX();
    sprite.drawString(":", cursor_x - 11 * m.length() * 2, 50);
*/
        auto cursor_x = sprite.getCursorX();
        sprite.drawString(h, 13 + 11, 54);
        sprite.drawString(F(":"), ((94 + 8) - 11 * strlen(":")) / 2 + 5, 52);
        sprite.drawString(m, (94 + 8) - 11 * (m.length()), 54);
        /*
    if (((StickCP2.Rtc.getDateTime().time.hours >= 10 && StickCP2.Rtc.getDateTime().time.hours < 20) && StickCP2.Rtc.getDateTime().time.hours != 11))
      sprite.drawString(":", 11 * h.length() + 50 - 11 * m.length() - 2, 50);
    else if ((StickCP2.Rtc.getDateTime().time.hours == 11 && StickCP2.Rtc.getDateTime().time.minutes != 11) || StickCP2.Rtc.getDateTime().time.hours == 21)
      sprite.drawString(":", 11 * h.length() + 50 - 11 * m.length() - 5, 50);
    else if (StickCP2.Rtc.getDateTime().time.hours == 11 && StickCP2.Rtc.getDateTime().time.minutes == 11)
      sprite.drawString(":", 11 * h.length() + 50 - 11 * m.length() + 10, 50);
    else
      sprite.drawString(":", 11 * h.length() + 50 - 11 * m.length() + 2, 50);
    */
        //sprite.drawString(":", ((94 + 8) - 11 * (String(":").length())) / 2, 50);
        if (millis() - _time_d >= 2000)
          _time_d = millis();
      } else {
        //sprite.drawString(h + " " + m, 52, 52);
        //sprite.drawString(h + " " + m, 52, 52);
        sprite.drawString(h, 13 + 11, 54);
        sprite.drawString(F(" "), ((94 + 8) - 11 * strlen(" ")) / 2 + 5, 52);
        sprite.drawString(m, (94 + 8) - 11 * (m.length()), 54);
      }
      sprite.unloadFont();
      sprite.loadFont(Noto);


      //////brightness region
      sprite.fillSmoothRoundRect(110, 70, 20, 58, 2, blue, TFT_BLACK);
      sprite.fillSmoothCircle(sx, sy, 5, grays[1], blue);
      for (int i = 0; i < 8; i++)
        sprite.drawWedgeLine(px[i], py[i], x[i], y[i], 1, 1, grays[1], blue);
      for (int i = 0; i < b + 1; i++)
        sprite.fillRect(115, 122 - (i * 5), 10, 3, grays[1]);

      for (int i = 0; i < 10; i++)
        sprite.drawWedgeLine(px2[i], py2[i], x2[i], y2[i], 2, 2, 0x7000, TFT_BLACK);

      for (int i = 0; i < sleepTime; i++)
        sprite.drawWedgeLine(px2[i], py2[i], x2[i], y2[i], 2, 2, TFT_RED, TFT_BLACK);

      // butons
      if (poz == 1 && fase == 2) {
        sprite.setTextColor(TFT_BLACK, TFT_ORANGE);
        sprite.drawString(F("SET"), 22, 225);
      }
      if (!WiFi.isConnected()) {
        if (_count == 0) {
          sprite.setTextColor(grays[1], blue2);
          sprite.drawString(F("STA/STP"), 87, 220);
        }
      }
      sprite.setTextColor(grays[3], TFT_BLACK);

      //batery region

      sprite.unloadFont();
      sprite.setTextColor(grays[5], TFT_BLACK);
      sprite.drawString(String(vol / 1000.00), 121, 51);
      //sprite.drawString(String(getPercentage()) + "%", 121, 63);
      sprite.drawString(String(StickCP2.Power.getBatteryLevel()) + "%", 121, 63);
      // charge_t charge_status = getChargeStatus();
      // if (charge_status != CHARGING) {
      sprite.drawRect(114, 12, 14, 28, grays[2]);
      sprite.fillRect(118, 9, 6, 3, grays[2]);
      for (int i = 0; i < volE; i++) {
        if (volE > 3)
          sprite.fillRect(116, 35 - (i * 5), 10, 3, TFT_GREEN);
        else if (volE == 3)
          sprite.fillRect(116, 35 - (i * 5), 10, 3, TFT_YELLOW);
        else
          sprite.fillRect(116, 35 - (i * 5), 10, 3, TFT_RED);
      }
      // } else {
      // sprite.drawBitmap(114, 12, image_bolt_bits, 16, 16, YELLOW);
      // }

      sprite.setTextDatum(0);
      sprite.setTextColor(grays[4], TFT_BLACK);
      sprite.drawString(F("SLEEP"), 9, 202);
      sprite.setTextColor(grays[2], TFT_BLACK);
      if (!WiFi.isConnected()) {
        max_count = 2;
        if (_count == 0)
          sprite.drawString(F("STOPWATCH"), 55, 165);
        else if (_count == 1 || _count == 2) {
          sprite.drawString(F("WEATHER"), 55, 165);
          //sprite.setTextColor(grays[4], TFT_BLACK);
          //sprite.drawString(timezone, 6 * String("WEATHER").length() + 15, 165);
        }
        /*
    else if (_count == 1)
      sprite.drawString("LITTLEFS", 55, 165);
    else if (_count == 2)
      sprite.drawString("WEATHER", 55, 165);
    */
      } else {
        max_count = 1;
        if (_count == 0) {
          sprite.drawString(F("WEATHER"), 55, 165);
          //sprite.setTextColor(grays[4], TFT_BLACK);
          //sprite.drawString(timezone, 6 * String("WEATHER").length() + 15, 165);
        } else if (_count == 1) {
          sprite.drawString(F("WEATHER"), 55, 165);
          //sprite.setTextColor(grays[4], TFT_BLACK);
          //sprite.drawString(timezone, 6 * String("WEATHER").length() + 15, 165);
        }
      }
      //sprite.drawString(dow + (charge) ? " | CHARGING" : "",12,14);
      //sprite.fillRect(10, 10, 6 * dow.length() + 5, 15, TFT_RED);
      //sprite.setTextColor(TFT_WHITE, TFT_RED);
      //sprite.setTextColor(TFT_WHITE, TFT_BLACK);
      sprite.setTextColor(TFT_RED, TFT_BLACK);

      sprite.drawString(dow, 13, 14);
      sprite.setTextColor(grays[2], TFT_BLACK);
      auto dt = StickCP2.Rtc.getDateTime();
      //sprite.drawString((charge) ? String(bullet) + " CHARGING" : "", (94 + 8) - 6 * (String(String(bullet) + " CHARGING").length()) - 4, 14);
      //sprite.drawString(String(bullet), ((94 + 8) - 6 * (String(bullet).length())) / 2, 14);
      sprite.drawLine(((94 + 8) - 6 * strlen(" ")) / 2, 7, ((94 + 8) - 6 * strlen(" ")) / 2, 26, grays[7]);
      sprite.drawLine(((94 + 8) - 6 * strlen(" ")) / 2 + 1, 7, ((94 + 8) - 6 * strlen(" ")) / 2 + 1, 26, grays[7]);

      //sprite.fillRect((94 + 8) - 6 * (String(dt.date.year).length()) - 6, 10, 6 * String(dt.date.year).length() + 3, 15, 0x7BEF);
      //sprite.setTextColor(TFT_WHITE, 0x7BEF);
      //sprite.setTextColor(TFT_WHITE, TFT_BLACK);
      sprite.setTextColor(TFT_GREEN, TFT_BLACK);

      sprite.drawString(String(dt.date.year), (94 + 8) - 6 * (String(dt.date.year).length()) - 4, 14);
      /*
    if (M5.Power.isCharging()) {
      sprite.drawString("Charging",dateOfWeek().length()+ 6,14);
    }*/

      //buzzer
      sprite.setTextColor(TFT_ORANGE, TFT_BLACK);
      sprite.drawString(F("BUZZ"), 110, 134);
      if (buzzer)
        sprite.drawString(F("ON"), 110, 146);
      else
        sprite.drawString(F("OFF"), 110, 146);


      //region
      sprite.setTextColor(grays[2], TFT_BLACK);
      sprite.drawString(F("REG"), 8, 135);
      if (Myregion == 0)
        sprite.drawString(F("EU"), 8, 146);
      else
        sprite.drawString(F("NA"), 8, 146);
      if (!WiFi.isConnected()) {
        if (_count == 0) {
          sprite.setTextColor(grays[6], TFT_BLACK);
          if (isActivateBT)
            sprite.drawString(F("BT activated"), 55, 198);
          else if (isAPstarted)
            sprite.drawString(F("AP started"), 55, 198);
          else {
            if (M5.Power.getVBUSVoltage() >= 0)
              sprite.drawString(String(imu_temp) + " C" + String(bullet) + String(M5.Power.getVBUSVoltage()) + "V", 55, 198);
            else
              sprite.drawString(String(imu_temp) + " C", 55, 198);
          }
        }
      }
      sprite.setTextColor(grays[6], TFT_BLACK);
      sprite.drawString(String(s), 88, 116);

      sprite.fillTriangle(94, 142, 104, 135, 104, 151, TFT_ORANGE);
      sprite.fillRect(94, 139, 8, 9, TFT_ORANGE);


      sprite.fillRect(36, 134, 3, 5, TFT_RED);           // poz==1
      sprite.fillRect(30 + 6 * 5, 134, 3, 5, TFT_BLUE);  //poz==5

      for (int i = 0; i < 10; i++)
        if (i == poz)
          sprite.fillRect(30 + (i * 6), 142, 3, 6, TFT_RED);
        else
          sprite.fillRect(30 + (i * 6), 142, 3, 6, grays[6]);


      StickCP2.Display.pushImage(0, 0, 135, 240, (uint16_t*)sprite.getPointer());

      //http.begin("http://api.openweathermap.org/data/2.5/weather?lon=" + String(lon) + "&lat=" + String(lat) + "&appid=5d89cf94d1787347cb458959d1890a54&units=metric");
      //http.begin("http://api.open-meteo.com/v1/forecast?latitude="+String(lat)+"&longitude="+String(lon)+"&hourly=temperature_2m,relative_humidity_2m,wind_speed_10m,wind_speed_80m,wind_speed_120m,wind_speed_180m,wind_direction_10m,wind_direction_80m,wind_direction_120m,wind_direction_180m,wind_gusts_10m,temperature_80m,temperature_180m,temperature_120m,visibility,cloud_cover,cloud_cover_low,cloud_cover_mid,cloud_cover_high,snowfall,showers,rain,snow_depth,uv_index,uv_index_clear_sky,is_day,temperature_1000hPa,temperature_925hPa,temperature_800hPa,temperature_500hPa,temperature_250hPa,temperature_100hPa,temperature_30hPa,temperature_975hPa,temperature_900hPa,temperature_700hPa,temperature_200hPa,temperature_400hPa,temperature_70hPa,temperature_950hPa,temperature_850hPa,temperature_600hPa,temperature_300hPa,temperature_150hPa,temperature_50hPa&forecast_hours=1&past_hours=1&minutely_15=temperature_2m,relative_humidity_2m,apparent_temperature,dew_point_2m,precipitation,wind_gusts_10m,visibility,is_day,rain,snowfall,snowfall_height,sunshine_duration,freezing_level_height,wind_speed_10m,wind_speed_80m,wind_direction_10m,wind_direction_80m&daily=sunrise,sunset,rain_sum,showers_sum,snowfall_sum,uv_index_max,uv_index_clear_sky_max,temperature_2m_max,temperature_2m_min,apparent_temperature_max,apparent_temperature_min,wind_speed_10m_max,wind_gusts_10m_max,wind_direction_10m_dominant,shortwave_radiation_sum,temperature_2m_mean,wind_gusts_10m_min,wind_speed_10m_min,wind_gusts_10m_mean,wind_speed_10m_mean&timezone=Asia/Ho_Chi_Minh");
      http.begin("http://api.open-meteo.com/v1/forecast?latitude=" + String(lat) + "&longitude=" + String(lon) + "&current=temperature_2m,relative_humidity_2m,apparent_temperature,is_day,wind_speed_10m,wind_direction_10m,wind_gusts_10m,pressure_msl,surface_pressure,rain,showers,snowfall,cloud_cover,weather_code,precipitation&timezone=Asia/Ho_Chi_Minh");

      sleepTime = 10;
      httpCode = http.GET();
      //sprite.drawLine(130 - 6 * String(httpCode).length() - 4, 165, 130 - 6 * String(httpCode).length() + 4, 165 + 8, grays[7]);
      sprite.drawString(String(httpCode), 130 - 6 * String(httpCode).length(), 165 + 3);

      if (httpCode == HTTP_CODE_OK) {
        fs::File filewrite = LITTLEFS.open("/weather.json", "w");
        payload = http.getString();
        if (payload != old_payload) {
          //if (httpCode == HTTP_CODE_OK) {

          filewrite.println(payload);
          old_payload = payload;
        }
        filewrite.close();
      }
      //}
      // log the error
      Serial.println("httpCode = " + String(httpCode) + " -> " + http.errorToString(httpCode));
      if (SerialBT.available()) SerialBT.println("httpCode = " + String(httpCode) + " -> " + http.errorToString(httpCode));
      lastLoad = millis();
    }
    if (httpCode == HTTP_CODE_OK) {
      max_count = 1;
      sprite.drawString(String(httpCode), 130 - 6 * String(httpCode).length(), 165 + 3);

      if (_count == 0) {
        DynamicJsonDocument doc(9000 * 10);
        DeserializationError error = deserializeJson(doc, payload);
        temp = doc["current"]["temperature_2m"].as<String>() + " C";
        //weather = doc["weather"][0]["description"].as<String>();
        name = doc["current"]["rain"].as<String>() + "," + doc["current"]["showers"].as<String>() + "," + doc["current"]["snowfall"].as<String>() + "mm";
        timezone = doc["timezone_abbreviation"].as<String>();
        full_timezone = doc["timezone"].as<String>();

        country = doc["current"]["wind_speed_10m"].as<String>() + "km/h";  // replace with wind speed
        w_code = doc["current"]["weather_code"].as<int>();
        //Serial.println("Code: " + String(w_code) + " " + doc["current"]["weather_code"].as<String>() + "," + doc["current"]["weather_code"].as<int>());
        //if (SerialBT.available()) SerialBT.println("Code: " + String(w_code) + " " + doc["current"]["weather_code"].as<String>());
        //w_code = doc["current"]["weather_code"].as<int>();

        if (w_code == 0) weather = "clear sky";
        else if (w_code == 1) weather = "mainly clear";
        else if (w_code == 2) weather = "partly cloudy";
        else if (w_code == 3) weather = "overcast";
        else if (w_code == 45) weather = "fog";
        else if (w_code == 48) weather = "rime fog";
        else if (w_code == 51) weather = "light drizzle";
        else if (w_code == 53) weather = "moderate drizzle";
        else if (w_code == 55) weather = "heavy drizzle";
        else if (w_code == 56) weather = "light drizzle";
        else if (w_code == 57) weather = "heavy drizzle";
        else if (w_code == 61) weather = "slight rain";
        else if (w_code == 63) weather = "moderate rain";
        else if (w_code == 65) weather = "heavy rain";
        else if (w_code == 66) weather = "light rain";
        else if (w_code == 67) weather = "heavy rain";
        else if (w_code == 71) weather = "slight snowfall";
        else if (w_code == 73) weather = "moderate snowfall";
        else if (w_code == 75) weather = "heavy snowfall";
        else if (w_code == 77) weather = "snow grains";
        else if (w_code == 80) weather = "shower rain";
        else if (w_code == 81) weather = "moderate showers";
        else if (w_code == 82) weather = "violent showers";
        else if (w_code == 85) weather = "snow showers";
        else if (w_code == 86) weather = "heavy showers";
        else if (w_code == 95) weather = "thunderstorms";
        else if (w_code == 96) weather = "thunderstorm";
        else if (w_code == 99) weather = "thunderstorm";
        else weather = "Invalid";

        // Serial.println("Timezone: " + doc["timezone"].as<String>());
        // if (SerialBT.available()) SerialBT.println("Timezone: " + doc["timezone"].as<String>());
        lon_lat = doc["latitude"].as<String>() + "," + doc["longitude"].as<String>();
        sprite.drawString(lon_lat, 88, 176 + 6);
        sprite.drawString(country, 88, 186 + 10);

        sprite.drawString(name, 88, 186 + 10 + 10);
        sprite.drawString(weather, 88, 186 + 20 + 10);
        sprite.drawString(temp, 88, 186 + 20 + 20);
      } else if (_count == 1) {
        DynamicJsonDocument doc(9000 * 10);
        DeserializationError error = deserializeJson(doc, payload);
        weather_time = doc["current"]["time"].as<String>();
        humid = "Humidity:" + doc["current"]["relative_humidity_2m"].as<String>() + "%";
        clouds = "Cloud:" + doc["current"]["cloud_cover"].as<String>() + "%";
        pressure = doc["current"]["surface_pressure"].as<String>() + "hPa";
        sprite.drawString(weather_time, 88, 186 + 10 + 10);
        sprite.drawString(humid, 88, 186 + 10);

        sprite.drawString(lon_lat, 88, 176 + 6);
        sprite.drawString(clouds, 88, 186 + 20 + 10);
        sprite.drawString(pressure, 88, 186 + 20 + 20);
      }
    } else {
      String json;
      fs::File fileread = LITTLEFS.open("/weather.json", "r");
      sprite.drawString(String(httpCode), 130 - 6 * String(httpCode).length(), 165 + 3);

      max_count = 1;
      if (fileread.available()) {
        if (_count == 0) {
          while (fileread.available()) {
            json += (char)fileread.read();
          }
          fileread.close();
          //if (fileread.available()) {
          DynamicJsonDocument doc(9000 * 10);
          DeserializationError error = deserializeJson(doc, payload);
          temp = doc["current"]["temperature_2m"].as<String>() + " C";
          //weather = doc["weather"][0]["description"].as<String>();
          timezone = doc["timezone_abbreviation"].as<String>();
          full_timezone = doc["timezone"].as<String>();

          name = doc["current"]["rain"].as<String>() + "," + doc["current"]["showers"].as<String>() + "," + doc["current"]["snowfall"].as<String>() + "mm";
          country = doc["current"]["wind_speed_10m"].as<String>() + "km/h";  // replace with wind speed
          w_code = doc["current"]["weather_code"].as<int>();
          //Serial.println("Code: " + String(w_code) + " " + doc["current"]["weather_code"].as<String>() + "," + doc["current"]["weather_code"].as<int>());
          //if (SerialBT.available()) SerialBT.println("Code: " + String(w_code) + " " + doc["current"]["weather_code"].as<String>());
          //w_code = doc["current"]["weather_code"].as<int>();

          if (w_code == 0) weather = "clear sky";
          else if (w_code == 1) weather = "mainly clear";
          else if (w_code == 2) weather = "partly cloudy";
          else if (w_code == 3) weather = "overcast";
          else if (w_code == 45) weather = "fog";
          else if (w_code == 48) weather = "rime fog";
          else if (w_code == 51) weather = "light drizzle";
          else if (w_code == 53) weather = "moderate drizzle";
          else if (w_code == 55) weather = "heavy drizzle";
          else if (w_code == 56) weather = "light drizzle";
          else if (w_code == 57) weather = "heavy drizzle";
          else if (w_code == 61) weather = "slight rain";
          else if (w_code == 63) weather = "moderate rain";
          else if (w_code == 65) weather = "heavy rain";
          else if (w_code == 66) weather = "light rain";
          else if (w_code == 67) weather = "heavy rain";
          else if (w_code == 71) weather = "slight snowfall";
          else if (w_code == 73) weather = "moderate snowfall";
          else if (w_code == 75) weather = "heavy snowfall";
          else if (w_code == 77) weather = "snow grains";
          else if (w_code == 80) weather = "shower rain";
          else if (w_code == 81) weather = "moderate showers";
          else if (w_code == 82) weather = "violent showers";
          else if (w_code == 85) weather = "snow showers";
          else if (w_code == 86) weather = "heavy showers";
          else if (w_code == 95) weather = "thunderstorms";
          else if (w_code == 96) weather = "thunderstorm";
          else if (w_code == 99) weather = "thunderstorm";
          else weather = "Invalid";

          // Serial.println("Timezone: " + doc["timezone"].as<String>());
          if (SerialBT.available()) SerialBT.println("Timezone: " + doc["timezone"].as<String>());
          lon_lat = doc["latitude"].as<String>() + "," + doc["longitude"].as<String>();
          sprite.drawString(name, 88, 186 + 10 + 10);
          sprite.drawString(country, 88, 186 + 10);

          sprite.drawString(lon_lat, 88, 176 + 6);
          sprite.drawString(weather, 88, 186 + 20 + 10);
          sprite.drawString(temp, 88, 186 + 20 + 20);
        } else if (_count == 1) {
          while (fileread.available()) {
            json += (char)fileread.read();
          }
          fileread.close();
          DynamicJsonDocument doc(9000 * 10);
          DeserializationError error = deserializeJson(doc, payload);
          weather_time = doc["current"]["time"].as<String>();
          humid = "Humidity:" + doc["current"]["relative_humidity_2m"].as<String>() + "%";
          clouds = "Cloud:" + doc["current"]["cloud_cover"].as<String>() + "%";
          pressure = doc["current"]["surface_pressure"].as<String>() + "hPa";
          sprite.drawString(weather_time, 88, 186 + 10 + 10);
          sprite.drawString(humid, 88, 186 + 10);

          sprite.drawString(lon_lat, 88, 176 + 6);
          sprite.drawString(clouds, 88, 186 + 20 + 10);
          sprite.drawString(pressure, 88, 186 + 20 + 20);
        }
      } else {
        sprite.drawString(F("HTTP error:"), 88, 176 + 6);
        sprite.drawString(String(httpCode), 88, 186 + 10);
      }
    }
  }


  //}

  //if (name != "" || country != "" || lon_lat != "" || weather != "" || temp != "") {

  //Serial.printf("Name: %s, country: %s, geolocation: %s, weather: %s, temp: %s C\n", name, country, lon_lat, weather, temp);
  //if (SerialBT.available()) SerialBT.printf("Name: %s, country: %s, geolocation: %s, weather: %s, temp: %s C\n", name, country, lon_lat, weather, temp);
  /*} else {
      sprite.drawString("No weather", 88, 176 + 6);
      sprite.drawString("data", 88, 176 + 6 + 6);
    }
    */


  //time ##########################
  sprite.loadFont(bigFont);
  sprite.setTextColor(grays[0], TFT_BLACK);
  if (_time_d == 0 || millis() - _time_d >= 1000) {
    //sprite.drawString(h + ":" + m, 52, 52);
    /*
    sprite.drawString(h + " " + m, 52, 52);
    auto cursor_x = sprite.getCursorX();
    sprite.drawString(":", cursor_x - 11 * m.length() * 2, 50);
*/
    auto cursor_x = sprite.getCursorX();
    sprite.drawString(h, 13 + 11, 54);
    sprite.drawString(F(":"), ((94 + 8) - 11 * strlen(":")) / 2 + 5, 52);
    sprite.drawString(m, (94 + 8) - 11 * (m.length()), 54);
    /*
    if (((StickCP2.Rtc.getDateTime().time.hours >= 10 && StickCP2.Rtc.getDateTime().time.hours < 20) && StickCP2.Rtc.getDateTime().time.hours != 11))
      sprite.drawString(":", 11 * h.length() + 50 - 11 * m.length() - 2, 50);
    else if ((StickCP2.Rtc.getDateTime().time.hours == 11 && StickCP2.Rtc.getDateTime().time.minutes != 11) || StickCP2.Rtc.getDateTime().time.hours == 21)
      sprite.drawString(":", 11 * h.length() + 50 - 11 * m.length() - 5, 50);
    else if (StickCP2.Rtc.getDateTime().time.hours == 11 && StickCP2.Rtc.getDateTime().time.minutes == 11)
      sprite.drawString(":", 11 * h.length() + 50 - 11 * m.length() + 10, 50);
    else
      sprite.drawString(":", 11 * h.length() + 50 - 11 * m.length() + 2, 50);
    */
    //sprite.drawString(":", ((94 + 8) - 11 * (String(":").length())) / 2, 50);
    if (millis() - _time_d >= 2000)
      _time_d = millis();
  } else {
    //sprite.drawString(h + " " + m, 52, 52);
    //sprite.drawString(h + " " + m, 52, 52);
    sprite.drawString(h, 13 + 11, 54);
    sprite.drawString(F(" "), ((94 + 8) - 11 * strlen(" ")) / 2 + 5, 52);
    sprite.drawString(m, (94 + 8) - 11 * (m.length()), 54);
  }
  sprite.unloadFont();
  sprite.loadFont(Noto);


  //////brightness region
  sprite.fillSmoothRoundRect(110, 70, 20, 58, 2, blue, TFT_BLACK);
  sprite.fillSmoothCircle(sx, sy, 5, grays[1], blue);
  for (int i = 0; i < 8; i++)
    sprite.drawWedgeLine(px[i], py[i], x[i], y[i], 1, 1, grays[1], blue);
  for (int i = 0; i < b + 1; i++)
    sprite.fillRect(115, 122 - (i * 5), 10, 3, grays[1]);

  for (int i = 0; i < 10; i++)
    sprite.drawWedgeLine(px2[i], py2[i], x2[i], y2[i], 2, 2, 0x7000, TFT_BLACK);

  for (int i = 0; i < sleepTime; i++)
    sprite.drawWedgeLine(px2[i], py2[i], x2[i], y2[i], 2, 2, TFT_RED, TFT_BLACK);

  // butons
  if (poz == 1 && fase == 2) {
    sprite.setTextColor(TFT_BLACK, TFT_ORANGE);
    sprite.drawString(F("SET"), 22, 225);
  } else {
  }
  if (!WiFi.isConnected()) {
    if (_count == 0) {
      sprite.setTextColor(grays[1], blue2);
      sprite.drawString(F("STA/STP"), 87, 220);
    }
  }
  sprite.setTextColor(grays[3], TFT_BLACK);

  //batery region

  sprite.unloadFont();
  sprite.setTextColor(grays[5], TFT_BLACK);
  sprite.drawString(String(vol / 1000.00), 121, 51);
  //sprite.drawString(String(getPercentage()) + "%", 121, 63);
  sprite.drawString(String(StickCP2.Power.getBatteryLevel()) + "%", 121, 63);
  sprite.drawRect(114, 12, 14, 28, grays[2]);
  sprite.fillRect(118, 9, 6, 3, grays[2]);
  for (int i = 0; i < volE; i++) {
    if (volE > 3)
      sprite.fillRect(116, 35 - (i * 5), 10, 3, TFT_GREEN);
    else if (volE == 3)
      sprite.fillRect(116, 35 - (i * 5), 10, 3, TFT_YELLOW);
    else
      sprite.fillRect(116, 35 - (i * 5), 10, 3, TFT_RED);
  }

  sprite.setTextDatum(0);
  sprite.setTextColor(grays[4], TFT_BLACK);
  sprite.drawString(F("SLEEP"), 9, 202);
  sprite.setTextColor(grays[2], TFT_BLACK);
  if (!WiFi.isConnected()) {
    max_count = 2;
    if (_count == 0)
      sprite.drawString(F("STOPWATCH"), 55, 165);
    else if (_count == 1 || _count == 2) {
      sprite.drawString(F("WEATHER"), 55, 165);
      //sprite.setTextColor(grays[4], TFT_BLACK);
      //sprite.drawString(timezone, 6 * String("WEATHER").length() + 15, 165);
    }
    /*
    else if (_count == 1)
      sprite.drawString("LITTLEFS", 55, 165);
    else if (_count == 2)
      sprite.drawString("WEATHER", 55, 165);
    */
  } else {
    max_count = 1;
    if (_count == 0) {
      sprite.drawString(F("WEATHER"), 55, 165);
      //sprite.setTextColor(grays[4], TFT_BLACK);
      //sprite.drawString(timezone, 6 * String("WEATHER").length() + 15, 165);
    } else if (_count == 1) {
      sprite.drawString(F("WEATHER"), 55, 165);
      //sprite.setTextColor(grays[4], TFT_BLACK);
      //sprite.drawString(timezone, 6 * String("WEATHER").length() + 15, 165);
    }
  }
  //sprite.drawString(dow + (charge) ? " | CHARGING" : "",12,14);
  //sprite.fillRect(10, 10, 6 * dow.length() + 5, 15, TFT_RED);
  //sprite.setTextColor(TFT_WHITE, TFT_RED);
  //sprite.setTextColor(TFT_WHITE, TFT_BLACK);
  sprite.setTextColor(TFT_RED, TFT_BLACK);

  sprite.drawString(dow, 13, 14);
  sprite.setTextColor(grays[2], TFT_BLACK);
  auto dt = StickCP2.Rtc.getDateTime();
  //sprite.drawString((charge) ? String(bullet) + " CHARGING" : "", (94 + 8) - 6 * (String(String(bullet) + " CHARGING").length()) - 4, 14);
  //sprite.drawString(String(bullet), ((94 + 8) - 6 * (String(bullet).length())) / 2, 14);
  sprite.drawLine(((94 + 8) - 6 * strlen(" ")) / 2, 7, ((94 + 8) - 6 * strlen(" ")) / 2, 26, grays[7]);
  sprite.drawLine(((94 + 8) - 6 * strlen(" ")) / 2 + 1, 7, ((94 + 8) - 6 * strlen(" ")) / 2 + 1, 26, grays[7]);

  //sprite.fillRect((94 + 8) - 6 * (String(dt.date.year).length()) - 6, 10, 6 * String(dt.date.year).length() + 3, 15, 0x7BEF);
  //sprite.setTextColor(TFT_WHITE, 0x7BEF);
  //sprite.setTextColor(TFT_WHITE, TFT_BLACK);
  sprite.setTextColor(TFT_GREEN, TFT_BLACK);

  sprite.drawString(String(dt.date.year), (94 + 8) - 6 * (String(dt.date.year).length()) - 4, 14);
  /*
    if (M5.Power.isCharging()) {
      sprite.drawString("Charging",dateOfWeek().length()+ 6,14);
    }*/

  //buzzer
  sprite.setTextColor(TFT_ORANGE, TFT_BLACK);
  sprite.drawString(F("BUZZ"), 110, 134);
  if (buzzer)
    sprite.drawString(F("ON"), 110, 146);
  else
    sprite.drawString(F("OFF"), 110, 146);


  //region
  sprite.setTextColor(grays[2], TFT_BLACK);
  sprite.drawString(F("REG"), 8, 135);
  if (Myregion == 0)
    sprite.drawString(F("EU"), 8, 146);
  else
    sprite.drawString(F("NA"), 8, 146);
  if (!WiFi.isConnected()) {
    if (_count == 0) {
      sprite.setTextColor(grays[6], TFT_BLACK);
      if (isActivateBT)
        sprite.drawString(F("BT activated"), 55, 198);
      else if (isAPstarted)
        sprite.drawString(F("AP started"), 55, 198);
      else {
        if (M5.Power.getVBUSVoltage() >= 0)
          sprite.drawString(String(imu_temp) + " C" + String(bullet) + String(M5.Power.getVBUSVoltage()) + "V", 55, 198);
        else
          sprite.drawString(String(imu_temp) + " C", 55, 198);
      }
    }
  }
  sprite.setTextColor(grays[6], TFT_BLACK);
  sprite.drawString(String(s), 88, 116);

  sprite.fillTriangle(94, 142, 104, 135, 104, 151, TFT_ORANGE);
  sprite.fillRect(94, 139, 8, 9, TFT_ORANGE);


  sprite.fillRect(36, 134, 3, 5, TFT_RED);           // poz==1
  sprite.fillRect(30 + 6 * 5, 134, 3, 5, TFT_BLUE);  //poz==5

  for (int i = 0; i < 10; i++)
    if (i == poz)
      sprite.fillRect(30 + (i * 6), 142, 3, 6, TFT_RED);
    else
      sprite.fillRect(30 + (i * 6), 142, 3, 6, grays[6]);

  StickCP2.Display.pushImage(0, 0, 135, 240, (uint16_t*)sprite.getPointer());
}

void drawSet() {
  sprite.fillSprite(TFT_BLACK);
  sprite.setTextDatum(4);
  sprite.fillSmoothRoundRect(48, 214, 76, 18, 2, blue2, TFT_BLACK);
  sprite.fillSmoothRoundRect(6, 214, 34, 18, 2, TFT_ORANGE, TFT_BLACK);
  sprite.fillSmoothRoundRect(91, 190, 34, 18, 2, TFT_ORANGE, TFT_BLACK);

  sprite.loadFont(Noto);
  sprite.setTextColor(TFT_BLACK, TFT_ORANGE);
  sprite.drawString(F("OK"), 22, 225);
  sprite.drawString(F("CNG"), 108, 200);
  sprite.setTextColor(grays[1], blue2);
  sprite.drawString(F("SET"), 87, 225);
  sprite.unloadFont();

  sprite.loadFont(smallFont);
  sprite.setTextColor(TFT_ORANGE, TFT_BLACK);
  sprite.drawString(F("SETUP"), 28, 18);
  sprite.unloadFont();

  for (int i = 0; i < 8; i++) {
    if (chosen == i) tmpColor = grays[5];
    else tmpColor = grays[9];

    if (i < 6) {
      sprite.fillRect(setPosX[i], setPosY[i], 36, 40, tmpColor);
      sprite.loadFont(smallFont);
      sprite.setTextColor(0x35D7, TFT_BLACK);
      sprite.drawString(setDataLbl[i], setPosX[i] + 18, setPosY[i] - 12);
      sprite.unloadFont();

      sprite.loadFont(secFont);
      sprite.setTextColor(grays[1], tmpColor);
      sprite.drawString(String(setData[i]), setPosX[i] + 17, setPosY[i] + 22);
      sprite.unloadFont();
    } else {
      sprite.setTextDatum(0);
      sprite.fillRect(setPosX[i], setPosY[i], 76, 16, tmpColor);
      sprite.setTextColor(grays[1], tmpColor);
      sprite.drawString(setDataLbl[i], setPosX[i] + 4, setPosY[i] + 5);
      if (i == 6)
        if (setData[i] == 0)
          sprite.drawString(F("EU"), setPosX[i] + 50, setPosY[i] + 5);
        else
          sprite.drawString(F("NA"), setPosX[i] + 50, setPosY[i] + 5);

      if (i == 7)
        if (setData[i] == 0)
          sprite.drawString(F("OFF"), setPosX[i] + 50, setPosY[i] + 5);
        else
          sprite.drawString(F("ON"), setPosX[i] + 50, setPosY[i] + 5);
    }
  }

  StickCP2.Display.pushImage(0, 0, 135, 240, (uint16_t*)sprite.getPointer());
}


void loop()  //##########################  LOOP  ##################################
{
  StickCP2.update();
  if (!WiFi.isConnected())
    isSetup = false;

  if (delay_time == 0 || millis() - delay_time >= 5000) {
    auto get_temp = StickCP2.Imu.getTemp(&imu_temp);
    delay_time = millis();
  }
  //if ((_delay == 0 || millis() - _delay >= (10000 / (max_count + 1))) && (fase > 0)) {
  if ((_delay == 0 || millis() - _delay >= (10000 / (max_count + 1))) && (fase == 0)) {
    _count++;
    if (_count > max_count) _count = 0;

    _delay = millis();
  }
  auto now = StickCP2.Rtc.getDateTime();
  auto imu_update = StickCP2.Imu.update();
  auto data = StickCP2.Imu.getImuData();
  poz = map(data.accel.y * 100, -30, 100, 0, 10);
  //dow_int = rtc.getDayofWeek();
  dow_int = getDayOfWeek(now.date.year - 2000, now.date.month, now.date.date);
  charge = M5.Power.isCharging();
  /*
  switch (dow_int) {
    case 0: dow = "SUN";
    case 1: dow = "MON";
    case 2: dow = "TUE";
    case 3: dow = "WED";
    case 4: dow = "THU";
    case 5: dow = "FRI";
    case 6: dow = "SAT";
    default: Serial.printf(F("[%lu] E: cannot read the days of week\n"), millis());
  }
  */
  dow = dow_str[dow_int];
  if (SerialBT.available()) {
    String recv = SerialBT.readStringUntil('\n');
    int cma_i = recv.indexOf(",");
    String ssid, pwd;
    if (cma_i != -1) {
      //set ssid and password
      ssid = recv.substring(0, cma_i);
      pwd = recv.substring(cma_i + 1);
      SerialBT.printf("Received: %s, %s\n", ssid, pwd);
      Serial.printf("Received: %s, %s\n", ssid, pwd);

    } else {
      //set to default
      //enter your wifi ssid and password
      ssid = "Minh Thu";
      pwd = "0903746091";
    }
    if (!isWiFistarted) {
      //start wifi
      uint8_t timeout = 0;
      WiFi.begin(ssid, pwd);
      Serial.printf("Connecting to Wi-Fi: %s, pwd=%s", ssid, pwd);
      SerialBT.printf("Connecting to Wi-Fi: %s, pwd=%s", ssid, pwd);
      while (WiFi.status() != WL_CONNECTED && timeout <= 30) {
        Serial.print(F("."));
        if (SerialBT.available()) SerialBT.print(F("."));
        if (digitalRead(35) == 0) break;
        timeout++;
        delay(1000);
      }
      Serial.println();
      SerialBT.println();
      if (WiFi.isConnected()) {
        Serial.println(F("WiFi connected!"));
        SerialBT.println(F("WiFi connected!"));
        isWiFistarted = true;
      } else {
        Serial.printf("WiFi is not connected [%s]\n", String(WiFi.status()));
        SerialBT.printf("WiFi is not connected [%s]\n", String(WiFi.status()));
        isWiFistarted = false;
      }
    }
  } else {
    if (Serial.available()) {
      String recv = Serial.readStringUntil('\n');
      String ssid, pwd;
      uint8_t cma_i = recv.indexOf(",");
      if (cma_i != -1) {
        ssid = recv.substring(0, cma_i);
        pwd = recv.substring(cma_i + 1);
        Serial.printf("Received: %s, %s\n", ssid, pwd);

      } else {
        //default ssid and pwd
        //enter your wifi ssid and password
        ssid = "Minh Thu";
        pwd = "0903746091";
      }
    }
  }
  if (!inGPS) {
    if (digitalRead(35) == 0 && poz != 1 && poz != 5 && !setTimeDate) {
      inGPS = true;
    }
    while (digitalRead(35) == 0 && poz != 1 && poz != 5 && !setTimeDate) {
      yield();
    }
  }
  if (inGPS) {
    if (true) {
      GPSRaw.begin(9600, SERIAL_8N1, 33, 32);
    }
    while (inGPS) {
      drawGPS();
      if (isSetup)
        server.handleClient();
      sleepTime = 10;
      if (digitalRead(35) == 0 && poz != 1 && poz != 5 && !setTimeDate) {
        inGPS = false;
        GPSRaw.end();
        while (digitalRead(35) == 0 && poz != 1 && poz != 5 && !setTimeDate) {
          yield();
        }
        break;
      }
      delay(10);
    }
  }
  if (digitalRead(39) == 0 && poz == 5 && fase == 0) {
    if (true) {
      StickCP2.Display.fillScreen(BLACK);
      AudioComm_begin();
    }
    while (true) {
      AudioComm_update();
      sleepTime = 10;
      if (digitalRead(35) == 0) break;
    }
    LoRaSerial.end();
    while (digitalRead(35) == 0) {
      yield();
    }
  }
  // boot.jpg image
  if (digitalRead(35) == 0 && poz == 1 && !inGPS && !setTimeDate && fase != 2) {
    StickCP2.Display.setRotation(3);
    StickCP2.Display.fillScreen(BLACK);
    File boot = LITTLEFS.open("/boot.jpg", "r");
    if (!boot || boot.isDirectory()) {
      Serial.println(F("[BOOT FILE] E: cannot open boot.jpg as file"));
      StickCP2.Display.println(F("[BOOT FILE] E: cannot open boot.jpg as file"));
    } else {
      size_t jpg_size = boot.size();
      uint8_t* jpg_buf = (uint8_t*)malloc(jpg_size);
      if (!jpg_buf) {
        Serial.println(F("[BOOT FILE] E: cannot get the buf of boot.jpg"));
        StickCP2.Display.println(F("[BOOT FILE] E: cannot get the buf of boot.jpg"));
        boot.close();
        while (digitalRead(37) != 0 || digitalRead(39) != 0) {
          if (digitalRead(37) == 0 || digitalRead(39) == 0) {
            break;
          }
          if (isSetup)
            server.handleClient();
          yield();
        }
        sleepTime = 10;
        delay(1000);
        StickCP2.Display.setRotation(0);
      } else {
        boot.read(jpg_buf, jpg_size);
        boot.close();
        StickCP2.Display.drawJpg(jpg_buf, jpg_size, 0, 0);
        free(jpg_buf);

        while (digitalRead(37) != 0 || digitalRead(39) != 0) {
          if (digitalRead(37) == 0 || digitalRead(39) == 0) {
            break;
          }
          if (isSetup)
            server.handleClient();
          yield();
        }
        sleepTime = 10;
        delay(1000);
        StickCP2.Display.setRotation(0);
      }
    }
  }
  if (digitalRead(35) == 0 && fase == 0 && poz == 5) {
    bool needRedraw = true;
    while (digitalRead(37) == 0) {
      yield();
    }
    if (WiFi.isConnected()) {
      StickCP2.Display.setRotation(3);
      StickCP2.Display.fillScreen(BLACK);
      StickCP2.Display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GREEN);
      StickCP2.Display.drawRect(1, 1, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, GREEN);
      StickCP2.Display.setTextColor(WHITE, BLACK);
      StickCP2.Display.setTextSize(2);
      StickCP2.Display.setCursor(12, 30);
      StickCP2.Display.println(F("Checking\n Telegram API..."));
      bot_status_t bot_out;
      if (httpCode == HTTP_CODE_OK && check_api_conn(scr_cli, bot_out) == BOT_CONNECTED) {
        inputText = "";
        StickCP2.Display.fillScreen(BLACK);
        if (CHAT_ID != "")
          bot.sendMessage(CHAT_ID, "Bot is on", "");
        while (true) {
          StickCP2.update();
          //bot.sendChatAction(CHAT_ID, "typing");
          StickCP2.Display.setRotation(3);
          read_message();
          //if (bot.getUpdates(bot.last_message_received + 1) <= 0) break;
          if (needRedraw) {
            StickCP2.Display.fillScreen(BLACK);
            StickCP2.Display.setRotation(3);
            drawKeyboard("[" + FROM + "]:" + MSG, 1);
            needRedraw = false;
          }
          // StickCP2.Display.setRotation(3);
          // drawKeyboard("[" + FROM + "]:" + MSG, 1);
          if (StickCP2.BtnA.wasPressed()) {
            String key = keys[row][col];

            if (row == 0 && upper && col < 11) {
              inputText += shiftedNums[col];
            } else if (row == 3 && upper && col >= 7) {
              inputText += shiftedRow4[col - 7];
            } else if (row == 3 && upper && col < 7) {
              key = key.substring(0, 1);
              key.toUpperCase();
              inputText += key;
            } else if (row == 2 && upper && col >= 9) {
              inputText += shiftedRow3[col - 9];
            } else if (row == 2 && upper && col < 9) {
              key = key.substring(0, 1);
              key.toUpperCase();
              inputText += key;
            } else if (row > 0 && row < 4) {
              if (upper) {
                key = key.substring(0, 1);
                key.toUpperCase();
                inputText += key;
              } else {
                inputText += String(key);
              }
            } else if (row == 4) {
              if (key == "CAP") {
                upper = !upper;
              } else if (key == "DEL") {
                if (inputText.length() > 0) inputText.remove(inputText.length() - 1);
              } else if (key == "OK") {
                if (inputText.length() > 0 && CHAT_ID != "") {
                  bot.sendMessage(CHAT_ID, inputText, "");
                  Serial.println("[Đã gửi]: " + inputText);
                } else {
                  Serial.println(F("[Lỗi]: Không có CHAT_ID hoặc nội dung trống."));
                }
              } else if (key == "BCK") {
                break;
              } else if (key == "LST") {
                break;
              } else if (key == "RST") {
                inputText = "";
              } else if (key == " ") {
                inputText += " ";
              }
            } else if (row == 0) {
              inputText += String(key);
            }
            needRedraw = true;
            drawKeyboard("[" + FROM + "]:" + MSG, 1);
            if (buzzer)
              StickCP2.Speaker.tone(6000, 100);
          }
          if (M5.BtnB.wasPressed()) {
            needRedraw = true;
            moveHorizontal(1);
            drawKeyboard("[" + FROM + "]:" + MSG, 1);
          } else if (M5.BtnB.pressedFor(500)) {
            needRedraw = true;
            moveHorizontal(-1);
            drawKeyboard("[" + FROM + "]:" + MSG, 1);
            delay(300);
          }
          bool btnC = digitalRead(BTN_C_PIN) == LOW;
          if (btnC && btnCTimer == 0) {
            btnCTimer = millis();
          }
          if (!btnC && btnCTimer > 0) {
            needRedraw = true;
            unsigned long pressTime = millis() - btnCTimer;
            if (pressTime < 500) {
              needRedraw = true;
              moveVertical(1);
            } else {
              needRedraw = true;
              moveVertical(-1);
            }
            btnCTimer = 0;
            btnCHolding = false;
            drawKeyboard("[" + FROM + "]:" + MSG, 1);
          }
          delay(100);
        }

        while (digitalRead(37) == 0) {
          yield();
        }
        StickCP2.Display.setRotation(0);
      } else {
        StickCP2.Display.setRotation(3);
        StickCP2.Display.fillScreen(BLACK);
        StickCP2.Display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GREEN);
        StickCP2.Display.drawRect(1, 1, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, GREEN);
        StickCP2.Display.setTextColor(WHITE, BLACK);
        StickCP2.Display.setTextSize(2);
        StickCP2.Display.setCursor(12, 30);
        StickCP2.Display.println(F("Cannot access to\n Telegram API"));
        StickCP2.Display.setTextSize(1);

        StickCP2.Display.setCursor(12, StickCP2.Display.getCursorY());
        StickCP2.Display.setTextColor(RED, BLACK);
        if (bot_out == BOT_NOT_CONNECTED)
          StickCP2.Display.println(F("[Bot is not connected]"));
        else if (bot_out == BOT_CONNECT_TIMEOUT)
          StickCP2.Display.println(F("[Bot connect timeout]"));
        StickCP2.Display.setTextColor(WHITE, BLACK);
        StickCP2.Display.setCursor(12, StickCP2.Display.getCursorY());
        StickCP2.Display.println(F("Click any button to exit"));
        while (true) {
          if (digitalRead(37) == 0 || digitalRead(39) == 0) break;
          delay(100);
        }
        StickCP2.Display.setRotation(0);
      }
    }
    while (digitalRead(37) == 0 || digitalRead(39) == 0) {
      // if (digitalRead(37) == 0) break;
      yield();
    }
    sleepTime = 10;
    StickCP2.Display.setRotation(0);
  }
  if (digitalRead(37) == 0 && fase == 1 && poz == 5) {
    while (digitalRead(37) == 0) {
      yield();
    }
    StickCP2.Display.fillScreen(BLACK);
    apple_juice();
    while (digitalRead(35) == 0) {
      yield();
    }
    StickCP2.Display.setRotation(0);
    digitalWrite(19, LOW);
  }
  if (isAPstarted || WiFi.isConnected()) {
    server.handleClient();
  }
  if (setTimeDate == 0) {

    vol = StickCP2.Power.getBatteryVoltage();
    bat = StickCP2.Power.getBatteryLevel();
    //volE = map(bat, 3000, 4180, 0, 5);
    volE = map(bat, 0, 100, 0, 5);
    auto dt = StickCP2.Rtc.getDateTime();

    if (digitalRead(35) == 0 && poz == 1 && fase == 2) {
      setTimeDate = !setTimeDate;
      lastPressTime = millis();
      setData[0] = dt.time.hours;
      setData[1] = dt.time.minutes;
      setData[2] = dt.time.seconds;
      setData[3] = dt.date.date;
      setData[4] = dt.date.month;
      setData[5] = dt.date.year - 2000;
      setData[6] = Myregion;
      setData[7] = buzzer;

      if (buzzer)
        StickCP2.Speaker.tone(6000, 100);
      delay(200);
    } else if (digitalRead(35) == 0 && poz != 1 && poz != 5) {
    }


    if (slp) {
      StickCP2.Display.setBrightness((StickCP2.Power.getBatteryLevel() <= 15) ? 8 : brightnes[b]);
      slp = false;
      fase = -1;
      sleepTime = 10;
    }

    if (fase == 1) {

      if (rtc.getSecond() < 10) secc = "0" + String(rtc.getSecond());
      else secc = String(rtc.getSecond());
      if (rtc.getMinute() < 10) minn = "0" + String(rtc.getMinute());
      else minn = String(rtc.getMinute());
      if (rtc.getMillis() / 10 < 10) mill = "0" + String(rtc.getMillis() / 10);
      else mill = String(rtc.getMillis() / 10);
    }



    if (StickCP2.BtnB.wasPressed() && poz != 1 && poz != 5) {

      lastPressTime = millis();
      if (slp) {
        StickCP2.Display.wakeup();
        StickCP2.Display.setBrightness((StickCP2.Power.getBatteryLevel() <= 15) ? 8 : brightnes[b]);
        isInMenu = true;
        slp = false;
      }
      sleepTime = 10;
      b++;
      if (b == 6) b = 0;
      EEPROM.write(0, b);
      EEPROM.commit();
      StickCP2.Display.setBrightness((StickCP2.Power.getBatteryLevel() <= 15) ? 8 : brightnes[b]);
      if (buzzer)
        StickCP2.Speaker.tone(6000, 100);
    }

    if (StickCP2.BtnA.wasPressed()) {
      lastPressTime = millis();
      if (slp) {
        StickCP2.Display.wakeup();
        StickCP2.Display.setBrightness((StickCP2.Power.getBatteryLevel() <= 15) ? 8 : brightnes[b]);
        slp = false;
        fase = 0;
      }
      sleepTime = 10;
      if (fase == 0 && poz == 5) {
        if (WiFi.isConnected()) {
          if (_count >= 0) {
            fase = 0;
            displayMenu();
          }
        } else {
          if (_count > 0) {
            fase = 0;
            displayMenu();
          }
        }
      }
      if (fase == 0 && poz == 1) {
        // String code;
        //         if (Serial.available()) {
        //           code = Serial.readStringUntil('\n');
        //           int code1 = code.toInt();
        // #ifdef IRLED
        // #undef IRLED
        // #endif
        // #define IRLED code1
        //           Serial.printf("IR GPIO: %d\n", code1);
        //         } else {
        // #ifdef IRLED
        // #undef IRLED
        // #endif
        // #define IRLED 19  //default
        //         }
        sendAllCodes();
      } else if (fase == 1 && poz == 1) {
        if (!isActivateBT) {
          SerialBT.begin(BT_ID);
          isSetup = false;
          isActivateBT = true;
        } else {
          SerialBT.end();
          isActivateBT = false;
        }
        if (isAPstarted) {
          WiFi.softAPdisconnect();
          isAPstarted = false;
        }
      } else if (fase == 2 && poz == 1) {
        if (!isAPstarted) {
          WiFi.softAP(WIFI_SSID, WIFI_PWD);
          if (!isSetup) {
            setupWebServer();
            isSetup = true;
          }
          isAPstarted = true;
        } else {
          WiFi.softAPdisconnect();
          isAPstarted = false;
        }
        if (isActivateBT) {
          SerialBT.end();
          isActivateBT = false;
        }


      } else if (fase == 2 && poz == 5) {
        playSong();
      } else {
        if (!WiFi.isConnected()) {
          if (_count == 0) {
            fase++;

            if (fase == 3) fase = 0;
            if (fase == 1) {
              _delay = millis();
              rtc.setTime(0, 0, 0, 1, 1, 2023, 0);
            }
            if (fase == 0) {
              minn = "00";
              secc = "00";
              mill = "00";
            }
          } else {
            fase = 0;
          }
        } else {
          fase = 0;
        }

        if (buzzer)
          StickCP2.Speaker.tone(6000, 100);
      }
    }

    if (dt.time.seconds < 10) s = "0" + String(dt.time.seconds);
    else s = String(dt.time.seconds);
    if (dt.time.minutes < 10) m = "0" + String(dt.time.minutes);
    else m = String(dt.time.minutes);

    if (Myregion == 1) {

      if (dt.time.hours > 12) {
        if (dt.time.hours < 10) h = "0" + String(dt.time.hours);
        else h = String(dt.time.hours);
      } else {
        if (dt.time.hours < 10) h = "0" + String(dt.time.hours);
        else h = String(dt.time.hours);
      }
    }

    if (Myregion == 0)
      if (dt.time.hours < 10) h = "0" + String(dt.time.hours);
      else h = String(dt.time.hours);

    day = dt.date.date;
    month = dt.date.month;

    ts = dt.time.seconds;
    if (tts != ts && fase == 0) {
      sleepTime--;
      tts = ts;
    }

    if (sleepTime == 0 && fase != 1) {
      slp = true;
      StickCP2.Display.setBrightness(0);
      //StickCP2.Display.sleep();
      delay(20);

      //esp_sleep_enable_ext1_wakeup((1ULL << 37) | (1ULL << 39), ESP_EXT1_WAKEUP_ANY_HIGH);
      esp_sleep_enable_ext0_wakeup(GPIO_NUM_37, LOW);
      esp_light_sleep_start();
      //if (millis() - lastPressTime > 60000)
      //esp_deep_sleep_start();
      //if (millis() - lastPressTime > 30000)
      //StickCP2.Power.lightSleep();
      /*
if (StickCP2.BtnA.wasPressed()) {
  sleepTime = 10;
  slp=false;
  fase=0;
}
*/
    }

    //if (!isInMenu)
    drawMain();
    //else displayMenu();
  }

  if (setTimeDate) {
    if (StickCP2.BtnB.wasPressed()) {
      lastPressTime = millis();

      chosen++;
      if (chosen == 8) chosen = 0;
      if (buzzer)
        StickCP2.Speaker.tone(6000, 100);
    } else if (StickCP2.BtnB.pressedFor(500)) {
      lastPressTime = millis();
      chosen--;
      if (chosen < 0) chosen = 7;
      if (buzzer)
        StickCP2.Speaker.tone(6000, 100);
    }

    if (StickCP2.BtnA.wasPressed()) {
      lastPressTime = millis();

      setData[chosen]++;
      if (setData[chosen] == setMax[chosen])
        setData[chosen] = setMin[chosen];
      if (buzzer)
        StickCP2.Speaker.tone(6000, 100);
    } else if (StickCP2.BtnA.pressedFor(500)) {
      lastPressTime = millis();

      setData[chosen]--;
      if (setData[chosen] < setMin[chosen])
        setData[chosen] = setMax[chosen] - 1;
      if (buzzer)
        StickCP2.Speaker.tone(6000, 100);
    }

    if (digitalRead(35) == 0) {
      lastPressTime = millis();
      buzzer = setData[7];
      Myregion = setData[6];
      EEPROM.write(1, Myregion);
      EEPROM.write(2, buzzer);
      EEPROM.commit();
      sleepTime = 10;
      setTimeDate = !setTimeDate;

      StickCP2.Rtc.setDateTime({ { setData[5] + 2000, setData[4], setData[3] }, { setData[0], setData[1], setData[2] } });

      if (buzzer)
        StickCP2.Speaker.tone(6000, 100);
      delay(300);
    }

    drawSet();
  }
}

void displayMenu() {
  const int SCREEN_WIDTH = 135;
  const int SCREEN_HEIGHT = 240;
  StickCP2.Display.fillScreen(BLACK);
  /*sprite.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT);
  sprite.fillSprite(TFT_BLACK);
  sprite.setTextDatum(4);  // Center
  sprite.setTextSize(2);
  */
  //StickCP2.Display.setTextDatum(4);
  //StickCP2.Display.setTextSize(1);

  //sprite.drawString(F("Scanning..."), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  bool isInWiFi = true;
  int curr_wifi = 0;
  auto dt = StickCP2.Rtc.getDateTime();
  String hours, minutes;
  if (dt.time.hours < 10) hours = "0" + String(dt.time.hours);
  else hours = String(dt.time.hours);
  if (dt.time.minutes < 10) minutes = "0" + String(dt.time.minutes);
  else minutes = String(dt.time.minutes);
  unsigned long lpt = millis();
  StickCP2.Display.setTextDatum(0);
  StickCP2.Display.setCursor(16, 8);
  StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLUE);
  StickCP2.Display.fillRect(0, 0, SCREEN_HEIGHT, 20, TFT_BLUE);
  StickCP2.Display.setTextSize(1);
  StickCP2.Display.println(String(hours) + ":" + String(minutes));
  StickCP2.Display.setCursor(100, 8);
  StickCP2.Display.println(String(StickCP2.Power.getBatteryLevel()) + "%");
  StickCP2.Display.setCursor(5, 30);
  StickCP2.Display.setTextSize(2);
  StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLACK);

  StickCP2.Display.println(F("Wi-Fi"));
  StickCP2.Display.setTextSize(1);

  static unsigned long _prstart_a = 0;
  goto _wifi;

//sprite.pushSprite(0, 0);
_wifi:

  WiFi.mode(WIFI_AP_STA);
  if (WiFi.isConnected()) {
    WiFi.disconnect();  // Clean start
    StickCP2.Display.setTextColor(TFT_GREEN);
    StickCP2.Display.setCursor((SCREEN_WIDTH - 6 * strlen("\n\nDisconnected from Wi-Fi")) / 2, StickCP2.Display.getCursorY());
    StickCP2.Display.println(F("\n\nDisconnected from Wi-Fi"));
    StickCP2.Display.setTextColor(TFT_WHITE);
    StickCP2.Display.println(F("Click [NEXT] to continue scan\nClick [BACK] to go back"));
    while (digitalRead(39) != 0 || digitalRead(35) != 0) {
      if (digitalRead(39) == 0) {
        isInWiFi = true;
        break;
      } else if (digitalRead(35) == 0) {
        isInWiFi = false;
        break;
      }
      delay(10);
    }
  }
  //delay(100);
  if (isInWiFi) {
    StickCP2.Display.fillScreen(BLACK);
    StickCP2.Display.setTextDatum(0);
    StickCP2.Display.setCursor(16, 8);
    StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLUE);
    StickCP2.Display.fillRect(0, 0, SCREEN_HEIGHT, 20, TFT_BLUE);
    StickCP2.Display.setTextSize(1);
    StickCP2.Display.println(String(hours) + ":" + String(minutes));
    StickCP2.Display.setCursor(100, 8);
    StickCP2.Display.println(String(StickCP2.Power.getBatteryLevel()) + "%");
    StickCP2.Display.setCursor(5, 30);
    StickCP2.Display.setTextSize(2);
    StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLACK);

    StickCP2.Display.println(F("Wi-Fi"));
    StickCP2.Display.setTextSize(1);
    StickCP2.Display.setCursor((SCREEN_WIDTH - 6 * strlen("Scanning...")) / 2, 50);
    StickCP2.Display.println(F("Scanning..."));
    bool nrdraw = true;
    bool inInfo = false;
    bool close = false;
    int n = WiFi.scanNetworks(false, true);
    goto _main_wifi;
_main_wifi:
    while (isInWiFi) {
      goto _display_wifi;
_display_wifi:
      auto dt = StickCP2.Rtc.getDateTime();
      selected_wifi = WiFi.SSID(curr_wifi);
      StickCP2.update();
      if (dt.time.hours < 10) hours = "0" + String(dt.time.hours);
      else hours = String(dt.time.hours);
      if (dt.time.minutes < 10) minutes = "0" + String(dt.time.minutes);
      else minutes = String(dt.time.minutes);
      //StickCP2.Display.fillScreen(BLACK);
      //sprite.fillSprite(TFT_BLACK);
      //sprite.setTextDatum(0);  // Top-left

      if (nrdraw) {
        StickCP2.Display.fillScreen(BLACK);
        StickCP2.Display.setTextDatum(0);
        StickCP2.Display.setCursor(16, 8);
        StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLUE);
        StickCP2.Display.fillRect(0, 0, SCREEN_HEIGHT, 20, TFT_BLUE);
        StickCP2.Display.setTextSize(1);
        StickCP2.Display.println(String(hours) + ":" + String(minutes));
        StickCP2.Display.setCursor(100, 8);
        StickCP2.Display.println(String(StickCP2.Power.getBatteryLevel()) + "%");
        StickCP2.Display.setCursor(5, 30);

        StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLACK);
        //StickCP2.Display.setTextSize(2);

        //StickCP2.Display.println(F("Wi-Fi"));
        StickCP2.Display.setTextSize(1);
        if (n > 0) {
          //grammar fix
          if (n == 1) {
            StickCP2.Display.setCursor((SCREEN_WIDTH - 6 * String(String(n) + " network found").length()) / 2, 25);
            StickCP2.Display.println(String(n) + " network found");
          } else {
            StickCP2.Display.setCursor((SCREEN_WIDTH - 6 * String(String(n) + " networks found").length()) / 2, 25);
            StickCP2.Display.println(String(n) + " networks found");
          }
          int start = (curr_wifi / 15) * 15;
          for (int i = 0; i < 15 && (start + i) < n; i++) {
            //StickCP2.update();  // Needed to catch button presses
            int index = start + i;
            String ssid = WiFi.SSID(index);
            if (ssid.isEmpty()) ssid = "[Hidden network]";
            int y1 = 40 + i * 12;
            //if (String(ssid + " ("+String(WiFi.RSSI(index)) + "dBm)").length()+5>=20)
            //y1 = 20 + i * 12;
            //else y1 = 35 + i * 12;
            if (WiFi.encryptionType(index) == WIFI_AUTH_OPEN)
              StickCP2.Display.fillRect(0, y1 - 2, SCREEN_WIDTH, 12, TFT_GREEN);
            else
              StickCP2.Display.fillRect(0, y1 - 2, SCREEN_WIDTH, 12, grays[6]);
            if (index == curr_wifi) {
              //sprite.drawRect(0, y - 2, SCREEN_WIDTH, 12, TFT_WHITE);
              StickCP2.Display.drawRect(0, y1 - 2, SCREEN_WIDTH, 12, TFT_WHITE);

            } else {
              //sprite.drawRect(0, y - 2, SCREEN_WIDTH, 12, TFT_BLACK);
              StickCP2.Display.drawRect(0, y1 - 2, SCREEN_WIDTH, 12, TFT_BLACK);
            }

            //sprite.drawString(String(index) + ". " + ssid, 10, y);
            /*
          if (WiFi.RSSI(index) <= -90) {
            if (WiFi.encryptionType(index) != WIFI_AUTH_OPEN)
              StickCP2.Display.setTextColor(TFT_RED, grays[6]);
            else
              StickCP2.Display.setTextColor(TFT_RED, TFT_GREEN);
          } else {*/
            if (WiFi.encryptionType(index) != WIFI_AUTH_OPEN)
              StickCP2.Display.setTextColor(TFT_WHITE, grays[6]);
            else
              StickCP2.Display.setTextColor(TFT_BLACK, TFT_GREEN);
            //}
            StickCP2.Display.setCursor(3, y1);
            if (ssid.length() > 12) {
              /*
          unsigned long _p = millis();
          for (uint8_t j = 0; j < selected_wifi.length(); j++) {
            StickCP2.Display.println(selected_wifi.substring(j, j+14)+ "(" + String(WiFi.RSSI(index)) + "dBm)");
            if (millis()-_p>1000) {
              _p = millis();
              continue;
            }
          }
          */
              StickCP2.Display.println(ssid.substring(0, 11) + "...(" + String(WiFi.RSSI(index)) + "dBm)");
            } else
              StickCP2.Display.println(ssid + "(" + String(WiFi.RSSI(index)) + "dBm)");
          }

        } else {
          //sprite.drawString(F("No networks found"), 10, 20);
          StickCP2.Display.setCursor(5, 40);
          StickCP2.Display.println("No networks found (" + String(n) + ")");
          if (isActivateBT) {
            StickCP2.Display.println(F("Turning off Bluetooth..."));
            SerialBT.end();
            isActivateBT = false;
          }
          StickCP2.Display.println(F("\n\nRestarting..."));
          delay(2000);
          isInWiFi = false;
          ESP.restart();
        }
        nrdraw = false;
      }
      if (millis() - lpt > 30000) {
        StickCP2.Display.fillScreen(BLACK);
        StickCP2.Display.setTextDatum(0);
        StickCP2.Display.setCursor(16, 8);
        StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLUE);
        StickCP2.Display.fillRect(0, 0, SCREEN_HEIGHT, 20, TFT_BLUE);
        StickCP2.Display.setTextSize(1);
        StickCP2.Display.println(String(hours) + ":" + String(minutes));
        StickCP2.Display.setCursor(100, 8);
        StickCP2.Display.println(String(StickCP2.Power.getBatteryLevel()) + "%");
        StickCP2.Display.setCursor(5, 30);
        StickCP2.Display.setTextSize(2);
        StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLACK);

        StickCP2.Display.println(F("Wi-Fi"));
        StickCP2.Display.setTextSize(1);
        StickCP2.Display.setCursor(5, 50);
        StickCP2.Display.println(F("Scanning...\n\n* It will scan after 30 seconds without any action"));
        curr_wifi = 0;
        lpt = millis();
        goto _wifi;
      }

      // Button A: exit
      //sprite.pushSprite(0, 0);


      if (StickCP2.BtnA.wasPressed()) {
        sleepTime = 10;
        if (_prstart_a == 0) _prstart_a = millis();
        lpt = millis();
        if (n > 0) {

          inInfo = true;
          bool _needRedraw = true;

          while (inInfo) {
            if (_needRedraw) {
              StickCP2.Display.drawRect(0, 50, SCREEN_WIDTH, 135, TFT_WHITE);
              StickCP2.Display.fillRect(0, 50, SCREEN_WIDTH, 135, TFT_BLACK);
              StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLACK);
              StickCP2.Display.setCursor(6, 60);
              StickCP2.Display.println("SSID: " + WiFi.SSID(curr_wifi) + "\n RSSI: " + String(WiFi.RSSI(curr_wifi)) + " dBm\n Channel:" + String(WiFi.channel(curr_wifi)) + ", " + String((WiFi.channel(curr_wifi) > 13) ? "5GHz" : "2.4GHz") + "\n (" + WiFi.BSSIDstr(curr_wifi) + ")\n [" + getEncryptionType(WiFi.encryptionType(curr_wifi)) + "]");
              if (WiFi.RSSI(curr_wifi) > -100 && WiFi.RSSI(curr_wifi) < -90) {
                StickCP2.Display.setTextColor(0x8800, TFT_BLACK);
                StickCP2.Display.println(F(" [Very weak]"));
              } else if (WiFi.RSSI(curr_wifi) >= -90 && WiFi.RSSI(curr_wifi) < -80) {
                StickCP2.Display.setTextColor(TFT_RED, TFT_BLACK);
                StickCP2.Display.println(F(" [Weak]"));
              } else if (WiFi.RSSI(curr_wifi) >= -80 && WiFi.RSSI(curr_wifi) < -60) {
                StickCP2.Display.setTextColor(TFT_YELLOW, TFT_BLACK);
                StickCP2.Display.println(F(" [Fair]"));
              } else if (WiFi.RSSI(curr_wifi) >= -60 && WiFi.RSSI(curr_wifi) < -40) {
                StickCP2.Display.setTextColor(TFT_GREEN, TFT_BLACK);
                StickCP2.Display.println(F(" [Good]"));
              } else if (WiFi.RSSI(curr_wifi) >= -40) {
                StickCP2.Display.setTextColor(0x0FC1, TFT_BLACK);
                StickCP2.Display.println(F("[Excellent]"));
              } else {
                StickCP2.Display.setTextColor(0x4A49, TFT_BLACK);
                StickCP2.Display.println(F("[No signal]"));
              }
              StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLACK);
              StickCP2.Display.println(F("\n\n Click [BACK] to close\n Click [NEXT] to go next"));
              _needRedraw = false;
            }
            if (digitalRead(39) == 0) {
              close = false;
              inInfo = false;
            }
            if (digitalRead(35) == 0) {
              close = true;
              inInfo = false;
            }
            while (digitalRead(39) == 0 || digitalRead(35) == 0) {
              //yield();
              delay(1);
            }
            /*
            if (digitalRead(39) == 0) {
              inInfo = false;
              isInWiFi = true;
              goto _main_wifi;
            }
            */
            delay(1);
          }
          if (!close) {
            uint8_t timeout = 0;
            if (WiFi.encryptionType(curr_wifi) == WIFI_AUTH_OPEN) {
              WiFi.begin(selected_wifi);
              StickCP2.Display.drawRect(0, 50, SCREEN_WIDTH, 135, TFT_WHITE);
              StickCP2.Display.fillRect(0, 50, SCREEN_WIDTH, 135, TFT_BLACK);
              StickCP2.Display.setTextColor(TFT_WHITE);

              StickCP2.Display.setCursor(10, 62);
              StickCP2.Display.print("Connecting to " + selected_wifi);

              while (WiFi.status() != WL_CONNECTED && timeout <= 30) {
                StickCP2.Display.print(F("."));
                //if (StickCP2.BtnA.wasPressed()) break;
                if (digitalRead(35) == 0) break;
                delay(1000);
                timeout++;
              }
              StickCP2.Display.println();
              if (WiFi.isConnected()) {
                StickCP2.Display.drawRect(0, 50, SCREEN_WIDTH, 135, TFT_WHITE);
                StickCP2.Display.fillRect(0, 50, SCREEN_WIDTH, 135, TFT_BLACK);
                StickCP2.Display.setTextColor(TFT_GREEN);
                StickCP2.Display.setCursor(10, 62);
                StickCP2.Display.println("Connected to Wi-Fi: " + selected_wifi);
                //if (WiFi.isConnected())
                if (!isSetup) {
                  setupWebServer();
                  isSetup = true;
                }
                scr_cli.setCACert(TELEGRAM_CERTIFICATE_ROOT);

                delay(1000);
                //WiFi.scanDelete();
                isInWiFi = false;
                //break;
              } else {
                StickCP2.Display.drawRect(0, 50, SCREEN_WIDTH, 135, TFT_WHITE);
                StickCP2.Display.fillRect(0, 50, SCREEN_WIDTH, 135, TFT_BLACK);
                StickCP2.Display.setTextColor(TFT_RED);
                StickCP2.Display.setCursor(10, 62);

                StickCP2.Display.println("Failed to connect to Wi-Fi: " + selected_wifi + "(" + WiFi.status() + "," + getStatus(WiFi.status()) + ")");
                delay(1000);
                //WiFi.scanDelete();
                isInWiFi = false;
                //break;
              }
            } else {
              bool needRedraw = true;
              /*
            if (findCredentialBySSID(selected_wifi, inputText, ssid_index)) {
              Serial.printf(F("[%lu] [Wi-Fi] I: Found credential of the SSID: %s as: index %d, password: %s\n"), millis(), selected_wifi, ssid_index, inputText);
            } else {
              inputText = "";
              Serial.printf(F("[%lu] [Wi-Fi] E: Cannot find the credential of the SSID: %s\n"), millis(), selected_wifi);
            }*/
              for (uint8_t i = 0; i < MAX_NETWORKS; i++) {
                String ssid = readStringFromEEPROM(SSID_ADDR(i));
                if (ssid == selected_wifi) {
                  if (readStringFromEEPROM(PASS_ADDR(i)).length() > 0) {
                    inputText = readStringFromEEPROM(PASS_ADDR(i));
                    row = 4;
                    col = 6;
                    break;
                  } else {
                    inputText = "";
                    row = 0;
                    col = 0;
                  }
                }
                inputText = "";
                row = 0;
                col = 0;
                //String pass = readStringFromEEPROM(PASS_ADDR(i));
              }
              while (!Serial.available() || !SerialBT.available()) {
                StickCP2.update();
                StickCP2.Display.setRotation(3);
                //needRedraw=true;
                if (needRedraw) {
                  StickCP2.Display.fillScreen(BLACK);
                  StickCP2.Display.setRotation(3);
                  drawKeyboard("Enter password to connect to " + WiFi.SSID(curr_wifi) + ":", 2);
                  needRedraw = false;
                }
                if (StickCP2.BtnA.wasPressed()) {
                  String key = keys[row][col];

                  if (row == 0 && upper && col < 11) {
                    inputText += shiftedNums[col];
                  } else if (row == 3 && upper && col >= 7) {
                    inputText += shiftedRow4[col - 7];
                  } else if (row == 3 && upper && col < 7) {
                    key = key.substring(0, 1);
                    key.toUpperCase();
                    inputText += key;
                  } else if (row == 2 && upper && col >= 9) {
                    inputText += shiftedRow3[col - 9];
                  } else if (row == 2 && upper && col < 9) {
                    key = key.substring(0, 1);
                    key.toUpperCase();
                    inputText += key;
                  } else if (row > 0 && row < 4) {
                    if (upper) {
                      key = key.substring(0, 1);
                      key.toUpperCase();
                      inputText += key;
                    } else {
                      inputText += String(key);
                    }
                  } else if (row == 4) {
                    if (key == "CAP") {
                      upper = !upper;
                    } else if (key == "DEL") {
                      if (inputText.length() > 0) inputText.remove(inputText.length() - 1);
                    } else if (key == "OK") {
                      break;
                    } else if (key == "BCK") {
                      close = true;
                      isInWiFi = false;
                      break;
                    } else if (key == "LST") {
                      close = true;
                      break;
                    } else if (key == "RST") {
                      inputText = "";
                    } else if (key == " ") {
                      inputText += " ";
                    }
                  } else if (row == 0) {
                    inputText += String(key);
                  }
                  needRedraw = true;

                  drawKeyboard("Enter password to connect to " + WiFi.SSID(curr_wifi) + ":", 2);
                  if (buzzer)
                    StickCP2.Speaker.tone(6000, 100);
                }
                if (M5.BtnB.wasPressed()) {
                  needRedraw = true;
                  moveHorizontal(1);
                  drawKeyboard("Enter password to connect to " + WiFi.SSID(curr_wifi) + ":", 2);
                } else if (M5.BtnB.pressedFor(500)) {
                  needRedraw = true;
                  moveHorizontal(-1);
                  drawKeyboard("Enter password to connect to " + WiFi.SSID(curr_wifi) + ":", 2);
                  delay(300);
                }
                bool btnC = digitalRead(BTN_C_PIN) == LOW;
                if (btnC && btnCTimer == 0) {
                  btnCTimer = millis();
                }
                if (!btnC && btnCTimer > 0) {
                  needRedraw = true;
                  unsigned long pressTime = millis() - btnCTimer;
                  if (pressTime < 500) {
                    //needRedraw = true;
                    moveVertical(1);
                  } else {
                    //needRedraw = true;
                    moveVertical(-1);
                  }
                  btnCTimer = 0;
                  btnCHolding = false;
                  drawKeyboard("Enter password to connect to " + WiFi.SSID(curr_wifi) + ":", 2);
                }
                if (Serial.available() || SerialBT.available()) break;
                if (!isActivateBT) {
                  SerialBT.begin(BT_ID);
                  isActivateBT = true;
                }
                delay(10);

                //if (StickCP2.BtnA.wasPressed()) break;
              }

              //if (Serial.available() || SerialBT.available()) {
              StickCP2.Display.setRotation(0);
              StickCP2.Display.setTextSize(1);
              if (!close) {
                String pwd;
                if (Serial.available())
                  pwd = Serial.readStringUntil('\n');
                else if (SerialBT.available())
                  pwd = SerialBT.readStringUntil('\n');
                else
                  pwd = inputText;
                WiFi.begin(selected_wifi, pwd);
                StickCP2.Display.drawRect(0, 50, SCREEN_WIDTH, 135, TFT_WHITE);
                StickCP2.Display.fillRect(0, 50, SCREEN_WIDTH, 135, TFT_BLACK);
                StickCP2.Display.setTextColor(TFT_WHITE);

                StickCP2.Display.setCursor(10, 62);
                StickCP2.Display.print("Connecting to " + selected_wifi + " with pwd=" + pwd);
                uint8_t timeout = 0;
                while (WiFi.status() != WL_CONNECTED && timeout <= 30) {
                  StickCP2.Display.print(F("."));
                  //if (StickCP2.BtnA.wasPressed()) break;
                  if (digitalRead(35) == 0) break;
                  delay(1000);
                  timeout++;
                }
                StickCP2.Display.println();
                if (WiFi.isConnected()) {
                  StickCP2.Display.drawRect(0, 50, SCREEN_WIDTH, 135, TFT_WHITE);
                  StickCP2.Display.fillRect(0, 50, SCREEN_WIDTH, 135, TFT_BLACK);
                  StickCP2.Display.setTextColor(TFT_GREEN);
                  StickCP2.Display.setCursor(10, 62);
                  StickCP2.Display.println("Connected to Wi-Fi: " + selected_wifi);
                  if (last_ssid != selected_wifi) {
                    saveCredential(ssid_cnt, selected_wifi, pwd);
                    ssid_cnt++;  // Move to next slot
                    EEPROM.write(3, ssid_cnt);
                    EEPROM.commit();
                    last_ssid = selected_wifi;
                  }
                  if (!isSetup) {
                    setupWebServer();
                    isSetup = true;
                  }
                  scr_cli.setCACert(TELEGRAM_CERTIFICATE_ROOT);
                  delay(1000);
                  isInWiFi = false;
                  //break;
                } else {
                  StickCP2.Display.drawRect(0, 50, SCREEN_WIDTH, 135, TFT_WHITE);
                  StickCP2.Display.fillRect(0, 50, SCREEN_WIDTH, 135, TFT_BLACK);
                  StickCP2.Display.setTextColor(TFT_RED);
                  StickCP2.Display.setCursor(10, 62);

                  StickCP2.Display.println("Failed to connect to Wi-Fi: " + selected_wifi + "(" + WiFi.status() + "," + getStatus(WiFi.status()) + ")");
                  delay(1000);
                  isInWiFi = false;
                  //break;
                }
              }
              //}
            }
          }
        }
        //_prstart_a = 0;
        if (buzzer) StickCP2.Speaker.tone(6000, 100);
      }

      if (StickCP2.BtnB.wasPressed()) {
        nrdraw = true;
        sleepTime = 10;
        lpt = millis();
        if (n > 0) curr_wifi = (curr_wifi + 1) % n;
        delay(150);
        if (buzzer) StickCP2.Speaker.tone(6000, 100);
      }
      if (digitalRead(35) == 0) {
        /*
        if (millis() - _prstart_a > 500) {
          isInWiFi = false;
          break;
        }
      } else {*/
        nrdraw = true;
        sleepTime = 10;
        lpt = millis();
        /*
      if (curr_wifi == 0) {
        isInWiFi = false;
        break;
      } else {*/
        if (n > 0) curr_wifi = (curr_wifi - 1 + n) % n;
        //}
        if (buzzer) StickCP2.Speaker.tone(6000, 100);
        delay(150);
        _prstart_a = 0;
      }
      delay(100);
      //sprite.deleteSprite();  // Cleanup
    }
  }
  if (WiFi.isConnected()) {
    if (Serial.available()) {
      Serial.println(WiFi.SSID() + "," + WiFi.psk());
    } else if (SerialBT.available()) {
      SerialBT.println(WiFi.SSID() + "," + WiFi.psk());
    }
  }
  isActivateBT = false;
  WiFi.scanDelete();
  fase = 0;
  while (digitalRead(35) == 0) {
    fase = 0;
    delay(10);
  }
}


void delay_ten_us(uint16_t us) {
  uint8_t timer;
  while (us != 0) {
    // for 8MHz we want to delay 80 cycles per 10 microseconds
    // this code is tweaked to give about that amount.
    for (timer = 0; timer <= DELAY_CNT; timer++) {
      NOP;
      NOP;
    }
    NOP;
    us--;
  }
}

void drawKeyboard(String str_label, uint8_t font_size) {
  StickCP2.Display.fillScreen(BLACK);
  /*
  if (dt.time.hours < 10) hours = "0" + String(dt.time.hours);
  else hours = String(dt.time.hours);
  if (dt.time.minutes < 10) minutes = "0" + String(dt.time.minutes);
  else minutes = String(dt.time.minutes);
  StickCP2.Display.setTextDatum(0);
  StickCP2.Display.setRotation(3);
  StickCP2.Display.setCursor(16, 8);
  StickCP2.Display.setTextColor(TFT_WHITE, TFT_BLUE);
  StickCP2.Display.fillRect(0, 0, SCREEN_HEIGHT, 20, TFT_BLUE);
  StickCP2.Display.setTextSize(1);
  StickCP2.Display.println(String(hours) + ":" + String(minutes));
  StickCP2.Display.setCursor(100, 8);
  StickCP2.Display.println(String(StickCP2.Power.getBatteryLevel()) + "%");
  StickCP2.Display.setCursor(5, 30);
  */

  StickCP2.Display.setRotation(3);
  StickCP2.Display.setTextSize(1);

  for (int r = 0; r < 5; r++) {
    int y = 50 + r * 15;

    for (int c = 0; c < keyLengths[r]; c++) {
      String label = keys[r][c];
      if (r == 0 && upper && c < 11) label = shiftedNums[c];
      else if (r == 3 && upper && c >= 7) label = shiftedRow4[c - 7];
      else if (r == 3 && upper && c < 7) {
        label = label.substring(0, 1);
        label.toUpperCase();
      } else if (r == 2 && upper && c >= 9) label = shiftedRow3[c - 9];
      else if (r == 2 && upper && c < 9) {
        label = label.substring(0, 1);
        label.toUpperCase();
      } else if (r > 0 && r < 4 && upper) {
        label = label.substring(0, 1);
        label.toUpperCase();
      }

      int x = 10 + c * 22;
      if (r == row && c == col) {
        StickCP2.Display.fillRect(x - 2, y - 2, 28 - 6, 24 - 6, BLUE);
        StickCP2.Display.setTextColor(WHITE);
      } else {
        //if (row < 4) {
        StickCP2.Display.fillRect(x - 2, y - 2, 28 - 6, 24 - 6, BLACK);
        StickCP2.Display.drawRect(x - 2, y - 2, 28 - 6, 24 - 6, DARKGREY);
        /*
        } else if (row >= 4) {
          StickCP2.Display.fillRect(x + 20, y - 2, 28 - 6, 24 - 6, BLACK);
          StickCP2.Display.drawRect(x + 20, y - 2, 28 - 6, 24 - 6, DARKGREY);
        }
        */
        StickCP2.Display.setTextColor(WHITE);
      }

      StickCP2.Display.setCursor(x, y);
      StickCP2.Display.print(label);
    }
  }

  //StickCP2.Display.setTextColor(WHITE);
  //StickCP2.Display.setTextSize(1);
  StickCP2.Display.fillRect(0, 160, 240, 30, BLACK);
  StickCP2.Display.setTextColor(WHITE);
  StickCP2.Display.setTextSize(1);
  StickCP2.Display.setCursor(10, 1);
  StickCP2.Display.println(str_label);

  //StickCP2.Display.println(F("Input: "));
  StickCP2.Display.fillRect(10, 0 + 10 + 8, SCREEN_WIDTH - 18, 18 + 10, BLACK);
  StickCP2.Display.drawRect(10, 0 + 10 + 8, SCREEN_WIDTH - 18, 18 + 10, DARKGREY);
  StickCP2.Display.setTextColor(GREEN);
  StickCP2.Display.setCursor(17, (2 + 5) + 10 + 8);
  StickCP2.Display.setTextSize(font_size);
  StickCP2.Display.print(inputText);
}

void moveHorizontal(int dir) {
  col += dir;
  if (col >= keyLengths[row]) col = 0;
  if (col < 0) col = keyLengths[row] - 1;
}

void moveVertical(int dir) {
  row += dir;
  if (row >= 5) row = 0;
  if (row < 0) row = 4;
  if (col >= keyLengths[row]) col = 0;
}
void playSong() {
  const int totalNotes = sizeof(notes) / sizeof(int);
  StickCP2.Speaker.setVolume(255);

  // Loop through each note
  for (int i = 0; i < totalNotes; i++) {
    const int currentNote = notes[i];
    float wait = durations[i] / songSpeed;
    // Play tone if currentNote is not 0 frequency, otherwise pause (noTone)
    if (currentNote != 0) {
      StickCP2.Speaker.tone(notes[i] * 4, wait);  // tone(frequency, duration)
    } else {
      StickCP2.Speaker.end();
    }
    if (digitalRead(35) == 0) {
      while (digitalRead(35) == 0) {
        delay(10);
      }
      break;
    }

    // delay is used to wait for tone to finish playing before moving to next loop
    delay(wait);
  }
}
