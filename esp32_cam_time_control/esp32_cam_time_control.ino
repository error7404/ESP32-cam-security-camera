#include "esp_http_client.h"
#include "esp_camera.h"
#include <WiFi.h>
#include "Arduino.h"
#include "time.h"





const char* ssid = "SSID";
const char* password = "PASS";

const int capture_interval = 5000; // Microseconds between captures
const char *post_url = "http://SERVER.php"; // Location where images are POSTED

const char* open_hour = "11"; //the opening hour
const char* close_hour= "14"; //the closing hour

//select the resolution
//#define FRAMESIZE FRAMESIZE_QQVGA   // 160x120
//#define FRAMESIZE FRAMESIZE_QQVGA2  // 128x160
//#define FRAMESIZE FRAMESIZE_QCIF    // 176x144
//#define FRAMESIZE FRAMESIZE_HQVGA   // 240x176
//#define FRAMESIZE FRAMESIZE_QVGA    // 320x240
//#define FRAMESIZE FRAMESIZE_CIF     // 400x296
//#define FRAMESIZE FRAMESIZE_VGA     // 640x480
//#define FRAMESIZE FRAMESIZE_SVGA    // 800x600
#define FRAMESIZE FRAMESIZE_XGA       // 1024x768






bool internet_connected = false;
long current_millis;
long last_capture_millis = 0;
int fails = 0;
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

// CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

void setup()
{
  Serial.begin(115200);

  if (init_wifi()) { // Connected to WiFi
    internet_connected = true;
    Serial.println("");
    Serial.println("Internet connected");
  }

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  //init with high specs to pre-allocate larger buffers
  if (psramFound()) {
    config.frame_size = FRAMESIZE;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE; 
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  pinMode(4, OUTPUT);

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); //init of the time

}

bool init_wifi()
{
  int connAttempts = 0;
  Serial.println("\r\nConnecting to: " + String(ssid));
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
    if (connAttempts > 10) return false;
    connAttempts++;
  }
  return true;
}


esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
  switch (evt->event_id) {
    case HTTP_EVENT_ERROR:
      Serial.println("HTTP_EVENT_ERROR");
      break;
    case HTTP_EVENT_ON_CONNECTED:
      Serial.println("HTTP_EVENT_ON_CONNECTED");
      break;
    case HTTP_EVENT_HEADER_SENT:
      Serial.println("HTTP_EVENT_HEADER_SENT");
      break;
    case HTTP_EVENT_ON_HEADER:
      Serial.println();
      Serial.printf("HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
      break;
    case HTTP_EVENT_ON_DATA:
      Serial.println();
      Serial.printf("HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
      if (!esp_http_client_is_chunked_response(evt->client)) {
        // Write out data
        // printf("%.*s", evt->data_len, (char*)evt->data);
      }
      break;
    case HTTP_EVENT_ON_FINISH:
      Serial.println("");
      Serial.println("HTTP_EVENT_ON_FINISH");
      break;
    case HTTP_EVENT_DISCONNECTED:
      Serial.println("HTTP_EVENT_DISCONNECTED");
      break;
  }
  return ESP_OK;
}

static esp_err_t take_send_photo()
{
  Serial.println("Taking picture...");
  camera_fb_t * fb = NULL;
  esp_err_t res = ESP_OK;

  fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return ESP_FAIL;
  }

  esp_http_client_handle_t http_client;
  
  esp_http_client_config_t config_client = {0};
  config_client.url = post_url;
  config_client.event_handler = _http_event_handler;
  config_client.method = HTTP_METHOD_POST;

  http_client = esp_http_client_init(&config_client);

  esp_http_client_set_post_field(http_client, (const char *)fb->buf, fb->len);

  esp_http_client_set_header(http_client, "Content-Type", "image/jpg");

  esp_err_t err = esp_http_client_perform(http_client);
  if (err == ESP_OK) {
    Serial.print("esp_http_client_get_status_code: ");
    Serial.println(esp_http_client_get_status_code(http_client));
  }

  esp_http_client_cleanup(http_client);

  esp_camera_fb_return(fb);
}


bool go(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return false;
  }
  char timeWeekDay[10];
  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  strftime(timeWeekDay,10, "%u", &timeinfo);
  String Day = String(timeWeekDay);
  String Hour = String(timeHour);
  if(Day == "6" or Day == "7"){Serial.println("le self n'est pas ouvert le weekend");return false;}
  else if(Hour > open_hour and Hour < close_hour){Serial.println("Le self est ouvert"); return true;}//14 11
  else{Serial.println("le self est fermer ce n'est pas l'heure");return false;}
  Serial.println(&timeinfo, "%a, %B %d %Y %H:%M:%S");
}


void loop()
{
  if(WiFi.SSID() == ""){
    Serial.println();
    Serial.print("no connexion: reconecting");
    init_wifi();
    if(WiFi.SSID() != ""){Serial.println("Reconected! ヽ(^o^)ノ");delay(2000);fails = 0;}
    else if(fails > 30){Serial.println("not reconected T_T trying every 2sec");delay(2000);fails++;}
    else{{Serial.println("not reconected T_T trying every 2min");delay(120000);}}
    }
  else{current_millis = millis();
    if (current_millis - last_capture_millis > capture_interval) { // Take another picture
      last_capture_millis = millis();
      if(go()){
        //digitalWrite(4, HIGH);
        //delay(250);
        take_send_photo();
        //digitalWrite(4, LOW);
      }
    }
  }
}
