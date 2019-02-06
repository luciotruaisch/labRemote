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
    if(config.count("registers"))
      {
	for(auto field : config["registers"].items())
	  {
	    amac->findField(field.key())->setDefaultVal(field.value().get<uint32_t>());
	    if(write)
	      amac->wrField  (field.key(), field.value().get<uint32_t>());
	  }
      }

    //
    // Configure ADC calibration
    if(config.count("calib") && config["calib"].count("slope"))
      {
	amac->setADCslope(config["calib"]["slope"].get<double>());
      }

    //
    // Configure ADC offset
    if(config.count("calib") && config["calib"].count("offset"))
      {
	for(uint i=0; i<config["calib"]["offset"].size(); i++)
	  if(!config["calib"]["offset"][i].is_null())
	    amac->setADCoffset(i, config["calib"]["offset"][i].get<uint32_t>());
      }
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

    config["registers"]["VDDbgen"]=1;
    config["registers"]["VDDbg"]  =bestVDDbg;

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

    config["registers"]["AMbgen"]=1;
    config["registers"]["AMbg"]  =bestAMbg;
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

 	if(bestSlope.is_null() || fabs(slope["calib"]["slope"].get<double>()-1)<fabs(bestSlope["calib"]["slope"].get<double>()-1))
	  {
	    bestSlope=slope;
	    bestAMintCalib=AMintCalib;
	  }
      }

    amac->wrField(&AMACv2Reg::AMintCalib, bestAMintCalib);
    json bestOffset=calibrateOffset(amac);

    logger(logINFO) << "\tAMintCalib = " << bestAMintCalib;
    logger(logINFO) << "\tslope = " << bestSlope["calib"]["slope"].get<double>()*1e3 << " mV/count";

    config["registers"]["AMintCalib"]=bestAMintCalib;
    config.merge_patch(bestSlope );
    config.merge_patch(bestOffset);

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

    config["calib"]["slope"]=m*1e3;

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
    config["calib"]["offset"][ 0] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch1Value );
    config["calib"]["offset"][ 1] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch2Value );
    config["calib"]["offset"][ 2] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch3Value );
    config["calib"]["offset"][ 3] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch4Value );
    config["calib"]["offset"][ 4] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch5Value );
    config["calib"]["offset"][ 5] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch6Value );
    config["calib"]["offset"][ 6] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch7Value );
    config["calib"]["offset"][ 7] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch8Value );
    config["calib"]["offset"][ 8] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch9Value );
    config["calib"]["offset"][ 9] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch10Value);
    config["calib"]["offset"][10] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch11Value);
    config["calib"]["offset"][11] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch12Value);
    config["calib"]["offset"][12] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch13Value);
    config["calib"]["offset"][13] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch14Value);
    config["calib"]["offset"][14] = counts;

    counts=amac->rdField(&AMACv2Reg::Ch15Value);
    config["calib"]["offset"][15] = counts;


    // Disable offset calibration
    amac->wrField(&AMACv2Reg::AMzeroCalib , 0);
    amac->wrField(&AMACv2Reg::AMzeroCalibC, 0);

    configAMAC(amac, config, true);

    return config;
  }
}
