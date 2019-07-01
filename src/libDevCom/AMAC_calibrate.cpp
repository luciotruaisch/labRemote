#include "AMAC_calibrate"

#include <fstream>

AMAC_calibrate::AMAC_calibrate(string ID, Channel CH, uint8_t BandgapControl, uint8_t RampGain)m_ID(ID)
{
  
  //Generate the file name
  string AMAC_ID = "icalib_AMAC_" << ID << ".csv";
  
  //Open the file
  fstream calib_list;
  calib_list.open(AMAC_ID, ios::int);

  //Test if BandGap and RampGain are allowed values
  if ((BandgapControl<16)&&(RampGain<4))
    {
      
