#ifndef LOGGER_H
#define LOGGER_H

// ####################
// Lightweight logger
// Author: Timon Heim
// Date: Feb 2017
// Notes: 
// ###################

#include <iostream>
#include <sstream>

/* consider adding boost thread id since we'll want to know whose writting and
 * won't want to repeat it for every single call */

/* consider adding policy class to allow users to redirect logging to specific
 * files via the command line
 */

enum loglevel_e
{logERROR=0, logWARNING=1, logINFO=2, logDEBUG=3, logDEBUG1=4, logDEBUG2=5, logDEBUG3=6, logDEBUG4=7};

class logIt {
    public:
        logIt(loglevel_e _loglevel = logERROR) {
            _buffer << logString[_loglevel];
        }

        template <typename T>
            logIt & operator<<(T const & value)
            {
                _buffer << value;
                return *this;
            }

        ~logIt()
        {
            _buffer << std::endl;
            // This is atomic according to the POSIX standard
            // http://www.gnu.org/s/libc/manual/html_node/Streams-and-Threads.html
            std::cerr << _buffer.str();
        }

    private:
        std::ostringstream _buffer;
        const std::string logString[8] = {
            "\033[1;31m[ERROR]   : \033[0m",
            "\033[1;33m[WARNING] : \033[0m",
            "\033[1;32m[INFO]    : \033[0m",
            "\033[1;34m[DEBUG]   : \033[0m",
            "\033[1;34m[DEBUG1]  : \033[0m" ,
            "\033[1;34m[DEBUG2]  : \033[0m" ,
            "\033[1;34m[DEBUG3]  : \033[0m" ,
            "\033[1;34m[DEBUG4]  : \033[0m" };

};

extern loglevel_e loglevel;

#define log(level) \
    if (level > loglevel) ; \
    else logIt(level)

#endif
