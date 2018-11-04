#include <memory>
#include <iostream>

#include "PBv2TB.h"
#inlcude "PBv2Test.h"

int main(int argc, char* argv[])
{
  if (argc != 3)
    {
      std::cout << "Not enough argument" << std::endl;
      return -1;
    }

  //Test name
  std::string TestName = argv[1];
  //Tested PB
  uint8_t pbidx = std::stoi(argv[2]);

  //Initialisation of TB
  std::shared_ptr<PBv2TB> tb = std::make_shared<PBv2TB>();
  PBv2Test test(TestName, tb, pbidx);

  tb->getPB(pbidx)->init();


  test.runHVEnable();
  return 0;
}
