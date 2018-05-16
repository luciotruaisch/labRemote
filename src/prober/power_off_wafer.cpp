#include <iostream>
#include <fstream>

#include "Logger.h"
#include "AgilentPs.h"

using namespace std;

loglevel_e loglevel = logDEBUG4;

int main(int argc, char*argv[]){

        AgilentPs meter(argv[1], 4); // ( , GPIB address)
        meter.turnOff();

        return 0;
}
