#include <Arduino.h>
#include <M5StickCPlus2.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>
#include <WiFi.h>
#include <esp_ping.h>
#include <lwip/inet.h>
#include <esp_event.h>
#define TELEGRAM_API_IP "149.154.167.220"
static bool ping_result_ok = false;

// Gọi lại khi ping thành công

//#include "keyboard.hpp"

#define BOT_TOKEN "YOUR_BOT_TOKEN"
//#endif
WiFiClientSecure scr_cli;

typedef enum {
  BOT_CONNECTED = 0,
  BOT_NOT_CONNECTED,
  BOT_CONNECT_TIMEOUT
} bot_status_t;

//String CHAT_ID = "7075849895";
String CHAT_ID = "";
String MSG = "";
String FROM = "";
unsigned long last_check = 0;
const unsigned long check_interval = 20000;

bot_status_t check_api_conn(WiFiClientSecure &client, bot_status_t &out) {
  // client.setCACert(TELEGRAM_CERTIFICATE_ROOT);  // telegram certificate
  if (!client.connect("api.telegram.org", 443)) {
    Serial.printf("[%lu] [Telegram API] [E]: Cannot access to Telegram API website\n", millis());
    out = BOT_NOT_CONNECTED;
    return BOT_NOT_CONNECTED;
  }
  client.println("GET /bot" + String(BOT_TOKEN) + "/getMe HTTP/1.1");
  client.println(F("Host: api.telegram.org"));
  client.println(F("Connection: close"));
  client.println();
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 3000) {
      Serial.println(F("[⚠] Timeout while waiting for response from Telegram"));
      client.stop();
      out = BOT_CONNECT_TIMEOUT;
      return BOT_CONNECT_TIMEOUT;
    }
  }

  // Đọc phản hồi
  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println("[Telegram API] " + line);
    if (line.indexOf("200 OK") > 0) {
      out = BOT_CONNECTED;
      return BOT_CONNECTED;
    }
  }
  out = BOT_NOT_CONNECTED;
  return BOT_NOT_CONNECTED;  // Không thấy "200 OK"
}
void read_message() {
  extern UniversalTelegramBot bot;
  if (millis() - last_check > check_interval) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    Serial.printf("[DEBUG] Số tin mới: %d\n", numNewMessages);
    while (numNewMessages) {
      for (int i = 0; i < numNewMessages; i++) {
        String chat_id = bot.messages[i].chat_id;
        String text = bot.messages[i].text;
        String from = bot.messages[i].from_name;

        Serial.printf("[Tin đến từ %s]: %s\n", from.c_str(), text.c_str());
        if (chat_id != "") {
          CHAT_ID = chat_id;
        }
        MSG = text;
        FROM = from;
        Serial.printf("[%lu] [I]: Chat_ID: %s, msg", millis());
      }
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    last_check = millis();
  }
}