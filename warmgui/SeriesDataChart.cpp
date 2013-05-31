#include "StdAfx.h"
#include "warmgui.h"


namespace WARMGUI {
CSeriesDataChart::CSeriesDataChart(const char* name)
    : IDataGraph(name)
    , _coord(0)
{
    setClass();
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

inline void CSeriesDataChart::SetWorldRect(WORLD_RECT& limit)
{
    limit.xn = (_rtcset._down_intval < 2) ? limit.xn : limit.xn * _rtcset._down_intval;
	_cart.SetWorldRect(limit);
}


void CSeriesDataChart::SetGlyphRect(RECT& cartrect)
{
    //rectGraph.left 

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
    char key[MAX_PATH];

    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.increase", _strconf, _name);
    _config->getValueIncrease(_rtcset._vi, key);

    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.rulersize", _strconf, _name);
    _config->getRulerWidth(_rtcset._rule_size, key);

    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.spacelen", _strconf, _name);
    _rtcset._space_len = _config->getInt(key);

    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.prdtlen", _strconf, _name);
    _rtcset._predict_len = _config->getInt(key);

    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.down-sampler-interval", _strconf, _name);
    _rtcset._down_intval = _config->getInt(key);

    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.stroke-width", _strconf, _name);
    _rtcset._stroke_width = (float)(_config->getDouble(key));

    _snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.world", _strconf, _name);
    _config->getWorldRect(_rtcset._limit, key);
    //add the additional length to limit
    _rtcset._limit.xn += (_rtcset._predict_len + _rtcset._space_len);
    //must set the world after got down-sampler-interval
    SetWorldRect(_rtcset._limit);
}


HRESULT CSeriesDataChart::Init()
{
    GetChartSettings();

    char name[MAX_WARMGUI_NAME_LEN];

	_snprintf_s(name, MAX_WARMGUI_NAME_LEN, _TRUNCATE, "coord-%s", _name);
    _coord = new CCoordGrid(name);
    _canvas->AppendChild(_iter, _coord);
    //_coord->SetWorldRect(&(_cart.GetWorldRect()));
    _coord->SetSideBarWidth(_rtcset._rule_size);

    return S_OK;
}

HRESULT CSeriesDataChart::AddGraph(IGlyph* g, bool bDataLine/* = true*/)
{
    IGlyph* graph = _canvas->AppendChild(_iter, g);
    if (!graph) return (-1);
    //g->SetCartesian(&_cart);
    //if (bDataLine)
    //    ((CDataLineGraph*)g)->SetRtChartSettings(&_rtcset);

    return S_OK;
}

GLYPH_CHANGED_TYPE CSeriesDataChart::NewData(DataObjectPtr dop)
{
    GLYPH_CHANGED_TYPE changed = GLYPH_CHANGED_TYPE_NONE;
    int num = _canvas->getChildNumber(_iter);
    if (2 < num) {
        for (int i = 2; i < num; i++) {
            //if (_canvas->getChild(_iter, i)->AppendData(dop) & GLYPH_CHANGED_TYPE_COORDFRAME)
            //if (_canvas->getChild(_iter, i)->AddNewData() & GLYPH_CHANGED_TYPE_COORDFRAME)
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