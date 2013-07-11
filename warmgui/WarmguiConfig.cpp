#include "StdAfx.h"
#include "warmgui_summer.h"

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

static bool atob(const char* str)
{
    if (strstr(str, "true"))
        return true;
    else if (strstr(str, "false"))
        return false;

    return false;
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
        MYTRACE(L"CWarmguiConfig::getRect\n");
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
        MYTRACE(L"CWarmguiConfig::getRulerWidth\n");
		return r;
	}
	return r;
}


bool CWarmguiConfig::getWorldRect(WORLD_RECT& world, const char* config_str)
{
	bool r = false;
	try {
		std::string str = getString(config_str);
		char *buf = strdup(str.c_str());
        float value;
    	//      x0,   y0,    xn,   yn, minpos, miny, maxpos, maxy
    	//<world>0, 3000, 32400, 3000,      0, 2985,  32400, 3015</world>
		char* tmp = get_one_float(buf, ',', &value);
        world.x0 = value; //X0
		if (tmp) {
			tmp = get_one_float(++tmp, ',', &value);
            world.y0 = value; //Y0
			if (tmp) {
				tmp = get_one_float(++tmp, ',', &value);
                world.xn = value;  //Xn
				if (tmp) {
					tmp = get_one_float(++tmp, ',', &value);
                    world.yn = value;  //Yn
					if (tmp) {
						tmp = get_one_float(++tmp, ',', &value);
                        world.minpos = value;  //MINPOS
						if (tmp) {
						    tmp = get_one_float(++tmp, ',', &value);
                            world.miny = value;  //MINY
						    if (tmp) {
						        tmp = get_one_float(++tmp, ',', &value);
                                world.maxpos = value;  //MAXPOS
						        if (tmp) {
						            world.maxy = (float)atof(++tmp);  //MAXY
						            r = true;
                                }
                            }
						}
					}
				}
			}
		}
		delete buf;
	} catch(Poco::NotFoundException&) {
        MYTRACE(L"CWarmguiConfig::getWorldRect\n");
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
        char* tmp = get_one_float(buf, ',', &increase._min_decres_mag);
		if (tmp) {
            tmp = get_one_float(++tmp, ',', &increase._max_incres_mag);
			if (tmp) {
                char* t = strstr(tmp + 1, "DATA_BREADTH_TYPE_VALUE");
                {//get breadth type
                    if (t) {
                        increase._breadth_type = DATA_BREADTH_TYPE_VALUE;
                        tmp = t + strlen("DATA_BREADTH_TYPE_VALUE");
                        goto get_fixed_type;
                    }
                    t = strstr(tmp + 1, "DATA_BREADTH_TYPE_PERCENT");
                    if (t) {
                        increase._breadth_type = DATA_BREADTH_TYPE_PERCENT;
                        tmp = t + strlen("DATA_BREADTH_TYPE_PERCENT");
                        goto get_fixed_type;
                    }

                    goto will_exit;
                }
get_fixed_type:
                {
                    t = strchr(tmp, ',');
                    if (t) {
                        tmp = t + 1;
                        float value;
                        tmp = get_one_float(tmp, ',', &value);
                        increase._init_width = value;
                        if (tmp) {
                            tmp = get_one_float(tmp + 1, ',', &value);
                            increase._hold_right_space = value;
                            if (tmp) {
                                increase._b_fix_width = atob(tmp + 1);
                                r = true;
                            }
                        }
                    }
                }

			}
		}
will_exit:
		delete buf;
	} catch(Poco::NotFoundException&) {
        MYTRACE(L"CWarmguiConfig::getValueIncrease\n");
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
        MYTRACE(L"CWarmguiConfig::getSize\n");
		return r;
	}
	return r;
}

bool CWarmguiConfig::getToolbarPos(TOOLBAR_POSITION& pos, const char* config_str)
{
	try {
		std::string strpos = getString(config_str);
		if (strpos == "top")
			pos = TOOLBAR_POSITION_TOP;
		else if (strpos == "left")
			pos = TOOLBAR_POSITION_LEFT;
		else if (strpos == "right")
			pos = TOOLBAR_POSITION_RIGHT;
		else if (strpos == "bottom")
			pos = TOOLBAR_POSITION_BOTTOM;
        else if (strpos == "middle")
            pos = TOOLBAR_POSITION_MIDDLE;
		return true;
	} catch(Poco::NotFoundException&) {
        MYTRACE(L"CWarmguiConfig::getToolbarPos\n");
		return false;
	}
}


bool CWarmguiConfig::getColorAlpha(COLORALPHA   &color, const char* config_str)
{
	bool r = false;
	try {
		std::string str = getString(config_str);
		char *buf = strdup(str.c_str());
        char* tmp = get_one_float(buf, ',', &color.b);
		if (tmp) {
            tmp = get_one_float(++tmp, ',', &color.g);
			if (tmp) {
                tmp = get_one_float(++tmp, ',', &color.r);
				if (tmp) {
                    color.a = (float)atof(++tmp);
                    r = true;
                }
			}
		}
		delete buf;
	} catch(Poco::NotFoundException&) {
        MYTRACE(L"CWarmguiConfig::getColorAlpha\n");
		return r;
	}
    color.b /= 255, color.g /= 255, color.r /= 255;
	return r;
}

bool CWarmguiConfig::getTripleFloat(TripleTuple &tt, const char* config_str)
{
	bool r = false;
	try {
		std::string str = getString(config_str);
		char *buf = strdup(str.c_str());
        char* tmp = get_one_float(buf, ',', &tt.f1);
		if (tmp) {
            tmp = get_one_float(++tmp, ',', &tt.f2);
			if (tmp) {
                tt.f3 = (float)atof(++tmp);
                r = true;
            }
		}
		delete buf;
	} catch(Poco::NotFoundException&) {
        MYTRACE(L"CWarmguiConfig::getTripleFloat\n");
		return r;
	}
	return r;
}


bool CWarmguiConfig::getFontSetting(FONT& font, const char* config_str)
{
    bool r = false;
	try {
		std::string str = getString(config_str);
		char *buf = strdup(str.c_str());
        char* tmp = strchr(buf, ',');
        if (tmp) {
            font.fontSize = (float)atof(tmp+1);
            *tmp = 0;
            CChineseCodeLib::Gb2312ToUnicode(font.fontName, sizeof(font.fontName), buf);
            *tmp = ',';
            r = true;
        } else
            r = false;
		delete buf;
	} catch(Poco::NotFoundException&) {
        MYTRACE(L"CWarmguiConfig::getFontSetting\n");
		return r;
	}
    return r;
}


bool CWarmguiConfig::getStringVector(StringArray& strs, const char* config_str)
{
    bool r = false;
	try {
		std::string str = getString(config_str);
        char* buf = strdup(str.c_str());
        char* t1  = buf;
        while(t1 && *t1) {
            if (isspace(*t1)) {
                t1++;
                continue;
            }

            char* t2  = strchr(t1, ',');
            if (t2) {
                *t2 = 0;
                strs.push_back(t1);
                t1 = t2 + 1;
            } else {
                strs.push_back(t1);
                break;
            }
        }
        free(buf);
	} catch(Poco::NotFoundException&) {
        MYTRACE(L"CWarmguiConfig::getStringVector\n");
		return r;
	}
    return r;
}

bool CWarmguiConfig::getStringVector(WStringArray& strs, const char* config_str)
{
    bool r = false;
	try {
		std::string str = getString(config_str);
        char* buf = strdup(str.c_str());
        size_t len = 2 * strlen(buf);
        WCHAR* wstr = new WCHAR[len];
        char* t1  = buf;
        while(t1 && *t1) {
            if (isspace(*t1)) {
                t1++;
                continue;
            }

            char* t2  = strchr(t1, ',');
            if (t2) {
                *t2 = 0;
                CChineseCodeLib::Gb2312ToUnicode(wstr, len, t1);
                strs.push_back(wstr);
                t1 = t2 + 1;
            } else {
                CChineseCodeLib::Gb2312ToUnicode(wstr, len, t1);
                strs.push_back(wstr);
                break;
            }
        }
        delete wstr;
        free(buf);
	} catch(Poco::NotFoundException&) {
        MYTRACE(L"CWarmguiConfig::getStringVector\n");
		return r;
	}
    return r;
}


int CWarmguiConfig::getWaveletType(const char* config_str)
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
        MYTRACE(L"CWarmguiConfig::getWaveletType\n");
		type = 0;
	}
	return type;
}

}

