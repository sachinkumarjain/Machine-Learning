<?php

/*
  ESP8266: send data to your Domain(or mine)
  Embedded-iot.net/dht11/dataCollector.php

  Uses POST command to send DHT data to a designated website
  The circuit:
  * DHT
  * Post to Domain

   Stephen Borsay
   Embedded-iot.net
   www.udemy.com/all-about-arduino-wireless
   https://www.hackster.io/detox
   https://github.com/sborsay/Arduino_Wireless
*/


date_default_timezone_set("America/Los_Angeles");
$TimeStamp = "The current time is " . date("h:i:sa");
file_put_contents('dataDisplayer.html', $TimeStamp, FILE_APPEND);


   if( $_REQUEST["Humidity"] ||  $_REQUEST["Temperature_Cel"] ||
                      $_REQUEST["Temperature_Fehr"]
       ||  $_REQUEST["HeatIndex_Cel"] ||  $_REQUEST["HeatIndex_Fehr"] ) 
   {
   echo " The Humidity is: ". $_REQUEST['Humidity']. "%<br />";
   echo " The Temperature is: ". $_REQUEST['Temperature_Cel']. " Celcius<br />";
   echo " The Temperature is: ". $_REQUEST['Temperature_Fehr']. " Fehr<br />";
   echo " The Heat Index: ". $_REQUEST['HeatIndex_Cel']. " Heat Idx Cel<br />";
   echo " The Heat Index: ". $_REQUEST['HeatIndex_Fehr']. " Heat Idx Fehr<br />";
   }
	  
	
$var1 = $_REQUEST['Humidity'];
$var2 = $_REQUEST['Temperature_Cel'];
$var3 = $_REQUEST['Temperature_Fehr'];
$var4 = $_REQUEST['HeatIndex_Cel'];
$var5 = $_REQUEST['HeatIndex_Fehr'];

$WriteMyRequest=
"<p> The Humidity is : "       . $var1 . "% </p>".
"<p>And the Temperature is : " . $var2 . " Celcius </p>".
"<p>And the Temperature is : " . $var3 . " Fehreinheit</p>".
"<p>And The Heat Index is : "  . $var4 . " Heat Index Celcius </p>".
"<p>And The Heat Index is : "  . $var5 . " Heat Index Fehrenheit </p><br/>";


file_put_contents('dataDisplayer.html', $WriteMyRequest, FILE_APPEND);


?>