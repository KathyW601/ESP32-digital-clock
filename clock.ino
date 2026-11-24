#include <WiFi.h>
#include <time.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

const char* WIFI_SSID = "EE-KF6P95";
const char* WIFI_PASSWORD = "9DtLTHJn6Jrc6FVf";

const char* NTP_SERVER = "pool.ntp.org";
const long GMT_OFFSET_SEC = 0; 
const long DAYLIGHT_OFFSET_SEC = 3600; 
unsigned long lastSyncMillis=0;
const unsigned long SYNC_INTERVAL=60UL*60*1000;

const char* weekName[]= {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};

void setup() {
  Serial.begin(115200);
  Serial.println("Startup...");

  WiFi.mode(WIFI_STA);
  WiFi.begin("EE-KF6P95", "9DtLTHJn6Jrc6FVf");
  
  while ((WiFi.status() != WL_CONNECTED))
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n WiFi Connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  configTime(GMT_OFFSET_SEC,DAYLIGHT_OFFSET_SEC,NTP_SERVER);
  lastSyncMillis=millis();
  delay(2000);

  struct tm t;
  if(getLocalTime(&t,15000))
  {
    Serial.printf("%02d:%02d:%02d %02d-%02d-%04d\n",t.tm_hour,t.tm_min,t.tm_sec,t.tm_mday,t.tm_mon+1,t.tm_year+1900);
    
  }
  else{
    Serial.println("Failed to get time!");
  }
  
}

void loop() {
  unsigned long nowMs=millis();
  if ((nowMs-lastSyncMillis>=SYNC_INTERVAL)&&(WiFi.status()==WL_CONNECTED)) 
  {
    Serial.println("Time: do NTP re-sync");
    configTime(GMT_OFFSET_SEC,DAYLIGHT_OFFSET_SEC,NTP_SERVER);
    lastSyncMillis=nowMs;
  }

  struct tm timeinfo;

  if(getLocalTime(&timeinfo,1000)){
    Serial.printf("%02d:%02d:%02d %02d-%02d-%04d\n",timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec,timeinfo.tm_mday,timeinfo.tm_mon+1,timeinfo.tm_year+1900); //打印在监视器里的数据

    int h = timeinfo.tm_hour;
    int min = timeinfo.tm_min;
    int sec = timeinfo.tm_sec;
    int day = timeinfo.tm_mday;
    int mon = timeinfo.tm_mon+1;
    int year = timeinfo.tm_year+1900;
    int wd = timeinfo.tm_wday;

    char bufTime[32];
    char bufDate[32];

    sprintf(bufTime, "%02d:%02d:%02d", h, min, sec);
    sprintf(bufDate, "%02d-%02d-%04d  %s", day, mon, year, weekName[wd]);

    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.setTextSize(3);
    tft.setCursor(20,30);
    tft.print(bufTime);

    tft.setTextSize(2);
    tft.setCursor(20,90);
    tft.print(bufDate);
  }
  else{
    Serial.println("Falied to get time");
  }
  delay(1000);  
}
