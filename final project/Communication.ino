
void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(SSID);
  
  WiFi.begin(SSID, PWD);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    // we can even make the ESP32 to sleep
  }
 
  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());
}
 
 
void add_json_object(char *tag, float value) {
  JsonObject obj = jsonDocument.createNestedObject();
  obj["tag"] = tag;
  obj["value"] = value;
  
}

void handlePost() {
  if (server.hasArg("plain") == false) {
    //handle error here
  }

  String body = server.arg("plain");
  Serial.println(body);
  deserializeJson(jsonDocument, body);
  
  // Get position and arm positions 
  manual = jsonDocument["manual"];
  mode = jsonDocument["mode"];
  base = jsonDocument["base_angle"];
  link_servo = jsonDocument["link_angle"];
  grabber = jsonDocument["grabber_angle"];
  x_pos = jsonDocument["x_pos"];
  y_pos = jsonDocument["y_pos"];
 
// pass parameters to control
// Respond to the client
  jsonDocument.clear();
  add_json_object("base_angle", base);
  add_json_object("link_angle", link_servo);
  add_json_object("grabber_angle", grabber);


  serializeJson(jsonDocument, buffer);
  server.send(200, "application/json", buffer);
}
 
 
// setup API resources
void setup_routing() {
 
  server.on("/control", HTTP_POST, handlePost);
 
  // start server
  server.begin();
}

 