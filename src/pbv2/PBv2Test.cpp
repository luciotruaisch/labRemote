#include "PBv2Test.h"

#include <fstream>

#include "Logger.h"
loglevel_e loglevel = logINFO;

PBv2Test::PBv2Test(const std::string& name, std::shared_ptr<PBv2TB> tb, uint8_t pbidx, std::shared_ptr<SorensenPs> ps)
  : m_name(name), m_tb(tb), m_pbidx(pbidx), m_pb(m_tb->getPB(pbidx)), m_ps(ps)
{ }

PBv2Test::~PBv2Test()
{ }

bool PBv2Test::runGeneral()
{
  logger(logINFO) << "  ++ Baseline values:";
  std::cout << "Input LV Current (PS): " << m_ps->getCurrent()    << std::endl;  
  std::cout << "Input LV Current (TB): " << m_tb->getVinCurrent() << std::endl;
  
  
  unsigned ota_l, ota_r, bgo, dvdd2;
  m_pb->read(AMACreg::VALUE_LEFT_CH5, ota_l);
  m_pb->read(AMACreg::VALUE_RIGHT_CH5, ota_r);
  m_pb->read(AMACreg::VALUE_LEFT_CH1, dvdd2);
  m_pb->read(AMACreg::VALUE_LEFT_CH2, bgo);
  std::string logpath = "log/" + m_name + "_General.log";
  std::fstream logfile(logpath, std::fstream::out);
  logfile << "OTALEFT OTARIGHT DVDD2 BGO InBase" << std::endl;
  logfile << ota_l << " " << ota_r << " " << dvdd2 << " " << bgo << " " << m_ps->getCurrent() << std::endl;
  logfile.close();

  std::cout << "OTA_LEFT: "  << ota_l << std::endl;
  std::cout << "OTA_RIGHT: " << ota_r << std::endl;
  std::cout << "DVDD2: "     << dvdd2 << std::endl;
  std::cout << "BGO: "       << bgo   << std::endl;

  return true;
}

bool PBv2Test::runLVEnable()
{
  m_pb->write(AMACreg::LV_ENABLE, 0x1);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  double lv_on = m_tb->getVout(m_pbidx);//V

  m_pb->write(AMACreg::LV_ENABLE, 0x0);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  double lv_off = m_tb->getVout(m_pbidx);//V

  if (!(lv_on > 1.4 && lv_on < 1.6 && lv_off < 0.1))
    {
      logger(logERROR) << " ++ LV enable not working! " << lv_on << " " << lv_off;
      return false;
    }

  logger(logINFO) << " ++ LV enable good! " << lv_on << " " << lv_off;
  return true;
}

bool PBv2Test::runDCDCEfficiency()
{
  double load_min = 0;
  double load_max = 2; //(lv_enable_works)?2.75:0;
  double load_step = 0.1;

  logger(logINFO) << "Testing Current Sense Amp & Efficiency...";

  m_pb->write(AMACreg::LV_ENABLE, 0x1);
  
  std::string logpath = "log/" + m_name + "_DCDCEfficiency.log";
  std::fstream logfile(logpath, std::fstream::out);
  logfile << "Iout Vout Iin Vin IoutADC ntc ptat" << std::endl;  

  for (double load = load_min; load <= load_max; load+=load_step)
    {
      unsigned cur = 0;
      unsigned ptat = 0;
      unsigned ntc = 0;
      double iout = m_tb->setLoad(m_pbidx, load)*1000;

      m_pb->read(AMACreg::VALUE_RIGHT_CH1, cur);
      m_pb->read(AMACreg::VALUE_RIGHT_CH3, ptat);
      m_pb->read(AMACreg::VALUE_RIGHT_CH2, ntc);
      logfile   << iout << " " << m_tb->getVout(m_pbidx) << " " << m_ps->getCurrent() << " " << m_ps->getVoltage() << " " << cur << " " << ntc << " " << ptat << std::endl;
      std::cout << iout << " " << m_tb->getVout(m_pbidx) << " " << m_ps->getCurrent() << " " << m_ps->getVoltage() << " " << cur << " " << ntc << " " << ptat << std::endl;
    }

  m_tb->setLoad(m_pbidx, 0);

  m_pb->write(AMACreg::LV_ENABLE, 0x0);

  return true;
}

bool PBv2Test::runVin()
{
  logger(logINFO) << "Testing VIN measurement...";

  std::string logpath = "log/" + m_name + "_VIN.log";
  std::fstream logfile(logpath, std::fstream::out);
  logfile << "Vin VinADC VinTB" << std::endl;

  double vin_min = 6.0;
  double vin_max = 12.0;
  double vin_step = 0.1;

  for (double vin=vin_min; vin<=vin_max; vin+=vin_step)
    {
      m_ps->setVoltage(vin);
      std::this_thread::sleep_for(std::chrono::seconds(1));
      unsigned val = 0;
      m_pb->read(AMACreg::VALUE_RIGHT_CH0, val);
      std::cout << vin << " " << val << " " << m_tb->getVin() << std::endl;
      logfile   << vin << " " << val << " " << m_tb->getVin() << std::endl;
  }
  m_ps->setVoltage(11.0);
  m_ps->setCurrent(2.00);
  logfile.close();

  return true;
}

bool PBv2Test::runVinIn()
{
  logger(logINFO) << "Testing current usage as a function of input voltage...";

  std::string logpath = "log/" + m_name + "_VinIin.log";
  std::fstream logfile(logpath, std::fstream::out);
  logfile << "Vin Iin Vout" << std::endl;

  double vin_min = 4.0;
  double vin_max = 5.0;
  double vin_step = 0.05;

  m_pb->write(AMACreg::LV_ENABLE, 0x1);  
  m_tb->setLoad(m_pbidx,0.1);

  for (double vin=vin_min; vin<=vin_max; vin+=vin_step)
    {
      m_ps->turnOff();
      m_ps->setVoltage(vin);
      m_ps->turnOn();
      m_pb->init();
      m_pb->write(AMACreg::LV_ENABLE, 0x1);  
      std::this_thread::sleep_for(std::chrono::seconds(1));

      double lv = m_tb->getVout(m_pbidx);
      std::cout << vin << " " << m_ps->getCurrent() << " " << lv << std::endl;
      logfile   << vin << " " << m_ps->getCurrent() << " " << lv << std::endl;
    }

  m_tb->setLoad(m_pbidx,0);
  m_pb->write(AMACreg::LV_ENABLE, 0x0);

  logfile.close();

  return true;
}
