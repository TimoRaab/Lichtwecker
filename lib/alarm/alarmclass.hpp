#ifndef alarmclass_hpp
#define alarmclass_hpp

#include "Esp.h"

#define ONLY_WEEKDAYS 248;
#define ONLY_WEEKEND 6;

class alarmclass {
    private:
        boolean active;
        byte weekdays; 
        byte hour;
        byte minute;
        boolean sunrise;
        byte sunriseMinutes;
        
        String wakeupTune;
        String sunriseTune;

        byte wakeupTuneVolume;
        String sunriseTuneVolume;
}


#endif
//EOF