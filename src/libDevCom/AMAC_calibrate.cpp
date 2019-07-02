#include "AMAC_calibrate.h"

#include <limits>

void Go2Line(std::fstream& file, unsigned num)
{
  file.seekg(std::ios::beg);
  for(unsigned int i=0; i < num ; ++i)
    {
      file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
}

AMAC_calibrate::AMAC_calibrate(std::string ID, Channel CH, uint8_t BandgapControl, uint8_t RampGain):m_ID(ID)
{
  //Generate the file name
  std::string AMAC_ID = "AMAC_i_calibrate/calib_AMAC_" + ID + ".csv";

  //Open the file
  std::fstream calib_list;
  calib_list.open(AMAC_ID, std::ios::in);

  //Test if BandGap and RampGain are allowed values
  if ((BandgapControl<16)&&(RampGain<4)&&(calib_list.is_open()))
    {
      //Right line in the file
      unsigned numLine = 1+64*CH+4*BandgapControl+RampGain;
      std::cout << "numebr:" << numLine << std::endl;
      //Go to the line
      Go2Line(calib_list, numLine);

      std::string name;
      getline(calib_list,name,',');
      std::string Chan;
      getline(calib_list,Chan,',');
      std::string BandGap;
      getline(calib_list,BandGap,',');
      std::string Ramp;
      getline(calib_list,Ramp, ',');
      std::string m;
      getline(calib_list,m,',');
      std::string b;
      getline(calib_list,b);

      std::cout<<name<<" "<<Chan<< "  "<<BandGap <<"  "<< Ramp<< "  "<<m << "  "<<b<<std::endl;
    }
  calib_list.close();
}

AMAC_calibrate::~AMAC_calibrate()
{}
