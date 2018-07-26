

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <at200pc.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char *ssid = "network";
const char *password = "password";

ESP8266WebServer server ( 80 );
At200pc at(Serial, 5);

const int led = 13;

void handleRoot() {
  char temp[400];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  at.write_and_read(REQ_ALLUPDATE);

  snprintf ( temp, 400,

             "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>AT200pc web command</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from ESP8266!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <img src=\"/test.svg\" />\
  </body>\
</html>",

             hr, min % 60, sec % 60
           );
  server.send ( 200, "text/html", temp );
  
}

void handleAllupdate(){
 String json;
 
 json =  " { \"swr_threshold\" : \"" + at.swr_threshold_string() + "\",";
 json += " \"hilo_z_string\" : \"" + at.hilo_z_string() + "\","; 
 json += " \"capval_string\" : \"" + at.capval_string() + "\","; 
 json += " \"indval_string\" : \"" + at.indval_string() + "\","; 
 json += " \"instandby_string\" : \"" + at.instandby_string() + "\","; 
 json += " \"at_status_string\" : \"" + at.at_status_string() + "\","; 
 json += " \"powerfwd_string\" : \"" + at.powerfwd_string() + "\","; 
 json += " \"powerrev_string\" : \"" + at.powerrev_string() + "\","; 
 json += " \"swr_string\" : \"" + at.swr_string() + "\","; 
 json += " \"live_update_string\" : \"" + at.live_update_string() + "\","; 
 json += " \"tx_frequency_string\" : \"" + at.tx_frequency_string() + "\"}"; 
 
 
  server.send ( 200, "text/html", json );
 }

void handleNotFound() {
  digitalWrite ( led, 1 );
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
  digitalWrite ( led, 0 );
}




void setup ( void ) {


  delay(1000);
  at.write_and_read(REQ_ALLUPDATE);


  pinMode ( led, OUTPUT );
  digitalWrite ( led, 0 );

  WiFi.mode ( WIFI_STA );
  WiFi.begin ( ssid, password );


  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );

  }

  if ( MDNS.begin ( "at200pc" ) ) {
    //Serial.println ( "MDNS responder started" );
  }

  server.on ( "/", handleRoot );

 server.on (F("/REQ_ALLUPDATE"), handleAllupdate );


  //Antena Selection and information
  server.on (F("/REQ_ANT1"), []() { at.write_and_read(REQ_ANT1); server.send ( 200, "text/plain", at.antenna_selected() == 1 ? "1" : "2"  ); } );
  server.on (F("/REQ_ANT2"), []() { at.write_and_read(REQ_ANT2); server.send( 200, "text/plain", at.antenna_selected() == 1 ? "1" : "2"  ); } );
  server.on (F("/CMD_ANTENNA"), []() { server.send ( 200, "text/plain", at.antenna_selected() == 1 ? "1" : "2"  ); } );



  //Swr threshold
  server.on (F("/REQ_SWR11"), []() { at.write_and_read(REQ_SWR11); server.send ( 200,  F("text/plain"), F("1.1:1")  );} );
  server.on (F( "/REQ_SWR13"), []() { at.write_and_read(REQ_SWR13); server.send ( 200, F( "text/plain"),F( "1.3:1") );} );
  server.on (F( "/REQ_SWR15"), []() { at.write_and_read(REQ_SWR15); server.send ( 200, F( "text/plain"),F("1.5:1")  );} );
  server.on (F( "/REQ_SWR17"), []() { at.write_and_read(REQ_SWR17); server.send ( 200, F("text/plain"), F( "1.7:1") );} );
  server.on (F( "/REQ_SWR20"), []() { at.write_and_read(REQ_SWR20); server.send ( 200, F("text/plain"), F( "2.0:1") );} );
  server.on (F( "/REQ_SWR25"), []() { at.write_and_read(REQ_SWR25); server.send ( 200, F("text/plain"), F( "2.5:1") );} );
  server.on (F( "/REQ_SWR30"), []() { at.write_and_read(REQ_SWR30); server.send ( 200, F("text/plain"), F( "3.0:1") );} );
  server.on (F( "/CMD_SWRTHRESH"), []() { server.send ( 200, F("text/plain"), at.swr_threshold_string());} );

server.on (F("/REQ_INDUP"), []() { at.write_and_read(REQ_INDUP); server.send ( 200, F("text/plain"),  "{ \"indval_string\" : \"" + at.indval_string() + "\" }" ); });
server.on (F("/REQ_INDDN"), []() { at.write_and_read(REQ_INDDN); server.send ( 200, F("text/plain"),   "{ \"indval_string\" : \"" + at.indval_string() + "\" }"); });
server.on (F("/REQ_CAPUP"), []() { at.write_and_read(REQ_CAPUP); server.send ( 200, F("text/plain"),  "{ \"capval_string\" : \"" + at.capval_string() + "\"}"  ); });
server.on (F("/REQ_CAPDN"), []() { at.write_and_read(REQ_CAPDN); server.send ( 200, F("text/plain"),  "{ \"capval_string\" : \"" + at.capval_string() + "\"}"  ); });
server.on (F("/REQ_FULLTUNE"), []() { at.write_and_read(REQ_FULLTUNE); server.send ( 200, F("text/plain"), F("Tunning")); });


  

  server.onNotFound(handleNotFound);
  server.begin();

  ArduinoOTA.setPort(8266);
  ArduinoOTA.setHostname("ldg200pc");
  ArduinoOTA.setPassword("admin");

  ArduinoOTA.onStart([]() { 
    String type; 
    if (ArduinoOTA.getCommand() == U_FLASH) type = "sketch";
    else type = "filesystem";  
  });
  
  ArduinoOTA.begin();



}

void loop ( void ) {
  server.handleClient();

  ArduinoOTA.handle();

  at.read();

}

