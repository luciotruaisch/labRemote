// Tektronix programmer manual can be found at https://www.tek.com/manual/tds-family-programmer-manual

//				Scope	Channel Reference Filename(no need for .CSV)  [Reset] (resets waveform from previous aquisition)  
//  example ./bin/test_tek /dev/ttyUSB0    3        3          347_C94_unt             [rst]

#include <iostream>
#include <fstream>
#include <algorithm>
#include "Logger.h"
#include "Tektronix654C.h"
#include <string>
#include <sstream>

using namespace std;

int main(int argc, char*argv[])
{
Tektronix654C tek(argv[1],1);

string ch = argv[2];  
string ref = argv[3];
string file = argv[4];
string reset;

if (argc > 5){
reset = argv[5];}
tek.saveWaveToRef(ch, ref);     //Stores a waveform from CH<x> in one of four reference memory locations ( oscilloscipe Ref1, Ref2, Ref3, Ref4)
tek.setCh(ch);                //selection of oscilloscope channel to read waveform data from
tek.setData("ASCIi");        //encoding of waveform data format to be read DATa:ENCdg { ASCIi | RIBinary | RPBinary | SRIbinary | SRPbinary}
  
//prints to screen oscilloscope settings used 
	std::cout << tek.init(); //initializes scope and prints scope info
	std::cout << "Settings Used:" << std::endl;
	std::cout << "data source -> ";
 
	  string voltage = tek.read(); //puts all voltage bin values into string variable voltage
	  string temp_value_str;
	  int l=voltage.length(); //length of the voltage string
	   double extract_voltage[500];

//conversion of voltage values (string to int)
	int j=0;	
	for (int i=0; i<500; i++)
	{ 
 		temp_value_str ="";
                while (voltage[j]!=',' && j!=l)
		{ temp_value_str = temp_value_str + voltage[j];
                  j++;   
                 } 
 	         j++;
                 extract_voltage[i] = stoi(temp_value_str); //conversion of string values to integer
		 }

//converts integer bin values to voltage [V]
	string verticalPosStr = tek.verticalPos(ch);

double vertPos;
//test for negative value
	if (verticalPosStr[0]=='-') {
	 vertPos = stod(verticalPosStr.substr(1, verticalPosStr.length())); }
	  else {vertPos = stod(verticalPosStr.substr(0, verticalPosStr.length())) * (-1);
	}

	cout << "vertical position -> " << verticalPosStr << " converted to -> " << vertPos << "\n" << endl;

	 double voltDiv = stod(tek.voltDiv(ch));
	cout << "volts/div conversion -> " << voltDiv << "\n" << endl;

//correction of slope and y intercept values for plotting
	for (int i=0; i<500; i++)
		{// cout << extract_voltage[i] <<endl;
		   extract_voltage[i] = (extract_voltage[i]/32767) * voltDiv * 5 * 1.031412332 + voltDiv * vertPos - 0.00049; //5 - no of divisions above and below zero (10 total)
		 //  cout << extract_voltage[i] <<endl;
	}

//converts string timebase to integer
	string time_baseStr = tek.timeBase();
	double time_base = stod(time_baseStr.substr(0, time_baseStr.length()-3));
	cout << "time base -> " << time_baseStr << " converted to -> " << time_base << "\n" << endl;

//creates .CSV file with time column and voltage column
	  ofstream myfile;
	  myfile.open (file + ".CSV");
	    double time=0;
	    for (int i=0; i<500; i++)
		{  
		myfile << to_string(time) +"," + to_string(extract_voltage[i]) + "\n";
	  	   time = time + time_base * 10/500; // time has the units of the time base setting
		}
	  myfile.close();

cout << tek.curve(); // prints oscilloscope raw data to screen

if (reset == "rst"){
tek.rstWave(); //resets waveform on scope screen
}

  return 0;
}

