//====================================================
// debug_sensor_bme680: Prints BME680 environmental
// values and timing info to Arduino serial port.
//====================================================
void debug_sensor_bme680(int32_t temp, int32_t humidity, int32_t pressure, int16_t rtc_minute, int16_t rtc_second) {

    char buf[60];                 

    sprintf(buf, "%3d.%02d ",(int8_t)(temp / 100), (uint8_t)(temp % 100));        // Temp in degree C
    Serial.print(buf);
    
    sprintf(buf, "%3d.%03d ", (int8_t)(humidity / 1000), (uint16_t)(humidity % 1000)); 
    Serial.print(buf);
    
    sprintf(buf, "%5d.%02d", (int16_t)(pressure / 100), (uint8_t)(pressure % 100));    
    Serial.print(buf);

    sprintf(buf, "   %d  %d", rtc_minute, rtc_second);  
    Serial.print(buf);   
    
    sprintf(buf, " [C], [%%RH], [mbar] [min  sec]");
    Serial.println(buf);
    
}
