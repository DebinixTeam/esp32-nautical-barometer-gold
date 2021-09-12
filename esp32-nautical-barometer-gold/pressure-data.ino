#if MYDEBUG == 1
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MINPRESSURE 1003    // hPa/mbar
#define MAXPRESSURE 1023    // hPa/mbar
#define MAXHOURTIMESLOT 11

int16_t *update_pressure_array(int16_t pressure_now);
int16_t *map_pressure_values(int16_t *pressure_array);
int16_t is_outside_range(int16_t *pressure_array);
#endif

//====================================================
// update_pressure_array: Returns an updated pressure
// reading in an MAXHOURTIMESLOT long array.
//====================================================
int16_t *update_pressure_array(int16_t pressure_now) {
    #if MYDEBUG == 1
    //static int16_t pressure_data[11] = {1008, 1009, 1010, 1011, 1012, 1013, 1014, 1015, 1016, 1017, 1018};    // All values in [1008, 1018]
    //static int16_t pressure_data[11] = {1008, 1009, 1007, 1011, 1012, 1013, 1014, 1015, 1016, 1017, 1018};    // 1007 is outside range - Under pressure
    //static int16_t pressure_data[11] = {1008, 1009, 1010, 1011, 1012, 1013, 1014, 1015, 1016, 1019, 1018};    // 1019 is outside range - Over pressure
    //static int16_t pressure_data[11] = {1008, 1009, 1010, 1002, 1012, 1013, 1014, 1020, 1016, 1017, 1018};    // 1002 and 1020 are outside range, total range exceeds 10 mb
    //static int16_t pressure_data[11] = {1007, 1007, 1005, 1011, 1012, 1013, 1014, 1020, 1018, 1018, 1018};    // Multiple various values in and out [1008, 1018]
    //static int16_t pressure_data[11] = {1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013};  
    #endif

    // Default pressure array at power start up
    static int16_t pressure_data[MAXHOURTIMESLOT] = {MINPRESSURE, MINPRESSURE, MINPRESSURE, MINPRESSURE, MINPRESSURE, MINPRESSURE, MINPRESSURE, MINPRESSURE, MINPRESSURE, MINPRESSURE, MINPRESSURE};
          
    // Initial setup, shift out all previous pressure elements by one hour and add a new 'Now' pressure (at 0h) in pressure_data[0] position
    for (int8_t i = MAXHOURTIMESLOT-1; i > 0 ; i--) {
        pressure_data[i] = pressure_data[i-1];
    }
    pressure_data[0] = pressure_now;

    return pressure_data;
}

//====================================================
// map_pressure_values: Map actual pressure values in the
// a pressure range [1003,1023] 'mbar' to fit scale.
//====================================================
int16_t *map_pressure_values(int16_t *pressure_array) {

    static int16_t meter_data[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int8_t over_pressure = 0;
    int8_t under_pressure = 0;   // False

    for (int8_t i=0; i < MAXHOURTIMESLOT ; i++ ) {

            switch (pressure_array[i]) {

                case 1023:   meter_data[i] = 100 ;break;   // Maximum pressure shown
                case 1022:   meter_data[i] = 95 ;break;
                case 1021:   meter_data[i] = 90 ;break;
                case 1020:   meter_data[i] = 85 ;break;
                case 1019:   meter_data[i] = 80 ;break;
                case 1018:   meter_data[i] = 75 ;break;
                case 1017:   meter_data[i] = 70 ;break;
                case 1016:   meter_data[i] = 65 ;break;
                case 1015:   meter_data[i] = 60 ;break;
                case 1014:   meter_data[i] = 55 ;break;
                case 1013:   meter_data[i] = 50 ;break;   // Nominal pressure -- middle of scale
                case 1012:   meter_data[i] = 45 ;break;
                case 1011:   meter_data[i] = 40 ;break;
                case 1010:   meter_data[i] = 35 ;break;
                case 1009:   meter_data[i] = 30 ;break;
                case 1008:   meter_data[i] = 25 ;break;
                case 1007:   meter_data[i] = 20 ;break;
                case 1006:   meter_data[i] = 15 ;break;
                case 1005:   meter_data[i] = 10 ;break;
                case 1004:   meter_data[i] =  5 ;break;
                case 1003:   meter_data[i] =  0 ;break;    // Minimum pressure shown

                default:
                    #if MYDEBUG == 1
                    printf("\nmap_pressure_values(Value %d falls outside acceptable pressure range [%d, %d])",pressure_array[i], MINPRESSURE, MAXPRESSURE);
                    #endif

                    if (pressure_array[i] > MAXPRESSURE ) {
                        over_pressure = 1 ;
                        meter_data[i] = 100 ;   // Set at max meter value
                    }
                    if (pressure_array[i] < MINPRESSURE ) {
                        under_pressure = 1 ;
                        meter_data[i] = 0 ;     // Set at min meter value
                    }

            }

            // Need to offset array to fit range of values onto meter scale
            if (over_pressure && under_pressure) {
                ;                  // NOP, cannot do anything pressure values exceeds available range span
            }
            else if (over_pressure || under_pressure) {
                if (over_pressure) {
                    #if MYDEBUG == 1    
                    printf("\nmap_pressure_values(Over pressure > %d)", MAXPRESSURE);
                    #endif
                    over_pressure = 0;
                }
                if (under_pressure) {
                    #if MYDEBUG == 1
                    printf("\nmap_pressure_values(Under pressure < %d)", MINPRESSURE);
                    #endif
                    under_pressure = 0;
                }
            }

    }

return meter_data;

}

//====================================================
// is_outside_range: Verify if all pressure values
// will fit on the meter scale (returns 0), or if any
// single value is either to high or to low, and then
// teturns 999 or -999 respectively. Returns 0 if
// values span is greater than 2023 - 1003 = 20 'mbar'.
//====================================================

int16_t is_outside_range(int16_t *pressure_array) {

    int8_t over_pressure_flag = 0;   // false
    int8_t under_pressure_flag = 0;   // false

    // Return the outside pressure element value as -999 or 999 or 0 for either,
    // all elements in range, or find elements with over- and under-pressure at the same time.
    for (int8_t i=0; i < MAXHOURTIMESLOT ; i++ ) {

        if (pressure_array[i] > MAXPRESSURE ) {
            over_pressure_flag = 1 ;
        }
        if (pressure_array[i] < MINPRESSURE ) {
            under_pressure_flag = 1 ;
        }
    }

    if (over_pressure_flag && under_pressure_flag) {
        #if MYDEBUG == 1
        printf("\nis_outside_range(Detected both an over- and under pressure)");
        #endif
        // NOP, cannot do anything if pressure values exceeds available range span
        return 0;            
    }
    else if (over_pressure_flag || under_pressure_flag) {
        if (over_pressure_flag) {
            #if MYDEBUG == 1
            printf("\nis_outside_range(Detected over pressure value)");
            #endif
            return 999;
        }
        if (under_pressure_flag) {
            #if MYDEBUG == 1
            printf("\nis_outside_range(Detected under pressure value)");
            #endif
            return -999;
        }
    }

    return 0;   // All values are mappable within pressure range

}
