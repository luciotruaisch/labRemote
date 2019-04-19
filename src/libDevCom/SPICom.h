#ifndef SPICOM_H
#define SPICOM_H

#include "DeviceCom.h"

#include <string>
#include <vector>

class SPICom : public DeviceCom
{
public:
  SPICom();
  virtual ~SPICom();
};

#endif // SPICOM_H
