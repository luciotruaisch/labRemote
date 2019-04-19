#include "LinearCalibration.h"

#include <cmath>

LinearCalibration::LinearCalibration(double reference, uint32_t max)
  : m_reference(reference), m_max(max)
{ }

LinearCalibration::~LinearCalibration()
{ }

double LinearCalibration::calibrate(uint32_t counts)
{ return (double)counts/m_max*m_reference; }

uint32_t LinearCalibration::uncalibrate(double value)
{ return round(value*m_max/m_reference); }
