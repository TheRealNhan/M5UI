#ifndef LORA_AUDIO_UART_HPP
#define LORA_AUDIO_UART_HPP

#include <Arduino.h>
#include <Preferences.h>
#include <M5StickCPlus2.h>
#include <driver/i2s.h>

// === Pin configuration ===
#define LORA_RX 33
#define LORA_TX 32
#define LORA_BAUD 9600

#define I2S_BUFFER_SIZE 512
#define MAX_KNOWN_IDS 10

extern uint8_t DEVICE_ID;

// === Audio buffers ===
uint8_t audioTxBuffer[I2S_BUFFER_SIZE + 1];
uint8_t audioRxBuffer[I2S_BUFFER_SIZE + 1];

// === Known/allowed IDs ===
uint8_t knownIDs[MAX_KNOWN_IDS];
bool allowedIDs[MAX_KNOWN_IDS];
int numKnownIDs = 0;

Preferences prefs;
bool inMenu = false;
int selectedIndex = 0;

extern HardwareSerial LoRaSerial;

// === I2S Configuration ===
void initI2S() {
  const i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
    .communication_format = I2S_COMM_FORMAT_I2S_MSB,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 256,
    .use_apll = false,
    .tx_desc_auto_clear = true,
    .fixed_mclk = 0
  };

  const i2s_pin_config_t pin_config = {
    .bck_io_num = 0,    // BCK  (G0)
    .ws_io_num = 26,    // LRCK (G26)
    .data_out_num = 25, // DIN  (Speaker Out)
    .data_in_num = 34   // DOUT (Mic In)
  };

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
  i2s_zero_dma_buffer(I2S_NUM_0);
}

// === Function declarations ===
void initLoRaUART() {
  LoRaSerial.begin(LORA_BAUD, SERIAL_8N1, LORA_RX, LORA_TX);
  delay(500);
  StickCP2.Display.println("LoRa UART initialized");
}

void sendAudioPacket(uint8_t* data, size_t len) {
  LoRaSerial.write(data, len);
}

int receiveAudioPacket(uint8_t* buffer, size_t maxLen) {
  size_t i = 0;
  unsigned long start = millis();
  while (millis() - start < 50 && i < maxLen) {
    if (LoRaSerial.available()) {
      buffer[i++] = LoRaSerial.read();
    }
  }
  return i;
}

void addKnownID(uint8_t id) {
  for (int i = 0; i < numKnownIDs; i++) {
    if (knownIDs[i] == id) return;
  }
  if (numKnownIDs < MAX_KNOWN_IDS) {
    knownIDs[numKnownIDs] = id;
    allowedIDs[numKnownIDs] = true;
    numKnownIDs++;
  }
}

void saveKnownIDs() {
  prefs.begin("lora-audio", false);
  prefs.putBytes("ids", knownIDs, numKnownIDs);
  prefs.putBytes("allowed", allowedIDs, numKnownIDs);
  prefs.putUChar("count", numKnownIDs);
  prefs.end();
}

void loadKnownIDs() {
  prefs.begin("lora-audio", true);
  numKnownIDs = prefs.getUChar("count", 0);
  prefs.getBytes("ids", knownIDs, numKnownIDs);
  prefs.getBytes("allowed", allowedIDs, numKnownIDs);
  prefs.end();
}

void showSelectMenu() {
  StickCP2.Display.fillScreen(BLACK);
  StickCP2.Display.setTextColor(YELLOW);
  StickCP2.Display.setCursor(10, 0);
  StickCP2.Display.println("Select Allowed IDs");
  for (int i = 0; i < numKnownIDs; i++) {
    StickCP2.Display.setCursor(10, 20 + i * 20);
    if (i == selectedIndex) {
      StickCP2.Display.setTextColor(WHITE, BLUE);
    } else {
      StickCP2.Display.setTextColor(WHITE, BLACK);
    }
    StickCP2.Display.printf("ID %02X: %s", knownIDs[i], allowedIDs[i] ? "ON " : "OFF");
  }
}

void handleMenu() {
  if (M5.BtnB.wasPressed()) {
    selectedIndex = (selectedIndex + 1) % numKnownIDs;
    showSelectMenu();
  }
  if (M5.BtnA.wasPressed()) {
    allowedIDs[selectedIndex] = !allowedIDs[selectedIndex];
    saveKnownIDs();
    showSelectMenu();
  }
  if (M5.BtnC.wasPressed()) {
    inMenu = false;
    StickCP2.Display.fillScreen(BLACK);
  }
}

void AudioComm_begin() {
  initI2S();
  initLoRaUART();
  loadKnownIDs();
}

void AudioComm_update() {
  M5.update();

  if (inMenu) {
    handleMenu();
    return;
  }

  size_t bytes_read;
  i2s_read(I2S_NUM_0, audioTxBuffer + 1, I2S_BUFFER_SIZE, &bytes_read, portMAX_DELAY);
  if (bytes_read > 0) {
    audioTxBuffer[0] = DEVICE_ID;
    sendAudioPacket(audioTxBuffer, bytes_read + 1);
  }

  int len = receiveAudioPacket(audioRxBuffer, sizeof(audioRxBuffer));
  if (len > 1) {
    uint8_t senderID = audioRxBuffer[0];
    addKnownID(senderID);
    int idx = -1;
    for (int i = 0; i < numKnownIDs; i++) {
      if (knownIDs[i] == senderID) {
        idx = i;
        break;
      }
    }
    if (idx != -1 && allowedIDs[idx]) {
      size_t written;
      i2s_write(I2S_NUM_0, audioRxBuffer + 1, len - 1, &written, portMAX_DELAY);
    }
  }

  if (M5.BtnB.wasPressed() && numKnownIDs > 0) {
    inMenu = true;
    selectedIndex = 0;
    showSelectMenu();
  }
}

#endif
