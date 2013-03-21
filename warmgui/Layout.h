#ifndef __warmgui_layout_h_include__
#define __warmgui_layout_h_include__

namespace WARMGUI {

class CAtelier;

/// Layout interface, give a Layout the configure of something, Layout will set the its position
class WARMGUI_API ILayout
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

	virtual void Disposal(CAtelier* atelier, RECT& rect) = 0;

protected:
	int  _m, _n;
	int _margin;	//left, r, t, b, 4 margins.
	RECT _rectFull;
	RECT*  _region;

};

class WARMGUI_API COneLayout : public ILayout
{
public:
	COneLayout();
	~COneLayout();

	virtual void Disposal(CAtelier* atelier, RECT& rect);
};

}//namespace WARMGUI

#endif //__warmgui_layout_h_include__