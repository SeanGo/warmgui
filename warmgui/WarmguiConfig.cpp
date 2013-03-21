#include "StdAfx.h"
#include "warmgui_incs.h"

namespace WARMGUI {
///@brife retrive a integer from string splited by p and return next position of number
///       "123, 456" will set value is 123 and return pointer to the p
///       the param buf must be writeble!
static char* get_one_int(char* buf, char p, long* value)
{
	if (buf && *buf && value) {
		char* ptr = strchr(buf, p);
		if (ptr) {
			char c = *ptr;	//backup current char
			*ptr = 0;
			*value = atoi(buf);
			*ptr = c;		//get the char back
			return ptr;	//return pointer to next number
		}
	}
	return (0);
}

static char* get_one_float(char* buf, char p, float* value)
{
	if (buf && *buf && value) {
		char* ptr = strchr(buf, p);
		if (ptr) {
			char c = *ptr;	//backup current char
			*ptr = 0;
			*value = (float)atof(buf);
			*ptr = c;		//get the char back
			return ptr;	//return pointer to next number
		}
	}
	return (0);
}




//////////////////////////////////////////////////////////////////////////////////////////////////
// CWarmguiConfig
CWarmguiConfig::CWarmguiConfig(void)
{
}

CWarmguiConfig::~CWarmguiConfig()
{
}


bool CWarmguiConfig::getRect(RECT& rect, const char* config_str)
{
	bool r = false;
	try {
		std::string str = getString(config_str);
		char* buf = strdup(str.c_str());
		char* tmp = get_one_int(buf, ',', &rect.left);
		if (tmp) {
			tmp = get_one_int(++tmp, ',', &rect.top);
			if (tmp) {
				tmp = get_one_int(++tmp, ',', &rect.right);
				if (tmp) {
					rect.bottom = atoi(++tmp);
					r = true;
				}
			}
		}
		delete buf;
	} catch(Poco::NotFoundException&) {
		return r;
	}
	return r;
}

bool CWarmguiConfig::getRulerWidth(RULER_WIDTH& rw, const char* config_str)
{
	bool r = false;
	try {
		std::string str = getString(config_str);
		char* buf = strdup(str.c_str());
		char* tmp = get_one_int(buf, ',', &rw.left);
		if (tmp) {
			tmp = get_one_int(++tmp, ',', &rw.top);
			if (tmp) {
				tmp = get_one_int(++tmp, ',', &rw.right);
				if (tmp) {
					rw.bottom = atoi(++tmp);
					r = true;
				}
			}
		}
		delete buf;
	} catch(Poco::NotFoundException&) {
		return r;
	}
	return r;
}


bool CWarmguiConfig::getWorldRect(LIMIT_2D& limit, const char* config_str)
{
	bool r = false;
	try {
		std::string str = getString(config_str);
		char *buf = strdup(str.c_str());
		char* tmp = get_one_float(buf, ',', &limit.minx);
		if (tmp) {
			tmp = get_one_float(++tmp, ',', &limit.maxx);
			if (tmp) {
				tmp = get_one_float(++tmp, ',', &limit.miny);
				if (tmp) {
					tmp = get_one_float(++tmp, ',', &limit.maxy);
					if (tmp) {
						tmp = get_one_float(++tmp, ',', &limit.x0);
						if (tmp) {
							limit.y0 = (float)atof(++tmp);
							r = true;
						}
					}
				}
			}
		}
		delete buf;
	} catch(Poco::NotFoundException&) {
		return r;
	}
	return r;
}

bool CWarmguiConfig::getValueIncrease(ValueIncrease& increase, const char* config_str)
{
	bool r = false;
	try {
		std::string str = getString(config_str);
		char *buf = strdup(str.c_str());
        char* tmp = get_one_float(buf, ',', &increase._init_y0_S_mag);
		if (tmp) {
            tmp = get_one_float(++tmp, ',', &increase._init_y0_L_mag);
			if (tmp) {
                increase._breadth_type = (DATA_BREADTH_TYPE)atoi(++tmp);
                r = true;
			}
		}
		delete buf;
	} catch(Poco::NotFoundException&) {
		return r;
	}
	return r;
}

bool CWarmguiConfig::getSize(SIZE& size, const char* config_str)
{
	bool r = false;
	try {
		std::string str = getString(config_str);
		char *buf = strdup(str.c_str());
		char* tmp = get_one_int(buf, ',', &size.cx);
		if (tmp) {
			size.cy = atoi(++tmp);
            r = true;
		}
		delete buf;
	} catch(Poco::NotFoundException&) {
		return r;
	}
	return r;
}

bool CWarmguiConfig::getToolbarPos(TOOLBAR_POSITION& pos, const char* config_str)
{
	try {
		std::string pos = getString(config_str);
		if (pos == "top")
			pos = TOOLBAR_POSITION_TOP;
		else if (pos == "left")
			pos = TOOLBAR_POSITION_LEFT;
		else if (pos == "right")
			pos = TOOLBAR_POSITION_RIGHT;
		else if (pos == "bottom")
			pos = TOOLBAR_POSITION_BOTTOM;
		return true;
	} catch(Poco::NotFoundException&) {
		return false;
	}
}

}

