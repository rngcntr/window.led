// Define routing
void setupRestServerRouting() {
  server.on("/", HTTP_GET, getIndex);
  server.on(F("/helloWorld"), HTTP_GET, getHelloWorld);
  server.on(F("/brightness"), HTTP_GET, getBrightness);
  server.on(F("/toggle"), HTTP_POST, toggle);
  server.on(F("/brightness"), HTTP_POST, setBrightness);
  server.onNotFound(handleNotFound);
}

void getIndex() {
  server.send(200, F("text/html"), F("window.led REST server"));
}

void writeJson(int statusCode, DynamicJsonDocument& doc) {
  String buf;
  serializeJson(doc, buf);
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(statusCode, "application/json", buf);
}
 
void getHelloWorld() {
  DynamicJsonDocument doc(512);
  doc["name"] = "Hello world";
  writeJson(200, doc);
}

void getBrightness() {
  DynamicJsonDocument doc(512);
  doc["on"] = ledState.isOn();
  doc["colorTemperature"] = ledState.getColorTemperature();
  doc["overallBrightness"] = ledState.getOverallBrightness();
  doc["warmBrightness"] = ledState.getWarmBrightness();
  doc["coldBrightness"] = ledState.getColdBrightness();
  writeJson(200, doc);
}

void toggle() {
  ledState.toggle();
  getBrightness();
  ledState.write();
}

void setBrightness() {
  String postBody = server.arg("plain");
  DynamicJsonDocument doc(512);
  DeserializationError error = deserializeJson(doc, postBody);
  if (error) {
    String msg = error.c_str();
    server.send(400, F("text/html"), "Error in JSON request:<br>" + msg);
    return;
  }

  JsonObject postObj = doc.as<JsonObject>();
  bool requestWasValid = false;

  if (postObj.containsKey("on")) {
    bool on = postObj["on"];
    ledState.setOn(on);
    requestWasValid = true;
  }

  if (postObj.containsKey("overallBrightness") || postObj.containsKey("colorTemperature")) {
    int newOverallBrightness = postObj.containsKey("overallBrightness")
      ? postObj["overallBrightness"]
      : ledState.getOverallBrightness();
    int newColorTemperature = postObj.containsKey("colorTemperature")
      ? postObj["colorTemperature"]
      : ledState.getColorTemperature();
    // TODO: validate values
    ledState.setIndirect(newOverallBrightness, newColorTemperature);
    requestWasValid = true;
  }

  if (postObj.containsKey("warmBrightness") || postObj.containsKey("coldBrightness")) {
    int newWarmBrightness = postObj.containsKey("warmBrightness")
      ? postObj["warmBrightness"]
      : ledState.getWarmBrightness();
    int newColdBrightness = postObj.containsKey("coldBrightness")
      ? postObj["coldBrightness"]
      : ledState.getColdBrightness();
    // TODO: validate values
    ledState.setDirect(newWarmBrightness, newColdBrightness);
    requestWasValid = true;
  }

  if (requestWasValid) {
    ledState.write();
    getBrightness();
  } else {
    sendError("Invalid brightness parameters");
  }
}
 
// Manage not found URL
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(404, "text/plain", message);
}

void sendError(const char* msg) {
  DynamicJsonDocument doc(512);
  doc["status"] = "Error";
  doc["msg"] = msg;
  writeJson(400, doc);
}
