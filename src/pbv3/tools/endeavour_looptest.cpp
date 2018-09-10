#include <iostream>
#include <iomanip>

#include "EndeavourRaw.h"

int main()
{
  std::cout << "Hello world!" << std::endl;

  std::shared_ptr<DeviceCom> dev; //=std::make_shared<UIOCom>("/dev/uio2", 0x10000);
  EndeavourRaw end(dev);

  std::cout << "Running reset" << std::endl;
  end.reset();

  std::cout << "Writing data" << std::endl;
  end.sendData(0xCCCCDDDDEEEEFFFF, 63);
  std::cout << " nbitsin  = " << dev->read_reg32(1) << std::endl;
  std::cout << " datain  = 0x" << std::hex << std::setw(8) << std::setfill('0') << dev->read_reg32(3) << std::setw(8) << std::setfill('0') << dev->read_reg32(2) << std::dec << std::endl;

  std::cout << "Reading data" << std::endl;
  if(end.isDataValid())
    {
      unsigned long long int data=0;
      unsigned int size=0;
      end.readData(data,size);
      std::cout << " nbitsout = " << size << std::endl;
      std::cout << " dataout = 0x" << std::hex << std::setw(16) << std::setfill('0') << data << std::dec << std::endl;
    }
  else
    std::cout << " No valid data!" << std::endl;

  return 0;
}
