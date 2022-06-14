#define VALIDATE_CERT
#define USE_VIDEO
#define useNeopixelRing
#define useLedHeartbeat

//#define USE_EMF_INSECURE_WIFI
//#define USE_EMF_WIFI
//#define USE_EMF_OUTBOUND_ONLY_WIFI
//#define CHECK_8021x_CERTIFICATE
#define DISCONNECT_WIFI_WHEN_IDLE

//WiFi
#include <WiFi.h>
#if defined(USE_EMF_WIFI) || defined(USE_EMF_OUTBOUND_ONLY_WIFI)
  #include <esp_wpa2.h>
  #include <esp_wifi.h>
  #ifdef CHECK_8021x_CERTIFICATE
static const char emfcamp_ca[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFJDCCBAygAwIBAgISBBnPjkPYv7hC+hnAtrXHlt8NMA0GCSqGSIb3DQEBCwUA
MDIxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MQswCQYDVQQD
EwJSMzAeFw0yMjA1MTUwODQ5MDlaFw0yMjA4MTMwODQ5MDhaMBoxGDAWBgNVBAMT
D3JhZGl1cy5lbWYuY2FtcDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB
AKAV8PpGNlD9QLoRpTb/IrS80M3YRp10CndzGL7R6QKsSdrwuusXxhzcZ7Vbuh7k
wo8XeNohgKEiOzo2OqUO8jHkQn4QVwrYWzuJg4PCWJb9+BEDeW4kESHAL/TwHjiP
4+a/WdJ5lqPR872Slcm2oR/9QQ6ikMH7HN6/zbfbdoKDABFhcYrphaxqsV5qsCyu
WgXLLHG6B2bokDdAr2WWC/wq/1p0nXS0WsIQ1QwnWMi/Y3AIDN+23p8fBgrlJ3l3
kiQvgbutSczFijdKvySR+WHHhMOmw6KbCUFvBN3orfU59mNVIYrVdFjSMCcVyQvd
0bVs5z11Vkxpx+MhTzYCJtkCAwEAAaOCAkowggJGMA4GA1UdDwEB/wQEAwIFoDAd
BgNVHSUEFjAUBggrBgEFBQcDAQYIKwYBBQUHAwIwDAYDVR0TAQH/BAIwADAdBgNV
HQ4EFgQULjFedhwPiuznS/srdlfhIejUY78wHwYDVR0jBBgwFoAUFC6zF7dYVsuu
UAlA5h+vnYsUwsYwVQYIKwYBBQUHAQEESTBHMCEGCCsGAQUFBzABhhVodHRwOi8v
cjMuby5sZW5jci5vcmcwIgYIKwYBBQUHMAKGFmh0dHA6Ly9yMy5pLmxlbmNyLm9y
Zy8wGgYDVR0RBBMwEYIPcmFkaXVzLmVtZi5jYW1wMEwGA1UdIARFMEMwCAYGZ4EM
AQIBMDcGCysGAQQBgt8TAQEBMCgwJgYIKwYBBQUHAgEWGmh0dHA6Ly9jcHMubGV0
c2VuY3J5cHQub3JnMIIBBAYKKwYBBAHWeQIEAgSB9QSB8gDwAHYAb1N2rDHwMRnY
mQCkURX/dxUcEdkCwQApBo2yCJo32RMAAAGAxx8oKwAABAMARzBFAiBbc8sg4aKi
2RJC0XNL2UY4UoGRLZ4em8XxHElzPgWO7wIhAPN7TtfNf/azaut+DannCLpHPjdM
+gDLYWNWGhFD62qdAHYARqVV63X6kSAwtaKJafTzfREsQXS+/Um4havy/HD+bUcA
AAGAxx8oKwAABAMARzBFAiBVbHpie7fd3uEvxu4oAojYTRfFQmw1Yc6T5YWd8gRE
0QIhAKTayluC4LHoboWhvlnT+b102HBPBe0vWlr2kop91+3BMA0GCSqGSIb3DQEB
CwUAA4IBAQCrl1JAebDBegRSJHxkwiUL1TyZg8QY2FsB8W4ZtIaNOWCKgCDqlFxj
rgY8LaWJW9896bQdEDEx+/geic3TuvX71s73t3+qTe9iPXRDIrSEuCRa0pWS1f8E
PITJ+KNyTzbLcRAeDNlboqReAvVhZGatebBO9wR5p+ZaoGXwPm9nnKqu6goxoeRN
eoWrSvo3wstUZdQHFINHnQ7n9uxpVKRSliZJH+hnzFC2O6TIa5OVfwMbVBHSp7M4
7Za93O6Eo7792MfrMoJn927KgqeOayjzCSaONwdUTV/mKj4BD2MciT4xNe0bOogh
QnjqDgA6LkmS427SEvSnZ68T+IiAJ8aL
-----END CERTIFICATE-----
)EOF";
#endif
  #if defined(USE_EMF_WIFI)
    #define EAP_IDENTITY "emf"
    #define EAP_PASSWORD "emf"
  #endif
  #if defined(USE_EMF_OUTBOUND_ONLY_WIFI)
    #define EAP_IDENTITY "outboundonly"
    #define EAP_PASSWORD "outboundonly"
  #endif
#else
  //Get some starting credential in
  #include "credentials.h"
#endif
#include <WiFiClientSecure.h>
//Storage
#include "FS.h"
#include <LittleFS.h>
//JSON
#define CONFIG_DOCUMENT_SIZE 2048
#include <ArduinoJson.h>
//Composite video
#ifdef USE_VIDEO
  #include <ESP_8_BIT_GFX.h>
  // Create an instance of the graphics library
  ESP_8_BIT_GFX videoOut(false /* = NTSC */, 16 /* = RGB565 colors will be downsampled to 8-bit RGB332 */);
  #include <wordwrap.h>
#endif
//HTTPS client
WiFiClientSecure client;
//Configuration files
char installationInformationFile[] = "/installationInformation.json";
String URL = "https://raw.githubusercontent.com/PolybiusBiotech/screen-content/main/info-points/";
uint8_t macAddress[6];
char jsonFilename[18];
String ssid = "";
String psk = "";
uint32_t lastRetrieval = 0;
uint32_t updateFrequency = 30000;
uint32_t success = 0;
uint32_t fail = 0;
//Installation information
String roomTitle = "";
struct installationInformation {
  String title = "";
  String attribution = "";
  String infotext = "";
};
installationInformation* installations = nullptr;
uint8_t numberOfInstallations = 0;
uint8_t installationToDisplay = 0;
bool installationChanged = false;
//Animation
const uint8_t ANIMATE_ROOM_TITLE = 0;
const uint8_t ANIMATE_INSTALLATION_TITLE = 1;
const uint8_t ANIMATE_INSTALLATION_ATTRIBUTION = 2;
const uint8_t ANIMATE_INSTALLATION_INFOTEXT = 3;
const uint8_t ANIMATE_INSTALLATION_FINISHED = 4;
const uint8_t ANIMATE_INSTALLATION_UPDATING = 5;
uint8_t animationStage = ANIMATE_INSTALLATION_FINISHED;
uint32_t cursorAnimationTimer = 0;
uint32_t titleCursorAnimationDelay = 100;
uint32_t infoCursorAnimationDelay = 50;
uint32_t cursorAnimationSpeed = titleCursorAnimationDelay;
int cursorAnimationPoint = 0;
uint32_t cursorBlinkSpeed = 600;
uint32_t lastCursorBlink = 0;
bool cursorState = false;
uint32_t installationCycleTimer = 0;
uint32_t installationCycleDuration = 10000;
//Screen widths for wrapping
uint8_t size3wrapWidth = 18;
uint8_t size2wrapWidth = 22;
uint8_t size1wrapWidth = 40;
//Dimensions for line drawing
uint16_t fontHeight = 8;
uint16_t fontWidth = 6;
const uint16_t topMargin = 20;
const uint16_t boxMargin = 5;
uint16_t boxWidth;
uint16_t box1height = fontHeight * 3 + boxMargin * 2;
uint16_t box2height = fontHeight * 4 + boxMargin * 2; //Two lines
uint16_t box3height = fontHeight * 4 + boxMargin * 2; //Two lines
uint16_t box4height;
uint16_t box1y = topMargin + boxMargin;
uint16_t box2y = topMargin + box1height + (boxMargin * 2);
uint16_t box3y = topMargin + box1height + box2height + (boxMargin * 3);
uint16_t box4y = topMargin + box1height + box2height + box3height + (boxMargin * 4);
uint16_t canvasMargin = boxMargin * 2;
uint16_t canvasWidth;
uint16_t canvas1y = topMargin + (boxMargin * 2);
uint16_t canvas2y = topMargin + box1height + (boxMargin * 3);
uint16_t canvas3y = topMargin + box1height + box2height + (boxMargin * 4);
uint16_t canvas4y = topMargin + box1height + box2height + box3height + (boxMargin * 5);
uint16_t canvas1height = box1height - (boxMargin * 2);
uint16_t canvas2height = box2height - (boxMargin * 2);
uint16_t canvas3height = box3height - (boxMargin * 2);
uint16_t canvas4height;
//Wi-Fi connection variable
String InfopointName = "";
uint32_t wifiRetryInterval = 500;
uint8_t wifiRetries = 0;
uint8_t maxWifiRetries = 60;
#ifdef VALIDATE_CERT
  //Root server certificate to check identity
  const char *server_rootcert PROGMEM = "-----BEGIN CERTIFICATE-----\n"
                                "MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n"
                                "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
                                "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n"
                                "QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n"
                                "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n"
                                "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n"
                                "9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n"
                                "CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n"
                                "nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n"
                                "43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n"
                                "T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n"
                                "gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n"
                                "BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n"
                                "TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n"
                                "DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n"
                                "hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n"
                                "06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n"
                                "PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n"
                                "YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n"
                                "CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n"
                                "-----END CERTIFICATE-----\n";
#endif
//Neopixel ring
#ifdef useNeopixelRing
  #include <FastLED.h>
  #define NUM_LEDS 16
  #define PIN_RGB 27
  uint8_t neopixelMaxBrightness = 128;
  /*
   * Patterns
   * 0 - LARSON
   * 1 - Cycle colours
   * 2 - Speckle
   * 3 - Heavy speckle
   * 4 - Binary counter
   * 5 - Slow fade between colouts
   * 6 - Rotate single LED anitclockwise
   */
  uint8_t neopixelPattern = 0;
  uint8_t neopixelUpdatePattern = 7;
  bool changed = false;
  uint32_t lastMove = 0;
  uint32_t nextDrop = 0;
  uint32_t neopixelAnimationRate = 60;
  uint8_t ln = 0;
  CRGB leds[NUM_LEDS];
  CRGB cols[6] = { CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Yellow, CRGB::Magenta, CRGB::Cyan };
  uint8_t neopixelRed = 0xff;
  uint8_t neopixelGreen = 0xff;
  uint8_t neopixelBlue = 0xff;
  CRGB polybiusOrange = CRGB(neopixelRed,neopixelGreen,neopixelBlue);
#endif
#ifdef useLedHeartbeat
  #define PIN_LOGO 32
  bool dir = true;
  bool ledPulsing = true;
  int currentPwmBrightness = 0;
  int16_t ledPwmMaxBrightness = 1024;
  uint32_t lastLedPwmChange = 0;
  uint32_t ledPwmChangeInterval = 5;
#endif
bool running = false;
