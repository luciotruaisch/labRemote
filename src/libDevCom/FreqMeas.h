#ifndef FREQ_MEAS_H
#define FREQ_MEAS_H


#include "UIOCom.h"
#include <memory>
#include <cstdint>
#include <iostream>

#include <map>

class FreqMeasInst{
public:
	FreqMeasInst(){
		m_id = 0;
		m_hi_n = 0;
		m_lo_n = 0;
		m_hi_t = 0;
		m_dev = NULL;
	}
	~FreqMeasInst(){};
	
	void init_inst(std::shared_ptr<DeviceCom> dev, uint8_t id, uint32_t ts_cnt = 10000);
	void reset_inst(bool active = true);
  void toggle_reset_inst();
	void freeze_inst(bool active = true);
	void start_inst();
  uint32_t get_ts_cnt_inst();
	void set_ts_cnt_inst(uint32_t ts_cnt = 10000);
	void read_inst();
	
	const uint32_t& hi_n = m_hi_n;
	const uint32_t& lo_n = m_lo_n;
	const uint32_t& hi_t = m_hi_t;

private:
	uint8_t m_id;
	uint32_t m_hi_n;
	uint32_t m_lo_n;
	uint32_t m_hi_t;
	std::shared_ptr<DeviceCom> m_dev;
};

class FreqMeas{
public:
	FreqMeas(std::shared_ptr<DeviceCom> dev, uint32_t bClkHz = 100E6) : m_dev(dev), m_bClkHz(bClkHz){
		this->init();
	}
	~FreqMeas(){};
	
  void reset(FreqMeasInst FreqMeas::* ref, bool active = true);
	void reset(const std::string str, bool active = true);
  
  void toggle_reset(FreqMeasInst FreqMeas::* ref);
  void toggle_reset(const std::string str);
	void toggle_reset_all();
  
  void freeze(FreqMeasInst FreqMeas::* ref, bool active = true);
  void freeze(const std::string str, bool active = true);
	
  void start(FreqMeasInst FreqMeas::* ref);
	void start(const std::string str);
  
  void set_ts_cnt(FreqMeasInst FreqMeas::* ref, uint32_t ts_cnt);
  void set_ts_cnt(const std::string str, uint32_t ts_cnt);
	
  void read(FreqMeasInst FreqMeas::* ref);
  void read(const std::string str);
  
  uint32_t get_ts_cnt(FreqMeasInst FreqMeas::* ref);
	uint32_t get_ts_cnt(const std::string str);

  uint32_t get_hi_n(FreqMeasInst FreqMeas::* ref);
	uint32_t get_hi_n(const std::string str);

  uint32_t get_lo_n(FreqMeasInst FreqMeas::* ref);
  uint32_t get_lo_n(const std::string str);
		
  uint32_t get_hi_t(FreqMeasInst FreqMeas::* ref);
  uint32_t get_hi_t(const std::string str);
	
	float get_frq(FreqMeasInst FreqMeas::* ref);
	float get_frq(const std::string& str);
  
  float get_duty_cycle(FreqMeasInst FreqMeas::* ref);
  float get_duty_cycle(const std::string& str);
	
	std::map<std::string, FreqMeasInst FreqMeas::*> regMap; //To control them all directly
	
	FreqMeasInst HVOSC0;
	FreqMeasInst HVOSC1;
	FreqMeasInst HVOSC2;
	FreqMeasInst HVOSC3;
	FreqMeasInst CLKOUT;
	
private:
	void init();
	std::shared_ptr<DeviceCom> m_dev;
	uint32_t m_bClkHz;
};

#endif //FREQ_MEAS_H
