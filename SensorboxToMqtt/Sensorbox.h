#ifndef SENSORBOX_H
#define SENSORBOX_H

#include "Measurements.h"

class Sensorbox
{
public:
        Sensorbox() {}
        virtual ~Sensorbox() = default;

        virtual void setup() = 0;
        
        // Update the sensors. If the return value is true, new sensor
        // values are available in the measurements object passed in
        // as an argument. If false is returned, no new values are
        // available and the measurements object should be ignored.
        virtual void update(Measurements& measurements) = 0;
};

#endif // SENSORBOX_H
