//
// Monitor the PBv2 Massive Tester

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cerrno>
#include <cstring>
#include <clocale>
#include <memory>
#include <thread>
#include <chrono>

#include "PBv2TB.h"

#include "ComIOException.h"

int main()
{
  try
    {
      PBv2TB tb;

      std::cout << "P5VCurrent" << "\t"
		<< "M5VCurrent" << "\t"
		<< "Vin" << "\t"
		<< "VinCurrent" << std::endl;

      for(uint i=0; i<100; i++)
	{
	  std::cout << tb.getP5VCurrent() << "\t"
		    << tb.getM5VCurrent() << "\t"
		    << tb.getVin() << "\t"
		    << tb.getVinCurrent() << std::endl;

	  std::this_thread::sleep_for(std::chrono::seconds(1));
	}
    }
  catch(ComIOException &e)
    {
      std::cout << e.what() << std::endl;
    }

  // std::cout << "All good" << std::endl;
  
  return 0;
}
