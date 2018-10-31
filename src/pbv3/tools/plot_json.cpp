#include <json.hpp>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <ios>
#include <time.h>
#include <sstream>
#include <chrono>

using json = nlohmann::json;

int main(int argc, char* argv[]){
  if(argc != 2)
    {std::cout<<"ME NEED ONE AND ONLY ONE FILE NAME"<<std::endl; return 0;}
  std::string input = argv[1];
  std::ifstream f(input);
  json j = json::parse(f);
  for(int i=0; i<j["tests"].size() ; i++){
    json test = j["tests"][i];
    std::cout<<test["name"]<<std::endl;
  }
  std::cout<<"Hello World!"<<std::endl;
}//end mainw
