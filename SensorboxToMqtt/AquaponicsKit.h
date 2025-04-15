#ifndef AQUAPONICSKIT_H
#define AQUAPONICSKIT_H

#include "Sensorbox.h"

#include <Ezo_i2c.h> 
#include <sequencer4.h>

class AquaponicsKit : public Sensorbox
{
protected:
        // The I2C addresses of each circuit.
        static const int PH_ADDRESS = 99;
        static const int DO_ADDRESS = 97;
        static const int RTD_ADDRESS = 102;
        static const int EC_ADDRESS = 100;
        static const int HUM_ADDRESS = 111;
        static const int CO2_ADDRESS = 105;
        
        // Enable pins for each circuit. For board version 1.8.
        static const int EN_PH = 12;
        static const int EN_DO = 27;
        static const int EN_RTD = 15;
        static const int EN_EC = 33;
        static const int EN_HUM = 14;
        static const int EN_CO2 = 32;

        // How long we wait to receive a response, in milliseconds
        static const unsigned long reading_delay_ = 1000;                   

        // How long to wait between polls
        //static const unsigned int poll_delay_ = 5 * 60 * 1000; // five minutes   
        //static const unsigned long poll_delay_ = 1 * 60 * 1000; // one minute   
        static const unsigned int poll_delay_ = 2 * 1000; // two seconds   
        
        // How long to wait between polls after accounting for the
        // times it takes to send readings
        static const unsigned long remaining_poll_delay_ = poll_delay_ - 2 * reading_delay_ - 300;   

        enum reading_step {STEP1, STEP2, STEP3, STEP4};
        
        Ezo_board PH;
        Ezo_board DO;
        Ezo_board RTD;
        Ezo_board EC;
        Ezo_board HUM;
        Ezo_board CO2;
        enum reading_step current_step_;
        unsigned long t1_;
        unsigned long t2_;
        unsigned long t3_;
        unsigned long t4_;
        uint32_t next_step_time_;

public:

        AquaponicsKit();
        virtual ~AquaponicsKit() = default;

        virtual void setup();
        virtual void update(Measurements& measurements);

protected:
        void step1();
        void step2();
        void step3();
        void step4(Measurements& measurements);
};

#endif // AQUAPONICSKIT_H
