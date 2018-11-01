#ifndef AD5160_H
#define AD5160_H

#include "DeviceCom.h"

#include <memory>


class AD5160{
public:
	AD5160(std::shared_ptr<DeviceCom> dev);
	~AD5160();

        // Configurable R_W and R_AB for calibration of the potentiometers
        void set_R_W(float R_W){m_R_W = R_W;};
        void set_R_AB(float R_AB){m_R_AB = R_AB;};
	
	void setResistance(float rOhm);
	void setResistance(uint8_t regVal);
	uint8_t getResistance();

private:
	std::shared_ptr<DeviceCom> m_dev;

        // http://www.analog.com/media/en/technical-documentation/data-sheets/AD5160.pdf
        // For the resistance float -> counts conversion
        float m_R_W = 50;
        float m_R_AB = 100e3;

	uint8_t m_regVal;
	
	// To limit the resistance range
	uint8_t m_regMin = 0x00;
	uint8_t m_regMax = 0xFF;
};
#endif //AD5160_H
