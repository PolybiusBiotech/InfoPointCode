#ifdef useLedHeartbeat
  void manageLogoPWM(void* data)
  {
    if(millis() - lastLedPwmChange > ledPwmChangeInterval)
    {
      lastLedPwmChange = millis();
      if(ledPwmMaxBrightness == 0)
      {
        ledcWrite(0, 0);
      }
      else
      {
        if(dir)
        {
          currentPwmBrightness += ledPwmMaxBrightness / 4;
          if(currentPwmBrightness >= ledPwmMaxBrightness)
          {
            currentPwmBrightness = ledPwmMaxBrightness;
            dir = false;
          }
        }
        else
        {
          if(currentPwmBrightness > 0)
          {
            currentPwmBrightness -= ledPwmMaxBrightness / 32;
          }
          if(currentPwmBrightness <= 0)
          {
            currentPwmBrightness = 0;
            if(ledPulsing == true)
            {
              dir = true;
            }
          }
        }
        ledcWrite(0, max(0,min(4095, currentPwmBrightness)));
      }
    }
  }
#endif
