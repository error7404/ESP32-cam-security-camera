<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    <title>CAM</title>
    <link rel="stylesheet" type="text/css" href="css.css">
    <script src="cookies.js"></script>
  </head>


<body>

  <div class="video">

    <img class="video1" id="img" src="cam/cam1.jpg">
    <img class="video2" id="img" src="cam/cam2.jpg">

  </div>


  <div class="center">
    <a class="text"> Bad connexion ? </a>
    <br>
    <label class="switch"><input type="checkbox" id="id_connexion" onclick="if(this.checked){setInterval(updateImage, 15000);setC('true');}else{setInterval(updateImage, 5005);setC('false');}"><span class="slider round"></span></label>
    <br>
    <a href="hot"><button type="button" class="bottom">button 1</button></a>
    <a href="http://google.com"><button type="button" class="bottom button_2">button 2</button></a>
  </div>
</body>
  <script>
    function updateImage() {
      window.location.reload(false); 
    }
    window.onload = load()
    function load(){
      var connexion = getCookie("connexion");
      if(connexion == 'true'){
        setInterval(updateImage, 15000);
        document.getElementById("id_connexion").checked = true;
      }
      else{
        setInterval(updateImage, 5005);
        document.getElementById("id_connexion").checked = false;
      }
    }

    function setC(data){
      let date = new Date(Date.now() + 10000000); //86400000ms = 1 jour
      date = date.toUTCString();
      document.cookie = 'connexion=' + data + '; path=/; expires='+date;
      console.log("ajout du cookie 'connexion = " + data + "'");
    }
  </script>
</html>