#ifdef useNeopixelRing
  void manageNeopixelRing(void *data)
  {
    if (neopixelPattern == 0)
    {
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = CRGB::Black;
      }
    }
    else if (neopixelPattern == 1)
    {
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = CRGB::Black;
        leds[i].r = (uint8_t)(pow(sin(((millis()/1000) * 1.5) + (i * 0.4)) * 0.5 + 0.5, 1.5) * neopixelRed);
        leds[i].g = (uint8_t)(pow(sin(((millis()/1000) * 1.5) + (i * 0.4)) * 0.5 + 0.5, 1.5) * neopixelGreen);
        leds[i].b = (uint8_t)(pow(sin(((millis()/1000) * 1.5) + (i * 0.4)) * 0.5 + 0.5, 1.5) * neopixelBlue);
      }
    }
    else if (neopixelPattern == 2)
    {
      auto m = millis();
      m = (m - (m % 1000)) / 1000;
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = cols[m % 6];
      }
    }
    else if (neopixelPattern == 3)
    {
      static float nextDrop = 0;
      if (changed)
      {
        randomSeed(micros());
      }
      if (millis() - lastMove > nextDrop)
      {
        lastMove = millis();
        int dc = random(1, 4);
        for (int n = 0; n < dc; n++)
        {
          int led = random(0, NUM_LEDS);
          leds[led] = polybiusOrange;
        }
        nextDrop = random(0, 200) + 20;
      }
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i].r = (uint8_t)max(0.0, leds[i].r * 0.85);
        leds[i].g = (uint8_t)max(0.0, leds[i].g * 0.85);
        leds[i].b = (uint8_t)max(0.0, leds[i].b * 0.85);
      }
    }
    else if (neopixelPattern == 4)
    {
      static float nextChange = 0;
      if (nextChange == 0)
      {
        for (int i = 0; i < NUM_LEDS; i++)
        {
          leds[i] = CRGB::Black;
        }
      }
      if(millis() - nextChange > 250)
      {
        nextChange = millis();
        int start = random(0, NUM_LEDS);
        auto col = cols[random(0, 6)];
        for (int i = start; i < start + 4; i++)
        {
          leds[i % NUM_LEDS] = col;
        }
      }
    }
    else if (neopixelPattern == 5)
    {
      unsigned long m = millis();
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i].r = 0;
        leds[i].b = 0;
        leds[i].g = ((m >> i) & 1) * 255;
      }
    }
    else if (neopixelPattern == 6)
    {
      unsigned long m = millis();
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = CHSV(((i * 8) + (m / 32)) % 256, 255, 255);
      }
    }
    else if (neopixelPattern == 7)
    {
      if (millis() - lastMove >= neopixelAnimationRate)
      {
        lastMove = millis();
        for (int i = 0; i < NUM_LEDS; i++)
        {
          leds[i] = CRGB::Black;
        }
        if (ln == 0)
          ln = NUM_LEDS;
        ln--;
        //leds[ln] = CRGB::White;
        //leds[ln] = cols[installationToDisplay%6];
        //leds[ln] = polybiusOrange;
        leds[ln] = CRGB::DarkOrange;
      }
    }
    FastLED.show();
  }
#endif
