#include "FileCalibration.h"

#include <cmath>
#include <fstream>

FileCalibration::FileCalibration(const std::string& path)
{
  std::ifstream fh(path);
  // Read header
  std::string dummy;
  std::getline(fh, dummy);
  
  double counts,value;
  while(!fh.eof())
    {
      fh >> counts >> value;
      m_counts.push_back(counts);
      m_values.push_back(value );
    }
}

FileCalibration::~FileCalibration()
{ }

double FileCalibration::calibrate(uint32_t counts)
{
  for(uint32_t i=0; i<m_counts.size()-1; i++)
    {
      if(m_counts[i]<=counts && counts<m_counts[i+1]) // Found interval
	return m_values[i]+(m_counts[i]-counts)/(m_counts[i]-m_counts[i+1])*(m_values[i+1]-m_values[i]);
    }
  return 0.;
}
  
uint32_t FileCalibration::uncalibrate(double value)
{
  for(uint32_t i=0; i<m_values.size()-1; i++)
    {
      if(m_values[i]<=value && value<m_values[i+1]) // Found interval
	return m_counts[i]+(m_values[i]-value)/(m_values[i]-m_values[i+1])*(m_counts[i+1]-m_counts[i]);
    }
  return 0;
}
