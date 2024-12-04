//*** take_measurement() function outputs ADC reading in volts ***//
// Function to take a redox potential measurement
//Reference Probes- relay 1
float take_measurement(){
  int16_t in;
  float inVolts;
  digitalWrite(relay1, HIGH); //Connect probe to instrumentation amplifier [changed the polarity for SSR (changed by TH)]
  delay(10); //Give probe a (hundredth of a) second--maybe unnecessary?
  in = ads.readADC_SingleEnded(redoxChannel); //Take reading with ADC
  inVolts = ads.computeVolts(in); //Convert to volts
  digitalWrite(relay1, LOW); //Disconnect probe from amplifier [changed the polarity for SSR (changed by TH)]
  return inVolts;
}


// Calibration function to determine the amplifier offset with referrence to PT1
float zeroPT1(){
  int16_t in;
  float inVolts;
  digitalWrite(relay2, HIGH); // Close relay2, short inputs to the amplifier [changed the polarity for SSR (changed by TH)]
  delay(100); // I think there should be a delay after relay closes
  in = ads.readADC_SingleEnded(redoxChannel);
  inVolts = ads.computeVolts(in);
  digitalWrite(relay2, LOW); // [changed the polarity for SSR (changed by TH)]
  return inVolts;
}

// Calibration function to determine the amplifier offset with referrence to PT2
float zeroPT2(){
  int16_t in;
  float inVolts;
  digitalWrite(relay3, HIGH); // Close relay2, short inputs to the amplifier [changed the polarity for SSR (changed by TH)]
  delay(100); // I think there should be a delay after relay closes
  in = ads.readADC_SingleEnded(redoxChannel);
  inVolts = ads.computeVolts(in);
  digitalWrite(relay3, LOW); // [changed the polarity for SSR (changed by TH)]
  return inVolts;
}

// Calibration function to determine the amplifier offset with referrence to PT3
float zeroPT3(){
  int16_t in;
  float inVolts;
  digitalWrite(relay4, HIGH); // Close relay2, short inputs to the amplifier [changed the polarity for SSR (changed by TH)]
  delay(100); // I think there should be a delay after relay closes
  in = ads.readADC_SingleEnded(redoxChannel);
  inVolts = ads.computeVolts(in);
  digitalWrite(relay4, LOW); // [changed the polarity for SSR (changed by TH)]
  return inVolts;
}
