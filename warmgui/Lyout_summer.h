#ifndef __interface_warmgui_summer_lyout__
#define __interface_warmgui_summer_lyout__

namespace WARMGUI {

//////////////////////////////////////////////////////////////////////////////////////////
// ILayout_summer
/// Layout interface, give a Layout the configure of something, Layout will set the its position
class WARMGUI_API ILayout_summer : public IObject
{
public:
	ILayout_summer(void)
	{
	}

	
	virtual ~ILayout_summer(void)
	{
	}

	virtual void Disposal(IAtelier_summer* atelier, RECT& rect) = 0;
    virtual void SetConfig(CWarmguiConfig* config, const char* conf_str)
                 {_config = config; strcpy_s(_cnf_str, MAX_PATH, conf_str);}
protected:

    CWarmguiConfig* _config;
    char _cnf_str[MAX_PATH];
private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("ILayout_summer"); }
};

//////////////////////////////////////////////////////////////////////////////////////////
// COneLayout
class WARMGUI_API COneLayout_summer : public ILayout_summer
{
public:
	COneLayout_summer();
	~COneLayout_summer();

	virtual void Disposal(IAtelier_summer* atelier, RECT& rect);

private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("COneLayout"); }
};



} //namespace WARMGUI

#endif //__interface_warmgui_summer_lyout__