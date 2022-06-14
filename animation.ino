void manageAnimation()
{
  if(numberOfInstallations > 0)
  {
    //Cycle the installations
    if(animationStage == ANIMATE_INSTALLATION_FINISHED && millis() - installationCycleTimer > installationCycleDuration)
    {
      installationCycleTimer = millis();
      cursorAnimationTimer = millis();
      cursorAnimationPoint = 0;
      cursorAnimationSpeed = titleCursorAnimationDelay;
      animationStage = ANIMATE_ROOM_TITLE;
      installationToDisplay++;
      if(installationToDisplay == numberOfInstallations)
      {
        installationToDisplay = 0;
      }
      //Serial.println(F("Next installation"));
      #ifdef useNeopixelRing
        neopixelPattern = neopixelUpdatePattern;
      #endif
      #ifdef useLedHeartbeat
        ledPulsing = false;
      #endif
    }
    if(millis() - cursorAnimationTimer > cursorAnimationSpeed)
    {
      cursorAnimationTimer = millis();
      cursorAnimationPoint++;
    }
    if(millis() - lastCursorBlink > cursorBlinkSpeed)
    {
      lastCursorBlink = millis();
      cursorState = not cursorState;
    }
  }
}
