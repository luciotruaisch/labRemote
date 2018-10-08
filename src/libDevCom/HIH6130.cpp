#include "HIH6130.h"

#include <unistd.h>
#include <math.h>

#include "NotSupportedException.h"

HIH6130::HIH6130(std::shared_ptr<I2CCom> i2c)
  : m_i2c(i2c)
{ }

HIH6130::~HIH6130()
{ }

void HIH6130::init()
{ }

void HIH6130::reset()
{ }

void HIH6130::read()
{
  unsigned int humidata,tempdata;

  m_i2c->write_reg8(0x0);
  usleep(500e3);

  uint32_t intdata=m_i2c->read_reg32();

  // Parse the data
  m_status=(intdata>>29)&0x3;
  humidata=(intdata>>16)&0x3FFF;
  tempdata=(intdata>> 2)&0x3FFF;

  m_humidity=((float)humidata)/(pow(2,14)-2.);
  m_temperature=((float)tempdata)/(pow(2,14)-2.)*165.-40.;
}

uint HIH6130::status() const
{ return m_status; }

float HIH6130::temperature() const
{ return m_temperature; }

float HIH6130::humidity() const
{ return m_humidity; }

float HIH6130::pressure() const
{ throw NotSupportedException("HIH6130 does not have a pressure sensor"); return 0; }
