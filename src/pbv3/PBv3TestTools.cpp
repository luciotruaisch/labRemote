#include "PBv3TestTools.h"

#include "EndeavourComException.h"
#include "EndeavourRawFTDI.h"

#include <memory>

namespace PBv3TestTools {
    json testLvEnable(AMACv2 *amac, GenericPs *ps, Bk85xx *load) {
        logger(logINFO) << "## LV Enable test ## " << PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());

        json testSum;
        testSum["name"] = "lv_enable";
        testSum["time"]["start"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());
        testSum["success"] = false;
        // Init stuff
        load->setCurrent(0);
        load->turnOn();
        try {
            amac->wrField(&AMACv2::DCDCen, 0);
            amac->wrField(&AMACv2::DCDCenC, 0);
        } catch(EndeavourComException &e) {
            logger(logERROR) << e.what();
            return testSum;
        }
    
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        testSum["header"] = {"DCDC Enable", "Vout [V]"};

        double lv_off = load->getValues().vol;//mV
        logger(logINFO) << " --> Reading " << lv_off << "mV in off state.";
        testSum["data"][0] = {0, lv_off};
        logger(logINFO) << " --> Trying to turn on DCDC ...";
        try {
            amac->wrField(&AMACv2::DCDCen, 1);
            amac->wrField(&AMACv2::DCDCenC, 1);
        } catch(EndeavourComException &e) {
            logger(logERROR) << e.what();
            return testSum;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        double lv_on = load->getValues().vol;//mV
        logger(logINFO) << " --> Reading " << lv_on << "mV in off state.";
        testSum["data"][1] = {1, lv_on};

        if (!(lv_on > 1.4e3 && lv_on < 1.6e3 && lv_off < 0.1e3)) {
            logger(logERROR) << " --> LV enable not working! " << lv_on << " " << lv_off;
            return testSum;
        } else {
            logger(logINFO) << " --> LV enable good!";
        }
        testSum["time"]["end"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());
        testSum["success"] = true;
        return testSum;

    }

    // Range is iout in mA
    json measureEfficiency(AMACv2 *amac, GenericPs *ps, Bk85xx *load, int step, int min, int max) {
        logger(logINFO) << "## Measuring DCDC efficiency ## " << PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());
        json testSum;
        testSum["name"] = "measure_efficiency";
        testSum["success"] = false;
        testSum["time"]["start"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());
        load->setCurrent(0);
        load->turnOn();
        logger(logINFO) << " --> Turn off DCDC ..";
	try {
            amac->wrField(&AMACv2::DCDCen, 0);
            amac->wrField(&AMACv2::DCDCenC, 0);
        } catch(EndeavourComException &e) {
            logger(logERROR) << e.what();
            testSum["error"] = e.what();
            return testSum;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        logger(logINFO) << " --> Get baseline current: (" << ps->getCurrent() << ")A";
        double Iin_offset = std::stod(ps->getCurrent());
        testSum["Iin_offset"] = Iin_offset;
        
        logger(logINFO) << " --> Turn on DCDC ...";
        try {
            amac->wrField(&AMACv2::DCDCen, 1);
            amac->wrField(&AMACv2::DCDCenC, 1);
        } catch(EndeavourComException &e) {
            logger(logERROR) << e.what();
            testSum["error"] = e.what();
            return testSum;
        }

        unsigned dwell_time = 5; //s
        testSum["dwellTime"] = dwell_time;

        logger(logINFO) << " --> Starting measurement ...";
        std::cout << "Vin" << "\t" << "Iin" << "\t" << "Vout" << "\t" << "Iout" << "\t" << "Vdcdc"
            << "\t" << "VddLr" << "\t" << "DCDCin" << "\t" << "NTC" << "\t"
            << "Cur10V" << "\t" << "Cur1V" << "\t" << "PTAT" << "\t" << "Efficiency" << std::endl;
        testSum["header"] = {"Vin [V]", "Iin [A]", "Vout [V]", "Iout [mA]", "Vdcdc [counts]",
            "VddLR [counts]", "DCDCin [counts]", "NTC [counts]",
            "Cur10V [counts]", "Cur1V [counts]", "PTAT [counts]"};
        // Set sub-channel
        try {
            amac->wrField(&AMACv2::Ch12Mux, 0); //a
            amac->wrField(&AMACv2::Ch13Mux, 0); //a
        } catch(EndeavourComException &e) {
            logger(logERROR) << e.what();
            return testSum;
        }
        
        // Loop over currents
        int index = 0;
        for (int iout=min;iout<=max;iout+=step,index++) {
            logger(logDEBUG) << " --> Setting " << iout << "mA load!";
            // Set Current
            load->setCurrent(iout);
            // Wait for temp and everything to settle
            std::this_thread::sleep_for(std::chrono::seconds(dwell_time));
            // Read AMAC values
            int Vdcdc = amac->rdField(&AMACv2::Ch0Value);
            int VddLr = amac->rdField(&AMACv2::Ch1Value);
            int DCDCin = amac->rdField(&AMACv2::Ch2Value);
            int NTC = amac->rdField(&AMACv2::Ch9Value);
            int Cur10V = amac->rdField(&AMACv2::Ch12Value);
            int Cur1V = amac->rdField(&AMACv2::Ch13Value);
            int PTAT = amac->rdField(&AMACv2::Ch15Value);

            double Vin = std::stod(ps->getVoltage());
            double Iin = std::stod(ps->getCurrent());

            double Vout = load->getValues().vol;

            double efficiency = (1.5*iout*1e-3)/(Vin*(Iin-Iin_offset));
            std::cout << Vin << "\t" << Iin << "\t" << Vout << "\t" << iout << "\t" << Vdcdc
                << "\t" << VddLr << "\t" << DCDCin << "\t" << NTC << "\t"
                << Cur10V << "\t" << Cur1V << "\t" << PTAT << "\t" << efficiency << std::endl;
            testSum["data"][index] = {Vin, Iin, Vout, iout, Vdcdc, VddLr, DCDCin, NTC, Cur10V, Cur1V, PTAT};

        }

	logger(logINFO) << " --> Done!! Turng off load!";
	load->setCurrent(0);
        
        testSum["success"] = true;
        testSum["time"]["end"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());
        return testSum;
    }

    json testHvEnable(AMACv2 *amac, Keithley24XX *sm, unsigned frequency) {
        logger(logINFO) << "## Testing HV Enable circuit ## " << PBv3TestTools::getTimeAsString(std::chrono::system_clock::now()) ;
        logger(logINFO) << " --> Frequency: " << frequency << " (0=high, 1=25kHz, 2=50kHz, 3=100kHz)";
        logger(logINFO) << " --> Turning everything off.";
        
        json testSum;
        testSum["name"] = "hv_enable";
        testSum["success"] = false;
        testSum["time"]["start"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());
        testSum["config"]["CntSetHV0frq"] = frequency;
        
        // Turn off HV enable from AMAC
        amac->wrField(&AMACv2::CntSetHV0frq, frequency);
        amac->wrField(&AMACv2::CntSetCHV0frq, frequency);
        amac->wrField(&AMACv2::CntSetHV0en, 0);
        amac->wrField(&AMACv2::CntSetCHV0en, 0);

        // Configure sourcemeter
        sm->turnOff();
        sm->setSource(KeithleyMode::CURRENT, 1e-6, 1e-6);
        sm->setSense(KeithleyMode::VOLTAGE, 500, 500);

        testSum["header"] = {"HV Enable", "Voltage [V]", "Current [A]"};
        // See what we see
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        sm->turnOn();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        double hv_v_off = std::stod(sm->sense(KeithleyMode::VOLTAGE));
        double hv_i_off = std::stod(sm->sense(KeithleyMode::CURRENT));
        logger(logINFO) << " Seeing " << hv_v_off << "V and " << hv_i_off << "A in off state.";
        testSum["data"][0] = {0, hv_v_off, hv_i_off};

        // Turn on HV
        logger(logINFO) << " --> Turn on HV enable.";
        amac->wrField(&AMACv2::CntSetHV0en, 1);
        amac->wrField(&AMACv2::CntSetCHV0en, 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        double hv_v_on = std::stod(sm->sense(KeithleyMode::VOLTAGE));
        double hv_i_on = std::stod(sm->sense(KeithleyMode::CURRENT));
        logger(logINFO) << " Seeing " << hv_v_on << "V and " << hv_i_on << "A in on state.";
        testSum["data"][1] = {1, hv_v_on, hv_i_on};

        // TODO interpret
        if (hv_v_off > 490.0 && hv_i_off < 1.0e-9 && hv_v_on > 400 && hv_i_on > 0.5e-6) {
            logger(logINFO) << " --> Test successful!";
            testSum["success"] = true;
        } else {
            logger(logERROR) << " --> Something wrong with HV enable!";
            testSum["success"] = false;
        }

        testSum["time"]["end"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());
        return testSum;
    }

  json readStatus(AMACv2 *amac, GenericPs *ps, Bk85xx *load, Keithley24XX *sm) {
    logger(logINFO) << "## Reading current status ##";
    json testSum;
    testSum["name"] = "amac_status";
    testSum["success"] = false;
    testSum["time"]["start"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());

    // Control reg status
    int HVcurGain = amac->rdField(&AMACv2::HVcurGain);
    int RingOscFrq = amac->rdField(&AMACv2::RingOscFrq);
    int VDDbg = amac->rdField(&AMACv2::VDDbg);
    int AMbg = amac->rdField(&AMACv2::AMbg);
    int AMintCalib = amac->rdField(&AMACv2::AMintCalib);

    testSum["config"]["HvcurGain"] = HVcurGain;
    testSum["config"]["RingOscFrq"] = RingOscFrq;
    testSum["config"]["VDDbg"] = VDDbg;
    testSum["config"]["AMbg"] = AMbg;
    testSum["config"]["AMinCalib"] = AMintCalib;
        
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
      "Am900Bg [counts]]\tAm600Bg [counts]\tCal [counts]\tNTC [counts]\tCur10V [counts]\t" <<
      "Cur1V [counts]\tHVret [counts]\tPTAT [counts]\tVin [V]\tIin [A]\tVout [mV]\tIout [mA]\t" <<
      "HV_Vin [V]\tHV_Iin [A]" << std::endl;
    std::cout << Vdcdc << "\t" << VddLr << "\t" << DCDCin << "\t" << VDDREG << "\t" << AM900BG <<
      "\t" << AM600BG << "\t" << CALin << "\t" << NTC << "\t" << Cur10V << "\t" << Cur1V << 
      "\t" << HVret << "\t" << PTAT << "\t" << Vin << "\t" << Iin << "\t" << Vout << "\t" << 
      Iout << "\t" << HV_Vin << "\t" << HV_Iin << std::endl;

    testSum["header"] = {"Vdcdc [counts]", "VddLr [counts]", "DCDCin [counts]", "VddReg [counts]",
			 "Am900Bg [counts]", "Am600Bg [counts]", "Cal [counts]", "NTC [counts]", "Cur10V [counts]",
			 "Cur1V [counts]", "HVret [counts]", "PTAT [counts]", "Vin [V]", "Iin [A]", "Vout [mV]", "Iout [mA]",
			 "HV_Vin [V]", "HV_Iin [A]", "ADC0 [V]", "ADC1 [V]", "ADC2 [V]", "ADC3 [V]"};
    testSum["data"][0] = {Vdcdc, VddLr, DCDCin, VDDREG, AM900BG, AM600BG, CALin, NTC, Cur10V, Cur1V, HVret, PTAT,
			  Vin, Iin, Vout, Iout, HV_Vin, HV_Iin, ADC0, ADC1, ADC2, ADC3};
    testSum["time"]["end"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());
    testSum["success"] = true;

    return testSum;
  }

    json runBER(AMACv2 *amac) {
        logger(logINFO) << "## Running Bit Error Rate Test ##";
        json testSum;
        testSum["name"] = "amac_ber";
        testSum["success"] = false;
        testSum["time"]["start"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());
        
	// Run the test
	uint trails=100;
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
		//std::cout << e.what() << std::endl;
	      }
	  }
	float reliability=((float)good)/trails;

	// Store the results
        testSum["header"] = {"Reliability"};
        testSum["data"][0] = {reliability};
        testSum["time"]["end"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());
        testSum["success"] = reliability==1;

	if(reliability!=1) { logger(logERROR) << "Reliability = " << reliability; }

        return testSum;
    }

  json calibrateAMAC(AMACv2 *amac, double step)
  {
    logger(logINFO) << "## Calibrating AMAC ##";
    json testSum;
    testSum["name"] = "amac_calibrate";
    testSum["success"] = false;
    testSum["time"]["start"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());

    // Run the test
    int index=0;
    for(double CALin=0; CALin<1.01; CALin+=step,index++)
      {
	double CALact=dynamic_cast<EndeavourRawFTDI*>(amac->raw().get())->getDAC()->set(CALin*3)/3;
	amac->wrField(&AMACv2Reg::Ch4Mux , 1);
	usleep(5e3);
	uint CALamac = amac->rdField(&AMACv2Reg::Ch4Value);
	testSum["data"][index] = {CALact, CALamac};
      }

    // Store the results
    testSum["header"] = {"CAL","Counts"};
    testSum["time"]["end"] = PBv3TestTools::getTimeAsString(std::chrono::system_clock::now());
    testSum["success"] = true;

    return testSum;
  }

    std::string getTimeAsString(std::chrono::system_clock::time_point t) {
        auto as_time_t = std::chrono::system_clock::to_time_t(t);
        struct tm tm;
        char some_buffer[128];
        if (::gmtime_r(&as_time_t, &tm))
            if (std::strftime(some_buffer, sizeof(some_buffer), "%X-%d_%m_%Y", &tm))
                return std::string(some_buffer);
        throw std::runtime_error("Failed to get current date as string");
    }

}
