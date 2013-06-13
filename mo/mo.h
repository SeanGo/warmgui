#ifndef __mo_robot_wall_e_h__
#define __mo_robot_wall_e_h__

//#include "pocoinc.h"


#include "utility.h"
#include "datatype.h"
#include "datastructio.h"
#include "ChineseCodeLib.h"
#include "MemShareFile.h"
#include "getopt.h"
#include "mylog.h"

#include "euclid_incs.h"


#include <Poco/Util/PropertyFileConfiguration.h>
#include <Poco/Util/XMLConfiguration.h>

class XMLConfig : public Poco::Util::XMLConfiguration {
public:
    XMLConfig() {}
    XMLConfig(const std::string xmlpath) : Poco::Util::XMLConfiguration(xmlpath) {}
    ~XMLConfig() {}


    WAVELET_TYPE getWaveletType(const char* config_str)
    {
        int type = 0;
	    try {
		    std::string str = getString(config_str);
            if (str == "SYM") {
                type = 1;
            } else if (str == "COI") {
                type = 2;
            }
	    } catch(Poco::NotFoundException&) {
		    type = 0;
	    }
	    return (WAVELET_TYPE)type;
    }
};

#endif //__mo_robot_wall_e_h__