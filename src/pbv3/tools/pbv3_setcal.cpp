#include <unistd.h>
#include <string.h>

#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <memory>

#include "Logger.h"

#include "EndeavourRawFTDI.h"

loglevel_e loglevel = logINFO;


int main(int argc, char* argv[]) {
    
    if (argc < 2) {
        logger(logERROR) << "Not enough arguments!";
        logger(logERROR) << "Usage: " << argv[0] << " value";
        return -1;
    }
    
    std::unique_ptr<EndeavourRawFTDI> comm; //=std::make_unique<EndeavourRawFTDI>();
    comm.reset(new EndeavourRawFTDI());
    comm->getDAC()->set(2); //std::stof(argv[1]));

    return 0;
}
