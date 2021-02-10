$(function() {
  client = new Paho.MQTT.Client(MQTT_BROKER_IP, MQTT_WEBSOCKETS_PORT, MQTT_CLIENT_NAME);



  // set callback handlers
  client.onConnectionLost = onConnectionLost;
  client.onMessageArrived = onMessageArrived;
  

  // connect the client
  client.connect({onSuccess:onConnect});
  console.log("SPOTS:", SHELF_SPOTS);
});
// called when the client connects
function onConnect() {
  // Once a connection has been made, make a subscription and send a message.
  console.log("MQTT connected!");
  client.subscribe("shelves/"+MQTT_SHELF_ID+"/data");
  client.subscribe("shelves/"+MQTT_SHELF_ID+"/atmos");
  getAllShelfData(MQTT_SHELF_ID);
}

// called when the client loses its connection
function onConnectionLost(responseObject) {
  if (responseObject.errorCode !== 0) {
    console.log("onConnectionLost:"+responseObject.errorMessage);
  }
}

// called when a message arrives
function onMessageArrived(message) {
  console.log("onMessageArrived:"+message.payloadString);
  if(message.topic.includes("data"))
  {
    data = JSON.parse(message.payloadString);
    var i;
    for (i = 0; i < data.length; i++) {
        var posupdate = data[i];
        var pos = posupdate.pos;
        if("id" in posupdate && !isEmpty(posupdate.id))
        {
          if(posupdate.id != "00000000")
          {
            $.ajax({
              headers: { "X-CSRFToken": CSRF_TOKEN },
              url         : "/shelves/"+MQTT_SHELF_ID+"/set_tag", //gdzie się łączymy
              method      : "put", //typ połączenia, domyślnie get
              contentType : 'application/json',
              dataType    : "json",
              data        : JSON.stringify({"pos":pos,"tag":posupdate.id}),
              success     : function(response) {
                $("#spot-"+pos+"-bg").attr("src", PRODUCT_IMAGES_DIR+response['product_id']+".jpg");
                $("#spot-"+pos+"-level").attr("src", PRODUCT_IMAGES_DIR+response['product_id']+".jpg");
                $("#spot-"+pos+"-name").text(response['product_name']);
                $("#spot-"+pos+"-noinfo").hide();

                $("#spot-"+pos+"-bg").show();
                $("#spot-"+pos+"-level").show();
                $("#spot-"+pos+"-empty").hide();

                SHELF_SPOTS[pos].container_weight = response['container_weight'];
                SHELF_SPOTS[pos].max_measurement = response['max_measurement'];
              },
              error       : function(error) {
                $("#spot-"+pos+"-noinfo").show();
                console.log(error);
              }
            });
          }
          else 
          {
            $("#spot-"+pos+"-noinfo").show();
          }
        }
        if("w" in posupdate)
        {
          updateWeight(pos, posupdate.w);
        }
    }
  }
  else if(message.topic.includes("atmos"))
  {
    data = JSON.parse(message.payloadString);
    $("#temperature").text(data['t'] + " °C");
    $("#humidity").text(data['h'] + "%");
    $("#pressure").text(data['p'] + " hPa");
  }

}
function updateAtmos()
{

}

function updateWeight(position, weight)
{
  tare = SHELF_SPOTS[position].container_weight;
  max = SHELF_SPOTS[position].max_measurement;
  percent = (weight-tare)/max;
  percent = 1-clamp(percent, 0, 1); // invert for clip-path
  percent = 100.0*percent;
  console.log("Update weight at ", position, percent);
  $("#spot-"+position+"-level").css('clip-path', "inset("+percent+"% 0 0 0)");
}
function freeLastTag(position)
{
  $.ajax({
    headers: { "X-CSRFToken": CSRF_TOKEN },
    url         : "/shelves/"+MQTT_SHELF_ID+"/free_tag", //gdzie się łączymy
    method      : "put", //typ połączenia, domyślnie get
    contentType : 'application/json',
    data        : JSON.stringify({"pos":position}),
    success     : function(response) {
      $("#spot-"+position+"-bg").hide();
      $("#spot-"+position+"-level").hide();
      $("#spot-"+position+"-empty").show();
      $("#spot-"+position+"-name").text('-');
    },
    error       : function(error) {
      console.log(error);
    }
  });
}
function toggleLed(position)
{
  client.publish("shelves/"+MQTT_SHELF_ID+"/light/"+position, "");
}
function calibrate(position)
{
  if(confirm("Place the calibrating weight on position "+ position + " and press OK to calibrate."))
    client.publish("shelves/"+MQTT_SHELF_ID+"/calibrate/"+position, "");
}
function tare(position)
{
  if(confirm("Remove everything from position "+ position + " and press OK to tare."))
    client.publish("shelves/"+MQTT_SHELF_ID+"/tare/"+position, "");
}
function getAllShelfData(shelf_id)
{
  client.publish("shelves/"+shelf_id+"/getall", "");
}

function isEmpty(str) {
  return (!str || 0 === str.length);
}

function clamp(num, min, max) {
  return num <= min ? min : num >= max ? max : num;
}