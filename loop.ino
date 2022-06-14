void loop()
{
  //button.tick();
  manageServerUpdates();
  manageAnimation();
  #ifdef USE_VIDEO
    manageVideo();
  #endif
  #ifdef useNeopixelRing
    manageNeopixelRing(nullptr);
  #endif
  #ifdef useLedHeartbeat
    manageLogoPWM(nullptr);
  #endif
}
