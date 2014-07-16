
// OSCClass iOSC(iPhone App) test sketch
// OSCClass version 1.0.1 (Arduino ver0014)
// Copyright (c) recotana(http://recotana.com).  All right reserved.
// Modified by Nick Verwymeren (http://makesomecode.com) December 30, 2009

/*
iOSC is OSCsend Application for iPhone
http://recotana.com/iphone/iosc/

iOSC setting
            OSCMessage    type  value   on  off
slider1    /dmx/1    float          1   0   
slider2    /dmx/2    float          1   0
slider3    /dmx/3    float          1   0

host setting IP address 192.168.000.206 , port 10000

*/

#include "SPI.h"
#include "Ethernet.h"
#include "OSCClass.h"
#include <Conceptinetics.h>

// Define the number of DMX channels to use (1-512) limit to save memory
#define DMX_MASTER_CHANNELS   100

// Pin number to change read or write mode on the shield
#define RXEN_PIN                2

// Configure a DMX master controller, the master controller
// will use the RXEN_PIN to control its write operation on the bus
DMX_Master        dmx_master ( DMX_MASTER_CHANNELS, RXEN_PIN );


  OSCMessage recMes;
  
  OSCClass osc(&recMes);

  byte serverMac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  byte serverIp[]  = { 192, 168, 000, 206 };
  int  serverPort  = 10000;
  
//  byte gateway[]   = { 192, 168, 0, 1 };
//  byte subnet[]    = { 255, 255, 255, 0 };

  
  char *topAddress="dmx";

  
void setup() {
     // Enable DMX master interface and start transmitting
     dmx_master.enable ();
 
     // Set all channels to 0
     dmx_master.setChannelRange(1, 512, 0);
 
     Ethernet.begin(serverMac ,serverIp);
    
     //setting osc recieve server
     osc.begin(serverPort);
     
    //osc message buffer clear
     osc.flush();
     
}

void loop() {

    //Do we have an OSC message?
    if ( osc.available() ) {
      
      //Uncomment below line for debugging through serial port
      //logMessage(&recMes);
      
      //toplevel address matching
      if( !strcmp( recMes.getAddress(0) , topAddress ) ){
          
          //Get our variables from the message
          char *channel   = recMes.getAddress(1);
          float value     = (float)recMes.getArgFloat(0);
          
          //Convert our float value to integer
          int intvalue = int(255.0*value);
          
          //Send out the DMX values
          dmx_master.setChannelValue(atoi(channel), intvalue);
      } 
    
    }
    

}



// *********  utility  ***********************************


void logMessage(OSCMessage *mes){
  
    uint8_t *ip=mes->getIp();
  
     //disp ip & port
    Serial.print("from IP:");
    Serial.print(ip[0],DEC);
    Serial.print(".");
    Serial.print(ip[1],DEC);
    Serial.print(".");
    Serial.print(ip[2],DEC);
    Serial.print(".");
    Serial.print(ip[3],DEC);
    Serial.print(" port:");
    Serial.print(mes->getPort(),DEC);
    Serial.print("   ");
    
    //disp adr
    for(int i = 0 ; i < mes->getAddressNum() ; i++){
      
      Serial.print(mes->getAddress(i));
      
    }
    
    
    //disp type tags
    Serial.print("  ,");
    for(int i = 0 ; i < mes->getArgNum() ; i++){
      
      Serial.print(mes->getTypeTag(i));
      
    }
    Serial.print(" ");
   
   
   //disp args
    for(int i = 0 ; i < mes->getArgNum() ; i++){
      
      switch( mes->getTypeTag(i) ){
        
        case 'i': {
                      
                      Serial.print( mes->getArgInt(i) );
                  }
          break;
        
        case 'f':  {
                      
                      Serial.print( mes->getArgFloat(i) );
                  }
          break;
         
      }
      
       Serial.print(" ");
      
    }
    
    Serial.println(""); 
    
}

