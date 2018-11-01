#include "FreqMeas.h"

void FreqMeasInst::init_inst(std::shared_ptr<DeviceCom> dev, uint8_t id, uint32_t ts_cnt){
	m_dev = dev;
	m_id = id;
	this->reset_inst();
	this->freeze_inst();
	this->set_ts_cnt_inst(ts_cnt);
	this->start_inst();
}

void FreqMeasInst::reset_inst(bool active){
	uint32_t data = m_dev->read_reg32(4*m_id);
	data = (data & 0x7FFFFFFF) | ((uint32_t)~active << 31); //reset active low in FW
	m_dev->write_reg32((4*m_id), data);
}

void FreqMeasInst::toggle_reset_inst(){
  this->reset_inst(true);
  this->reset_inst(false);
}

void FreqMeasInst::freeze_inst(bool active){
	uint32_t data = m_dev->read_reg32(4*m_id);
	data = (data & 0xBFFFFFFF) | ((uint32_t)active << 30);
	m_dev->write_reg32((4*m_id), data);
}

void FreqMeasInst::start_inst(){
	this->reset_inst(false);
	this->freeze_inst(false);
}

void FreqMeasInst::set_ts_cnt_inst(uint32_t ts_cnt){
	uint32_t data = m_dev->read_reg32(4*m_id);
	data = (data & 0xC0000000) | (ts_cnt & 0x3FFFFFFF);
	m_dev->write_reg32((4*m_id), data);
}

void FreqMeasInst::read_inst(){
  m_hi_n = m_dev->read_reg32(4*m_id+1);
  m_lo_n = m_dev->read_reg32(4*m_id+2);
  m_hi_t = m_dev->read_reg32(4*m_id+3);
}

uint32_t FreqMeasInst::get_ts_cnt_inst(){
  uint32_t data = m_dev->read_reg32(4*m_id);
  return(data & 0x3FFFFFFF);
}

void FreqMeas::reset(FreqMeasInst FreqMeas::* ref, bool active){
		(this->*ref).reset_inst(active);
	}
void FreqMeas::reset(const std::string str, bool active){
  if(regMap.find(str) != regMap.end()){
    return(this->*regMap[str]).reset_inst(active);
  } else {
    std::cerr << " --> Error: Could not find register \""<< str << "\"" << std::endl;
  }
  return;
}

void FreqMeas::toggle_reset(FreqMeasInst FreqMeas::* ref){
  (this->*ref).toggle_reset_inst();
}
void FreqMeas::toggle_reset(const std::string str){
  if(regMap.find(str) != regMap.end()){
    toggle_reset(regMap.find(str)->second);
    return;
  } else {
    std::cerr << " --> Error: Could not find register \""<< str << "\"" << std::endl;
  }
  return;
}
void FreqMeas::toggle_reset_all(){
  for(auto const& it : regMap)
    toggle_reset(it.second);    
}

void FreqMeas::freeze(FreqMeasInst FreqMeas::* ref, bool active){
  (this->*ref).freeze_inst(active);
}
void FreqMeas::freeze(const std::string str, bool active){
  if(regMap.find(str) != regMap.end()){
    return(this->*regMap[str]).freeze_inst(active);
  } else {
    std::cerr << " --> Error: Could not find register \""<< str << "\"" << std::endl;
  }
  return;
}

void FreqMeas::start(FreqMeasInst FreqMeas::* ref){
  (this->*ref).start_inst();
}
void FreqMeas::start(const std::string str){
  if(regMap.find(str) != regMap.end()){
    return(this->*regMap[str]).start_inst();
  } else {
    std::cerr << " --> Error: Could not find register \""<< str << "\"" << std::endl;
  }
  return;
}

void FreqMeas::set_ts_cnt(FreqMeasInst FreqMeas::* ref, uint32_t ts_cnt){
  (this->*ref).set_ts_cnt_inst(ts_cnt);
}
void FreqMeas::set_ts_cnt(const std::string str, uint32_t ts_cnt){
  if(regMap.find(str) != regMap.end()){
    return(this->*regMap[str]).set_ts_cnt_inst(ts_cnt);
  } else {
    std::cerr << " --> Error: Could not find register \""<< str << "\"" << std::endl;
  }
  return;
}

void FreqMeas::read(FreqMeasInst FreqMeas::* ref){
  (this->*ref).read_inst();
}
void FreqMeas::read(const std::string str){
  if(regMap.find(str) != regMap.end()){
    return(this->*regMap[str]).read_inst();
  } else {
    std::cerr << " --> Error: Could not find register \""<< str << "\"" << std::endl;
  }
  return;
}

uint32_t FreqMeas::get_ts_cnt(FreqMeasInst FreqMeas::* ref){
  return (this->*ref).get_ts_cnt_inst();
}
uint32_t FreqMeas::get_ts_cnt(const std::string str){
  if(regMap.find(str) != regMap.end()){
    return(this->*regMap[str]).get_ts_cnt_inst();
  } else {
    std::cerr << " --> Error: Could not find register \""<< str << "\"" << std::endl;
  }
  return 0;
}

uint32_t FreqMeas::get_hi_n(FreqMeasInst FreqMeas::* ref){
  return (this->*ref).hi_n;
}
uint32_t FreqMeas::get_hi_n(const std::string str){
  if(regMap.find(str) != regMap.end()){
    return(this->*regMap[str]).hi_n;
  } else {
    std::cerr << " --> Error: Could not find register \""<< str << "\"" << std::endl;
  }
  return 0;
}
  
uint32_t FreqMeas::get_lo_n(FreqMeasInst FreqMeas::* ref){
  return (this->*ref).lo_n;
}
uint32_t FreqMeas::get_lo_n(const std::string str){
  if(regMap.find(str) != regMap.end()){
    return(this->*regMap[str]).lo_n;
  } else {
    std::cerr << " --> Error: Could not find register \""<< str << "\"" << std::endl;
  }
  return 0;
}

uint32_t FreqMeas::get_hi_t(FreqMeasInst FreqMeas::* ref){
  return (this->*ref).hi_t;
}
uint32_t FreqMeas::get_hi_t(const std::string str){
  if(regMap.find(str) != regMap.end()){
    return(this->*regMap[str]).hi_t;
  } else {
    std::cerr << " --> Error: Could not find register \""<< str << "\"" << std::endl;
  }
  return 0;
}

void FreqMeas::init(){
	HVOSC0.init_inst(m_dev, 0); regMap["HVOSC0"] = &FreqMeas::HVOSC0;
	HVOSC1.init_inst(m_dev, 1); regMap["HVOSC1"] = &FreqMeas::HVOSC1;
	HVOSC2.init_inst(m_dev, 2); regMap["HVOSC2"] = &FreqMeas::HVOSC2;
	HVOSC3.init_inst(m_dev, 3); regMap["HVOSC3"] = &FreqMeas::HVOSC3;
	CLKOUT.init_inst(m_dev, 4); regMap["CLKOUT"] = &FreqMeas::CLKOUT;
}

float FreqMeas::get_frq(FreqMeasInst FreqMeas::* ref)
{
  return ((float)get_hi_n(ref)/get_ts_cnt(ref))*m_bClkHz;
}

float FreqMeas::get_frq(const std::string& str)
{
  if(regMap.find(str) != regMap.end())
    return get_frq(regMap.find(str)->second);
  else
    std::cerr << " --> Error: Could not find register \""<< str << "\"" << std::endl;
  return 0.0;
}

float FreqMeas::get_duty_cycle(FreqMeasInst FreqMeas::* ref){
  return (float)get_hi_t(ref)/get_ts_cnt(ref);
}
float FreqMeas::get_duty_cycle(const std::string& str){
  if(regMap.find(str) != regMap.end())
    return get_duty_cycle(regMap.find(str)->second);
  else
    std::cerr << " --> Error: Could not find register \""<< str << "\"" << std::endl;
  return 0.0;
}
