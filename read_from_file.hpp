#include <Arduino.h>
#include <LITTLEFS.h>
#include <FS.h>
#include <set>
#include <vector>
void readConfFile(const char* path) {
  File file = LITTLEFS.open(path, "r");
  if (!file) {
    Serial.println(F("Cannot open .conf file"));
    return;
  }

  while (file.available()) {
    String line = file.readStringUntil('\n');
    line.trim();

    if (line.startsWith(F("#")) || line.length() == 0) continue;

    int sepIndex = line.indexOf('=');
    if (sepIndex > 0) {
      String key = line.substring(0, sepIndex);
      String value = line.substring(sepIndex + 1);
      key.trim();
      value.trim();

      Serial.printf(" %s = %s\n", key.c_str(), value.c_str());

      // xử lý tùy theo key
      if (key == "WiFi_mode") {
        if (value == "WIFI_AP_STA") {
        }
        // lưu SSID vào biến
      } else if (key == "WiFi_scan_method") {
        if (value == "WIFI_ALL_CHANNEL_SCAN") {
        }
        //int freq = value.toInt();
      }
    }
  }

  file.close();
}
void handleFileRequest();

// String generateFileListHTML(const String& inputPath) {
//   String html = R"rawliteral(
//     <!DOCTYPE html><html><head><meta charset="UTF-8">
//     <title>LittleFS Browser</title>
//     <style>
//       body { font-family: sans-serif; padding: 20px; background: #f5f7fa; }
//       table { width: 100%; border-collapse: collapse; background: white; border-radius: 8px; box-shadow: 0 0 10px rgba(0,0,0,0.05); }
//       th, td { padding: 10px 15px; text-align: left; border-bottom: 1px solid #eee; }
//       th { background: #f0f0f0; }
//       a.button { background: #3498db; color: white; padding: 5px 10px; text-decoration: none; border-radius: 5px; }
//       a.button:hover { background: #2980b9; }
//     </style>
//     </head><body>
//   )rawliteral";

//   String path = inputPath;
//   if (!path.startsWith("/")) path = "/" + path;
//   if (!path.endsWith("/")) path += "/";

//   html += "<h2>📁 Listing: " + path + "</h2>";
//   html += "<table><tr><th>Name</th><th>Type</th><th>Action</th></tr>";

//   if (path != "/") {
//     String parent = path;
//     parent.remove(parent.length() - 1);
//     int slash = parent.lastIndexOf('/');
//     parent = (slash <= 0) ? "/" : parent.substring(0, slash + 1);
//     html += "<tr><td>..</td><td>Parent</td><td><a class='button' href='/file?dir=" + parent + "'>Up</a></td></tr>";
//   }

//   std::set<String> shownDirs;
//   File root = LITTLEFS.open("/");
//   File file = root.openNextFile();

//   while (file) {
//     String fullPath = String(file.name());
//     if (fullPath.startsWith(path)) {
//       String relative = fullPath.substring(path.length());
//       if (relative.length() > 0) {
//         int slashIndex = relative.indexOf('/');
//         if (slashIndex >= 0) {
//           String folderName = relative.substring(0, slashIndex);
//           String folderPath = path + folderName + "/";
//           if (shownDirs.find(folderPath) == shownDirs.end()) {
//             shownDirs.insert(folderPath);
//             html += "<tr><td>" + folderName + "/</td><td>Directory</td><td><a class='button' href='/file?dir=" + folderPath + "'>Open</a></td></tr>";
//           }
//         } else {
//           html += "<tr><td>" + relative + "</td><td>File</td><td><a class='button' href='/open_file?name=" + fullPath + "' target='_blank'>View</a></td></tr>";
//         }
//       }
//     }
//     file = root.openNextFile();  // always move to next
//     delay(1);
//   }

//   html += "</table>";
//   html += "<h3>⬆️ Upload New File</h3>";
//   html += "<form method='POST' action='/upload_file' enctype='multipart/form-data'>";
//   html += "<input type='hidden' name='path' value='" + path + "'>";
//   html += "<input type='file' name='upload'><input type='submit' value='Upload'>";
//   html += "</form></body></html>";

//   return html;
// }



