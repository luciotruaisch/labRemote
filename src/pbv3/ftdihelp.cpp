#include "ftdihelp.h"

#include <libftdi1/ftdi.h>

#include <unistd.h>

int32_t ftdi_read_alldata(struct ftdi_context *ftdi, std::vector<uint8_t>& data, uint32_t requested)
{
  data.resize(requested);
  uint32_t cnt=0;
  for(uint8_t tryIdx=0;tryIdx<10;tryIdx++)
    { 
      int32_t ret=ftdi_read_data(ftdi, &data[cnt], data.size()-cnt);
      if(ret<0)
	return ret;
      cnt+=ret;
      if(cnt==requested) return cnt;
      usleep(1e3);
    }

  return cnt;
}

