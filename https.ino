bool manageServerUpdates() {
  if(lastRetrieval == 0 || ( animationStage == ANIMATE_INSTALLATION_FINISHED && millis() - lastRetrieval > updateFrequency))
  {
    lastRetrieval = millis();
    wifiRetries = 0;
    Serial.print(F("Free heap: "));
    Serial.println(ESP.getFreeHeap());
    if(WiFi.status() == WL_CONNECTED)
    {
      if(retrieveInstallationInformation(URL))
      {
        success++;
        Serial.print(F("Installation information retrieved OK - "));
        Serial.print(success);
        Serial.print(F(" successes / "));
        Serial.print(fail);
        Serial.println(F(" failures"));
        #ifdef DISCONNECT_WIFI_WHEN_IDLE
          Serial.println(F("Disconnecting WiFi"));
          WiFi.disconnect();
        #endif
        delay(5000);
        return true;
      }
      else
      {
        fail++;
        Serial.print(F("Failed to retrieve installation information - "));
        Serial.print(success);
        Serial.print(F(" successes / "));
        Serial.print(fail);
        Serial.println(F(" failures"));
        #ifdef DISCONNECT_WIFI_WHEN_IDLE
          Serial.println(F("Disconnecting WiFi"));
          WiFi.disconnect();
        #endif
        delay(5000);
        return false;
      }
    }
    else
    {
      Serial.print(F("Not connected to WiFi, reconnecting"));
      WiFi.reconnect();
      while (wifiRetries < maxWifiRetries && WiFi.status() != WL_CONNECTED)
      {
        delay(wifiRetryInterval);
        Serial.print('.');
        wifiRetries++;
      }
      if(WiFi.status() == WL_CONNECTED)
      {
        Serial.println(F(" connected"));
        if(retrieveInstallationInformation(URL))
        {
          success++;
          Serial.print(F("Installation information retrieved OK - "));
          Serial.print(success);
          Serial.print(F(" successes / "));
          Serial.print(fail);
          Serial.println(F(" failures"));
          #ifdef DISCONNECT_WIFI_WHEN_IDLE
            Serial.println(F("Disconnecting WiFi"));
            WiFi.disconnect();
          #endif
          delay(5000);
          return true;
        }
        else
        {
          fail++;
          Serial.print(F("Failed to retrieve installation information - "));
          Serial.print(success);
          Serial.print(F(" successes / "));
          Serial.print(fail);
          Serial.println(F(" failures"));
          #ifdef DISCONNECT_WIFI_WHEN_IDLE
            Serial.println(F("Disconnecting WiFi"));
            WiFi.disconnect();
          #endif
          delay(5000);
          return false;
        }
      }
      else
      {
        Serial.println(F(" failed to connect"));
        delay(5000);
        return false;
      }
    }
  }
  return false;
}


bool retrieveInstallationInformation(String urlToRequest) {
  // Opening connection to server (Use 80 as port if HTTP)
  uint16_t port = 443;
  if(urlToRequest.startsWith("https://"))
  {
    port = 443;
  }
  else
  {
    port = 80;
  }
  urlToRequest.trim(); //Trim random whitespace
  uint16_t firstSlash = urlToRequest.indexOf('/');
  uint16_t thirdSlash =   urlToRequest.indexOf('/', firstSlash + 2);
  String host = urlToRequest.substring(firstSlash + 2, thirdSlash);
  String path = urlToRequest.substring(thirdSlash, urlToRequest.length());
  Serial.print(F("Connecting to \""));
  Serial.print(host.c_str());
  Serial.print(F("\" port "));
  Serial.print(port);
  Serial.print(F(": "));
  if (!client.connect(host.c_str(), port))
  {
    Serial.println(F("failed"));
    return false;
  }
  else
  {
    Serial.println(F("OK"));
  }
  // give the esp a breather
  yield();
  Serial.print(F("Requesting "));
  Serial.print(path);
  Serial.print(F(": "));
  // Send HTTP request
  // This is the second half of a request (everything that comes after the base URL)
  client.print(F("GET "));
  client.print(path);
  client.println(F(" HTTP/1.1"));

  //Headers
  client.print(F("Host: "));
  client.println(host);
  client.println(F("Cache-Control: no-cache"));

  if (client.println() == 0)
  {
    Serial.println(F("failed to send request"));
    client.stop();
    return false;
  }
  //delay(100);
  // Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.1 200 OK") != 0)
  {
    Serial.print(F("unexpected response: \""));
    Serial.print(status);
    Serial.println('\"');
    client.stop();
    return false;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders))
  {
    Serial.println(F("Invalid response"));
    client.stop();
    return false;
  }

  // This is probably not needed for most, but I had issues
  // with the Tindie api where sometimes there were random
  // characters coming back before the body of the response.
  // This will cause no hard to leave it in
  // peek() will look at the character, but not take it off the queue
  while (client.available() && client.peek() != '{')
  {
    char c = 0;
    client.readBytes(&c, 1);
  }

  DynamicJsonDocument roomDocument(CONFIG_DOCUMENT_SIZE);
  DeserializationError roomDeserializationError = deserializeJson(roomDocument, client);

  if(not roomDeserializationError)
  {
    Serial.println(F("OK"));
    //Update frequency
    if(roomDocument["updateFrequency"] != nullptr)
    {
      uint32_t receivedUpdateFrequency = roomDocument["updateFrequency"].as<uint32_t>();
      Serial.print(F("Update frequency: "));
      Serial.print(receivedUpdateFrequency/1000);
      Serial.print('s');
      if(receivedUpdateFrequency != updateFrequency)
      {
        installationChanged = true;
        updateFrequency = receivedUpdateFrequency;
        Serial.println(F(" - changed"));
      }
      else
      {
        Serial.println();
      }
    }
    //Adjust Neopixels
    #ifdef useNeopixelRing
    if(roomDocument["neopixelMaxBrightness"] != nullptr)
    {
      uint8_t receivedNeopixelMaxBrightness = roomDocument["neopixelMaxBrightness"].as<uint8_t>();
      Serial.print(F("Neopixel max brightness: "));
      Serial.print(receivedNeopixelMaxBrightness);
      if(receivedNeopixelMaxBrightness != neopixelMaxBrightness)
      {
        installationChanged = true;
        neopixelMaxBrightness = receivedNeopixelMaxBrightness;
        Serial.println(F(" - changed"));
      }
      else
      {
        Serial.println();
      }
    }
    if(roomDocument["neopixelRed"] != nullptr)
    {
      uint8_t receivedNeopixelRed = roomDocument["neopixelRed"].as<uint8_t>();
      Serial.print(F("Neopixel red value: "));
      Serial.print(receivedNeopixelRed);
      if(receivedNeopixelRed != neopixelRed)
      {
        installationChanged = true;
        neopixelRed = receivedNeopixelRed;
        Serial.println(F(" - changed"));
      }
      else
      {
        Serial.println();
      }
    }
    if(roomDocument["neopixelGreen"] != nullptr)
    {
      uint8_t receivedNeopixelGreen = roomDocument["neopixelGreen"].as<uint8_t>();
      Serial.print(F("Neopixel green value: "));
      Serial.print(receivedNeopixelGreen);
      if(receivedNeopixelGreen != neopixelGreen)
      {
        installationChanged = true;
        neopixelGreen = receivedNeopixelGreen;
        Serial.println(F(" - changed"));
      }
      else
      {
        Serial.println();
      }
    }
    if(roomDocument["neopixelBlue"] != nullptr)
    {
      uint8_t receivedNeopixelBlue = roomDocument["neopixelBlue"].as<uint8_t>();
      Serial.print(F("Neopixel blue value: "));
      Serial.print(receivedNeopixelBlue);
      if(receivedNeopixelBlue != neopixelBlue)
      {
        installationChanged = true;
        neopixelBlue = receivedNeopixelBlue;
        Serial.println(F(" - changed"));
      }
      else
      {
        Serial.println();
      }
    }
    if(roomDocument["neopixelAnimationRate"] != nullptr)
    {
      uint32_t receivedNeopixelAnimationRate = roomDocument["neopixelAnimationRate"].as<uint32_t>();
      Serial.print(F("Neopixel animation rate: "));
      Serial.print(receivedNeopixelAnimationRate);
      if(receivedNeopixelAnimationRate != neopixelAnimationRate)
      {
        installationChanged = true;
        neopixelAnimationRate = receivedNeopixelAnimationRate;
        Serial.println(F(" - changed"));
      }
      else
      {
        Serial.println();
      }
    }
    if(roomDocument["neopixelUpdatePattern"] != nullptr)
    {
      uint8_t receivedNeopixelUpdatePattern = roomDocument["neopixelUpdatePattern"].as<uint8_t>();
      Serial.print(F("Neopixel animation during update: "));
      Serial.print(receivedNeopixelUpdatePattern);
      if(receivedNeopixelUpdatePattern != neopixelUpdatePattern)
      {
        installationChanged = true;
        neopixelUpdatePattern = receivedNeopixelUpdatePattern;
        Serial.println(F(" - changed"));
      }
      else
      {
        Serial.println();
      }
    }
    polybiusOrange = CRGB(neopixelRed,neopixelGreen,neopixelBlue);
    #endif
    #ifdef useLedHeartbeat
    //Adjust LED PWM
    if(roomDocument["ledPwmMaxBrightness"] != nullptr)
    {
      uint16_t receivedLedPwmMaxBrightness = roomDocument["ledPwmMaxBrightness"].as<uint16_t>();
      Serial.print(F("LED PWM max brightness: "));
      Serial.print(receivedLedPwmMaxBrightness);
      if(receivedLedPwmMaxBrightness != ledPwmMaxBrightness)
      {
        installationChanged = true;
        ledPwmMaxBrightness = receivedLedPwmMaxBrightness;
        Serial.println(F(" - changed"));
      }
      else
      {
        Serial.println();
      }
    }
    if(roomDocument["ledPwmChangeInterval"] != nullptr)
    {
      uint32_t receivedLedPwmChangeInterval = roomDocument["ledPwmChangeInterval"].as<uint32_t>();
      Serial.print(F("LED PWM change interval: "));
      Serial.print(receivedLedPwmChangeInterval);
      if(receivedLedPwmChangeInterval != ledPwmChangeInterval)
      {
        installationChanged = true;
        ledPwmChangeInterval = receivedLedPwmChangeInterval;
        Serial.println(F(" - changed"));
      }
      else
      {
        Serial.println();
      }
    }
    #endif
    //Cycle duration
    if(roomDocument["installationCycleDuration"] != nullptr)
    {
      uint32_t receivedInstallationCycleDuration = roomDocument["installationCycleDuration"].as<uint32_t>();
      Serial.print(F("Installation cycle duration: "));
      Serial.print(receivedInstallationCycleDuration);
      if(receivedInstallationCycleDuration != installationCycleDuration)
      {
        installationChanged = true;
        installationCycleDuration = receivedInstallationCycleDuration;
        Serial.println(F(" - changed"));
      }
      else
      {
        Serial.println();
      }
    }
    //Cursor animation
    if(roomDocument["titleCursorAnimationDelay"] != nullptr)
    {
      uint32_t receivedTitleCursorAnimationDelay = roomDocument["titleCursorAnimationDelay"].as<uint32_t>();
      Serial.print(F("Title cursor animation delay: "));
      Serial.print(receivedTitleCursorAnimationDelay);
      if(receivedTitleCursorAnimationDelay != titleCursorAnimationDelay)
      {
        installationChanged = true;
        titleCursorAnimationDelay = receivedTitleCursorAnimationDelay;
        Serial.println(F(" - changed"));
      }
      else
      {
        Serial.println();
      }
    }
    if(roomDocument["infoCursorAnimationDelay"] != nullptr)
    {
      uint32_t receivedInfoCursorAnimationDelay = roomDocument["infoCursorAnimationDelay"].as<uint32_t>();
      Serial.print(F("Info text cursor animation delay: "));
      Serial.print(receivedInfoCursorAnimationDelay);
      if(receivedInfoCursorAnimationDelay != infoCursorAnimationDelay)
      {
        installationChanged = true;
        infoCursorAnimationDelay = receivedInfoCursorAnimationDelay;
        Serial.println(F(" - changed"));
      }
      else
      {
        Serial.println();
      }
    }
    //Room title
    if(roomDocument["room-title"] != nullptr)
    {
      String receivedRoomTitle = roomDocument["room-title"].as<String>();
      Serial.print(F("Room title: "));
      Serial.print(receivedRoomTitle);
      if(receivedRoomTitle.equals(roomTitle) == false)
      {
        installationChanged = true;
        roomTitle = receivedRoomTitle;
        Serial.println(F(" - changed"));
      }
      else
      {
        Serial.println();
      }
    }
    //Installations
    if(roomDocument["installations"] != nullptr)
    {
      uint8_t receivedNumberOfInstallations = roomDocument["installations"].size();
      Serial.print(F("Installations: "));
      Serial.print(receivedNumberOfInstallations);    
      if(receivedNumberOfInstallations != numberOfInstallations)
      {
        installationChanged = true;
        numberOfInstallations = receivedNumberOfInstallations;
        if(installations != nullptr)
        {
          delete [] installations;
          installations = nullptr;
        }
        Serial.println(F(" - changed"));
      }
      else
      {
        Serial.println();
      }
      if(receivedNumberOfInstallations > 0)
      {
        uint8_t counter = 0;
        if(installations == nullptr)
        {
          installations = new installationInformation[receivedNumberOfInstallations];
        }
        JsonArray installationDocumentArray = roomDocument["installations"].as<JsonArray>();
        for(JsonVariant installation : installationDocumentArray)
        {
          DynamicJsonDocument installationDocument(2048);
          DeserializationError installationDeserializationError = deserializeJson(installationDocument, installation.as<String>());
          if(not installationDeserializationError)
          {
            if(counter < numberOfInstallations)
            {
              if(installationDocument["title"] != nullptr)
              {
                String receivedTitle = installationDocument["title"].as<String>();
                Serial.print(F("\tInstallation "));
                Serial.print(counter);
                Serial.print(F(": "));
                Serial.print(receivedTitle);
                if(receivedTitle.equals(installations[counter].title) == false)
                {
                  installationChanged = true;
                  installations[counter].title = receivedTitle;
                  Serial.println(F(" - changed"));
                }
                else
                {
                  Serial.println();
                }
              }
              if(installationDocument["atribution"] != nullptr)
              {
                String receivedAttribution = installationDocument["atribution"].as<String>();
                Serial.print(F("\tAttribution "));
                Serial.print(counter);
                Serial.print(F(": "));
                Serial.print(receivedAttribution);
                if(receivedAttribution.equals(installations[counter].attribution) == false)
                {
                  installationChanged = true;
                  installations[counter].attribution = receivedAttribution;
                  Serial.println(F(" - changed"));
                }
                else
                {
                  Serial.println();
                }
              }
              if(installationDocument["info-text"] != nullptr)
              {
                String receivedInfotext = installationDocument["info-text"].as<String>();
                Serial.print(F("\tInformation "));
                Serial.print(counter);
                Serial.print(F(": "));
                Serial.print(receivedInfotext);
                if(receivedInfotext.equals(installations[counter].infotext) == false)
                {
                  installationChanged = true;
                  installations[counter].infotext = receivedInfotext;
                  Serial.println(F(" - changed"));
                }
                else
                {
                  Serial.println();
                }
              }
              counter++;
            }
          }
          else
          {
            Serial.print(F("deserializeJson() of Installation object failed: "));
            Serial.println(installationDeserializationError.f_str());
          }
        }
        if(counter == numberOfInstallations)
        {
          client.stop();
        }
        else
        {
          installationChanged = false;
          Serial.println(F("Installation count mismatch in JSON parsing"));
          client.stop();
          return false;
        }
      }
    }
    if(installationChanged == true)
    {
      installationChanged = false;
      saveInstallationInformation(installationInformationFile);
    }
    else
    {
      Serial.println(F("Installation information unchanged"));
    }
    return true;
  }
  else
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(roomDeserializationError.f_str());
    client.stop();
    return false;
  }
  return false;
}
