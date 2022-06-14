void setup() {
  InfopointName = "Infopoint";
  InfopointName += WiFi.macAddress();
  Serial.begin(115200);
  delay(500);
  //Video
  #ifdef USE_VIDEO
    videoSetup();
  #endif
  Serial.print(F("WiFi MAC address: "));
  Serial.println(WiFi.macAddress());
  Serial.print(F("Infopoint name: "));
  Serial.println(InfopointName);
  WiFi.macAddress(macAddress);
  sprintf(jsonFilename,"%02x%02x%02x%02x%02x%02x.json",macAddress[0],macAddress[1],macAddress[2],macAddress[3],macAddress[4],macAddress[5]);
  URL += String(jsonFilename);
  Serial.print(F("Update URL: "));
  Serial.println(URL);
  #ifdef useNeopixelRing
    FastLED.addLeds<NEOPIXEL, PIN_RGB>(leds, NUM_LEDS);
    FastLED.setBrightness(neopixelMaxBrightness);
  #endif
  #ifdef useLedHeartbeat
    ledcSetup(0, 5000, 12);
    ledcAttachPin(PIN_LOGO, 0);
  #endif
  //LittleFS
  Serial.print(F("Mounting LittleFS: "));
  if (!LittleFS.begin())
  {
    Serial.println(F("failed"));
    Serial.print(F("Formatting LittleFS: "));
    LittleFS.format();
    if (!LittleFS.begin()) {
      Serial.println(F("failed"));
      delay(1000);
      ESP.restart();
    }
  }
  Serial.println(F("OK"));
  Serial.print(F("Total space (KB): "));
  Serial.println(LittleFS.totalBytes()/1024);
  Serial.print(F("Free space (KB): "));
  Serial.println((LittleFS.totalBytes() - LittleFS.usedBytes())/1024);
  loadInstallationInformation(installationInformationFile);
  #ifdef VALIDATE_CERT
    Serial.println(F("Validating certificate for connections to server"));
    client.setCACert(server_rootcert);
  #else
    client.setInsecure();
  #endif
  // Connect to the WiFI
  WiFi.mode(WIFI_STA);
  #if defined(USE_EMF_INSECURE_WIFI)
    Serial.print(F("Connecting to emfcamp-insecure22"));
    WiFi.begin("emfcamp-insecure22");
  #elif defined(USE_EMF_WIFI) || defined(USE_EMF_OUTBOUND_ONLY_WIFI)
    #ifdef CHECK_8021x_CERTIFICATE
      esp_wifi_sta_wpa2_ent_set_ca_cert((uint8_t *)emfcamp_ca, strlen(emfcamp_ca) + 1);
    #endif
    esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
    esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
    esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD));
    //esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT();
    //esp_wifi_sta_wpa2_ent_enable(&config);
    esp_wifi_sta_wpa2_ent_enable();
    Serial.print(F("Connecting to SSID: \"emfcamp\""));
    WiFi.begin("emfcamp");
  #else
    Serial.print(F("Connecting to SSID: "));
    Serial.print(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PSK);
  #endif
  // Wait for connection
  while (wifiRetries < maxWifiRetries && WiFi.status() != WL_CONNECTED) {
    delay(wifiRetryInterval);
    Serial.print('.');
    wifiRetries++;
  }
  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.print(F("\r\nConnected, "));
    Serial.print(F("IP address: "));
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.print(F(" failed to connect"));
  }
  wifiRetries = 0;
  manageServerUpdates();
  running = true;
}
