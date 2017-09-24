/* ============= mozait.js =========== */

// ------------ Global variables
var theMozaIt = {};

// ------------ MozaIt: main class

function MozaIt(ref) {
  try {
    // Initialise the properties
    this._ref = ref;
    this._loaded = false;
    this._minOrder = 1;
    this._maxOrder = 10;
    this._threshold = 0.3;
    this._margin = 2;
    SetMinOrder();
    SetMaxOrder();
    SetMargin();
    SetThreshold();
  } catch (err) {
    console.log("MozaIt " + err.stack);
  }
}

// ------------ Process an image

MozaIt.prototype.Paint = function() {
  try {
    // Prepare the arguments
    var arg = "ref=" + this._ref;
    arg += "&min=" + this._minOrder;
    arg += "&max=" + this._maxOrder;
    arg += "&margin=" + this._margin;
    arg += "&threshold=" + this._threshold;
    // Send the HTTP request
    this.Request(arg);
  } catch (err) {
    console.log("MozaIt.Paint " + err.stack);
  }
}

// ------------ HTTP Request

MozaIt.prototype.Request = function(arg) {
  try {
    // Prepare the url for the PHP interfacing with the database
    url = "./requestPicture.php?" + arg;
    // Create the HTTP request entity
    if (window.XMLHttpRequest) {
      xmlhttp = new XMLHttpRequest();
    } else {
      xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
    }
    xmlhttp.onreadystatechange = function() {
      if (xmlhttp.readyState == 4) {
        console.log(xmlhttp.responseText);
        if (xmlhttp.status == 200) {
          // The request was successful, return the JSON data
          data = xmlhttp.responseText;
        } else {
          // The request failed, return error as JSON
          data ="{\"error\":\"HTTPRequest failed : " + 
            xmlhttp.status + 
            "\"}";
        }
        theMozaIt.ProcessReply(data);
      }
    };
    // Display the wait icon
    $("#imgWait").css("display", "block");
    // Send the HTTP request
    console.log(url);
    xmlhttp.open("GET", url);
    xmlhttp.send();
  } catch (err) {
    console.log("MozaIt.Request " + err.stack);
  }
}

// ------------ Process the reply from the HTTPRequest

MozaIt.prototype.ProcessReply = function(data) {
  try {
    // Decode the JSON data
    var reply = JSON.parse(data);
    // Hide the info in output div
    $("#divInfoOut").css("display", "none");
    // Hide the wait icon
    $("#imgWait").css("display", "none");
    // Process the reply
    if (reply["error"] == "") {
      $("#divMsgOut").html("");
      $("#imgOut").attr("src", "./Res/" + this._ref + ".jpg?" + 
        new Date().getTime());
    } else {
      $("#divMsgOut").html(reply["error"] + "<br>" + reply["message"]);
      $("#imgOut").attr("src", "");
    }
  } catch (err) {
    console.log("MozaIt.ProcessReply " + err.stack);
  }
}

// ------------ OnLoad function

function BodyOnLoad(ref) {
  try {
    // Create the MozaIt entity
    theMozaIt = new MozaIt(ref);
    // Bind events
    window.onbeforeunload = BodyOnUnload;
  } catch (err) {
    console.log("BodyOnLoad " + err.stack);
  }
}

// ------------ OnUnload function

function BodyOnUnload() {
  try {
    // Prepare the url for the PHP interfacing with the database
    url = "./upload.php?m=2&i=1&r=" + theMozaIt._ref;
    // Create the HTTP request entity
    if (window.XMLHttpRequest) {
      xmlhttp = new XMLHttpRequest();
    } else {
      xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
    }
    xmlhttp.onreadystatechange = function() {
      // Do nothing
    };
    // Send the HTTP request
    xmlhttp.open("GET", url);
    xmlhttp.send();
  } catch (err) {
    console.log("BodyOnUnload " + err.stack);
  }
}

// ------------ function called when the user quit or refresh the page

function windowUnload() {
  try {

  } catch (err) {
    console.log("windowUnload " + err.stack);
  }
}

function documentOnClick(event) {
  try {

  } catch (err) {
    console.log("documentOnClick() " + err.stack);
  }
}

// ------------ function called when the user click on MozaIt!

function Process() {
  try {
    if (theMozaIt._loaded == true) {
      $("#imgOut").attr("src", "");
      theMozaIt.Paint();
    } else {
      $("#divInfoSrc").addClass("animated flash");
      setTimeout(function(){
        $("#divInfoSrc").removeClass("animated flash");
      }, 1000);
    }
  } catch (err) {
    console.log("Process() " + err.stack);
  }
}

// ------------ function to display the settings

function ShowSettings() {
  try {
    $("#divSettings").css("visibility", "visible");
  } catch (err) {
    console.log("ShowSettings() " + err.stack);
  }
}

// ------------ function to hide the settings

function HideSettings() {
  try {
    $("#divSettings").css("visibility", "hidden");
  } catch (err) {
    console.log("HideSettings() " + err.stack);
  }
}

// ------------ function called when the user modifies the min order

function SetMinOrder() {
  try {
    theMozaIt._minOrder = $("#rngMinOrder").val();
    $("#spanMinOrder").html(theMozaIt._minOrder);
    if (parseInt($("#rngMinOrder").val()) > 
      parseInt($("#rngMaxOrder").val())) {
      $("#rngMaxOrder").val(theMozaIt._minOrder);
      SetMaxOrder();
    }
  } catch (err) {
    console.log("SetMinOrder() " + err.stack);
  }
}

// ------------ function called when the user modifies the max order

function SetMaxOrder() {
  try {
    theMozaIt._maxOrder = $("#rngMaxOrder").val();
    $("#spanMaxOrder").html(theMozaIt._maxOrder);
    if (parseInt($("#rngMaxOrder").val()) < 
      parseInt($("#rngMinOrder").val())) {
      $("#rngMinOrder").val(theMozaIt._maxOrder);
      SetMinOrder();
    }
  } catch (err) {
    console.log("SetMaxOrder() " + err.stack);
  }
}

// ------------ function called when the user modifies the threshold

function SetThreshold() {
  try {
    theMozaIt._threshold = $("#rngThreshold").val();
    $("#spanThreshold").html(theMozaIt._threshold);
  } catch (err) {
    console.log("SetThreshold() " + err.stack);
  }
}

// ------------ function called when the user modifies the margin

function SetMargin() {
  try {
    theMozaIt._margin = $("#rngMargin").val();
    $("#spanMargin").html(theMozaIt._margin);
  } catch (err) {
    console.log("SetMargin() " + err.stack);
  }
}

// ------------ function called when the user wants to change 
// the source image

function ChangeSource() {
  try {
    theMozaIt._loaded = false;
    $("#btnChangeSrc").css("display", "none");
    $("#divInfoSrc").css("display", "block");
    $("#divInfoOut").css("display", "block");
    $("#imgSrc").attr("src", "");
    $("#imgOut").attr("src", "");
    $("#inpSrcImg").val("");
  } catch (err) {
    console.log("ChangeSource() " + err.stack);
  }
}

// ------------ function called when the user selects a source image

function SelectSrcImg() {
  try {
    url = "./upload.php?m=1&r=" + theMozaIt._ref + "&i=";
    var file = document.getElementById("inpSrcImg").files[0];
    if (file !== undefined) { 
      url += file.name;
    }
    // Create the HTTP request entity
    if (window.XMLHttpRequest) {
      xmlhttp = new XMLHttpRequest();
    } else {
      xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
    }
    xmlhttp.onreadystatechange = 
      function() {
        if (xmlhttp.readyState == 4) {
          if (xmlhttp.status == 200) {
            // The request was successful, check the JSON data
            console.log(xmlhttp.responseText);
            $("#imgWaitSrc").css("display", "none");
            data = JSON.parse(xmlhttp.responseText);
            if (data["return"] == 0) {
              theMozaIt._loaded = true;
              $("#divInfoSrc").css("display", "none");
              $("#btnChangeSrc").css("display", "inline-block");
              $("#imgSrc").attr("src", "./Src/" + theMozaIt._ref +
                ".jpg");
            } else {
              theMozaIt._loaded = false;
              $("#btnChangeSrc").css("display", "none");
            }
          }
        }
      }
    console.log(url);
    xmlhttp.open("POST",url);
    var formData = new FormData(document.getElementById("formSrcImg"));
    $("#imgWaitSrc").css("display", "block");
    xmlhttp.send(formData);
  } catch (err) {
    console.log("SelectSrcImg() " + err.stack);
  }
}
