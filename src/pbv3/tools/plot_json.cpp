#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <ios>
#include <time.h>
#include <sstream>
#include <chrono>
//#include "TFile.h"
/*#include "TCanvas.h"
#include "TH1D.h"
#inlcude "TGraph.h"*/
//#include "plot_json.h"

using json = nlohmann::json;

//Functions to plot (may or may not move for organizational reasons)
void plot_one(json jin, int xpos, int ypos);

int main(int argc, char* argv[]){
if(argc != 2)
  {std::cout<<"ME NEED ONE AND ONLY ONE FILE NAME"<<std::endl; return 0;}
std::string input = argv[1];
std::ifstream f(input);
json j = json::parse(f);

//looping through all the json files
for(int i=0; i<j["tests"].size() ; i++){
json test = j["tests"][i];
plot_one(test,0,1);
//std::cout<<test["name"]<<std::endl;
}//end loop over all json files

}//end mainw

//Function body

void plot_one(json jin, int xpos, int ypos){
double x[jin["data"].size()];
double y[jin["data"].size()];

for(int ii = 0; ii<jin["data"].size(); ii++){
x[ii] = jin["data"][xpos][ii];
y[ii] = jin["data"][ypos][ii];
}//loop filling arrays

//TGraph *jin_graph = new TGraph(jin["data"].size(),x,y);
//jin_graph->Draw("ap")

}//end plot_one
