// Tektronix programmer manual can be found at https://www.tek.com/manual/tds-family-programmer-manual

#include "Tektronix654C.h"
#include "Logger.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <unistd.h>


loglevel_e logleveltek;

Tektronix654C::Tektronix654C(std::string dev, unsigned addr) 
{
    m_com = new SerialCom(dev, B9600);
    m_addr = addr;
    m_com->write("++auto 0\n\r");
}

Tektronix654C::~Tektronix654C() 
{
    delete m_com;
}

void Tektronix654C::send(std::string cmd) {
    m_com->write("++addr " + std::to_string(m_addr) + "\n\r");
    logger(logDEBUG2) << __PRETTY_FUNCTION__ << " -> Sending: " << cmd;
    cmd += "\r\n";
    m_com->write(cmd);
    std::this_thread::sleep_for(std::chrono::milliseconds(m_wait));
}

std::string Tektronix654C::receive(std::string cmd) {
    m_com->write("++addr " + std::to_string(m_addr) + "\n\r");
    logger(logDEBUG2) << __PRETTY_FUNCTION__ << " -> Sending: " << cmd;
    cmd += "\r\n";
    m_com->write(cmd);
    m_com->write("++read eoi\n\r");
    std::this_thread::sleep_for(std::chrono::milliseconds(m_wait));
    std::string buf;
    m_com->read(buf);
    logger(logDEBUG2) << __PRETTY_FUNCTION__ << " -> Received: " << buf;
    return buf;
}

std::string Tektronix654C::init() {
     this->send("VERBOSE ON"); 
     this->send("DATA:WIDTH 2"); // sets data width to 16bit precision DATA:WIDTH { 1 (8 bit)| 2 (16 bit) }
     return this->receive("*IDN?");
     return this->receive("DATa:SOUrce?");		
}

//reads the time_base
std::string Tektronix654C::timeBase(){
  return this->receive("HORizontal:MAIn:SCAle?");
}

//retrieves channel vertical position of the "zero voltage baseline" in divisions (could be pos or neg value)
std::string Tektronix654C::verticalPos(std::string ch) {
  return this->receive("CH"+ ch + ":POSition?");
}

//retrieves volts/division ([V]/div)
std::string Tektronix654C::voltDiv(std::string ch) {
 return this->receive("CH"+ ch + ":VOLts?");
}

//reads waveform data as string
std::string Tektronix654C::read() 
{
  return this->receive("CURVE?").substr(0, std::string::npos); //std::string::npos (until the end of string) retrieves data all at once
}

//reads waveform data as string one character at a time
std::string Tektronix654C::read1char(int n) 
{
  return this->receive("CURVE?").substr(n, 1);
}

//reads waveform data (not string) and displays it to terminal 
std::string Tektronix654C::curve() {
  return this->receive("CURVE?");	
}

//sets oscilloscope channel to read from
void Tektronix654C::setCh(std::string ch) {
    this->send("DATA:SOURCE CH" + ch);
}

//sets oscilloscope reference to read from
void Tektronix654C::setRef(std::string ref) {
    this->send("DATA:SOURCE REF" + ref);
}

//resets waves on scope screen
void Tektronix654C::rstWave(){
    this->send("TRIGGER:MAIN:MODE AUTO");
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    this->send("TRIGger:MAIn:MODe NORMal");
}

//Sets the format of the waveform data { ASCIi | RIBinary | RPBinary | SRIbinary | SRPbinary}
void Tektronix654C::setData(std::string data) {
    this->send("DATA:ENCDG " + data);
}

//Stores a waveform from CH<x> in one of four reference memory locations ( oscilloscipe Ref1, Ref2, Ref3, Ref4 )
void Tektronix654C::saveWaveToRef(std::string ch, std::string ref) {
    this->send("SAVe:WAVEform CH" + ch +",REF" + ref);
}

//sets port of screenshot (picture) to GPIB
void Tektronix654C::hardCopyPort() {
    this->send("HARDCopy:PORT FILE");
}

//sets picture type
void Tektronix654C::hardCopyFormat() {
 this->send("HARDCopy:PALEtte HARDCOPY");
this->send("HARDCopy:FORMat BMPCOLOR");
}



