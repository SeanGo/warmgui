#include "StdAfx.h"
#include "warmgui.h"


namespace WARMGUI {
/*
CSeriesDataChart::CSeriesDataChart(void)
    : _blind(0)
    , _coord(0)
    , _graph(0)
    , _datalen(0)
{
    memset(&_rtcset._rule_size, 0, sizeof(RECT));
}
*/

CSeriesDataChart::CSeriesDataChart(const TCHAR* name)
    : IGlyph(name)
    , _blind(0)
    , _coord(0)
{
    memset(&_rtcset, 0, sizeof(RtChartSettings));
}


CSeriesDataChart::~CSeriesDataChart(void)
{
}



void CSeriesDataChart::SetRect(RECT& rect)
{
    _changed_type = GLYPH_CHANGED_TYPE_GLYPH_RESIZE, _rect = rect;

	RECT cartRect = {_rect.left + _rtcset._rule_size.left,
					 _rect.top  + _rtcset._rule_size.top,
                     _rect.right  - _rtcset._rule_size.right,
                     _rect.bottom - _rtcset._rule_size.bottom};

	_cart.SetRectSize(cartRect);
	SetGlyphRect(cartRect);
}

inline void CSeriesDataChart::SetWorldRect(LIMIT_2D& limit)
{
    limit.maxx = (_rtcset._down_intval < 2) ? limit.maxx : limit.maxx * _rtcset._down_intval;
	_cart.SetWorldRect(limit);
}

inline void CSeriesDataChart::SetWorldRect(float minx, float maxx, float miny, float maxy, float x0, float y0)
{
	_cart.SetWorldRect(minx, maxx, miny, maxy, x0, y0);
}


void CSeriesDataChart::SetGlyphRect(RECT& cartrect)
{
    //rectGraph.left 

    //MYTRACE(L"Chart Rect %s %d %d %d %d\n", _blind->_name, _rect.left, _rect.top, _rect.right, _rect.bottom);
    if (_blind)
        _blind->SetRect(_rect);

    //MYTRACE(L"Chart Rect %s %d %d %d %d\n", _coord->_name, _rect.left, _rect.top, _rect.right, _rect.bottom);
    if (_coord)
		_coord->SetRect(_rect);

    int num = _canvas->getChildNumber(_iter);
    for(int i = 2; i < num; ++i) {
        IGlyph* g = _canvas->getChild(_iter, i);
        g->SetRect(cartrect);
        //MYTRACE(L"data Chart %s %d %d %d %d\n", g->_name, cartrect.left, cartrect.top, cartrect.right, cartrect.bottom);
    }
}


void CSeriesDataChart::GetChartSettings()
{
    char key[MAX_PATH] ,cname[MAX_PATH], canvasname[MAX_PATH];
    CChineseCodeLib::UnicodeToGB2312(cname, MAX_PATH, _name);
    CChineseCodeLib::UnicodeToGB2312(canvasname, MAX_PATH, _canvas->_name);

    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.rtchart-%s.increase", canvasname, cname);
    _config->getValueIncrease(_rtcset._vi, key);

    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.rtchart-%s.rulersize", canvasname, cname);
    _config->getRulerWidth(_rtcset._rule_size, key);

    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.rtchart-%s.spacelen", canvasname, cname);
    _rtcset._space_len = _config->getInt(key);

    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.rtchart-%s.prdtlen", canvasname, cname);
    _rtcset._predict_len = _config->getInt(key);

    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.rtchart-%s.down-sampler-interval", canvasname, cname);
    _rtcset._down_intval = _config->getInt(key);

    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.rtchart-%s.stroke-width", canvasname, cname);
    _rtcset._stroke_width = (float)(_config->getDouble(key));

    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.rtchart-%s.world", canvasname, cname);
    _config->getWorldRect(_rtcset._limit, key);
    //add the additional length to limit
    _rtcset._limit.maxx += (_rtcset._predict_len + _rtcset._space_len);
    //must set the world after got down-sampler-interval
    SetWorldRect(_rtcset._limit);
}


HRESULT CSeriesDataChart::Init()
{
    GetChartSettings();

    TCHAR name[MAX_WARMGUI_NAME_LEN];
    _blind = new CBlind(_name, BGR(0, 0, 0), 0);
    _canvas->AppendChild(_iter, _blind);

	_sntprintf_s(name, MAX_WARMGUI_NAME_LEN, _TRUNCATE, L"coord-%s", _name);
    _coord = new CCoordGrid(name);
    _canvas->InsertNext(_blind->GetGlyphTreeIter(), _coord);
    _coord->SetWorldRect(&(_cart.GetWorldRect()));
    _coord->SetSideBarWidth(_rtcset._rule_size);

    return S_OK;
}

HRESULT CSeriesDataChart::AddGraph(IGlyph* g, bool bDataLine/* = true*/)
{
    IGlyph* graph = _canvas->AppendChild(_iter, g);
    if (!graph) return (-1);
    g->SetCartesian(&_cart);
    if (bDataLine)
        ((CDataLineGraph*)g)->SetRtChartSettings(&_rtcset);

    return S_OK;
}

GLYPH_CHANGED_TYPE CSeriesDataChart::NewData(DataObjectPtr dop)
{
    GLYPH_CHANGED_TYPE changed = GLYPH_CHANGED_TYPE_NONE;
    int num = _canvas->getChildNumber(_iter);
    if (2 < num) {
        for (int i = 2; i < num; i++) {
            if (_canvas->getChild(_iter, i)->AppendData(dop) & GLYPH_CHANGED_TYPE_COORDFRAME)
                _coord->PreDraw();
        }
    }

    return changed;
}

CDataLineGraph* CSeriesDataChart::GetGraph()
{
    //GLYPH_CHANGED_TYPE changed = GLYPH_CHANGED_TYPE_NONE;
    int num = _canvas->getChildNumber(_iter);
    if (2 < num) {
        CDataLineGraph* g = (CDataLineGraph*)_canvas->getChild(_iter, 2);
        return g;
    }

    return (0);
}

void CSeriesDataChart::SetGeometryData(dataptr pdata, int count, int datasize)
{
    int ndown = (_rtcset._down_intval < 2) ? 1 : _rtcset._down_intval;
    int mycount = (int)((float)count / (float)ndown) + 1;
    if (mycount < 2) return;

    char* p = (char*)pdata;
    GetGraph()->BeginSetData(p);

    p += datasize * ndown;
    int i = ndown;
    for (; i < count; i += ndown, p += datasize * ndown) {
        GetGraph()->AddDataToPathGeometry(p);
    }

    if ((i - ndown) != count - 1)
        GetGraph()->AddDataToPathGeometry((char*)pdata + (count - 1) * datasize);

    GetGraph()->EndSetData();
}


}//namespace WARMGUI