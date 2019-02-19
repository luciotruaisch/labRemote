#include "PBv3TestTools.h"

#include "EndeavourComException.h"
#include "EndeavourRawFTDI.h"

 #include <memory>

namespace PBv3TestTools {
  json testLvEnable(AMACv2 *amac, GenericPs *ps, Bk85xx *load) 
  {
    logger(logINFO) << "## LV Enable test ## " << PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());

    json testSum;
    testSum["testType"] = "LV_ENABLE";
    testSum["results"]["TIMESTART"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());

    testSum["passed"] = false;

    // Init stuff
    try
      {
	load->setCurrent(0);
	load->turnOn();
      }
    catch(std::string &s)
      {
	logger(logERROR) << s;
	testSum["error"] = s;
	return testSum;
      }

    try
      {
	amac->wrField(&AMACv2::DCDCen, 0);
	amac->wrField(&AMACv2::DCDCenC, 0);
      }
    catch(EndeavourComException &e)
      {
	logger(logERROR) << e.what();
	testSum["error"] = e.what();
	return testSum;
      }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    double lv_off = load->getValues().vol;//mV
    double Iin  = std::stod(ps->getCurrent());
    double Vin  = std::stod(ps->getVoltage());
    double iout = load->getValues().cur;
    int Vdcdc   = amac->rdField(&AMACv2::Ch0Value);
    int VddLr   = amac->rdField(&AMACv2::Ch1Value);
    int DCDCin  = amac->rdField(&AMACv2::Ch2Value);
    int NTC     = amac->rdField(&AMACv2::Ch9Value);
    int Cur10V  = amac->rdField(&AMACv2::Ch12Value);
    int Cur1V   = amac->rdField(&AMACv2::Ch13Value);
    int PTAT    = amac->rdField(&AMACv2::Ch15Value);

    logger(logINFO) << " --> Reading " << lv_off << "mV in off state.";

    testSum["results"]["LVENABLE"  ][0] = false;
    testSum["results"]["VIN"       ][0] = Vin;
    testSum["results"]["IIN"       ][0] = Iin;
    testSum["results"]["VOUT"      ][0] = lv_off*1e-3;
    testSum["results"]["IOUT"      ][0] = iout*1e-3;
    testSum["results"]["AMACVDCDC" ][0] = Vdcdc;
    testSum["results"]["AMACVDDLR" ][0] = VddLr;
    testSum["results"]["AMACDCDCIN"][0] = DCDCin;
    testSum["results"]["AMACNTCPB" ][0] = NTC;
    testSum["results"]["AMACCUR10V"][0] = Cur10V;
    testSum["results"]["AMACCUR1V" ][0] = Cur1V;
    testSum["results"]["AMACPTAT"  ][0] = PTAT;

    logger(logINFO) << " --> Trying to turn on DCDC ...";
    try
      {
	amac->wrField(&AMACv2::DCDCen, 1);
	amac->wrField(&AMACv2::DCDCenC, 1);
      }
    catch(EndeavourComException &e)
      {
	logger(logERROR) << e.what();
	testSum["error"] = e.what();
	return testSum;
      }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    Iin = std::stod(ps->getCurrent());
    Vin = std::stod(ps->getVoltage());
    double lv_on = load->getValues().vol;//mV
    iout   = load->getValues().cur;
    Vdcdc  = amac->rdField(&AMACv2::Ch0Value);
    VddLr  = amac->rdField(&AMACv2::Ch1Value);
    DCDCin = amac->rdField(&AMACv2::Ch2Value);
    NTC    = amac->rdField(&AMACv2::Ch9Value);
    Cur10V = amac->rdField(&AMACv2::Ch12Value);
    Cur1V  = amac->rdField(&AMACv2::Ch13Value);
    PTAT   = amac->rdField(&AMACv2::Ch15Value);
    logger(logINFO) << " --> Reading " << lv_on << "mV in on state.";

    testSum["results"]["LVENABLE"  ][1] = true;
    testSum["results"]["VIN"       ][1] = Vin;
    testSum["results"]["IIN"       ][1] = Iin;
    testSum["results"]["VOUT"      ][1] = lv_on*1e-3;
    testSum["results"]["IOUT"      ][1] = iout*1e-3;
    testSum["results"]["AMACVDCDC" ][1] = Vdcdc;
    testSum["results"]["AMACVDDLR" ][1] = VddLr;
    testSum["results"]["AMACDCDCIN"][1] = DCDCin;
    testSum["results"]["AMACNTCPB" ][1] = NTC;
    testSum["results"]["AMACCUR10V"][1] = Cur10V;
    testSum["results"]["AMACCUR1V" ][1] = Cur1V;
    testSum["results"]["AMACPTAT"  ][1] = PTAT;

    if (!(lv_on > 1.4e3 && lv_on < 1.6e3 && lv_off < 0.1e3))
      {
	logger(logERROR) << " --> LV enable not working! " << lv_on << " " << lv_off;
	testSum["passed"] = false;
      }
    else
      {
	logger(logINFO) << " --> LV enable good!";
	testSum["passed"] = true;
      }

    testSum["results"]["TIMEEND"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());

    amac->initRegisters();

    return testSum;
  }

  // Range is iout in mA
  json measureEfficiency(AMACv2 *amac, GenericPs *ps, Bk85xx *load,  int step, int min, int max, double VinSet) 
  {
    logger(logINFO) << "## Measuring DCDC efficiency ## " << PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());

    json testSum;
    testSum["testType"] = "DCDCEFFICIENCY";
    testSum["results"]["TIMESTART"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());
	
    try
      {
	load->setCurrent(0);
	load->turnOn();
      }
    catch(std::string &s)
      {
	logger(logERROR) << s;
	testSum["error"] = s;
	return testSum;
      }

    logger(logINFO) << " --> Vin = " << VinSet << "V";
    ps->setVoltage(VinSet);

    logger(logINFO) << " --> Turn off DCDC ..";
    try
      {
	amac->wrField(&AMACv2::DCDCen, 0);
	amac->wrField(&AMACv2::DCDCenC, 0);
      }
    catch(EndeavourComException &e)
      {
	logger(logERROR) << e.what();
	testSum["error"] = e.what();
	return testSum;
      }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    logger(logINFO) << " --> Get baseline current: (" << ps->getCurrent() << ")A";
    double Iin_offset = std::stod(ps->getCurrent());
    testSum["results"]["IINOFFSET"] = Iin_offset;

    logger(logINFO) << " --> Turn on DCDC ...";
    try
      {
	amac->wrField(&AMACv2::DCDCen, 1);
	amac->wrField(&AMACv2::DCDCenC, 1);
      }
    catch(EndeavourComException &e)
      {
	logger(logERROR) << e.what();
	testSum["error"] = e.what();
	return testSum;
      }

    //unsigned dwell_time = .1; //s
    unsigned dwell_time_s = .005; //s
    unsigned dwell_time = 5; //s
    testSum["properties"]["DWELLTIMELONG" ] = dwell_time;
    testSum["properties"]["DWELLTIMESHORT"] = dwell_time_s;

    logger(logINFO) << " --> Starting measurement ...";
    std::cout << "Vin" << "\t" << "Iin" << "\t" << "Vout" << "\t" << "Iout" << "\t" << "Vdcdc"
	      << "\t" << "VddLr" << "\t" << "DCDCin" << "\t" << "NTC" << "\t"
	      << "Cur10V" << "\t" << "Cur1V" << "\t" << "PTAT" << "\t" << "Efficiency" << std::endl;
    // Set sub-channel
    try
      {
	amac->wrField(&AMACv2::Ch12Mux, 0); //a
	amac->wrField(&AMACv2::Ch13Mux, 0); //a
      }
    catch(EndeavourComException &e)
      {
	logger(logERROR) << e.what();
	return testSum;
      }

    // Loop over currents
    int index = 0;

    for (int iout=min;iout<=max;iout+=step)
      {
	//allowing system to reach thermal equilibrium
	std::this_thread::sleep_for(std::chrono::seconds(dwell_time));
	logger(logDEBUG) << " --> Setting " << iout << "mA load!";
	// Set Current
	load->setCurrent(iout);
	// Wait for temp and everything to settle
	std::this_thread::sleep_for(std::chrono::seconds(dwell_time_s));
	// Read AMAC values
	int Vdcdc, VddLr, DCDCin, NTC, Cur10V, Cur1V, PTAT;
	try
	  {
	    Vdcdc  = amac->rdField(&AMACv2::Ch0Value);
	    VddLr  = amac->rdField(&AMACv2::Ch1Value);
	    DCDCin = amac->rdField(&AMACv2::Ch2Value);
	    NTC    = amac->rdField(&AMACv2::Ch9Value);
	    Cur10V = amac->rdField(&AMACv2::Ch12Value);
	    Cur1V  = amac->rdField(&AMACv2::Ch13Value);
	    PTAT   = amac->rdField(&AMACv2::Ch15Value);
	  }
	catch(EndeavourComException &e)
	  {
	    logger(logERROR) << e.what();
	    testSum["error"] = e.what();
	    return testSum;
	  }

      double Vin = std::stod(ps->getVoltage());
      double Iin = std::stod(ps->getCurrent());

      double Vout = 0;
      try
	{
	  Vout = load->getValues().vol;
	}
      catch(std::string &s)
	{
	  logger(logERROR) << s;
	  testSum["error"] = s;
	  return testSum;
	}

      double efficiency = (Vout*iout*1e-6)/(Vin*(Iin-Iin_offset));
      //double efficiency = (1.5*iout*1e-3)/(Vin*(Iin-Iin_offset));
      std::cout << Vin << "\t" << Iin << "\t" << Vout << "\t" << iout << "\t" << Vdcdc
		<< "\t" << VddLr << "\t" << DCDCin << "\t" << NTC << "\t"
		<< Cur10V << "\t" << Cur1V << "\t" << PTAT << "\t" << efficiency << std::endl;
      testSum["results"]["VIN"       ][index] = Vin;
      testSum["results"]["IIN"       ][index] = Iin;
      testSum["results"]["VOUT"      ][index] = Vout*1e-3;
      testSum["results"]["IOUT"      ][index] = iout*1e-3;
      testSum["results"]["AMACVDCDC" ][index] = Vdcdc;
      testSum["results"]["AMACVDDLR" ][index] = VddLr;
      testSum["results"]["AMACDCDCIN"][index] = DCDCin;
      testSum["results"]["AMACNTCPB" ][index] = NTC;
      testSum["results"]["AMACCUR10V"][index] = Cur10V;
      testSum["results"]["AMACCUR1V" ][index] = Cur1V;
      testSum["results"]["AMACPTAT"  ][index] = PTAT;
      testSum["results"]["EFFICIENCY"][index] = efficiency;
      index++;
    }
    logger(logINFO) << " --> Done!! Turng off load!";
    load->setCurrent(0);

    testSum["results"]["TIMEEND"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());

    amac->initRegisters();

    return testSum;
  }

  json testHvEnable(AMACv2 *amac, Keithley24XX *sm, unsigned frequency)
  {
    logger(logINFO) << "## Testing HV Enable circuit ## " << PBv3TestTools::getTimeAsString(std::chrono::system_clock::now()) ;
    logger(logINFO) << " --> Frequency: " << frequency << " (0=high, 1=25kHz, 2=50kHz, 3=100kHz)";
    logger(logINFO) << " --> Turning everything off.";

    json testSum;
    testSum["testType"] = "HV_ENABLE";
    testSum["results"]["TIMESTART"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());

    testSum["passed"] = false;

    try
      {
	// Turn off HV enable from AMAC
	amac->wrField(&AMACv2::CntSetHV0frq, frequency);
	amac->wrField(&AMACv2::CntSetCHV0frq, frequency);
	amac->wrField(&AMACv2::CntSetHV0en, 0);
	amac->wrField(&AMACv2::CntSetCHV0en, 0);
      }
    catch(EndeavourComException &e)
      {
	logger(logERROR) << e.what();
	testSum["error"] = e.what();
	return testSum;
      }

    // Configure sourcemeter
    sm->turnOff();
    sm->setSource(KeithleyMode::CURRENT, 1e-3, 1e-3);
    sm->setSense(KeithleyMode::VOLTAGE, 500, 500);
    //sm->setSource(KeithleyMode::VOLTAGE, 500, 500);
    //sm->setSense(KeithleyMode::CURRENT, 1.27e-3, 1.27e-3);

    // See what we see
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    sm->turnOn();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    double hv_v_off = std::stod(sm->sense(KeithleyMode::VOLTAGE));
    double hv_i_off = std::stod(sm->sense(KeithleyMode::CURRENT));
    logger(logINFO) << " Seeing " << hv_v_off << "V and " << hv_i_off << "A in off state.";

    testSum["results"]["CntSetHV0frq"][0] = frequency;
    testSum["results"]["HVENABLE"    ][0] = false;
    testSum["results"]["HVVIN"       ][0] = hv_v_off;
    testSum["results"]["HVIIN"       ][0] = hv_i_off;

    // Turn on HV
    logger(logINFO) << " --> Turn on HV enable.";
    try
      {
	amac->wrField(&AMACv2::CntSetHV0en, 1);
	amac->wrField(&AMACv2::CntSetCHV0en, 1);
      }
    catch(EndeavourComException &e)
      {
	logger(logERROR) << e.what();
	testSum["error"] = e.what();
	return testSum;
      }
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    double hv_v_on = std::stod(sm->sense(KeithleyMode::VOLTAGE));
    double hv_i_on = std::stod(sm->sense(KeithleyMode::CURRENT));
    logger(logINFO) << " Seeing " << hv_v_on << "V and " << hv_i_on << "A in on state.";

    testSum["results"]["CntSetHV0frq"][1] = frequency;
    testSum["results"]["HVENABLE"    ][1] = true;
    testSum["results"]["HVVIN"       ][1] = hv_v_on;
    testSum["results"]["HVIIN"       ][1] = hv_i_on;

    // TODO interpret
    if (hv_v_off > 350.0 && hv_i_off < 2e-6 && hv_v_on > 250 && hv_i_on > 0.4e-3)
      {
	logger(logINFO) << " --> Test successful!";
	testSum["passed"] = true;
      }
    else
      {
	logger(logERROR) << " --> Something wrong with HV enable!";
	testSum["passed"] = false;
      }

    sm->turnOff();

    testSum["results"]["TIMEEND"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());
  
    amac->initRegisters();
  
    return testSum;
  }

  json readStatus(AMACv2 *amac, GenericPs *ps, Bk85xx *load, Keithley24XX *sm)
  {
    logger(logINFO) << "## Reading current status ##";
    json testSum;
    testSum["testType"] = "STATUS";
    testSum["results"]["TIMESTART"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());

    // ADCs
    int Vdcdc = amac->rdField(&AMACv2::Ch0Value);
    int VddLr = amac->rdField(&AMACv2::Ch1Value);
    int DCDCin = amac->rdField(&AMACv2::Ch2Value);
    amac->wrField(&AMACv2::Ch3Mux, 0); //a
    int VDDREG = amac->rdField(&AMACv2::Ch3Value);
    amac->wrField(&AMACv2::Ch3Mux, 2); //c
    int AM900BG = amac->rdField(&AMACv2::Ch3Value);
    amac->wrField(&AMACv2::Ch4Mux, 0); //a
    int AM600BG = amac->rdField(&AMACv2::Ch4Value);
    amac->wrField(&AMACv2::Ch4Mux, 1); //b
    int CALin = amac->rdField(&AMACv2::Ch4Value);
    int NTC = amac->rdField(&AMACv2::Ch9Value);
    int Cur10V = amac->rdField(&AMACv2::Ch12Value);
    int Cur1V = amac->rdField(&AMACv2::Ch13Value);
    int HVret = amac->rdField(&AMACv2::Ch14Value);
    int PTAT = amac->rdField(&AMACv2::Ch15Value);

    double Vin = std::stod(ps->getVoltage());
    double Iin = std::stod(ps->getCurrent());

    double Vout = load->getValues().vol;
    double Iout = load->getValues().cur;

    double HV_Vin = std::stod(sm->sense(KeithleyMode::VOLTAGE));
    double HV_Iin = std::stod(sm->sense(KeithleyMode::CURRENT));

    double ADC0 = dynamic_cast<EndeavourRawFTDI*>(amac->raw().get())->getADC()->read(0);
    double ADC1 = dynamic_cast<EndeavourRawFTDI*>(amac->raw().get())->getADC()->read(1);
    double ADC2 = dynamic_cast<EndeavourRawFTDI*>(amac->raw().get())->getADC()->read(2);
    double ADC3 = dynamic_cast<EndeavourRawFTDI*>(amac->raw().get())->getADC()->read(3);

    std::cout << "Vdcdc [counts]\tVddLr [counts]\tDCDCin [counts]\tVddReg [counts]\t" <<
      "Am900Bg [counts]\tAm600Bg [counts]\tCal [counts]\tNTC [counts]\tCur10V [counts]\t" <<
      "Cur1V [counts]\tHVret [counts]\tPTAT [counts]\tVin [V]\tIin [A]\tVout [mV]\tIout [mA]\t" <<
      "HV_Vin [V]\tHV_Iin [A]\tADC0 [V]\tADC1 [V]\tADC2 [V]\tADC3 [V]" << std::endl;
    std::cout << Vdcdc << "\t" << VddLr << "\t" << DCDCin << "\t" << VDDREG << "\t" << AM900BG <<
      "\t" << AM600BG << "\t" << CALin << "\t" << NTC << "\t" << Cur10V << "\t" << Cur1V << 
      "\t" << HVret << "\t" << PTAT << "\t" << Vin << "\t" << Iin << "\t" << Vout << "\t" << 
      Iout << "\t" << HV_Vin << "\t" << HV_Iin << "\t" << ADC0 << "\t" << ADC1 << "\t" << ADC2 << "\t" << ADC3 << std::endl;

    testSum["results"]["AMACVDCDC"  ]=Vdcdc;
    testSum["results"]["AMACVDDLR"  ]=VddLr;
    testSum["results"]["AAMCDCDCIN" ]=DCDCin;
    testSum["results"]["AMACVDDREG" ]=VDDREG;
    testSum["results"]["AMACAM900BG"]=AM900BG;
    testSum["results"]["AMACAM600BG"]=AM600BG;
    testSum["results"]["AMACCAL"    ]=CALin;
    testSum["results"]["AMACNTCPB"  ]=NTC;
    testSum["results"]["AMACCUR10V" ]=Cur10V;
    testSum["results"]["AMACCUR1V"  ]=Cur1V;
    testSum["results"]["AMACHVRET"  ]=HVret;
    testSum["results"]["AMACPTAT"   ]=PTAT;
    testSum["results"]["VIN"        ]=Vin;
    testSum["results"]["IIN"        ]=Iin;
    testSum["results"]["VOUT"       ]=Vout*1e-3;
    testSum["results"]["IOUT"       ]=Iout*1e-3;
    testSum["results"]["HVVIN"      ]=HV_Vin;
    testSum["results"]["HVIIN"      ]=HV_Iin;
    testSum["results"]["ADC0"       ]=ADC0;
    testSum["results"]["ADC1"       ]=ADC1;
    testSum["results"]["ADC2"       ]=ADC2;
    testSum["results"]["ADC3"       ]=ADC3;

    testSum["results"]["TIMEEND"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());

    return testSum;
  }

  json runBER(std::shared_ptr<AMACv2> amac, uint32_t trails)
  {
    logger(logINFO) << "## Running Bit Error Rate Test ##";
    json testSum;
    testSum["testType"] = "BER";
    testSum["passed"] = false;
    testSum["results"]["TIMESTART"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());

    testSum["properties"]["TRAILS"] = trails;

    // Run the test
    uint good=0;
    for(uint i=0; i<trails; i++)
      {
	try
	  {      
	    uint valin=rand()*0xFFFFFFFF;
	    amac->write_reg(166,valin);
	    usleep(50);
	    uint valout=amac->read_reg(166);
	    usleep(50);
	    if(valin==valout)
	      good++;
	    else
	      logger(logERROR) << "Write: 0x" << std::hex << valin << ", Read: " << valout << std::dec;
	  }
	catch(EndeavourComException &e)
	  {
	    logger(logDEBUG) << e.what();
	  }
      }
    float reliability=((float)good)/trails;

    // Store the results
    testSum["results"]["RELIABILITY"] = reliability;

    testSum["results"]["TIMEEND"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());
    testSum["passed"] = reliability==1;

    if(reliability!=1) { logger(logERROR) << "Reliability = " << reliability; }

    return testSum;
  }

  json calibrateAMACoffset(std::shared_ptr<AMACv2> amac, bool scanSettings)
  {
    logger(logINFO) << "## Calibrating AMAC offset ##";
    json testSum;
    testSum["testType"] = "AMOFFSET";
    testSum["results"]["TIMESTART"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());

    int counts;

    amac->wrField(&AMACv2Reg::AMzeroCalib, 1);
    amac->wrField(&AMACv2Reg::AMzeroCalibC, 1);

    uint32_t gain_curr=amac->rdField(&AMACv2Reg::AMintCalib);

    int index = 0;
    for(uint32_t gain_set=((scanSettings)?0:gain_curr);
	gain_set<((scanSettings)?16:(gain_curr+1));
	gain_set++)
      {
	amac->wrField(&AMACv2Reg::AMintCalib, gain_set);
	usleep(5e3);

	counts = amac->rdField(&AMACv2Reg::Ch0Value );
	testSum["results"][ "CH0"][index] = {counts};
	counts = amac->rdField(&AMACv2Reg::Ch1Value );
	testSum["results"][ "CH1"][index] = {counts};
	counts = amac->rdField(&AMACv2Reg::Ch2Value );
	testSum["results"][ "CH2"][index] = {counts};
	counts = amac->rdField(&AMACv2Reg::Ch3Value );
	testSum["results"][ "CH3"][index] = {counts};
	counts = amac->rdField(&AMACv2Reg::Ch4Value );
	testSum["results"][ "CH4"][index] = {counts};
	counts = amac->rdField(&AMACv2Reg::Ch5Value );
	testSum["results"][ "CH5"][index] = {counts};
	counts = amac->rdField(&AMACv2Reg::Ch6Value );
	testSum["results"][ "CH6"][index] = {counts};
	counts = amac->rdField(&AMACv2Reg::Ch7Value );
	testSum["results"][ "CH7"][index] = {counts};
	counts = amac->rdField(&AMACv2Reg::Ch8Value );
	testSum["results"][ "CH8"][index] = {counts};
	counts = amac->rdField(&AMACv2Reg::Ch9Value );
	testSum["results"][ "CH9"][index] = {counts};
	counts = amac->rdField(&AMACv2Reg::Ch10Value);
	testSum["results"]["CH10"][index] = {counts};
	counts = amac->rdField(&AMACv2Reg::Ch11Value);
	testSum["results"]["CH11"][index] = {counts};
	counts = amac->rdField(&AMACv2Reg::Ch12Value);
	testSum["results"]["CH12"][index] = {counts};
	counts = amac->rdField(&AMACv2Reg::Ch13Value);
	testSum["results"]["CH13"][index] = {counts};
	counts = amac->rdField(&AMACv2Reg::Ch14Value);
	testSum["results"]["CH14"][index] = {counts};
	counts = amac->rdField(&AMACv2Reg::Ch15Value);
	testSum["results"]["CH15"][index] = {counts};

	testSum["results"]["AMintCalib"][index] = {gain_set};

	index++;
      }

    testSum["results"]["TIMEEND"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());
  
    amac->initRegisters();
  
    return testSum;
  }

  json calibrateAMACslope(std::shared_ptr<AMACv2> amac, double step, bool scanSettings)
  {
    logger(logINFO) << "## Calibrating AMAC slope ##";
    json testSum;
    testSum["testType"] = "AMSLOPE";
    testSum["results"]["TIMESTART"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());

    std::cout << "CAL" << "\t" << "BG" << "\t" << "RampGain" << "\t" << "ADCvalue" << std::endl;

    uint32_t gain_curr=amac->rdField(&AMACv2Reg::AMintCalib);
    uint32_t bg_curr=amac->rdField(&AMACv2Reg::AMbg);

    // Run the test
    uint32_t index=0;
    double CALact=0;
    uint32_t CALamac=0;
    amac->wrField(&AMACv2Reg::Ch4Mux , 1); // Set Ch4 mux to CAL input
    for(uint32_t bg_set = ((scanSettings)?0:bg_curr);
	bg_set<((scanSettings)?16:(bg_curr+1));
	bg_set++)
      {
	amac->wrField(&AMACv2Reg::AMbg , bg_set);

	for(uint32_t gain_set = (scanSettings)?0:gain_curr;
	    gain_set<((scanSettings)?16:(gain_curr+1));
	    gain_set++)
	  {
	    amac->wrField(&AMACv2Reg::AMintCalib, gain_set);

	    // Longer wait for the first value due to RC constant
	    dynamic_cast<EndeavourRawFTDI*>(amac->raw().get())->getDAC()->set(0);
	    usleep(10e3);
	    for(double CALin=0; CALin<1.01; CALin+=step)
	      {
		// actual voltage to be compared against digitized value
		CALact=dynamic_cast<EndeavourRawFTDI*>(amac->raw().get())->getDAC()->set(CALin*2)/2;

		// digital about
		usleep(5e3);
		CALamac = amac->rdField(&AMACv2Reg::Ch4Value);

		testSum["results"]["CALIN"     ][index] = CALact;
		testSum["results"]["AMACCAL"   ][index] = CALamac;
		testSum["results"]["AMintCalib"][index] = gain_set;
		testSum["results"]["AMbg"      ][index] = bg_set;
		index++;
		std::cout << CALact << "\t" << bg_set << "\t" << gain_set << "\t" << CALamac << std::endl;
	      }
	  } //end cycling through voltages
      } //end cycling through bandgap
    // Store the results

    testSum["results"]["TIMEEND"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());
  
    amac->initRegisters();

    return testSum;
  }

  json testOF(AMACv2 *amac, GenericPs *ps)
  {
    logger(logINFO) << "## Test OF ##";
    json testSum;
    testSum["testType"] = "OF";
    testSum["results"]["TIMESTART"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());
    testSum["passed"] = false;

    logger(logINFO) << " --> Disabling OF";
    dynamic_cast<EndeavourRawFTDI*>(amac->raw().get())->setOF(false);
    sleep(1);    
    double Iin = (ps)?std::stod(ps->getCurrent()):0;
    double Vin = (ps)?std::stod(ps->getVoltage()):0;
    double ADC0 = dynamic_cast<EndeavourRawFTDI*>(amac->raw().get())->getADC()->read(0);
    testSum["results"]["OF"     ][0] = 0;
    testSum["results"]["VIN"    ][0] = Vin;
    testSum["results"]["IIN"    ][0] = Iin;
    testSum["results"]["linPOLV"][0] = ADC0;
    bool OFoff_success=ADC0>1.3;
    logger(logINFO) << " ---> Input power: " << Vin << " V, " << Iin << " A";
    if(OFoff_success)
      logger(logINFO) << " ---> linPOL12V:  " << ADC0 << " V";
    else
      logger(logERROR) << " ---> linPOL12V:  " << ADC0 << " V";

    logger(logINFO) << " --> Enabling OF";
    dynamic_cast<EndeavourRawFTDI*>(amac->raw().get())->setOF(true);
    sleep(1);
    Iin = (ps)?std::stod(ps->getCurrent()):0;
    Vin = (ps)?std::stod(ps->getVoltage()):0;
    ADC0 = dynamic_cast<EndeavourRawFTDI*>(amac->raw().get())->getADC()->read(0);
    testSum["results"]["OF"     ][1] = 1;
    testSum["results"]["VIN"    ][1] = Vin;
    testSum["results"]["IIN"    ][1] = Iin;
    testSum["results"]["linPOLV"][1] = ADC0;
    bool OFon_success =ADC0<0.1;
    logger(logINFO) << " ---> Input power: " << Vin << " V, " << Iin << " A";
    if(OFon_success)
      logger(logINFO) << " ---> linPOL12V:  " << ADC0 << " V";
    else
      logger(logERROR) << " ---> linPOL12V:  " << ADC0 << " V";

    logger(logINFO) << " --> Disabling OF";
    dynamic_cast<EndeavourRawFTDI*>(amac->raw().get())->setOF(false);


    testSum["results"]["TIMEEND"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());
    testSum["passed"] = OFon_success && OFoff_success;
  
    amac->initRegisters();

    return testSum;
  }

  std::string getTimeAsString(std::chrono::system_clock::time_point t)
  {
    auto as_time_t = std::chrono::system_clock::to_time_t(t);
    struct tm tm;
    char some_buffer[128];
    if (::gmtime_r(&as_time_t, &tm))
      if (std::strftime(some_buffer, sizeof(some_buffer), "%Y_%m_%d-%X", &tm))
	return std::string(some_buffer);
    throw std::runtime_error("Failed to get current date as string");
  }

  json measureHvSense(AMACv2 *amac, Keithley24XX *sm)
  {
    logger(logINFO) << "## Measure HV sense ##";
    json testSum;
    testSum["testType"] = "HVSENSE";
    testSum["results"]["TIMESTART"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());

    double ileak_min = 0;
    double ileak_max = 1.0e-3;
    double ileak_step = 1.0e-8;
    double resist = 400e3; //400 kOhm resistor in HV circuit	

    logger(logINFO) << " --> Turn on SourceMeter";
    sm->turnOff();
    // sm->setSource(KeithleyMode::VOLTAGE, 500, 500);        
    // sm->setSense(KeithleyMode::CURRENT, ileak_min >= 1e-6 ? ileak_min : 1e-6, ileak_min);//??
    sm->setSource(KeithleyMode::CURRENT, ileak_min >= 1e-6 ? ileak_min : 1e-6, ileak_min);//??
    sm->setSense(KeithleyMode::VOLTAGE, 500, 500);        
    
    sm->turnOn();

    logger(logINFO) << " --> Turn on HVmux";
    amac->wrField(&AMACv2::CntSetHV0en, 1);
    amac->wrField(&AMACv2::CntSetCHV0en, 1);

    amac->wrField(&AMACv2::HVcurGain, 0);

    logger(logINFO) << " --> Starting measurement";
    std::cout << "HV_i_set\tHV_v_set\t\tHV_v\t\tHV_i\t\t\tGain 0\tGain 1\tGain 2\tGain 4\tGain 8" << std::endl;

    int counter = 0;
    int index = 0;  
    for (double ileak=ileak_min; ileak<=ileak_max; ileak += ileak_step,index++)
      {
	counter++;
	if (counter%10 == 0)
	ileak_step = ileak_step*10;
	//sm->setSource(KeithleyMode::VOLTAGE, ileak >= 1e-6 ? ileak : (1e-6)*resist, ileak*resist);
	sm->setSource(KeithleyMode::CURRENT, ileak >= 1e-6 ? ileak : (1e-6), ileak);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	double hv_v = std::stod(sm->sense(KeithleyMode::VOLTAGE));
	double hv_i = std::stod(sm->sense(KeithleyMode::CURRENT));
	std::this_thread::sleep_for(std::chrono::seconds(1));
	unsigned val[5];
	for (unsigned i=0; i<5; i++)
	  {
	    if (i == 0)
	      amac->wrField(&AMACv2::HVcurGain, 0);
	    else
	      amac->wrField(&AMACv2::HVcurGain, pow(2,i-1));

	    std::this_thread::sleep_for(std::chrono::milliseconds(500));
	    val[i] = amac->rdField(&AMACv2::Ch14Value);
	  }
	std::cout << ileak << "\t" << ileak*resist<< "\t" << hv_v << "\t" << hv_i << "\t" << val[0] << "\t" << val[1] << "\t" << val[2] << "\t" << val[3] << "\t" << val[4] << std::endl;
	testSum["data"]["ILEAK"    ][index]=ileak;
	testSum["data"]["HVV"      ][index]=hv_v;
	testSum["data"]["HVI"      ][index]=hv_i;
	testSum["data"]["AMACGAIN0"][index]=val[0];
	testSum["data"]["AMACGAIN1"][index]=val[1];
	testSum["data"]["AMACGAIN2"][index]=val[2];
	testSum["data"]["AMACGAIN4"][index]=val[3];
	testSum["data"]["AMACGAIN8"][index]=val[4];
	index++;
      }

    // sm->setSource(KeithleyMode::VOLTAGE, 500, 500);
    // sm->setSense(KeithleyMode::CURRENT, 1.27e-6, 1.27e-6);
    sm->setSource(KeithleyMode::CURRENT, 1.e-6, 1.e-6);
    sm->setSense(KeithleyMode::VOLTAGE, 500, 500);
    sm->turnOff();

    testSum["results"]["TIMEEND"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());
  
    amac->initRegisters();

    return testSum;
  }

  json measureLvIV(GenericPs *ps)
  {
    logger(logINFO) << "## Measure LV IV ##";
    json testSum;
    testSum["testType"] = "VINIIN";
    testSum["results"]["TIMESTART"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());

    double v_start = 2.0;
    double v_end = 6.0;
    double v_step = 0.1;
    logger(logINFO) << " --> Running from " << v_start << "V to " << v_end << "V in steps of " << v_step << "V";
    ps->turnOff();
    ps->setVoltage(v_start);
    ps->setCurrent(1.0);
    ps->turnOn();

    std::cout << "Vset\tVread\tIread" << std::endl;
    int index = 0;
    for (double vset = v_start; vset<=v_end; vset+=v_step)
      {
	ps->setVoltage(vset);
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	double v_read = std::stod(ps->getVoltage());
	double i_read = std::stod(ps->getCurrent());
	
	std::cout << vset << "\t" << v_read << "\t" << i_read << std::endl;
	testSum["results"]["VINSET"][index] = vset;
	testSum["results"]["VIN"   ][index] = v_read;
	testSum["results"]["IIN"   ][index] = i_read;
	index++;
      }

    logger(logINFO) << " --> Done!";

    testSum["results"]["TIMEEND"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());
  
    return testSum;
  }

  json calibVinResponse(AMACv2 *amac, GenericPs *ps)
  {
    logger(logINFO) << "## Measure LV IV ##";
    json testSum;
    testSum["testType"] = "VIN";
    testSum["results"]["TIMESTART"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());

    double v_start = 6.0;
    double v_end = 11.0;
    double v_step = 0.1;
    logger(logINFO) << " --> Running from " << v_start << "V to " << v_end << "V in steps of " << v_step << "V";

    std::cout << "Vset\tVread\tIread\tDCDCin [counts]" << std::endl;
    int index = 0;
    for (double vset = v_start; vset<=v_end; vset+=v_step)
      {
	ps->setVoltage(vset);
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	int DCDCin;
	try
	  {
	    DCDCin = amac->rdField(&AMACv2::Ch2Value);
	  }
	catch(EndeavourComException &e)
	  {
	    logger(logERROR) << e.what();
	    testSum["error"] = e.what();
	    return testSum;
	  }
	double v_read = std::stod(ps->getVoltage());
	double i_read = std::stod(ps->getCurrent());
                
	std::cout << vset << "\t" << v_read << "\t" << i_read << "\t" << DCDCin <<  std::endl;
	testSum["results"]["VINSET"    ][index] = vset;
	testSum["results"]["VIN"       ][index] = v_read;
	testSum["results"]["IIN"       ][index] = i_read;
	testSum["results"]["AMACDCDCIN"][index] = DCDCin;
	index++;
      }

    ps->setVoltage(11.0);

    testSum["results"]["TIMEEND"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());
  
    amac->initRegisters();

    return testSum;
  }

  json calibrateAMACcm(std::shared_ptr<AMACv2> amac, uint32_t tests) 
  {
    logger(logINFO) << "## Calibrating CM block ## " << PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());
    json testSum;
    testSum["testType"] = "CMOFFSET";
    testSum["results"]["TIMESTART"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());

    logger(logINFO) << " --> Turn on DCDC ...";
    try {
      amac->wrField(&AMACv2::DCDCen, 1);
      amac->wrField(&AMACv2::DCDCenC, 1);
    } catch(EndeavourComException &e) {
      logger(logERROR) << e.what();
      testSum["error"] = e.what();
      return testSum;
    }

    logger(logINFO) << " --> Starting measurement ...";
    //std::cout << "Cur10V" << "\t" << "Cur1V" << std::endl;
    // Set sub-channel
    try {
      amac->wrField(&AMACv2::Ch12Mux, 0); //a
      amac->wrField(&AMACv2::Ch13Mux, 0); //a
    } catch(EndeavourComException &e) {
      logger(logERROR) << e.what();
      return testSum;
    } 

    // Short the P/N
    amac->wrField(&AMACv2Reg::DCDCiZeroReading , 1);
    amac->wrField(&AMACv2Reg::DCDCoZeroReading , 1);

    // Take many measurements
    for (uint32_t i=0; i<tests; i++)
      {
	int32_t Cur10V, Cur1V;
	usleep(2e3);
	try
	  {
	    Cur10V = amac->rdField(&AMACv2::Ch12Value);
	    Cur1V  = amac->rdField(&AMACv2::Ch13Value);
	  }
	catch(EndeavourComException &e)
	  {
	    logger(logERROR) << e.what();
	    testSum["error"] = e.what();
	    return testSum;
	  }

	//if(i%=(tests/10)==0) std::cout << Cur10V << "\t" << Cur1V << std::endl;
	testSum["results"]["AMACCUR10V"][i] = Cur10V;
	testSum["results"]["AMACCUR1V"][i] = Cur1V;
      }

    // Separate the P/N
    amac->wrField(&AMACv2Reg::DCDCiZeroReading , 1);
    amac->wrField(&AMACv2Reg::DCDCoZeroReading , 1);

    testSum["results"]["TIMEEND"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());
  
    amac->initRegisters();

    return testSum;
  }


}

