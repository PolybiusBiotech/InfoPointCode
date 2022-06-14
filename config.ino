bool saveInstallationInformation(char* path)
{
  Serial.print(F("Saving installation information("));
  Serial.print(path);
  Serial.print(F("): "));
  File file = LittleFS.open(path, "w");
  if(file)
  {
    StaticJsonDocument<CONFIG_DOCUMENT_SIZE> doc;
    doc["updateFrequency"] = updateFrequency;
    #ifdef useNeopixelRing
    doc["neopixelMaxBrightness"] = neopixelMaxBrightness;
    doc["neopixelAnimationRate"] = neopixelAnimationRate;
    doc["neopixelRed"] = neopixelRed;
    doc["neopixelGreen"] = neopixelGreen;
    doc["neopixelBlue"] = neopixelBlue;
    doc["neopixelUpdatePattern"] = neopixelUpdatePattern;
    #endif
    #ifdef useLedHeartbeat
    doc["ledPwmChangeInterval"] = ledPwmChangeInterval;
    doc["ledPwmMaxBrightness"] = ledPwmMaxBrightness;
    #endif
    doc["installationCycleDuration"] = installationCycleDuration;
    doc["titleCursorAnimationDelay"] = titleCursorAnimationDelay;
    doc["infoCursorAnimationDelay"] = infoCursorAnimationDelay;
    doc["roomTitle"] = roomTitle;
    JsonArray array = doc.createNestedArray("installations");
    //JsonArray array = doc.to<JsonArray>("installations");
    for(uint8_t i = 0; i < numberOfInstallations; i++)
    {
      JsonObject nested = array.createNestedObject();
      nested["title"] = installations[i].title;
      nested["atribution"] = installations[i].attribution;
      nested["info-text"] = installations[i].infotext;
    }
    if (serializeJson(doc, file) == 0)
    {
      Serial.println(F("failed, unable to serialize"));
      file.close();
      return false;
    }
    else
    {
      Serial.println(F("OK"));
      file.close();
      //serializeJson(doc, Serial);
      return true;
    }
  }
  else
  {
    Serial.println(F("failed to open file for writing"));
  }
  return false;
}

bool loadInstallationInformation(char* path)
{
  Serial.print(F("Loading Installation configuration("));
  Serial.print(path);
  Serial.print(F("): "));
  File file = LittleFS.open(path, "r");
  if(file)
  {
    StaticJsonDocument<CONFIG_DOCUMENT_SIZE> doc;  //Make space for the document
    DeserializationError error = deserializeJson(doc, file);
    if(error != DeserializationError::Ok) {
      file.close();
      Serial.println(F("failed to deserialize file"));
      return false;
    }
    else
    {
      Serial.println(F("OK"));
      if(doc["updateFrequency"] != nullptr) {
        updateFrequency = doc["updateFrequency"].as<uint32_t>();
        Serial.print(F("Update frequency: "));
        Serial.println(updateFrequency);
      }
      #ifdef useNeopixelRing
      if(doc["neopixelMaxBrightness"] != nullptr)
      {
        neopixelMaxBrightness = doc["neopixelMaxBrightness"];
        Serial.print(F("Neopixel max brightness: "));
        Serial.println(neopixelMaxBrightness);
      }
      if(doc["neopixelRed"] != nullptr)
      {
        neopixelRed = doc["neopixelRed"];
        Serial.print(F("Neopixel red value: "));
        Serial.println(neopixelRed);
      }
      if(doc["neopixelGreen"] != nullptr)
      {
        neopixelGreen = doc["neopixelGreen"];
        Serial.print(F("Neopixel green value: "));
        Serial.println(neopixelGreen);
      }
      if(doc["neopixelBlue"] != nullptr)
      {
        neopixelBlue = doc["neopixelBlue"];
        Serial.print(F("Neopixel blue value: "));
        Serial.println(neopixelBlue);
      }
      polybiusOrange = CRGB(neopixelRed,neopixelGreen,neopixelBlue);
      if(doc["neopixelAnimationRate"] != nullptr)
      {
        neopixelAnimationRate = doc["neopixelAnimationRate"];
        Serial.print(F("Neopixel animate rate: "));
        Serial.println(neopixelAnimationRate);
      }
      if(doc["neopixelUpdatePattern"] != nullptr)
      {
        neopixelUpdatePattern = doc["neopixelUpdatePattern"];
        Serial.print(F("Neopixel animation for update: "));
        Serial.println(neopixelUpdatePattern);
      }
      #endif
      #ifdef useLedHeartbeat
      if(doc["ledPwmChangeInterval"] != nullptr)
      {
        ledPwmChangeInterval = doc["ledPwmChangeInterval"];
        Serial.print(F("LED PWM change interval: "));
        Serial.println(ledPwmChangeInterval);
      }
      #endif
      if(doc["installationCycleDuration"] != nullptr) {
        installationCycleDuration = doc["installationCycleDuration"].as<uint32_t>();
        Serial.print(F("Installation cycle duration: "));
        Serial.println(installationCycleDuration);
      }
      if(doc["titleCursorAnimationDelay"] != nullptr) {
        titleCursorAnimationDelay = doc["titleCursorAnimationDelay"].as<uint32_t>();
        Serial.print(F("Title cursor animation delay: "));
        Serial.println(titleCursorAnimationDelay);
      }
      if(doc["infoCursorAnimationDelay"] != nullptr) {
        infoCursorAnimationDelay = doc["infoCursorAnimationDelay"].as<uint32_t>();
        Serial.print(F("Information cursor animation delay: "));
        Serial.println(infoCursorAnimationDelay);
      }
      if(doc["roomTitle"] != nullptr)
      {
        roomTitle = doc["roomTitle"].as<String>();
        Serial.print(F("Room title: "));
        Serial.println(roomTitle);
      }
      if(doc["installations"] != nullptr)
      {
        numberOfInstallations = doc["installations"].size();
        Serial.print(F("Installations: "));
        Serial.println(numberOfInstallations);    
        if(numberOfInstallations > 0)
        {
          uint8_t counter = 0;
          if(installations == nullptr)
          {
            installations = new installationInformation[numberOfInstallations];
          }
          JsonArray installationDocumentArray = doc["installations"].as<JsonArray>();
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
                  installations[counter].title = installationDocument["title"].as<String>();
                  Serial.print(F("\tInstallation "));
                  Serial.print(counter);
                  Serial.print(F(": "));
                  Serial.println(installations[counter].title);
                }
                if(installationDocument["atribution"] != nullptr)
                {
                  installations[counter].attribution = installationDocument["atribution"].as<String>();
                  Serial.print(F("\tAttribution "));
                  Serial.print(counter);
                  Serial.print(F(": "));
                  Serial.println(installations[counter].attribution);
                }
                if(installationDocument["info-text"] != nullptr)
                {
                  installations[counter].infotext = installationDocument["info-text"].as<String>();
                  Serial.print(F("\tInformation "));
                  Serial.print(counter);
                  Serial.print(F(": "));
                  Serial.println(installations[counter].infotext);
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
        }
      }
      file.close();
      return true;
    }
  }
  else
  {
    Serial.println(F("failed to open file"));
  }
  return false;
}
