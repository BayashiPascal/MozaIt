<?php 
  // ------------------ index.php --------------------->
  // Start the PHP session
  session_start();

  // Ensure no message will interfere with output
  ini_set('display_errors', 'Off');
  error_reporting(0);

  // Turn on display of errors and warning for debug
  /*ini_set('display_errors', 'On');
  error_reporting(E_ALL ^ E_WARNING);
  error_reporting(E_ALL | E_STRICT);*/

?>
<!DOCTYPE html>
<html>
  <head>

    <!-- Meta -->
    <meta content="text/html; charset=UTF-8;">
    <meta name="viewport" 
      content="width=device-width, initial-scale=1, maximum-scale=1">
    <meta name="description" content="MozaIt! " />
    <meta name="keywords" content="mozait, mozaic, painting" />
      
    <!-- Icon -->
    <link rel="icon" type="image/x-icon" 
      href="./Img/mozait.ico" />

    <!-- Include the CSS files -->
    <link href = "./animate.css" 
      rel = "stylesheet" type = "text/css">
    <link href = "./mozait.css" 
      rel = "stylesheet" type = "text/css"> 

    <!-- Include the JS files -->
    <script charset = "UTF-8" src = "./jquery.min.js"></script>
    <script charset = "UTF-8" src = "./mozait.js"></script>

    <title>MozaIt!</title>
  </head>
  <body onload = <?php echo "'BodyOnLoad(" . date("YmdHis") . ");'" ?>>
    <!-- Main div -->
    <div id = "divMain">
      
      <!-- Title div -->
      <div id = "divTitle">
        MozaIt!<br>
        <div id = "divSubTitle">
          - Transform your pictures into mozaic of dots -
        </div>
      </div>
      
      <!-- Main div -->
      <div id = "divBoard">
        <div id = "divInputImg" class = "divTool">
          Source image:<br><br>
          <div id = "divInfoSrc">
            Click below to select a source image<br>
            (.jpg, RGB, less than 500x500 pixels only !).<br>
            The image will be displayed after upload is complete.<br>
            <form id = "formSrcImg" action = "" method = "post" 
            enctype = "multipart/form-data">
              <input type = "file" name = "inpSrcImg" 
              id = "inpSrcImg" style = "width:250px;" 
              onChange = "SelectSrcImg();">
            </form>
            <img id = "imgWaitSrc" src = "./Img/wait.gif" 
              style = "display:none;">
          </div>
          <img id = "imgSrc">
        </div>
        <div id = "divOutputImg" class = "divTool">
          Result image:<br><br>
          <div id = "divInfoOut">
            Click on the "MozaIt!" button below to display the result 
            image. <br>
            It may takes a few seconds to a few minutes, please be 
            patient.<br>
            If you're not satisfied with the result, experiment
            with the settings below.
          </div>
          <div id = "divMsgOut"></div>
          <img id = "imgWait" src = "./Img/wait.gif" 
            style = "display:none;">
          <img id = "imgOut">
        </div>
      </div>
      
      <!-- Cmd div -->
      <div id = "divCmd">
        <input type = "button" value = "MozaIt!" 
          onclick = "Process();">
        <input type = "button" value = "Settings" 
          onclick = "ShowSettings();">
        <input type = "button" value = "Change source" 
          id = "btnChangeSrc" onclick = "ChangeSource();" 
          style = "display: none;">
      </div>

      <!-- footer div -->
      <div id = "divFooter">
        Copyright <a href="mailto:Pascal@BayashiInJapan.net">
            P. Baillehache
        </a>, 2017.<br>
        Your images are stored on the server only for the time they are
        processed.<br>They are automatically deleted once you leave 
        this page.
      </div>

    </div>

    <!-- Setting div -->
    <div id = "divSettings">
      <div id = "divSettingsContent">
        <img src = "./Img/close.gif" id = "imgSettingsClose"
          onclick = "HideSettings();">
        <div id = "divSettingsTitle">- Settings -</div>
        <div class = "divOneSetting">
          Size of smallest dot (<span id = "spanMinOrder"></span>): 
          <input type = "range" min = "0" max = "12" value = "1"
            id = "rngMinOrder" class = "rng" onChange = "SetMinOrder();">
        </div>
        <div class = "divOneSetting">
          Size of largest dot (<span id = "spanMaxOrder"></span>): 
          <input type = "range" min = "0" max = "12" value = "10"
            id = "rngMaxOrder" class = "rng" onChange = "SetMaxOrder();">
        </div>
        <div class = "divOneSetting">
          Blurriness (<span id = "spanThreshold"></span>): 
          <input type = "range" min = "0.0" max = "1.0" value = "0.3"
            step = "0.01" id = "rngThreshold" class = "rng"  
            onChange = "SetThreshold();">
        </div>
        <div class = "divOneSetting">
          Margin between dots (<span id = "spanMargin"></span>): 
          <input type = "range" min = "0" max = "20" value = "2"
            id = "rngMargin" class = "rng" onChange = "SetMargin();">
        </div>
      </div>
    </div>
      
  </body>

</html>
