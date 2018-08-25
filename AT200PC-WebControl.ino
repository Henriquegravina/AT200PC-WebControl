#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266mDNS.h>
#include <at200pc.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Schedule.h>


#include "web/static/index.all.html.gz.h"


AsyncWebServer server(80);
At200pc at(Serial, 5);

const char* ssid = "**";
const char* password = "****";
char last_modified[50];


void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

// Callback for the index page
void onHome(AsyncWebServerRequest *request) {
    if (request->header("If-Modified-Since").equals(last_modified)) {     // Check if the client already has the same version
        request->send(304);
    } else {
        // Dump the byte array in PROGMEM with a 200 HTTP code (OK)
        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", webui_image, webui_image_len);
        // Tell the browswer the contemnt is Gzipped
        response->addHeader("Content-Encoding", "gzip");
        // And set the last-modified datetime so we can check if we need to send it again next time or not
        response->addHeader("Last-Modified", last_modified);
        request->send(response);
    }
}

void handleAllupdate(AsyncWebServerRequest *request){
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
 
 
  request->send( 200, "text/html", json );
 }






void setup() {

    
    delay(100);
    at.write_and_read(REQ_ALLUPDATE);
  
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    // Wait for connection
    while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );

  }

      if ( MDNS.begin ( "at200pc" ) ) {
    //Serial.println ( "MDNS responder started" );
  }


    sprintf(last_modified, "%s %s GMT", __DATE__, __TIME__);
 
    server.onNotFound(notFound);
    server.on("/index.html", HTTP_GET, onHome);
    server.on("/", HTTP_GET, onHome);
    
    server.on("/REQ_ALLUPDATE", HTTP_GET, handleAllupdate );



    server.on ("/REQ_ANT1",   HTTP_GET, [](AsyncWebServerRequest *request) { schedule_function( [](){ at.write_and_read(REQ_ANT1); }); request->send( 200, "text/plain", at.antenna_selected() == 1 ? "1" : "2"  );  } );
    server.on ("/REQ_ANT2",   HTTP_GET, [](AsyncWebServerRequest *request) { schedule_function( [](){ at.write_and_read(REQ_ANT2); }); request->send( 200, "text/plain", at.antenna_selected() == 1 ? "1" : "2"  ); } );
    server.on ("/CMD_ANTENNA",HTTP_GET, [](AsyncWebServerRequest *request) { request->send ( 200, "text/plain", at.antenna_selected() == 1 ? "1" : "2"  ); } );

    //Swr threshold
    server.on ("/REQ_SWR11", HTTP_GET, [](AsyncWebServerRequest *request) { schedule_function( [](){ at.write_and_read(REQ_SWR11); }); request->send ( 200,  F("text/plain"), F("1.1:1")  );} );
    server.on ("/REQ_SWR13", HTTP_GET, [](AsyncWebServerRequest *request) { schedule_function( [](){ at.write_and_read(REQ_SWR13); }); request->send ( 200, F( "text/plain"),F( "1.3:1") );} );
    server.on ("/REQ_SWR15", HTTP_GET, [](AsyncWebServerRequest *request) { schedule_function( [](){ at.write_and_read(REQ_SWR15); }); request->send ( 200, F( "text/plain"),F("1.5:1")  );} );
    server.on ("/REQ_SWR17", HTTP_GET, [](AsyncWebServerRequest *request) { schedule_function( [](){ at.write_and_read(REQ_SWR17); }); request->send ( 200, F("text/plain"), F( "1.7:1") );} );
    server.on ("/REQ_SWR20", HTTP_GET, [](AsyncWebServerRequest *request) { schedule_function( [](){ at.write_and_read(REQ_SWR20); }); request->send ( 200, F("text/plain"), F( "2.0:1") );} );
    server.on ("/REQ_SWR25", HTTP_GET, [](AsyncWebServerRequest *request) { schedule_function( [](){ at.write_and_read(REQ_SWR25); }); request->send ( 200, F("text/plain"), F( "2.5:1") );} );
    server.on ("/REQ_SWR30", HTTP_GET, [](AsyncWebServerRequest *request) { schedule_function( [](){ at.write_and_read(REQ_SWR30); }); request->send ( 200, F("text/plain"), F( "3.0:1") );} ); 
    server.on ("/CMD_SWRTHRESH",  HTTP_GET, [](AsyncWebServerRequest *request) { request->send ( 200, F("text/plain"), at.swr_threshold_string());} );
    
    server.on ("/REQ_INDUP", HTTP_GET, [](AsyncWebServerRequest *request) { schedule_function( [](){ at.write_and_read(REQ_INDUP); }); request->send ( 200, F("text/plain"),  "{ \"indval_string\" : \"" + at.indval_string() + "\" }" ); });
    server.on ("/REQ_INDDN", HTTP_GET, [](AsyncWebServerRequest *request) { schedule_function( [](){ at.write_and_read(REQ_INDDN); }); request->send ( 200, F("text/plain"),   "{ \"indval_string\" : \"" + at.indval_string() + "\" }"); });
    server.on ("/REQ_CAPUP", HTTP_GET, [](AsyncWebServerRequest *request) { schedule_function( [](){ at.write_and_read(REQ_CAPUP); }); request->send ( 200, F("text/plain"),  "{ \"capval_string\" : \"" + at.capval_string() + "\"}"  ); });
    server.on ("/REQ_CAPDN", HTTP_GET, [](AsyncWebServerRequest *request) { schedule_function( [](){ at.write_and_read(REQ_CAPDN); }); request->send ( 200, F("text/plain"),  "{ \"capval_string\" : \"" + at.capval_string() + "\"}"  ); });
    server.on ("/REQ_FULLTUNE", HTTP_GET, [](AsyncWebServerRequest *request) { schedule_function( [](){ at.write_and_read(REQ_FULLTUNE); }); request->send ( 200, F("text/plain"), F("{status:\"Tunning\"}")); });

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
void loop() {
   ArduinoOTA.handle();
  at.read();
  run_scheduled_functions();
}
