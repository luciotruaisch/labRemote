#include "PBv2TB.h"
#include "SorensenPs.h"
#include "AMAC_icalibrate.h"

#include <iostream>
#include <cmath>
#include <string>
#include <fstream>

int main()
{
  
  PBv2TB tb;
  SorensenPs ps("/dev/ttyUSB0",11);

  ps.setCh(1);
  ps.init();
  ps.setVoltage(11.0);
  ps.turnOn();
  //Determine Applied voltage limits
  double Vmin = 0.1;
  double Vmax = 60.0;

  //Creat a test file
  std::ofstream TestFile;
  TestFile.open("HVSystest.csv");
  TestFile << "HVsupply,HVmes,Isupply,Imeas,Gain"<<std::endl;

  for(double V=0.0; V<=Vmax; V+=Vmin)
    {

      //Add part for new system
      //This part may be changed if the hardware change
      double R1 = 520*pow(10.0,3.0);//ohm
      double R2 = 5.2*pow(10.0,3.0);//ohm
      double Vref = 3.3; //V

      //Calculate the actual current in circuit
      double I = V/(R1+R2);

      //Range of possible gains
      uint8_t Gain[] = {1,2,5,10,20,50,100,200};
      uint8_t nrGain = sizeof(Gain)/sizeof(Gain[0]);
      uint8_t currentGain = 1;

      //Determine Gain application
      for(uint8_t i=0; i<nrGain; i++)
	{
	  //Applicable current limit
	  double I_limits = (Vref/R2)/Gain[i];
	  //If the actual current is ion the range than set the right GAIN
	  if(I<I_limits)
	    {
	      currentGain = Gain[i];
	    }
	}

      //Set Gain
      tb.setHVamp(0,currentGain);
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      //Read sended parameter
      uint8_t tested_PB;
      uint8_t hv_Gain;

      tb.getHVamp(tested_PB,hv_Gain);

      //Read BandGapControl
      unsigned BandGap;
      (tb.getPB(0))->read(AMACreg::BANDGAP_CONTROL,BandGap);

      //Read RampGain
      unsigned RampGain;
      (tb.getPB(0))->read(AMACreg::LEFT_RAMP_GAIN,RampGain);

      AMAC_icalibrate AMAC_Leakage("L01");

      //Choise the right GAIN for AMAC measurement
      unsigned OpAmpGain = 0;

      if(I < 20e-6)
	{
	  OpAmpGain = 0;
	}
      else if((I >= 20e-6)&&(I < 100e-6))
	{
	  OpAmpGain = 2;
	}
      else if(I >= 100e-6) 
	{
	  OpAmpGain = 4;
	}
       //Set the Gain of Leakage measure
      (tb.getPB(0))->write(AMACreg::OPAMP_GAIN_LEFT, OpAmpGain);
      //Set Paramater for conversion
      AMAC_Leakage.setParameter(AMAC_icalibrate::LEFT,BandGap,RampGain,OpAmpGain);
      std::this_thread::sleep_for(std::chrono::milliseconds(100));

      ps.setCh(2);
      ps.setVoltage(V);
      ps.turnOn();
      std::this_thread::sleep_for(std::chrono::seconds(2));

      (tb.getPB(0))->write(AMACreg::HV_ENABLE,0x1);

      double HVmes = (tb.getHVout())/currentGain;
      double HVexpected = V/(R1+R2)*R2;

      double HVImes = ((tb.getHVout())/currentGain)/R2;

      //Read voltage value for Leakage measure
      unsigned I_ADC  = 0;
      (tb.getPB(0))->read(AMACreg::VALUE_LEFT_CH6, I_ADC);

      //Convert value in Ampere
      double Ileak = AMAC_Leakage.Leakage_calibrate(I_ADC);
     
      //Write on file
      TestFile << HVexpected<<","<<HVmes<<","<<I<<","<<HVImes<<","<<Ileak<<","<<(unsigned)currentGain<<std::endl;
      
      (tb.getPB(0))->write(AMACreg::HV_ENABLE,0x0);
      ps.turnOff();
    }
  ps.setCh(1);
  ps.turnOff();
  TestFile.close();
}
