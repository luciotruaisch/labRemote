#include "PBv3ConfigTools.h"

#include "EndeavourComException.h"
#include "EndeavourRawFTDI.h"

#include <algorithm>

namespace PBv3ConfigTools
{
  void configAMAC(std::shared_ptr<AMACv2> amac, const json& config, bool write)
  {
    //
    // Configure registers
    if(config.count("properties"))
      {
	for(auto field : config["properties"].items())
	  {
	    amac->findField(field.key())->setDefaultVal(field.value().get<uint32_t>());
	    if(write)
	      amac->wrField  (field.key(), field.value().get<uint32_t>());
	  }
      }

    //
    // Configure ADC calibration
    if(config.count("results") && config["results"].count("AMSLOPE"))
      {
	amac->setADCslope(config["results"]["AMSLOPE"].get<double>());
      }

    //
    // Configure ADC offset
    if(config.count("results") && config["results"].count("AMOFFSET"))
      {
	for(uint i=0; i<config["results"]["AMOFFSET"].size(); i++)
	  if(!config["results"]["AMOFFSET"][i].is_null())
	    amac->setADCoffset(i, config["results"]["AMOFFSET"][i].get<uint32_t>());
      }
  }

  void saveConfigAMAC(std::shared_ptr<AMACv2> amac, json& config)
  {
    //
    // Get register values
    for(const AMACv2Field* field : amac->getFields())
      {
	if(field->isReadWrite())
	  config["properties"][field->getFieldName()]=field->read();
      }
  }

  void decorateConfig(json& config)
  {
    config["testType"]="CONFIG";
    config["institution"]="LBL";

    std::string runNumber="0-0";
    if(config.count("runNumber")>0)
      {
	// Parse the existing version string
	std::istringstream ss(config["runNumber"].get<std::string>());
	std::vector<int32_t> tokens;
	std::string token;
	while(std::getline(ss, token, '-'))
	  tokens.push_back(std::stoi(token)); // TODO: try-catch bad format

	// Update version
	if(tokens.size()>0)
	  {
	    runNumber="";
	    for(uint32_t i=0;i<tokens.size();i++)
	      {
		if(i!=0) runNumber+="-";
	 	runNumber+=(i!=tokens.size()-1)?(std::to_string(tokens[i])):(std::to_string(tokens[i]+1));
	      }
	  }
      }
    config["runNumber"]=runNumber;
  }

  json tuneVDDBG(std::shared_ptr<AMACv2> amac)
  {
    logger(logINFO) << "## Tuning VDDREG ##";
    json config;

    amac->wrField(&AMACv2Reg::VDDbgen, 1);

    // Run the test
    double bestVDDREG=0;
    uint32_t bestVDDbg=0;

    for(uint VDDbg=0;VDDbg<pow(2,4);VDDbg++)
      {
	amac->wrField(&AMACv2Reg::VDDbg, VDDbg);
	usleep(5e3);

	double VDDREG=amac->getVDDREG();
	
	logger(logDEBUG) << "VDDbg = " << VDDbg << ", VDDREG = "  << VDDREG;
	
 	if(fabs(VDDREG-1200)<fabs(bestVDDREG-1200))
	  {
	    bestVDDREG=VDDREG;
	    bestVDDbg=VDDbg;
	  }
      }

    logger(logINFO) << "\tVDDbg = " << bestVDDbg;
    logger(logINFO) << "\tVDDREG = " << bestVDDREG/1e3 << " V";

    config["properties"]["VDDbgen"]=1;
    config["properties"]["VDDbg"]  =bestVDDbg;
    config["results"]["VDDREG"]    =bestVDDREG;

    configAMAC(amac, config, true);

    return config;
  }

  json tuneAMBG(std::shared_ptr<AMACv2> amac)
  {
    logger(logINFO) << "## Tuning AMBG ##";
    json config;

    // Run the test
    double bestAM=0;
    uint32_t bestAMbg=0;
    json bestSlope, bestOffset;

    amac->wrField(&AMACv2Reg::AMbgen, 1);

    for(uint AMbg=0;AMbg<pow(2,4);AMbg++)
      {
	amac->wrField(&AMACv2Reg::AMbg, AMbg);
	usleep(5e3);
	json slope =calibrateSlope (amac);
	json offset=calibrateOffset(amac);

	double AM=amac->getAM();
	
	logger(logDEBUG) << "AMbg = " << AMbg << ", AM = "  << AM;
	
 	if(fabs(AM-600)<fabs(bestAM-600))
	  {
	    bestAM     =AM;
	    bestAMbg   =AMbg;
	    bestSlope  =slope;
	    bestOffset=offset;
	  }
      }

    logger(logINFO) << "\tAMbg = " << bestAMbg;
    logger(logINFO) << "\tAM = " << bestAM << " mV";

    config["properties"]["AMbgen"]=1;
    config["properties"]["AMbg"  ]=bestAMbg;
    config["results"   ]["AM"    ]=bestAM;
    config.merge_patch(bestSlope );
    config.merge_patch(bestOffset);

    configAMAC(amac, config, true);

    return config;
  }

  json tuneRampGain(std::shared_ptr<AMACv2> amac)
  {
    logger(logINFO) << "## Tuning Ramp Gain ##";

    json config;

    // Run the test
    uint32_t bestAMintCalib=0;
    json bestSlope;

    amac->wrField(&AMACv2Reg::Ch4Mux , 1); // Set Ch4 mux to CAL input
    for(uint32_t AMintCalib=0;
	AMintCalib<16;
	AMintCalib++)
      {
	amac->wrField(&AMACv2Reg::AMintCalib, AMintCalib);
	logger(logDEBUG) << "Set AMintCalib = " << AMintCalib;

	json slope=calibrateSlope(amac);

 	if(bestSlope.is_null() || fabs(slope["results"]["AMSLOPE"].get<double>()-1)<fabs(bestSlope["results"]["AMSLOPE"].get<double>()-1))
	  {
	    bestSlope=slope;
	    bestAMintCalib=AMintCalib;
	  }
      }

    amac->wrField(&AMACv2Reg::AMintCalib, bestAMintCalib);
    json bestOffset=calibrateOffset(amac);

    logger(logINFO) << "\tAMintCalib = " << bestAMintCalib;
    logger(logINFO) << "\tslope = " << bestSlope["results"]["AMSLOPE"].get<double>()*1e3 << " mV/count";

    config["properties"]["AMintCalib"]=bestAMintCalib;
    config.merge_patch(bestSlope );
    config.merge_patch(bestOffset);

    configAMAC(amac, config, true);

    return config;
  }

  json tuneCur10V(std::shared_ptr<AMACv2> amac)
  {
    logger(logINFO) << "## Tuning input current monitor block ##";

    json config;

    // Power on the current mirrors
    amac->wrField(&AMACv2Reg::DCDCiZeroReading , 1); // Short the DCDCi inputs

    //
    // Tune the offset
    amac->wrField(&AMACv2Reg::Ch12Mux , 0);

    double bestDV=-1;
    uint32_t bestOffset=0; 
    for(uint i=0;i<pow(2,4);i++)
      {
    	amac->wrField(&AMACv2Reg::DCDCiOffset, i);
    	usleep(5e3);
    	double DV=amac->getADC(12);
    	logger(logDEBUG) << " DCDCiOffset = " << i << ", DV = " << DV << " mV";
    	if(bestDV==-1 || fabs(DV-100)<fabs(bestDV-100))
    	  {
    	    bestDV=DV;
    	    bestOffset=i;
    	  }
      }

    amac->wrField(&AMACv2Reg::DCDCiOffset, bestOffset);
    bestDV=0;
    for(uint i=0; i<100; i++)
      {
    	usleep(5e3);
    	bestDV+=amac->getADC(12);
      }
    bestDV/=100;

    logger(logINFO) << "\tDCDCiOffset = " << bestOffset;
    logger(logINFO) << "\toffset = " << bestDV << " mV";

    amac->wrField(&AMACv2Reg::DCDCiZeroReading , 0); // Separate the DCDCi inputs

    //
    // Tune the current mirrors (does not do anything other than write values)
    
    logger(logDEBUG) << "Tuning input CM mirror";
    double bestCMDV=-666;
    uint32_t bestidcdcP=0, bestidcdcN=0; 

    uint32_t trails=2;
    for(uint32_t idcdcP=0;idcdcP<pow(2,3);idcdcP++)
      {
	amac->wrField(&AMACv2Reg::DCDCiP, idcdcP);
	for(uint32_t idcdcN=0;idcdcN<pow(2,3);idcdcN++)
	  {
	    amac->wrField(&AMACv2Reg::DCDCiN, idcdcN);

	    sleep(1);

	    amac->wrField(&AMACv2Reg::Ch12Mux , 0); // Read the mid point
	    double Vout=0;
	    for(uint i=0;i<trails;i++)
	      {
		usleep(5e3);
		Vout+=amac->getADC(12);
	      }
	    Vout/=trails;

	    amac->wrField(&AMACv2Reg::Ch12Mux , 1); // Read the low point
	    double VtestL=0;
	    for(uint i=0;i<trails;i++)
	      {
		usleep(5e3);
		VtestL+=amac->getADC(12);
	      }
	    VtestL/=trails;

	    amac->wrField(&AMACv2Reg::Ch12Mux , 2); // Read the high point
	    double VtestH=0;
	    for(uint i=0;i<trails;i++)
	      {
		usleep(5e3);
		VtestH+=amac->getADC(12);
	      }
	    VtestH/=trails;

	    logger(logDEBUG) << "idcdcP,idcdcN,Vout,VtestL,VtestH = " << idcdcP << "\t" << idcdcN << "\t" << Vout << "\t" << VtestL << "\t" << VtestH;

	    double CMDV=VtestH-VtestL;
	    if(bestCMDV==-666 || fabs(CMDV-0)<fabs(bestCMDV-0))
	      {
		bestCMDV=CMDV;
		bestidcdcP=idcdcP;
		bestidcdcN=idcdcN;
	      }
	  }
      }

    logger(logINFO) << "\tDCDCiP/NidcdcP/N = " << bestidcdcP<<","<<bestidcdcN;
    logger(logINFO) << "\toffset = " << bestCMDV << " mV";

    // Save values
    config["properties"]["DCDCiOffset"] = bestOffset;
    config["properties"]["DCDCiP"] = bestidcdcP;
    config["properties"]["DCDCiN"] = bestidcdcN;
    config["results"]["CUR10VOFFSET"] = bestDV;
    config["results"]["CUR10VCM"] = bestCMDV;
    configAMAC(amac, config, true);

    return config;
  }

  json tuneCur1V(std::shared_ptr<AMACv2> amac)
  {
    logger(logINFO) << "## Tuning output current monitor block ##";

    json config;

    // Power on the current mirrors
    amac->wrField(&AMACv2Reg::DCDCen, 1);
    amac->wrField(&AMACv2Reg::DCDCenC, 1);

    amac->wrField(&AMACv2Reg::DCDCoZeroReading , 1); // Short the DCDCo inputs

    //
    // Tune the offset
    amac->wrField(&AMACv2Reg::Ch13Mux , 0);

    double bestDV=-1;
    uint32_t bestOffset=0; 
    for(uint i=0;i<pow(2,4);i++)
      {
    	amac->wrField(&AMACv2Reg::DCDCoOffset, i);
    	usleep(5e3);
    	double DV=amac->getADC(13);
    	logger(logDEBUG) << " DCDCoOffset = " << i << ", DV = " << DV << " mV";
    	if(bestDV==-1 || fabs(DV-100)<fabs(bestDV-100))
    	  {
    	    bestDV=DV;
    	    bestOffset=i;
    	  }
      }

    amac->wrField(&AMACv2Reg::DCDCoOffset, bestOffset);
    bestDV=0;
    for(uint i=0; i<100; i++)
      {
    	usleep(5e3);
    	bestDV+=amac->getADC(13);
      }
    bestDV/=100;

    logger(logINFO) << "\tDCDCoOffset = " << bestOffset;
    logger(logINFO) << "\toffset = " << bestDV << " mV";

    amac->wrField(&AMACv2Reg::DCDCoZeroReading , 0); // separate the DCDCo inputs

    //
    // Tune the current mirrors (does not do anything other than write values)

    logger(logDEBUG) << "Tuning input CM mirror";
    double bestCMDV=-666;
    uint32_t bestodcdcP=0, bestodcdcN=0; 

    uint32_t trails=2;
    for(uint32_t odcdcP=0;odcdcP<2;odcdcP++)
      {
	amac->wrField(&AMACv2Reg::DCDCoP, odcdcP);
	for(uint32_t odcdcN=0;odcdcN<2;odcdcN++)
	  {
	    amac->wrField(&AMACv2Reg::DCDCoN, odcdcN);

	    sleep(1);

	    amac->wrField(&AMACv2Reg::Ch13Mux , 0); // Read the mid point
	    double Vout=0;
	    for(uint i=0;i<trails;i++)
	      {
		usleep(5e3);
		Vout+=amac->getADC(13);
	      }
	    Vout/=trails;

	    amac->wrField(&AMACv2Reg::Ch13Mux , 1); // Read the low point
	    double VtestL=0;
	    for(uint i=0;i<trails;i++)
	      {
		usleep(5e3);
		VtestL+=amac->getADC(13);
	      }
	    VtestL/=trails;

	    amac->wrField(&AMACv2Reg::Ch13Mux , 2); // Read the high point
	    double VtestH=0;
	    for(uint i=0;i<trails;i++)
	      {
		usleep(5e3);
		VtestH+=amac->getADC(13);
	      }
	    VtestH/=trails;

	    logger(logDEBUG) << "odcdcP,odcdcN,Vout,VtestL,VtestH = " << odcdcP << "\t" << odcdcN << "\t" << Vout << "\t" << VtestL << "\t" << VtestH;

	    double CMDV=VtestH-VtestL;
	    if(bestCMDV==-666 || fabs(CMDV-0)<fabs(bestCMDV-0))
	      {
		bestCMDV=CMDV;
		bestodcdcP=odcdcP;
		bestodcdcN=odcdcN;
	      }
	  }
      }

    logger(logINFO) << "\tDCDCoP/N = " << bestodcdcP<<","<<bestodcdcN;
    logger(logINFO) << "\toffset = " << bestCMDV << " mV";

    amac->wrField(&AMACv2Reg::DCDCen, 0);
    amac->wrField(&AMACv2Reg::DCDCenC, 0);

    config["properties"]["DCDCoOffset"] = bestOffset;
    config["properties"]["DCDCoP"] = bestodcdcP;
    config["properties"]["DCDCoN"] = bestodcdcN;
    config["results"]["CUR1VOFFSET"] = bestDV;
    config["results"]["CUR1VCM"] = bestCMDV;
    configAMAC(amac, config, true);

    return config;
  }

  json calibrateSlope(std::shared_ptr<AMACv2> amac)
  {
    logger(logINFO) << "## Calibrating AMAC slope ##";

    json config;

    // Run the test
    double CALact=0;
    uint32_t CALamac=0;

    amac->wrField(&AMACv2Reg::Ch4Mux , 1); // Set Ch4 mux to CAL input

    // Run ramp
    std::vector<double> x,y;
    uint n=0;

    // Longer wait for the first value due to RC constant
    dynamic_cast<EndeavourRawFTDI*>(amac->raw().get())->getDAC()->set(0);
    usleep(10e3);
    for(double CALin=0; CALin<1.01; CALin+=0.05)
      {
	// actual voltage to be compared against digitized value
	CALact=dynamic_cast<EndeavourRawFTDI*>(amac->raw().get())->getDAC()->set(CALin*2)/2;

	// digital about
	usleep(5e3);
	CALamac = amac->rdField(&AMACv2Reg::Ch4Value);
	if(CALamac==1023) break; //saturated, don't include

	x.push_back(CALamac);
	y.push_back(CALact);
	n++;

	logger(logDEBUG) << "\t" << x.back() << "\t" << y.back();
      }

    double xavg=std::accumulate(x.begin(), x.end(), 0.)/n;
    double yavg=std::accumulate(y.begin(), y.end(), 0.)/n;

    double num=0;
    double den=0;
    for(uint i=0;i<x.size();i++)
      {
	num+=(x[i]-xavg)*(y[i]-yavg);
	den+=(x[i]-xavg)*(x[i]-xavg);
      }
    double m=num/den;
    double b=yavg-m*xavg;

    logger(logDEBUG) << "\tm = " << m << ", b = " << b;

    config["results"]["AMSLOPE"]=m*1e3;

    configAMAC(amac, config, true);

    return config;
  }

  json calibrateOffset(std::shared_ptr<AMACv2> amac)
  {
    logger(logINFO) << "## Calibrating AMAC offset ##";

    json config;

    // Enable offset calibration
    amac->wrField(&AMACv2Reg::AMzeroCalib , 1);
    amac->wrField(&AMACv2Reg::AMzeroCalibC, 1);
    usleep(5e3);

    // Calculate the calibration
    uint32_t counts;

    counts=amac->rdField(&AMACv2Reg::Ch0Value );
    config["results"]["AMOFFSET"][ 0] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch1Value );
    config["results"]["AMOFFSET"][ 1] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch2Value );
    config["results"]["AMOFFSET"][ 2] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch3Value );
    config["results"]["AMOFFSET"][ 3] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch4Value );
    config["results"]["AMOFFSET"][ 4] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch5Value );
    config["results"]["AMOFFSET"][ 5] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch6Value );
    config["results"]["AMOFFSET"][ 6] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch7Value );
    config["results"]["AMOFFSET"][ 7] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch8Value );
    config["results"]["AMOFFSET"][ 8] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch9Value );
    config["results"]["AMOFFSET"][ 9] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch10Value);
    config["results"]["AMOFFSET"][10] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch11Value);
    config["results"]["AMOFFSET"][11] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch12Value);
    config["results"]["AMOFFSET"][12] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch13Value);
    config["results"]["AMOFFSET"][13] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch14Value);
    config["results"]["AMOFFSET"][14] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch15Value);
    config["results"]["AMOFFSET"][15] = counts;

    logger(logDEBUG) << config["results"]["AMOFFSET"];


    // Disable offset calibration
    amac->wrField(&AMACv2Reg::AMzeroCalib , 0);
    amac->wrField(&AMACv2Reg::AMzeroCalibC, 0);

    configAMAC(amac, config, true);

    return config;
  }

  json calibrateNTC(std::shared_ptr<AMACv2> amac)
  {
    logger(logINFO) << "## Calibrating NTCs ##";

    json config;

    // Enable NTC calibration
    amac->wrField(&AMACv2Reg::NTCpbCal, 0);
    amac->wrField(&AMACv2Reg::NTCx0Cal, 0);
    amac->wrField(&AMACv2Reg::NTCy0Cal, 0);
    usleep(5e3);

    double NTCx =amac->getNTCx ();
    double NTCy =amac->getNTCy ();
    double NTCpb=amac->getNTCpb();

    logger(logDEBUG) << "NTCx = " << NTCx << " mV";
    logger(logDEBUG) << "NTCy = " << NTCy << " mV";
    logger(logDEBUG) << "NTCpb= " << NTCpb<< " mV";

    config["results"]["NTCX"] = NTCx ;
    config["results"]["NTCY"] = NTCy ;
    config["results"]["NTCPB"]= NTCpb;

    // Disable NTC calibration
    amac->wrField(&AMACv2Reg::NTCpbCal, 1);
    amac->wrField(&AMACv2Reg::NTCx0Cal, 1);
    amac->wrField(&AMACv2Reg::NTCy0Cal, 1);

    configAMAC(amac, config, true);

    return config;
  }

  json calibrateCur10V(std::shared_ptr<AMACv2> amac)
  {
    logger(logINFO) << "## Calibrating input current monitor block ##";

    json config;

    amac->wrField(&AMACv2Reg::DCDCiZeroReading , 1); // Short the DCDCi inputs

    amac->wrField(&AMACv2Reg::Ch12Mux , 0);

    double DV=0;
    for(uint i=0; i<100; i++)
      {
    	usleep(5e3);
    	DV+=amac->getADC(12);
      }
    DV/=100;

    config["results"]["CUR10VOFFSET"] = DV;
    configAMAC(amac, config, true);

    amac->wrField(&AMACv2Reg::DCDCiZeroReading , 0); // Separate the DCDCi inputs

    return config;
  }

  json calibrateCur1V(std::shared_ptr<AMACv2> amac)
  {
    logger(logINFO) << "## Calibrating output current monitor block ##";

    json config;

    amac->wrField(&AMACv2Reg::DCDCoZeroReading , 1); // Short the DCDCo inputs

    amac->wrField(&AMACv2Reg::Ch13Mux , 0);

    double DV=0;
    for(uint i=0; i<100; i++)
      {
    	usleep(5e3);
    	DV+=amac->getADC(13);
      }
    DV/=100;

    config["results"]["CUR1VOFFSET"] = DV;
    configAMAC(amac, config, true);

    amac->wrField(&AMACv2Reg::DCDCoZeroReading , 0); // Separate the DCDCo inputs

    return config;
  }

  json calibrateAll(std::shared_ptr<AMACv2> amac)
  {
    // json config_ntc;
    // json config_offset;
    // json config_slope;
    // json config_cm;
    json config;

    config["config_ntc"] = calibrateNTC(amac);
    config["config_slope"] = calibrateSlope(amac);
    config["config_offset"] = calibrateOffset(amac);
    config["config_Cur10V"] = calibrateCur10V(amac);
    config["config_Cur1V"] = calibrateCur1V(amac);
    
    
    return config;
    
  }
  
}
