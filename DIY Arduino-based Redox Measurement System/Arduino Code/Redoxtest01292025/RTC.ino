// Setup function for the RTC module
void rtc_setup(){
   rtc.begin(); //initialize the realtime clock

  if (! rtc.begin())
  { 
    lcd.clear();
    lcd.setCursor (0, 0 ); // go to the next line
    lcd.print F(("RTC Fail"));
    while (1) delay(100);
  }


  if (rtc.isrunning())
  {
    lcd.setCursor (0, 0 );
    lcd.print F(("RTC good"));
    delay(10);  
  }
}