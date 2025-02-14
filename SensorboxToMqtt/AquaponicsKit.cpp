#include <Arduino.h>
#include "AquaponicsKit.h"
#include <Wire.h>
#include <Ezo_i2c_util.h> 

AquaponicsKit::AquaponicsKit()
        : Sensorbox(),
          PH(PH_ADDRESS, "PH"),
          DO(DO_ADDRESS, "DO"),
          RTD(RTD_ADDRESS, "RTD"),
          EC(EC_ADDRESS, "EC"),
          HUM(HUM_ADDRESS, "HUM"),
          CO2(CO2_ADDRESS, "CO2"),
          current_step_(STEP1),
          t1_(reading_delay_),
          t2_(300),
          t3_(reading_delay_),
          t4_(remaining_poll_delay_),
          next_step_time_(0)
{
}

void AquaponicsKit::setup()
{
        // Start the I2C
        Wire.begin();
        
        // Set enable pins as outputs
        pinMode(EN_PH, OUTPUT);                                                         
        pinMode(EN_DO, OUTPUT);
        pinMode(EN_RTD, OUTPUT);
        pinMode(EN_EC, OUTPUT);
        pinMode(EN_HUM, OUTPUT);
        pinMode(EN_CO2, OUTPUT);

        // Set enable pins to enable the circuits
        digitalWrite(EN_PH, LOW);                                                       
        digitalWrite(EN_DO, LOW);
        digitalWrite(EN_RTD, HIGH);
        digitalWrite(EN_EC, LOW);
        digitalWrite(EN_HUM, HIGH);
        digitalWrite(EN_CO2, HIGH);
}

void AquaponicsKit::update(Measurements& measurements)
{    
        switch (current_step_) {
        
        case STEP1:
                if (millis() >= next_step_time_) {  
                        step1();
                        next_step_time_ = millis() + t1_; 
                        current_step_ = STEP2; 
                }
                break;
        
        case STEP2:
                if (millis() >= next_step_time_) {  
                        step2();
                        next_step_time_ = millis() + t2_; 
                        current_step_ = STEP3; 
                }
                break;
        
        case STEP3:
                if (millis() >= next_step_time_) {  
                        step3();
                        next_step_time_ = millis() + t3_; 
                        current_step_ = STEP4; 
                }
                break;
        
        case STEP4:
                if (millis() >= next_step_time_) {  
                        step4(measurements);
                        next_step_time_ = millis() + t4_; 
                        current_step_ = STEP1; 
                }
                break;
        }
}

void AquaponicsKit::step1() 
{
        // Send a read command. we use this command instead of
        // RTD.send_cmd("R") to let the library know to parse the
        // reading
        RTD.send_read_cmd();
}

void AquaponicsKit::step2() 
{
        RTD.receive_read_cmd();             

        if ((RTD.get_error() == Ezo_board::SUCCESS) 
            && (RTD.get_last_received_reading() > -1000.0)) { 
                // If the temperature reading has been received and it
                // is valid
                PH.send_cmd_with_num("T,", RTD.get_last_received_reading());
                DO.send_cmd_with_num("T,", RTD.get_last_received_reading());
                EC.send_cmd_with_num("T,", RTD.get_last_received_reading());
        } else {
                // If the temperature reading is invalid send default
                // temp = 25 deg C to PH sensor
                PH.send_cmd_with_num("T,", 25.0);
                DO.send_cmd_with_num("T,", 20.0);
                EC.send_cmd_with_num("T,", 25.0);
        }
}

void AquaponicsKit::step3() 
{
        // Send a read command. we use this command instead of
        // PH.send_cmd("R") to let the library know to parse the
        // reading
        PH.send_read_cmd();
        DO.send_read_cmd();
        EC.send_read_cmd();
        HUM.send_read_cmd();
        CO2.send_read_cmd();
}

void AquaponicsKit::step4(Measurements& measurements) 
{
        if ((RTD.get_error() == Ezo_board::SUCCESS) 
            && (RTD.get_last_received_reading() > -1000.0)) {
                measurements.set_temperature(RTD.get_last_received_reading());
        }
        PH.receive_read_cmd();
        if (PH.get_error() == Ezo_board::SUCCESS) {
                measurements.set_ph(PH.get_last_received_reading());
        }
        DO.receive_read_cmd();
        if (DO.get_error() == Ezo_board::SUCCESS) {
                measurements.set_dissolved_oxygen(DO.get_last_received_reading());
        }
        EC.receive_read_cmd(); 
        if (EC.get_error() == Ezo_board::SUCCESS) { 
                measurements.set_electrical_conductivity(EC.get_last_received_reading());
        }
        HUM.receive_read_cmd();
        if (HUM.get_error() == Ezo_board::SUCCESS) {
                measurements.set_humidity(HUM.get_last_received_reading());
        }
        CO2.receive_read_cmd();
        if (CO2.get_error() == Ezo_board::SUCCESS) { 
                measurements.set_co2(CO2.get_last_received_reading());
        }
}

