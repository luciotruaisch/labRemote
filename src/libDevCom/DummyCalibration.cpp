#include "DummyCalibration.h"

#include <cmath>

double DummyCalibration::calibrate(uint32_t counts)
{ return counts; }

uint32_t DummyCalibration::uncalibrate(double value)
{ return round(value); }
