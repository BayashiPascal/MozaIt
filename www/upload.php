 <?php 
ini_set('display_errors', 'On');
//error_reporting(E_ALL ^ E_WARNING);
//error_reporting(E_ALL | E_STRICT);
error_reporting(0);

// Start the PHP session
session_start();

function UploadImage($ref) {
  $errCode = 0;
  try {
    $target_dir = "./Src/";
    $name = $_FILES["inpSrcImg"]["name"];
    $target_file = $target_dir . $ref . ".jpg"; //basename($name);
    $tmpName = $_FILES["inpSrcImg"]["tmp_name"];
    $imageFileType = pathinfo($target_file, PATHINFO_EXTENSION);
    // Check if image file is a actual image or fake image
    // TODO, below doesn't work
    /*$check = getimagesize(tmpName);
    if($check === false) {
      $errCode = 1;
    }*/
    // Check if file already exists
    if (file_exists($target_file)) {
      $errCode = 2;
    }
    // Check file size
    if ($_FILES["inpEvtNewImage"]["size"] > 500000) {
      $errCode = 3;
    }
    // Allow certain file formats
    if($imageFileType != "jpg") {
      $errCode = 4;
    }
    // If everything is ok
    if ($errCode == 0) {
      if (is_dir($target_dir) && is_writable($target_dir)) {
        if (!move_uploaded_file($tmpName, $target_file)) {
          $errCode = 5;
        }
      } else {
        $errCode = 6;
      }
    }
  } catch (Exception $e) {

  }
  return $errCode;
}

try {
  // Check arguments
  if (isset($_GET["m"]) && isset($_GET["r"]) && isset($_GET["i"]) && 
    preg_match("/^[0-9. ]+$/", $_GET["r"]) != 0) {
    if ($_GET["m"] == 1) {
      // Delete the images
      $cmd = "rm -f ./Src/" . $_GET["r"] . ".jpg ";
      $cmd .= "./Src/" . $_GET["r"] . ".tga ";
      $cmd .= "./Res/" . $_GET["r"] . ".jpg ";
      $cmd .= "./Res/" . $_GET["r"] . ".tga ";
      unset($output);
      unset($return);
      exec($cmd, $output, $return);
      // Upload the image
      if ($_FILES["inpSrcImg"]["name"] != "") {
        $uploadOk = UploadImage($_GET["r"]);
      }
      $data = array();
      $data["return"] = $uploadOk;
    } else if ($_GET["m"] == 2 && 
      preg_match("/^[0-9 ]+$/", $_GET["r"]) != 0) {
      // Delete the images
      $cmd = "rm -f ./Src/" . $_GET["r"] . ".jpg ";
      $cmd .= "./Src/" . $_GET["r"] . ".tga ";
      //$cmd .= "./Res/" . $_GET["r"] . ".jpg ";
      $cmd .= "./Res/" . $_GET["r"] . ".tga ";
      unset($output);
      unset($return);
      exec($cmd, $output, $return);
      $data = array();
      $data["return"] = $return;
      $data["output"] = $output;
    }
  } else {
    $data = array();
    $data["return"] = 1;
  }
  // Convert the object to JSON format
  $ret = json_encode($data);
  // Return the JSON formatted result
  echo $ret;
} catch (Exception $e) {
  echo "Exception " . $e;
}
?>
