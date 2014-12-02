#include "utility/font_helvB24.h"
#include "utility/font_helvB14.h"
#include "utility/font_helvB12.h"
#include "utility/font_clR4x6.h"
#include "utility/font_clR6x8.h"

//------------------------------------------------------------------
// Draws a page showing a single power and energy value in big font
//------------------------------------------------------------------
void draw_temp_page(char* upper, double top_val, char* middle,  double mid_val,  char* lower,  double low_val)
{ 
  glcd.clear();
  glcd.fillRect(0,0,128,64,0);
  
  char str[50];    			 //variable to store conversion 
  glcd.setFont(font_clR6x8);            
  
  strcpy(str,upper);                  // Draw the value of temp1
  strcat(str," of Tank:"); 
  glcd.drawString(0,0,str);
  
  strcpy(str,middle);  
  strcat(str," of Tank:"); 
  glcd.drawString(0,16,str); //was 38
  
  strcpy(str,lower);  
  strcat(str," of Tank:"); 
  glcd.drawString(0,32,str); 
  
  // Top of tank
  glcd.setFont(font_helvB12);  
  itoa((int)top_val,str,10);
  strcat(str,"c");   
  glcd.drawString(80,0,str);
  
  // Middle of tank.
  glcd.setFont(font_helvB12); 
  itoa((int)mid_val,str,10);
  strcat(str,"c");
  glcd.drawString(80,16,str);  

  // Bottom of tank.
  glcd.setFont(font_helvB12); 
  itoa((int)low_val,str,10);
  strcat(str,"c");
  glcd.drawString(80,32,str);   
}


//------------------------------------------------------------------
// Draws a footer showing GLCD temperature and the time
//------------------------------------------------------------------
// void draw_temperature_time_footer(double temp, double mintemp, double maxtemp, double hour, double minute)

void draw_temperature_volts_footer(double temp, double volts)

{
  glcd.drawLine(0, 47, 128, 47, WHITE);     //middle horizontal line 

  char str[50];
  // Draw Temperature
  glcd.setFont(font_helvB12);  
  dtostrf(temp,0,1,str); 
  strcat(str,"C");
  glcd.drawString(0,50,str);  
  
  // Draw Volts
  glcd.setFont(font_helvB12);             
  itoa((int)volts,str,10);
  strcat(str,"V");
    glcd.drawString(80,50,str); 
}