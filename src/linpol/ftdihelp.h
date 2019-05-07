#ifndef FTDIHELP_H
#define FTDIHELP_H

#include <vector>
#include <cstdint>

// read all requested data
int32_t ftdi_read_alldata(struct ftdi_context* ftdi, std::vector<uint8_t>& data, uint32_t requested);

#endif // FTDIHELP_H

