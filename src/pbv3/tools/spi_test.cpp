#include <iostream>
#include <iomanip>

#include <unistd.h>

#include "EndeavourRawFTDI.h"
#include "EndeavourComException.h"

int main()
{
  std::cout << "Hello world!" << std::endl;

  EndeavourRawFTDI end;

  end.setOF(true);
  
  end.getDAC()->set(1);

  for(uint8_t ch=0; ch<4; ch++)
    std::cout << "CH" << (uint32_t)ch << ": " << end.getADC()->read(ch) << std::endl;

  return 0;
}
