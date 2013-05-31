#include "StdAfx.h"
#include "warmgui.h"

namespace WARMGUI {

IDataCanvas::IDataCanvas(const char* name)
    : ICanvas(name)
    , _blind(0)
    //, _dc(0)
{
    setClass();
    *_my_cont = 0;
}


IDataCanvas::~IDataCanvas(void)
{
}


HRESULT IDataCanvas::Init()
{
    if (_bHasBackground) {
        _blind = new CBlind(_name, BGR(0, 0, 0), 0.5);
        AppendChild(_gt.begin(), _blind);
    }

    try {
        //_my_disp;
        char temp[MAX_PATH];
        _snprintf_s(temp, MAX_PATH, "%s.containers", this->_strconf);
        strcpy_s(_my_cont, MAX_PATH, _config->getString(temp).c_str());
    } catch(...) {
    }

    /**
    _chart = new CSeriesDataChart("price");
    InsertNext(_blind->GetGlyphTreeIter(), _chart);
    _chart->Init();


    //add price lines graph
    _snprintf_s(name, MAX_WARMGUI_NAME_LEN, _TRUNCATE, "graph-%s-price", _name);
    CDataLineGraph* graph = new CDataLineGraph(name, true, _chart);
    _chart->AddGraph(graph);
    graph->SetDrawType(CDataLineGraph::SDATA_GRAPH_LINE_TYPE_LINE);
    */

    return S_OK;
}


void IDataCanvas::SetGlyphRect()
{
	if (_blind)
		_blind->SetRect(_rect);

    //RECT tmp = {0, 0, RectWidth(_rect), RectHeight(_rect)};
    //MYTRACE(L"IDataCanvas::price %d %d %d %d\n", tmp.left, tmp.top, tmp.right, tmp.bottom);
}

GLYPH_CHANGED_TYPE IDataCanvas::NewData(IDataContainer* dataa_cont, DataObject::MARKET_DATA_TYPE /*datatype*/)
{
    GLYPH_CHANGED_TYPE changed = GLYPH_CHANGED_TYPE_NONE;
    return changed;
}



inline void IDataCanvas::SetWorldRect(WORLD_RECT& limit)
{
    //limit.maxx = (_rtcset._down_intval < 2) ? limit.maxx : limit.maxx * _rtcset._down_intval;
	//_cart.SetWorldRect(limit);
}

inline void IDataCanvas::SetWorldRect(float minx, float maxx, float miny, float maxy, float x0, float y0)
{
	//_cart.SetWorldRect(minx, maxx, miny, maxy, x0, y0);
}

} //namespace WARMGUI

