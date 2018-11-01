#include "LineCalibration.h"

#include <cmath>
#include <iostream>

LineCalibration::LineCalibration(double m, double b)
  : m_m(m), m_b(b)
{ }

LineCalibration::~LineCalibration()
{ }

double LineCalibration::calibrate(uint32_t counts)
{ return m_m*counts+m_b; }

uint32_t LineCalibration::uncalibrate(double value)
{ return round((value-m_b)/m_m); }
