#ifdef USE_VIDEO
  void videoSetup()
  {
    Serial.println(F("Configuring composite video"));
    videoOut.begin();
    boxWidth = videoOut.width() - boxMargin * 2;
    canvasWidth = videoOut.width() - boxMargin * 4;
    size3wrapWidth = canvasWidth / (fontWidth * 3);
    size2wrapWidth = canvasWidth / (fontWidth * 2);
    size1wrapWidth = canvasWidth / fontWidth;
    box4height = videoOut.height() - (box1height + box2height + box3height + topMargin + boxMargin * 5);
    canvas4height = box4height - (boxMargin * 2);
    videoOut.fillScreen(0);
    videoOut.setCursor(0,20);
    videoOut.setTextSize(1);
    videoOut.setTextWrap(false);
    videoOut.setTextColor(0xFFFF);
    videoOut.waitForFrame();
    Serial.println(F("Composite video started"));
  }
  
  void manageVideo()
  {
    videoOut.waitForFrame();
    videoOut.fillScreen(0);
    videoOut.drawRect(0, topMargin, videoOut.width(), videoOut.height() - topMargin, 0xFFFF);
    videoOut.drawRect(boxMargin, box1y, boxWidth, box1height, 0xFFFF);
    videoOut.drawRect(boxMargin, box2y, boxWidth, box2height, 0xFFFF);
    videoOut.drawRect(boxMargin, box3y, boxWidth, box3height, 0xFFFF);
    videoOut.drawRect(boxMargin, box4y, boxWidth, box4height, 0xFFFF);
    videoOut.setTextColor(0xFFFF);
    if(numberOfInstallations > 0)
    {
      //Title
      if(animationStage == ANIMATE_ROOM_TITLE)
      {
        if(cursorAnimationPoint > 0 && cursorAnimationPoint < roomTitle.length())
        {
          GFXcanvas1 canvas(canvasWidth, canvas1height);
          canvas.setTextSize(3);
          canvas.print(roomTitle.substring(0, cursorAnimationPoint));
          if(cursorState)
          {
            canvas.setTextColor(0xFFFF);
            canvas.print('_');
          }
          else
          {
            canvas.setTextColor(0x0000,0xFFFF);
            canvas.print(' ');
            canvas.setTextColor(0xFFFF);
          }
          videoOut.drawBitmap(canvasMargin, canvas1y, canvas.getBuffer(), canvasWidth, canvas1height, 0xffff, 0x0000);
        }
        else if(roomTitle.length() == cursorAnimationPoint)
        {
          GFXcanvas1 canvas(canvasWidth, canvas1height);
          canvas.setTextSize(3);
          canvas.print(roomTitle);
          if(cursorState)
          {
            canvas.setTextColor(0xFFFF);
            canvas.print('_');
          }
          else
          {
            canvas.setTextColor(0x0000,0xFFFF);
            canvas.print(' ');
            canvas.setTextColor(0xFFFF);
          }
          videoOut.drawBitmap(canvasMargin, canvas1y, canvas.getBuffer(), canvasWidth, canvas1height, 0xffff, 0x0000);
          animationStage++;
          cursorAnimationPoint = 0;
          cursorAnimationTimer = millis();
        }
      }
      else if(animationStage > ANIMATE_ROOM_TITLE)
      {
        GFXcanvas1 canvas(canvasWidth, canvas1height);
        canvas.setTextSize(3);
        canvas.print(roomTitle);
        videoOut.drawBitmap(canvasMargin, canvas1y, canvas.getBuffer(), canvasWidth, canvas1height, 0xffff, 0x0000);
      }
      if(animationStage == ANIMATE_INSTALLATION_TITLE)
      {
        if(cursorAnimationPoint > 0 && cursorAnimationPoint < installations[installationToDisplay].title.length())
        {
          GFXcanvas1 canvas(canvasWidth, canvas2height);
          canvas.setTextSize(2);
          canvas.print(wordwrap.wrap(installations[installationToDisplay].title.substring(0, cursorAnimationPoint),size2wrapWidth));
          if(cursorState)
          {
            canvas.setTextColor(0xFFFF);
            canvas.print('_');
          }
          else
          {
            canvas.setTextColor(0x0000,0xFFFF);
            canvas.print(' ');
            canvas.setTextColor(0xFFFF);
          }
          videoOut.drawBitmap(canvasMargin, canvas2y, canvas.getBuffer(), canvasWidth, canvas2height, 0xffff, 0x0000);
        }
        else if(cursorAnimationPoint >= installations[installationToDisplay].title.length())
        {
          GFXcanvas1 canvas(canvasWidth, canvas2height);
          canvas.setTextSize(2);
          canvas.print(wordwrap.wrap(installations[installationToDisplay].title,size2wrapWidth));
          if(cursorState)
          {
            canvas.setTextColor(0xFFFF);
            canvas.print('_');
          }
          else
          {
            canvas.setTextColor(0x0000,0xFFFF);
            canvas.print(' ');
            canvas.setTextColor(0xFFFF);
          }
          videoOut.drawBitmap(canvasMargin, canvas2y, canvas.getBuffer(), canvasWidth, canvas2height, 0xffff, 0x0000);
          animationStage++;
          cursorAnimationPoint = 0;
          cursorAnimationTimer = millis();
        }
      }
      else if(animationStage > ANIMATE_INSTALLATION_TITLE)
      {
        GFXcanvas1 canvas(canvasWidth, canvas2height);
        canvas.setTextSize(2);
        canvas.print(wordwrap.wrap(installations[installationToDisplay].title,size2wrapWidth));
        //canvas.print(wordwrap.wrap("Installation AB is all the rage right now",size2wrapWidth));
        //canvas.print(installations[installationToDisplay].title);
        //canvas.print("Hello");
        videoOut.drawBitmap(canvasMargin, canvas2y, canvas.getBuffer(), canvasWidth, canvas2height, 0xffff, 0x0000);
      }
      //Attribution
      if(animationStage == ANIMATE_INSTALLATION_ATTRIBUTION)
      {
        if(cursorAnimationPoint > 0 && cursorAnimationPoint < installations[installationToDisplay].attribution.length())
        {
          GFXcanvas1 canvas(canvasWidth, canvas3height);
          canvas.setTextSize(2);
          canvas.print(wordwrap.wrap(installations[installationToDisplay].attribution.substring(0, cursorAnimationPoint),size2wrapWidth));
          if(cursorState)
          {
            canvas.setTextColor(0xFFFF);
            canvas.print('_');
          }
          else
          {
            canvas.setTextColor(0x0000,0xFFFF);
            canvas.print(' ');
            canvas.setTextColor(0xFFFF);
          }
          videoOut.drawBitmap(canvasMargin, canvas3y, canvas.getBuffer(), canvasWidth, canvas3height, 0xffff, 0x0000);
        }
        else if(cursorAnimationPoint >= installations[installationToDisplay].attribution.length())
        {
          GFXcanvas1 canvas(canvasWidth, canvas3height);
          canvas.setTextSize(2);
          canvas.print(wordwrap.wrap(installations[installationToDisplay].attribution,size2wrapWidth));
          if(cursorState)
          {
            canvas.setTextColor(0xFFFF);
            canvas.print('_');
          }
          else
          {
            canvas.setTextColor(0x0000,0xFFFF);
            canvas.print(' ');
            canvas.setTextColor(0xFFFF);
          }
          videoOut.drawBitmap(canvasMargin, canvas3y, canvas.getBuffer(), canvasWidth, canvas3height, 0xffff, 0x0000);
          animationStage++;
          cursorAnimationPoint = 0;
          cursorAnimationSpeed = infoCursorAnimationDelay;
        }
      }
      else if(animationStage > ANIMATE_INSTALLATION_ATTRIBUTION)
      {
        GFXcanvas1 canvas(canvasWidth, canvas3height);
        canvas.setTextSize(2);
        canvas.println(wordwrap.wrap(installations[installationToDisplay].attribution,size2wrapWidth));
        videoOut.drawBitmap(canvasMargin, canvas3y, canvas.getBuffer(), canvasWidth, canvas3height, 0xffff, 0x0000);
      }
      //Info text
      if(animationStage == ANIMATE_INSTALLATION_INFOTEXT)
      {
        if(cursorAnimationPoint > 0 && cursorAnimationPoint < installations[installationToDisplay].infotext.length())
        {
          GFXcanvas1 canvas(canvasWidth, canvas4height);
          canvas.setTextSize(1);
          canvas.print(wordwrap.wrap(installations[installationToDisplay].infotext.substring(0, cursorAnimationPoint),size1wrapWidth));
          if(cursorState)
          {
            canvas.setTextColor(0xFFFF);
            canvas.print('_');
          }
          else
          {
            canvas.setTextColor(0x0000,0xFFFF);
            canvas.print(' ');
            canvas.setTextColor(0xFFFF);
          }
          videoOut.drawBitmap(canvasMargin, canvas4y, canvas.getBuffer(), canvasWidth, canvas4height, 0xffff, 0x0000);
        }
        else if(cursorAnimationPoint == installations[installationToDisplay].infotext.length())
        {
          GFXcanvas1 canvas(canvasWidth, canvas4height);
          canvas.setTextSize(1);
          canvas.print(wordwrap.wrap(installations[installationToDisplay].infotext,size1wrapWidth));
          if(cursorState)
          {
            canvas.setTextColor(0xFFFF);
            canvas.print('_');
          }
          else
          {
            canvas.setTextColor(0x0000,0xFFFF);
            canvas.print(' ');
            canvas.setTextColor(0xFFFF);
          }
          videoOut.drawBitmap(canvasMargin, canvas4y, canvas.getBuffer(), canvasWidth, canvas4height, 0xffff, 0x0000);
          animationStage++;
          cursorAnimationPoint = 0;
          cursorAnimationTimer = millis();
          installationCycleTimer = millis();
          //Serial.println(F("Info text done"));
          #ifdef useNeopixelRing
            neopixelPattern = 0;
          #endif
          #ifdef useLedHeartbeat
            ledPulsing = true;
          #endif
        }
      }
      else if(animationStage > ANIMATE_INSTALLATION_INFOTEXT)
      {
        GFXcanvas1 canvas(canvasWidth, canvas4height);
        canvas.setTextSize(1);
        canvas.print(wordwrap.wrap(installations[installationToDisplay].infotext,size1wrapWidth));
        videoOut.drawBitmap(canvasMargin, canvas4y, canvas.getBuffer(), canvasWidth, canvas4height, 0xffff, 0x0000);
      }
    }
    else
    {
      videoOut.setCursor(0,20);
      videoOut.setTextSize(3);
      videoOut.print(wordwrap.wrap(F("No information"),size3wrapWidth));
      videoOut.setCursor(0,80);
      videoOut.setTextSize(2);
      videoOut.print(wordwrap.wrap(F("Awaiting server connection"),size2wrapWidth));
      videoOut.setCursor(0,120);
      videoOut.setTextSize(1);
      videoOut.print(wordwrap.wrap(F("Please alert your local Polybius Infopoint service technician"),size1wrapWidth));
    }
  }
  /*void printCursor()
  {
    if(cursorState)
    {
      videoOut.setTextColor(0xFFFF);
      videoOut.print('_');
    }
    else
    {
      videoOut.setTextColor(0x0000,0xFFFF);
      videoOut.print(' ');
      videoOut.setTextColor(0xFFFF);
    }
  }*/
  /*void printCursor(GFXcanvas1& cursorCanvas)
  {
    if(cursorState)
    {
      cursorCanvas.setTextColor(0xFFFF);
      cursorCanvas.print('_');
    }
    else
    {
      cursorCanvas.setTextColor(0x0000,0xFFFF);
      cursorCanvas.print(' ');
      cursorCanvas.setTextColor(0xFFFF);
    }
  }*/
#endif
