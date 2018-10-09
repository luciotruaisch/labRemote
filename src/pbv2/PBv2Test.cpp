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
}
