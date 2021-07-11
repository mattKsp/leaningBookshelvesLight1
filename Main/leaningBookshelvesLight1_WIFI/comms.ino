/*----------------------------comms----------------------------*/
void setupWIFI()  {
  if (USE_SERIAL && DEBUG_COMMS) { 
    Serial.print("Connecting to ");
    Serial.print(STATION_SSID);
    Serial.println(".");
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(STATION_SSID, STATION_PASSWORD);

  if (USE_SERIAL && DEBUG_COMMS) { Serial.print("Connecting"); }

  while (WiFi.status() != WL_CONNECTED) {
    if (USE_SERIAL && DEBUG_COMMS) { Serial.print("."); }
    delay(_wifiConnectionDelay);
  }

  if (USE_SERIAL && DEBUG_COMMS) { 
    Serial.println("");
    Serial.print("Connected. Local IP: ");
    Serial.println(WiFi.localIP());
  }
}

/*----------------------------setup MQTT----------------------------*/
void mqttSubscribe() {
                                                      // ..how to do subscribe to all ???
  mqttClient.subscribe(DEVICE_SUB_LEANINGBOOKSHELVES1);
                                                      
  //mqttClient.subscribe(cmnd_power_topic);
  //mqttClient.subscribe(cmnd_color_topic);
//  mqttClient.subscribe(MQTT_LIGHTS_TOPIC_COMMAND);
//  mqttClient.subscribe(MQTT_LIGHTS_BRIGHTNESS_TOPIC_COMMAND);
  //mqttClient.subscribe(MQTT_LIGHTS_HUE_TOPIC_COMMAND);
//  mqttClient.subscribe(MQTT_LIGHTS_TOP_RGB_TOPIC_COMMAND);
//  mqttClient.subscribe(MQTT_LIGHTS_BOT_RGB_TOPIC_COMMAND);

  mqttClient.subscribe("house/sunrise");
  mqttClient.subscribe("house/sunset");
  mqttClient.subscribe("house/breath");
  mqttClient.subscribe("house/restart");
  mqttClient.subscribe("house/lockdown");
}

void mqttReconnect() {
  for (int attempt = 0; attempt < 3; ++attempt)
  {
    //Loop until we're reconnected
    if (USE_SERIAL && DEBUG_COMMS) { Serial.print("Attempting MQTT connection..."); }
    const String clientId = "MlC_LeaningBookshelvesLight1";
    // Attempt to connect
    if (true == mqttClient.connect(clientId.c_str(), _username, _password, "house/leaningbookshelves1/available", 0, true, "offline"))
    {
      if (USE_SERIAL && DEBUG_COMMS) { Serial.println("connected"); }
      mqttClient.publish(DEVICE_CD1_LEANINGBOOKSHELVES1, DEVICE_ONLINE);
      publishStatusAll(true);   // Publish current stats
      mqttSubscribe();
      break;
    }
    else
    {
      if (USE_SERIAL && DEBUG_COMMS) { 
        Serial.print("failed, rc=");
        Serial.print(mqttClient.state());
        Serial.println(" try again in 5 seconds");
      }
      delay(5000);              // Wait 5 seconds before retrying
    }
  }
}

void setupMQTT() {
  if (USE_SERIAL && DEBUG_COMMS) { 
    Serial.print("MQTT Server: ");
    Serial.println(MQTT_BROKER_IP); //mqtt_server ???
    Serial.print("MQTT Port: ");
    Serial.println(_mqtt_port);
    Serial.print("MQTT Username: ");
    Serial.println(_username);
    // Hide password from the log and show * instead
    char hiddenpass[20] = "";
    for (size_t charP=0; charP < strlen(_password); charP++)
    {
        hiddenpass[charP] = '*';
    }
    hiddenpass[strlen(_password)] = '\0';
    Serial.print("MQTT Password: ");
    Serial.println(hiddenpass);
  }

  const int mqttPort = atoi(_mqtt_port);
  mqttClient.setServer(_mqtt_server, mqttPort);
  mqttClient.setCallback(receiveMessage);

  mqttReconnect();
}

void turnOffComms() 
{
  publishDeviceOffline();
  DEBUG_COMMS = false;
  turnOffWifi();
  DEBUG_GEN = false;
  turnOffSerial();
}

/*----------------------------loop MQTT----------------------------*/
void loopMQTT() {
  mqttClient.loop();
  
  // Reconnect if there is an issue with the MQTT connection
  const unsigned long mqttConnectionMillis = millis();
  if ( (false == mqttClient.connected()) && (_mqttConnectionInterval <= (mqttConnectionMillis - _mqttConnectionPreviousMillis)) )
  {
    _mqttConnectionPreviousMillis = mqttConnectionMillis;
    mqttReconnect();
  }
}
