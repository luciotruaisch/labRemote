#include "AMAC.h"
#include "EndeavourRawFTDI.h"

#include "ComIOException.h"
#include "EndeavourComException.h"

#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>

int main()
{
  AMAC amac(0xF,std::unique_ptr<EndeavourRaw>(new EndeavourRawFTDI()));

  uint trails=1000;
  uint good=0;
  for(uint i=0; i<trails; i++)
    {
      try
        {      
          uint valin=rand()*0xFFFFFFFF;
          amac.write_reg(166,valin);
          usleep(50);
          uint valout=amac.read_reg(166);
          usleep(50);
          if(valin==valout)
            {
              good++;
            }
          else
            {
	      std::cout << "Write: 0x" << std::hex << valin << ", Read: " << valout << std::dec << std::endl;
            }
        }
      catch(EndeavourComException &e)
        {
          //std::cout << e.what() << std::endl;
        }
    }

  std::cout << "Reliability: " << ((float)good)/trails << std::endl;



  return 0;
}
