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

//#include <i2c/smbus.h>

int main()
{
  std::cout << "Hello World!" << std::endl;

  //int adapter_nr = 2; /* probably dynamically determined */

  int file = open("/dev/i2c-0", O_RDWR);
  if (file < 0)
    {
      std::cerr << "open failed: " << std::strerror(errno) << std::endl;
      return 1;
  }


  int addr = 0x22; /* The I2C address */

  // if (ioctl(file, I2C_SLAVE, addr) < 0)
  //   {
  //     std::cerr << "ioctrl I2C_SLAVE failed: " << std::strerror(errno) << std::endl;
  //     return 1;
  //   }


  int retval;
  uint8_t outbuf[32], inbuf[1];
  struct i2c_msg msgs[2];
  struct i2c_rdwr_ioctl_data msgset[1];

  msgs[0].addr = 0x22;
  msgs[0].flags = 0;
  msgs[0].len = 1;
  msgs[0].buf = inbuf;

  msgs[1].addr = 0x22;
  msgs[1].flags = I2C_M_RD | I2C_M_NOSTART;
  msgs[1].len = 32;
  msgs[1].buf = outbuf;

  msgset[0].msgs = msgs;
  msgset[0].nmsgs = 2;

  inbuf[0] = 0xF0;

  //*result = 0;
  if (ioctl(file, I2C_RDWR, &msgset) < 0)
    {
      std::cerr << "command write failed" << std::endl;
      return 1;
    }
  
  for(int chi=0;chi<16;chi++)
    {
      std::cout << "Read channel " << chi << " = 0x" << std::hex
		<< std::setw(2) << std::setfill('0') << (uint)outbuf[2*chi+0]
		<< std::setw(2) << std::setfill('0') << (uint)outbuf[2*chi+1]
		<< std::dec << std::endl;
      uint result=(outbuf[2*chi+0]<<8)|(outbuf[2*chi+1]<<0);
      uint ch    =((result>>12)&0x003);
      uint counts=((result>> 2)&0x3ff);
      
      std::cout << "\tchannel = " << ch << std::endl;
      std::cout << "\treading  = " << (double)counts/1024*3.3 << " V" << std::endl;
      if(ch==3)
	std::cout << "\tcurrent = " << (double)counts/1024*3.3/50/0.1 << " A" << std::endl;
    }

  std::cout << "All good" << std::endl;
  
  return 0;
}
