#include "AMAC_icalibrate.h"

#include <limits>

//Ignore the other line
void Go2Line(std::fstream& file, unsigned num)
{
  file.seekg(std::ios::beg);
  for(unsigned int i=0; i < num ; ++i)
    {
      file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
}

AMAC_icalibrate::AMAC_icalibrate(std::string ID, Channel CH, uint8_t BandgapControl, uint8_t RampGain, uint8_t OpAmpGain):m_ID(ID)
{
  //Generate the file name
  std::string AMAC_ID = "AMAC_i_calibrate/icalib_AMAC_" + ID + ".csv";

  //Open the file
  std::fstream calib_list;
  calib_list.open(AMAC_ID, std::ios::in);

  //Test if the file is correct open
  if (calib_list.is_open())
    {
      //Test if BandGap and RampGain are allowed values
      if ((BandgapControl<16)&&(RampGain<4)&&(OpAmpGain<16))
	{
	  //Right line in the file
	  unsigned numLine = 1+1024*CH+64*BandgapControl+16*RampGain+OpAmpGain;

	  //Go to the line
	  Go2Line(calib_list, numLine);

	  //Get AMAC name
	  std::string name;
	  getline(calib_list,name,',');

	  //Get channel name
	  std::string Chan;
	  getline(calib_list,Chan,',');
	  std::string ch_check;

	  switch(CH)
	    {
	    case LEFT:
	      ch_check = "LEFT";
	      break;
	   case RIGHT:
	      ch_check = "RIGHT";
	      break;
	    }

	  //Get BandGap value
	  std::string BandGap;
	  getline(calib_list,BandGap,',');

	  //Get RampGain value
	  std::string Ramp;
	  getline(calib_list,Ramp, ',');
      
	  //test if the value are right
	  if ((name==("AMAC_"+ID)) && (Chan==ch_check) && (BandGap==std::to_string(BandgapControl)) && (Ramp==std::to_string(RampGain)))
	    {

	      //Set parameter m
	      std::string m;
	      getline(calib_list,m,',');
	      m_m = std::stod(m);

	      //Set parameter b
	      std::string b;
	      getline(calib_list,b,',');
	      m_b = std::stod(b);

	      //Set parameter Voff
	      std::string Voff;
	      getline(calib_list,Voff,',');
	      m_Voff = std::stod(Voff);

	      //Set parameter RI
	      std::string RI;
	      getline(calib_list,RI);
	      m_RI = std::stod(RI);

	    }else
	    {
	      std::cout << "Value not found" << std::endl;
	    }

	}else
	{
	  std::cout << "BandgapControl and RampGain values are note allowed!" << std::endl;
	} 
    }else
    {
      std::cout << "Can't open the file!" << std::endl;
    }
  calib_list.close();
}

AMAC_icalibrate::~AMAC_icalibrate()
{}

void AMAC_icalibrate::setParameter(Channel CH, uint8_t BandgapControl, uint8_t RampGain, uint8_t OpAmpGain)
{
  //Generate the file name
  std::string AMAC_ID = "AMAC_i_calibrate/icalib_AMAC_" + m_ID + ".csv";

  //Open the file
  std::fstream calib_list;
  calib_list.open(AMAC_ID, std::ios::in);

  //Test if the file is correct open
  if (calib_list.is_open())
    {
      //Test if BandGap and RampGain are allowed values
      if ((BandgapControl<16)&&(RampGain<4)&&(OpAmpGain<16))
	{
	  //Right line in the file
	  unsigned numLine = 1+1024*CH+64*BandgapControl+16*RampGain+OpAmpGain;

	  //Go to the line
	  Go2Line(calib_list, numLine);

	  //Get AMAC name
	  std::string name;
	  getline(calib_list,name,',');

	  //Get channel name
	  std::string Chan;
	  getline(calib_list,Chan,',');
	  std::string ch_check;

	  switch(CH)
	    {
	    case LEFT:
	      ch_check = "LEFT";
	      break;
	   case RIGHT:
	      ch_check = "RIGHT";
	      break;
	    }

	  //Get BandGap value
	  std::string BandGap;
	  getline(calib_list,BandGap,',');

	  //Get RampGain value
	  std::string Ramp;
	  getline(calib_list,Ramp, ',');

	  //Get OpAmpGain
	  std::string AmpGain;
	  getline(calib_list,AmpGain, ',');
      
	  //test if the value are right
	  if ((name==("AMAC_"+m_ID)) && (Chan==ch_check) && (BandGap==std::to_string(BandgapControl)) && (Ramp==std::to_string(RampGain)) && (AmpGain==std::to_string(OpAmpGain)))
	    {

	      //Set parameter m
	      std::string m;
	      getline(calib_list,m,',');
	      m_m = std::stod(m);

	      //Set parameter b
	      std::string b;
	      getline(calib_list,b,',');
	      m_b = std::stod(b);

	      //Set parameter Voff
	      std::string Voff;
	      getline(calib_list,Voff,',');
	      m_Voff = std::stod(Voff);

	      //Set parameter RI
	      std::string RI;
	      getline(calib_list,RI);
	      m_RI = std::stod(RI);

	    }else
	    {
	      std::cout << "Value not found" << std::endl;
	    }

	}else
	{
	  std::cout << "BandgapControl and RampGain and OpAmpGain values are not allowed!" << std::endl;
	} 
    }else
    {
      std::cout << "Can't open the file!" << std::endl;
    }
  calib_list.close();
}

//Calibrate the measured the Leakage current
double AMAC_icalibrate::Leakage_calibrate(unsigned count)
{
  double result =m_m*count+m_b;
  return result;
}
