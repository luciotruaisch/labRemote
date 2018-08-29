//
// Random tests for the active board

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cerrno>
#include <cstring>
#include <clocale>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/ioctl.h>

#include <unistd.h>

#include "I2CDevCom.h"

int main()
{
  I2CDevCom com(0x22, "/dev/i2c-0");

  std::vector<uint8_t> result=com.read_block(0xF0, 8);
  
  for(int chi=0;chi<4;chi++)
    {
      std::cout << "Read index " << chi << " = 0x" << std::hex
		<< std::setw(2) << std::setfill('0') << (uint)result[2*chi+0]
		<< std::setw(2) << std::setfill('0') << (uint)result[2*chi+1]
		<< std::dec << std::endl;
      uint chrest=(result[2*chi+0]<<8)|(result[2*chi+1]<<0);
      uint ch    =((chrest>>12)&0x007);
      uint counts=((chrest>> 2)&0x3ff);
      
      std::cout << "\tchannel = " << ch << std::endl;
      std::cout << "\treading  = " << (double)counts/1024*3.3 << " V" << std::endl;
    }

  // std::cout << "All good" << std::endl;
  
  return 0;
}
