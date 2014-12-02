/*
	Tank Temperature Display
	Written by Simon K - 16/11/2014
	Last updated: - See footer.
  
	Based on wireless emonGLCD by: Glyn Hudson, Trystan Lea
	Builds upon JeeLabs RF12 library and Arduino.
	Note that although the physical module is called 'RFM12B', 
	the library is 'RF12'
*/

//------------------------------------------------------------------------------------------------------------------------------------------------
// emonGLCD Home Energy Monitor example
// emonGLCD documentation http://openEnergyMonitor.org/emon/emonglcd
// Correct time is updated via NanodeRF which gets time from internet

// GLCD library by Jean-Claude Wippler: JeeLabs.org
// 2010-05-28 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php

// THIS SKETCH REQUIRES:

//	- OneWire library	http://www.pjrc.com/teensy/td_libs_OneWire.html
//	- DallasTemperature	http://download.milesburton.com/Arduino/MaximTemperature
//                           or https://github.com/milesburton/Arduino-Temperature-Control-Library
//	- JeeLib		https://github.com/jcw/jeelib
//	- RTClib		https://github.com/jcw/rtclib
//	- GLCD_ST7565		https://github.com/jcw/glcdlib
//
// Other files in project directory (should appear in the arduino tabs above)
//	- icons.ino
//	- temp_templates.ino
//
//-------------------------------------------------------------------------------------------------------------------------------------------------

#include <JeeLib.h>
#include <GLCD_ST7565.h>
#include <avr/pgmspace.h>
GLCD_ST7565 glcd;

#include <RTClib.h>                 
// Real time clock (RTC) - used for software RTC to reset kWh counters at midnight

#include <Wire.h>                   
// Part of Arduino libraries - needed for RTClib

RTC_Millis RTC;

//--------------------------------------------------------------------------------------------
// RFM12B Settings
//--------------------------------------------------------------------------------------------
#define MYNODE 15
// Should be unique on network, node ID 30 reserved for base station

#define freq RF12_868MHZ     
// frequency - match to same frequency as RFM12B module (change to 868Mhz or 915Mhz if appropriate)

#define group 200 

unsigned long fast_update;
unsigned long slow_update;

double temp,maxtemp,mintemp;

//---------------------------------------------------
// Data structures for transferring data between units
//---------------------------------------------------

typedef struct { int temp1, temp2, temp3; } PayloadTX;         
// neat way of packaging data for RF comms

PayloadTX emontx;

// Kept for when I transmit data up to the emoncms site.
// typedef struct { int temp1, temp2, temp3, battery; } PayloadTemp;
// PayloadTemp emontemp;

int hour = 12, minute = 0;
double usekwh = 0;

//int cval_use;  Replace with 'top_val' etc.
int top_val;
int mid_val;
int low_val;
int volts =5;

//-------------------------------------------------------------------------------------------- 
// Flow control
//-------------------------------------------------------------------------------------------- 

unsigned long last_emontx;                   
// Used to count time from last emontx update

unsigned long last_emonbase;
// Used to count time from last emontx update


void setup()
{
	Serial.begin(9600);
	delay(500);
	//wait for power to settle before firing up the RF
	
	rf12_initialize(MYNODE, freq, group);
	
	delay(100);
	//wait for RF to settle before turning on display
	
	glcd.begin(0x18);

	glcd.backLight(200);
  
}


void loop()
{
  
if (rf12_recvDone())
	{
		if (rf12_crc == 0 && (rf12_hdr & RF12_HDR_CTL) == 0)  // and no rf errors
			{
				int node_id = (rf12_hdr & 0x1F);
				if (node_id == 10) {emontx = *(PayloadTX*) rf12_data; last_emontx = millis();}  
				//Assuming 10 is the emonTx NodeID
			/*
				if (node_id == 12)			//Assuming 12 is the emonBase node ID
				{
				RTC.adjust(DateTime(2012, 1, 1, rf12_data[1], rf12_data[2], rf12_data[3]));
				last_emonbase = millis();
				} 
			*/	
			}
	}

//--------------------------------------------------------------------------------------------
// Display update every 200ms
//--------------------------------------------------------------------------------------------
if ((millis()-fast_update)>200)
	{
		fast_update = millis();
    
		DateTime now = RTC.now();
		int last_hour = hour;
		hour = now.hour();
		minute = now.minute();

 
		top_val = (emontx.temp1);
		mid_val = (emontx.temp2);
		low_val = (emontx.temp3);
		//volts   = (emontx.battery);
		
		Serial.print(emontx.temp1);
		Serial.println( );
				    
		draw_temp_page( "Top" ,top_val, "Mid", mid_val, "Low", low_val);

		draw_temperature_volts_footer(temp, volts);
		glcd.refresh();

		// int LDR = analogRead(LDRpin);  (Not used as no LDR fitted).
		// Read the LDR Value so we can work out the light level in the room.
		
		// int LDRbacklight = map(LDR, 0, 1023, 50, 250);
		// Map the data from the LDR from 0-1023 (Max seen 1000) to var GLCDbrightness min/max
		
		//int LDRbacklight = (50);
		// Set back light level as no LDR fitted.  Must be between 0 and 255.
		
		
		//LDRbacklight = constrain(LDRbacklight, 0, 255);   
		// Constrain the value to make sure its a PWM value 0-255
		
		//if ((hour > 22) ||  (hour < 5)) glcd.backLight(0); else glcd.backLight(LDRbacklight);
		// Turn the back-light off at night.
	} 
/*
if ((millis()-slow_update)>5000)
	{
		slow_update = millis();

		
		Temperature sensing removed for now;
		sensors.requestTemperatures();
		temp = (sensors.getTempCByIndex(0));
		if (temp > maxtemp) maxtemp = temp;
		if (temp < mintemp) mintemp = temp;
		
   
		// Temperature sending remove for now.
		// emonglcd.temperature = (int) (temp * 100);                          
		// set emonglcd payload
		
		// rf12_sendNow(0, &emonglcd, sizeof emonglcd);
		// send temperature data via RFM12B using new rf12_sendNow wrapper -glynhudson
		
		rf12_sendWait(2);
		
	}
*/  
}