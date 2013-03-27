#include "StdAfx.h"
#include "warmgui.h"

namespace WARMGUI {

CRtDataCanvas::CRtDataCanvas(const TCHAR* name)
    : ICanvas(name)
    , _blind(0)
    , _chart(0)
{

}


CRtDataCanvas::~CRtDataCanvas(void)
{
}


HRESULT CRtDataCanvas::Init()
{
    TCHAR name[MAX_PATH];
    if (_bHasBackground) {
        _blind = new CBlind(_name, BGR(0, 0, 0), 0.5);
        AppendChild(_gt.begin(), _blind);
    }

    _chart = new CSeriesDataChart(L"price");
    InsertNext(_blind->GetGlyphTreeIter(), _chart);
    _chart->Init();

    //add price lines graph
    _sntprintf_s(name, MAX_WARMGUI_NAME_LEN, _TRUNCATE, L"graph-%s-price", _name);
    CDataLineGraph* graph = new CDataLineGraph(name, true, _chart);
    _chart->AddGraph(graph);
    graph->SetDrawType(CDataLineGraph::SDATA_GRAPH_LINE_TYPE_LINE);

    return S_OK;
}


void CRtDataCanvas::SetGlyphRect()
{
	if (_blind)
		_blind->SetRect(_rect);

    RECT tmp = {0, 0, RectWidth(_rect), RectHeight(_rect)};
    //MYTRACE(L"CRtDataCanvas::price %d %d %d %d\n", tmp.left, tmp.top, tmp.right, tmp.bottom);
    _chart->SetRect(tmp);

}

void CRtDataCanvas::setDataOffset(int pxo, int pyo)
{
    _chart->GetGraph()->SetDataOffset(1, pxo, pyo);
}

GLYPH_CHANGED_TYPE CRtDataCanvas::NewData(DataObjectPtr data)
{
    GLYPH_CHANGED_TYPE changed = GLYPH_CHANGED_TYPE_NONE;
    changed = (GLYPH_CHANGED_TYPE)((int)changed | (int)_chart->NewData(data));
    return changed;
}


void CRtDataCanvas::SetChatToDataContanier()
{
}

} //namespace WARMGUI

