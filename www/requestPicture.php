 <?php 
  /* ============= requestMove.php =========== */
  // Ensure no message will interfere with output
  ini_set('display_errors', 'Off');
  error_reporting(0);

  // Turn on display of errors and warning for debug
  /*ini_set('display_errors', 'On');
  error_reporting(E_ALL ^ E_WARNING);
  error_reporting(E_ALL | E_STRICT);*/

  // Start the PHP session
  session_start();

  try {
    // Sanitize args
    $flagArgOk = true;
    $args[0] = "ref";
    $args[1] = "min";
    $args[2] = "max";
    $args[3] = "margin";
    $args[4] = "threshold";
    foreach ($args as $arg) {
      if (isset($_GET[$arg])) {
        $match = preg_match("/^[0-9. ]+$/", $_GET[$arg]);
        if ($match == 0) {
          $_GET[$arg] = "";
          $flagArgOk = false;
        }
      } else {
        $flagArgOk = false;
      }
    }
    // If args are ok
    if ($flagArgOk == true) {
      // Convert the source image to JPG
      $cmdTGA = "convert ./Src/" . $_GET["ref"];
      $cmdTGA .= ".jpg ./Src/" . $_GET["ref"] . ".tga";
      unset($outputTGA);
      unset($returnTGA);
      exec($cmdTGA, $outputTGA, $returnTGA);
      if ($returnTGA != 0) {
        // The conversion to TGA failed
        $data["error"] = "conversion to TGA failed " . $returnTGA;
        $data["message"] = $outputTGA;
      } else {
        // Create the command
        $cmd = "./mozait ./Src/";
        $cmd .= $_GET["ref"] . ".tga ./Res/" . $_GET["ref"] . ".tga ";
        $cmd .= "-order " . $_GET["min"] . " "  . $_GET["max"] . " ";
        $cmd .= "-margin " . $_GET["margin"] . " ";
        $cmd .= "-threshold " . $_GET["threshold"] . " ";
        // Set the time limit
        if (set_time_limit(1800) == false) {
          $data["error"] = "set time limit failure ";
        } else {
          // Execute the command
          unset($output);
          unset($returnVal);
          exec($cmd, $output, $returnVal); 
          // Prepare the returned data
          $data["return"] = $returnVal;
          $data["message"] = $output;
          if ($returnVal == 0) {
            $data["error"] = "";
            // Convert the result image to JPG
            $cmdJPG = "convert ./Res/" . $_GET["ref"];
            $cmdJPG .= ".tga ./Res/" . $_GET["ref"] . ".jpg";
            unset($outputJPG);
            unset($returnJPG);
            exec($cmdJPG, $outputJPG, $returnJPG);
            if ($returnJPG != 0) {
              // The conversion to JPG failed
              $data["error"] = "conversion to JPG failed " . $returnJPG;
              $data["message"] = $outputJPG;
            }
          } else {
            $data["error"] = "binary failure " . $returnVal;
          }
        }
      }
    } else {
      $data = array();
      $data["error"] = "arguments invalid";
      $data["message"] = "";
      $data["return"] = 1;
    }
    // Convert the object to JSON format
    $ret = json_encode($data);
    // Return the JSON formatted result
    echo $ret;
  } catch (Exception $e) {
     ManageException("requestPicture.php " . $e);
  }
?>
