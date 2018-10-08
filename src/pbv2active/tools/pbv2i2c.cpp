//
// Random tests for the active board

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cerrno>
#include <cstring>
#include <clocale>
#include <memory>

#include "I2CDevCom.h"
#include "PCA9548ACom.h"

#include "ComIOException.h"

#include "AMAC.h"

int main(int argc, char* argv[])
{
  if(argc!=2)
    {
      std::cerr << "usage: " << argv[0] << " 1-9" << std::endl;
      return 1;
    }
  uint pbidx=std::stoi(argv[1])-1;
  
  try
    {
      std::shared_ptr<I2CCom> mux0_com=std::make_shared<I2CDevCom>(0x70, "/dev/i2c-0");
      std::shared_ptr<I2CCom> mux1_com=std::make_shared<I2CDevCom>(0x74, "/dev/i2c-0");

      std::vector<std::shared_ptr<AMAC>> pbs(9);

      pbs[0]=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x0, 0, mux1_com));
      pbs[1]=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x0, 1, mux1_com));
      pbs[2]=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x0, 2, mux1_com));
      pbs[3]=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x0, 3, mux1_com));
      pbs[4]=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x0, 4, mux1_com));
      pbs[5]=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x0, 5, mux1_com));
      pbs[6]=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x0, 6, mux1_com));
      pbs[7]=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x0, 7, mux1_com));
      pbs[8]=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x0, 7, mux0_com));

      std::shared_ptr<AMAC> pb=pbs[pbidx];

      int error_ctr = 0, num_attempts_per_val=100;
      for(int j = 0; j < num_attempts_per_val; j++)
	{
	  unsigned val_to_write = rand()%256;
	  unsigned val_received = 0;
	  //Write to any RW register
	  if(pb->write(AMACreg::ILOCK_HV_THRESH_HI_L_CH0,val_to_write))
	    {
	      std::cout << "Write error" << std::endl;
	      error_ctr++;
	    }
	  else
	    {
	      if(pb->read(AMACreg::ILOCK_HV_THRESH_HI_L_CH0,val_received))
		{
		  std::cout << "Read error" << std::endl;
		  error_ctr++;
		}
	      else if(val_to_write != val_received)
		{
		  std::cout << "Data error: 0x" << std::hex << val_to_write << " != 0x" << val_received << std::dec << std::endl;
		  error_ctr++;
		}
	    }
	}
      double result = 1.0 - (double)error_ctr / num_attempts_per_val;
      std::cout << "reliability = " << result << std::endl;

    }
  catch(ComIOException &e)
    {
      std::cout << e.what() << std::endl;
    }

  std::cout << "All good" << std::endl;

  return 0;
}
