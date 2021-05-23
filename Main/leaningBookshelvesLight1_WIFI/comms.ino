/*----------------------------comms----------------------------*/
void setupWIFI()  {
  if (DEBUG_COMMS) { 
    Serial.print("Connecting to ");
    Serial.print(STATION_SSID);
    Serial.println(".");
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(STATION_SSID, STATION_PASSWORD);

  if (DEBUG_COMMS) { Serial.print("Connecting"); }

  while (WiFi.status() != WL_CONNECTED) {
    if (DEBUG_COMMS) { Serial.print("."); }
    delay(_wifiConnectionDelay);
  }

  if (DEBUG_COMMS) { 
    Serial.println("");
    Serial.print("Connected. Local IP: ");
    Serial.println(WiFi.localIP());
  }
}

/*----------------------------setup MQTT----------------------------*/
void setupMQTT() {
  
  if (DEBUG_COMMS) { 
    Serial.print("MQTT Server: ");
    Serial.println(MQTT_BROKER_IP); //mqtt_server ???
    Serial.print("MQTT Port: ");
    Serial.println(mqtt_port);
    // Print MQTT Username
    Serial.print("MQTT Username: ");
    Serial.println(username);
    // Hide password from the log and show * instead
    char hiddenpass[20] = "";
    for (size_t charP=0; charP < strlen(password); charP++)
    {
        hiddenpass[charP] = '*';
    }
    hiddenpass[strlen(password)] = '\0';
    Serial.print("MQTT Password: ");
    Serial.println(hiddenpass);
  }

  const int mqttPort = atoi(mqtt_port);
  mqttClient.setServer(mqtt_server, mqttPort);
  mqttClient.setCallback(receiveMessage);

  mqttReconnect();
}

/*----------------------------loop MQTT----------------------------*/
void loopMQTT() {
  mqttClient.loop();
  
  // Reconnect if there is an issue with the MQTT connection
  const unsigned long mqttConnectionMillis = millis();
  if ( (false == mqttClient.connected()) && (mqttConnectionInterval <= (mqttConnectionMillis - mqttConnectionPreviousMillis)) )
  {
    mqttConnectionPreviousMillis = mqttConnectionMillis;
    mqttReconnect();
  }
}

void mqttReconnect() {
  for (int attempt = 0; attempt < 3; ++attempt)
  {
    //Loop until we're reconnected
    if (DEBUG_COMMS) { Serial.print("Attempting MQTT connection..."); }
    // Create a random client ID
    //String clientId = "ESP8266Client-";
    //clientId += String(random(0xffff), HEX);
    const String clientId = "LeaningBookshelvesLight1";
    // Attempt to connect
    if (true == mqttClient.connect(clientId.c_str(), username, password))
    {
      if (DEBUG_COMMS) { Serial.println("connected"); }
  
      publishStatusAll(true);   // Publish current stats
  
      // Subscribe to MQTT topics
      //mqttClient.subscribe(cmnd_power_topic);
      //mqttClient.subscribe(cmnd_color_topic);
      mqttClient.subscribe(MQTT_LIGHTS_TOPIC_COMMAND);
      mqttClient.subscribe(MQTT_LIGHTS_BRIGHTNESS_TOPIC_COMMAND);
      //mqttClient.subscribe(MQTT_LIGHTS_HUE_TOPIC_COMMAND);
      mqttClient.subscribe(MQTT_LIGHTS_TOP_RGB_TOPIC_COMMAND);
      mqttClient.subscribe(MQTT_LIGHTS_BOT_RGB_TOPIC_COMMAND);
      break;
    }
    else
    {
      if (DEBUG_COMMS) { 
        Serial.print("failed, rc=");
        Serial.print(mqttClient.state());
        Serial.println(" try again in 5 seconds");
      }
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
