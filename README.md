# ESP32 cam security camera
 Take a picture every X and send it via a post to a PHP server


## How to use
### ESP32 cam side

you need to put your WiFi SSID and PASSWORD in the variable 
```cpp
const char* ssid = "SSID";
const char* password = "PASS";
```

you need to put the interval that you want the pictures to be taken and where by indicating the server address  
⚠don't forget to put http:// even if it is a local server like http://192.168.1.10/index.php and to put the full URL that finish by .php⚠
```cpp
const int capture_interval = 5000; // Microseconds between captures
const char *post_url = "http://website.com/cam/cam1"; // Location where images are POSTED
```

next, you need to put the up hours of your camera
```cpp
const char* open_hour = "11"; //the opening hour
const char* close_hour= "14"; //the closing hour
```

and at last, you need to choose the resolution of the picture by uncommenting witch resolution you want and commenting the resolution already uncommented(note that the bigger the picture the more demanding the requests will be)
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

### Server side
on a server add index.php, css.css, cokkies.js, cam/cam1.php, cam/cam2.php in the same folder    
the picutre taken by the esp32cam will be put in the folder cam of it as cam1.jpg and cam2.jpg (if you have an other ESP32 cam and you modify the code to point to cam2.php) so be sure that it is available to write by the programme  
a simple fix is juste to do a `sudo chmod 777 /var/www/html/cam` if you have a simple rasbian webserver

## Customization

if your image is not the right orientation add `rotate-90` or `rotate90` or `rotate180` to the class of the pictures in the index.php  
at line 15 or 16
```html
<img class="video1" id="img" src="cam1.jpg">
```
to
```html
<img class="video1 rotate-90" id="img" src="cam1.jpg">
```

if you’d like to change the button and where they are getting change the `href` in the index.php to where you want to point and the text on the button is the text in the middle of the tags  
at line 26 and 27
```html
<a href="hot"><button type="button" class="bottom">config hotspot</button></a>
```
to
```html
<a href="http://www.mywebsite.com"><button type="button" class="bottom">back to my website</button></a>
```
and if you want only one button just delete ligne 27  
  
if you wanna change the color of the button go in css.css and change at line 7 the color of the first button and at line 19 change the color of the second button