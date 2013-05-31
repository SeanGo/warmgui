#ifndef __warmgui_layout_h_include__
#define __warmgui_layout_h_include__

namespace WARMGUI {

class IAtelier;


//////////////////////////////////////////////////////////////////////////////////////////
// ILayout
/// Layout interface, give a Layout the configure of something, Layout will set the its position
class WARMGUI_API ILayout : public IObject
{
public:
	ILayout(void)
		: _m(0), _n(0), _margin(5), _region(0)
	{
		memset(&_rectFull, 0, sizeof(RECT));
	}

	ILayout(int m, int n, int margin, RECT& rect)
		: _m(m), _n(n), _margin(margin), _rectFull(rect)
	{
		_region = new RECT[m * n];
		memset(_region, 0, sizeof(RECT) * m * n);
	}
	
	virtual ~ILayout(void)
	{
		if (_region) delete _region;
	}

	void SetLayout(int m, int n, int margin, RECT& rect)
	{ 
		_m = m, _n = n, _margin = margin;
		_rectFull = rect;
		if (_region) delete _region;
		_region = new RECT[m * n];
		memset(_region, 0, sizeof(RECT) * m * n);
	}

	virtual void Disposal(IAtelier* atelier, RECT& rect) = 0;
    virtual void SetConfig(CWarmguiConfig* config, const char* conf_str)
                 {_config = config; strcpy_s(_cnf_str, MAX_PATH, conf_str);}
protected:
	int              _m, _n;
	int             _margin;    //left, r, t, b, 4 margins.
	RECT          _rectFull;
	RECT*           _region;
    CWarmguiConfig* _config;
    char _cnf_str[MAX_PATH];
private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("ILayout"); }
};



//////////////////////////////////////////////////////////////////////////////////////////
// COneLayout
class WARMGUI_API COneLayout : public ILayout
{
public:
	COneLayout();
	~COneLayout();

	virtual void Disposal(IAtelier* atelier, RECT& rect);

private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("COneLayout"); }
};




//////////////////////////////////////////////////////////////////////////////////////////
// ILayoutCreator
class WARMGUI_API ILayoutCreator
{
public:
    ILayoutCreator() {}
    ~ILayoutCreator() {}

    void SetConfigure(CWarmguiConfig* config) { _config = config; }
    virtual ILayout* CreateLayout(const char* layout_config) = 0;


protected:
    CWarmguiConfig* _config;
};

}//namespace WARMGUI

#endif //__warmgui_layout_h_include__