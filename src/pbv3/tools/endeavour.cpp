#include <unistd.h>
#include <string.h>

#include <iostream>
#include <iomanip>

#include "EndeavourRawFTDI.h"
#include "EndeavourCom.h"
#include "EndeavourComException.h"

void print_help()
{
  std::cout << "usage: endeavour [driver options] command [command parameters]" << std::endl;
  std::cout << std::endl;
  std::cout << "driver options" << std::endl;
  std::cout << " -a amacid: amacid to use for communication (default: 0x15)" << std::endl;
  std::cout << std::endl;
  std::cout << "commands" << std::endl;
  std::cout << " reset: reset the Endeavour master firmware" << std::endl;
  std::cout << " setid idpads/efuse internalid: set amacid for internalid efuse or idpads" << std::endl;
  std::cout << " write address data: write data to address " << std::endl;
  std::cout << " read address: read address " << std::endl;
  std::cout << " readnext: read next register " << std::endl;
}

int main(int argc, char* argv[])
{
  unsigned short int amacid=0xF;

  // Parse options
  int opt;
  while( (opt = getopt(argc, argv, "d:a:")) != -1 )
    {
      switch( opt )
	{
	case 'a':
	  amacid=std::stoul(optarg, nullptr, 0);
	  break;
	case '?':  // unknown option...
	  print_help();
	  return 1;
	  break;
	}
    }

  // Determine the command
  if(argc<=optind)
    {
      print_help();
      return 1;
    }

  std::string command=argv[optind];

  // Run the commands
  try
    {
      // Create the communication object
      EndeavourCom end(amacid,std::unique_ptr<EndeavourRaw>(new EndeavourRawFTDI()));

      if(command=="reset")
	{
	  end.reset();
	}
      else if(command=="setid")
	{
	  // Need two more
	  if(argc<=(optind+2))
	    {
	      print_help();
	      return 1;
	    }

	  EndeavourCom::REFMODE refmode=(strcmp(argv[optind+1],"idpads")==0)?EndeavourCom::REFMODE::IDPads:EndeavourCom::REFMODE::EfuseId;
	  unsigned int refid=std::stoul(argv[optind+2], nullptr, 0);

	  end.setid(refmode,refid);
	}
      else if(command=="write")
	{
	  // Need two more
	  if(argc<=(optind+2))
	    {
	      print_help();
	      return 1;
	    }

	  unsigned int address=std::stoul(argv[optind+1], nullptr, 0);
	  unsigned int data=std::stoul(argv[optind+2], nullptr, 0);

	  end.write_reg(address,data);
	}
      else if(command=="read")
	{
	  // Need one more
	  if(argc<=(optind+1))
	    {
	      print_help();
	      return 1;
	    }

	  unsigned int address=std::stoul(argv[optind+1], nullptr, 0);

	  unsigned int read=end.read_reg(address);
	  std::cout << "0x" << std::hex << std::setw(8) << std::setfill('0') << read << std::endl;
	}
      else if(command=="readnext")
	{
	  // Need one more
	  if(argc<=(optind+0))
	    {
	      print_help();
	      return 1;
	    }

	  unsigned int read=end.readnext_reg();
	  std::cout << "0x" << std::hex << std::setw(8) << std::setfill('0') << read << std::endl;
	}
   }
  catch(EndeavourComException e)
    {
      std::cout << e.what() << std::endl;
    }

  return 0;
}
