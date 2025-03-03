// Setup function for the ADS1115 ADC module
void ads_setup(){
  ads.begin(); //initialize the ADC

  if (!ads.begin())
  {
    Serial.println("Failed to initialize ADS.");
    lcd.setCursor (0, 3 ); // go to the next line
    lcd.print F(("ADS Fail"));
    while (1);
  }
  if (ads.begin())
  {
    lcd.setCursor (0, 3 ); // go to the next linelcd.print F(("ADS good"));
  }
}