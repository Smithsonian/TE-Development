// Setup function for the SD card module
void sdcard_setup(){
  if (SD.begin(chipSelect))
  {
    //lcd.clear();
    lcd.setCursor (0, 1); // go to the next line
    lcd.print F(("CARD PASS"));
  }
  // Open up the file we're going to log to
   File dataFile = SD.open("datalog.txt", FILE_WRITE);
   if (! dataFile)
  {
     //lcd.clear();
     lcd.setCursor(0, 2);
     lcd.println F(("error datalog"));
     // Wait forever since we cant write data
     while (1);
  }
   if (dataFile)
  {
     //lcd.clear();
     lcd.setCursor(0, 2);
     lcd.print F(("datalog cool"));
  }
}

// Function to log the redox potential measurement to the SD card
void sd_task(){
  File dataFile= SD.open("datalog.txt", FILE_WRITE);
  DateTime now = rtc.now();
  if (dataFile) {
    //if (data_count == 1) //record calibration at beginning of data batch
    //{
      //dataFile.print("\n\n"); //put an empty line to indicate a new batch of data
   //   dataFile.print("New Batch: offset = ");
   //   dataFile.print(offset);
   //   dataFile.print(" mV");
    //}
    //data will be in the format: {Data count, Time stamp, Time since data batch began (between resets),Redox in mV}
    //dataFile.print("\n");
    //dataFile.print(data_count);//TBT
    //dataFile.print(",");
    dataFile.print(offsetPT1);//TBT
    dataFile.print(",");
    dataFile.print(offsetPT2);//TBT
    // dataFile.print(",");
    // dataFile.print(offsetPT3);
    dataFile.print(",");
    dataFile.print(now.timestamp(DateTime::TIMESTAMP_FULL));
    dataFile.print(",");
    dataFile.print(cM);
    dataFile.print(",");
    dataFile.print(redoxmV_adjustedPT1);//TBT
    dataFile.print(",");
    dataFile.print(redoxmV_adjustedPT2);//TBT
    // dataFile.print(",");
    // dataFile.print(redoxmV_adjustedPT3);//TBT
    dataFile.print("\n");
    dataFile.flush();
    dataFile.close();
  }
}