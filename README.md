# ESP32 cam security camera
 Take a picure every X and send it via php to a PHP server


## How to use

you need to put your WiFi SSID and PASSWORD in the variable 
```cpp
const char* ssid = "SSID";
const char* password = "PASS";
```

you need to put the interval that you want the pictures to be tacken and where by indicating the server adress /!\ don't foget to put http:// even if it is a local server like 192.168.1.10 and to put the full URL that finish by .php
```cpp
const int capture_interval = 5000; // Microseconds between captures
const char *post_url = "http://SERVER.php"; // Location where images are POSTED
```

next you need to put the up hours of your camera
```cpp
const char* open_hour = "11"; //the opening hour
const char* close_hour= "14"; //the closing hour
```

and at last you need to choose the resolution of the picture by uncommenting witch resolution you want and commenting the resolution already uncommented(note that the bigger the picture the more demnading the requests will be)
```cpp
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
```

