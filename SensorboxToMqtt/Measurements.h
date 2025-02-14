#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

class Measurements
{
protected:
        static constexpr float default_ph_ = -1.0f;
        static constexpr float default_dissolved_oxygen_ = -1.0f;
        static constexpr float default_electrical_conductivity_ = -1.0f;
        static constexpr float default_temperature_ = -273.15f;
        static constexpr float default_humidity_ = -1.0f;
        static constexpr float default_co2_ = -1.0f;

        bool has_temperature_;
        bool has_ph_;
        bool has_dissolved_oxygen_;
        bool has_electrical_conductivity_;
        bool has_humidity_;
        bool has_co2_;

        float temperature_;
        float ph_;
        float dissolved_oxygen_;
        float electrical_conductivity_;
        float humidity_;
        float co2_;

public:

        Measurements() {
                reset();
        }

        virtual ~Measurements() = default;

        void reset() {
                has_temperature_ = false;
                has_ph_ = false;
                has_dissolved_oxygen_ = false;
                has_electrical_conductivity_ = false;
                has_humidity_ = false;
                has_co2_ = false;
                ph_ = default_ph_;
                dissolved_oxygen_ = default_dissolved_oxygen_;
                temperature_ = default_temperature_;
                electrical_conductivity_ = default_electrical_conductivity_;
                humidity_ = default_humidity_;
                co2_ = default_co2_;
        }

        bool has_measurements() {
                return has_temperature()
                        || has_ph()
                        || has_dissolved_oxygen()
                        || has_electrical_conductivity()
                        || has_humidity()
                        || has_co2();
        }

        bool has_temperature() {
                return has_temperature_;
        }

        bool has_ph() {
                return has_ph_;
        }

        bool has_dissolved_oxygen() {
                return has_dissolved_oxygen_;
        }

        bool has_electrical_conductivity() {
                return has_electrical_conductivity_;
        }

        bool has_humidity() {
                return has_humidity_;
        }

        bool has_co2() {
                return has_co2_;
        }

        float get_temperature() {
                return temperature_;
        }

        float get_ph() {
                return ph_;
        }

        float get_dissolved_oxygen() {
                return dissolved_oxygen_;
        }

        float get_electrical_conductivity() {
                return electrical_conductivity_;
        }

        float get_humidity() {
                return humidity_;
        }

        float get_co2() {
                return co2_;
        }

        void set_temperature(float v) {
                temperature_ = v;
                has_temperature_ = true;
        }

        void set_ph(float v) {
                ph_ = v;
                has_ph_ = true;
        }

        void set_dissolved_oxygen(float v) {
                dissolved_oxygen_ = v;
                has_dissolved_oxygen_ = true;
        }

        void set_electrical_conductivity(float v) {
                electrical_conductivity_ = v;
                has_electrical_conductivity_ = true;
        }

        void set_humidity(float v) {
                humidity_ = v;
                has_humidity_ = true;
        }

        void set_co2(float v) {
                co2_ = v;
                has_co2_ = true;
        }

};

#endif // MEASUREMENTS_H
