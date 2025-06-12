#include <Arduino.h>
#include <WiFi.h>
#include <Esp.h>

String getStatus(wl_status_t status) {
  switch (status) {
    case WL_NO_SHIELD:
      return "No shield";
    case WL_CONNECTED:
      return "Connected";
    case WL_IDLE_STATUS:
      return "IDLE Status (0)";
    case WL_NO_SSID_AVAIL:
      return "No SSID available";
    case WL_SCAN_COMPLETED:
      return "Scan completed";
    case WL_CONNECT_FAILED:
      return "Connect failed";
    case WL_CONNECTION_LOST:
      return "Connection lost";
    case WL_DISCONNECTED:
      return "Disconnected";
  }
}
String getEncryptionType(wifi_auth_mode_t encrypt_type) {
  switch (encrypt_type) {
    case WIFI_AUTH_OPEN:
      return "OPEN";
    case WIFI_AUTH_WEP:
      return "WEP";
    case WIFI_AUTH_WPA_PSK:
      return "WPA-PSK";
    case WIFI_AUTH_WPA2_PSK:
      return "WPA2-PSK";
    case WIFI_AUTH_WPA_WPA2_PSK:
      return "WPA/WPA2-PSK";
    case WIFI_AUTH_WPA3_PSK:
      return "WPA3-PSK";
    case WIFI_AUTH_WPA2_WPA3_PSK:
      return "WPA2/WPA3-PSK";
    case WIFI_AUTH_WAPI_PSK:
      return "WAPI-PSK";
    case WIFI_AUTH_MAX:
      return "UNKNOWN";
    default:
      return "INVALID/UNKNOWN";
  }
}

String upload_html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>OTA Update</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      font-family: Arial, sans-serif;
      background: #f2f2f2;
      padding: 20px;
      text-align: center;
    }
    h1 {
      color: #333;
    }
    form {
      background: #fff;
      padding: 20px;
      margin: auto;
      display: inline-block;
      border-radius: 8px;
      box-shadow: 0 2px 6px rgba(0,0,0,0.2);
    }
    input[type="file"] {
      margin: 10px 0;
    }
    input[type="submit"] {
      background-color: #28a745;
      color: white;
      border: none;
      padding: 10px 20px;
      border-radius: 5px;
      cursor: pointer;
      font-size: 16px;
    }
    input[type="submit"]:hover {
      background-color: #218838;
    }
    @media (max-width: 600px) {
      form {
        width: 90%;
      }
    }
    input[type="file"] {
    	background-color: rgba(0,0,0,0.2);
    }
  </style>
</head>
<body>
  <h1>OTA Firmware Update</h1>
  <h4>For M5StickC Plus2</h4>
  <p>Sketch used: )rawliteral"
                     + String(ESP.getSketchSize()) + R"rawliteral(bytes</p><p>Sketch free: )rawliteral" + String(ESP.getFreeSketchSpace()) + R"rawliteral(bytes</p>
  <form method="POST" action="/update" enctype="multipart/form-data">
    <label for="file">Upload .bin firmware below</label>
    <hr>
    <input type="file" name="firmware" accept=".bin" id="file" required><br>
    <input type="submit" value="Upload & Update">
  </form>
</body>
</html>
)rawliteral";
