#include "StdAfx.h"
#include "qks_incs.h"


CZenInTwiningCanvas::CZenInTwiningCanvas(const char* name)
    : IDataCanvas(name)
    , _blind(0)
    , _chart(0)
    ,  _zitg(0)
{
    setClass();
}


CZenInTwiningCanvas::~CZenInTwiningCanvas(void)
{
}


const HRESULT CZenInTwiningCanvas::Init(const char* name/* = 0*/)
{
    char temp[MAX_PATH];
    _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.blind", _strconf);
    _bHasBackground = _config->getBool(temp);

    if (_bHasBackground) {
        _blind = new CBlind(_name, BGR(0, 0, 0), 0.5);
        AppendChild(_gt.begin(), _blind);
    }

    _chart = new CSeriesDataChart("price");
    InsertNext(_blind->GetGlyphTreeIter(), _chart);
    _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.rtchart-price", _strconf);
    _chart->setConfig(_config, temp);
    _chart->Init();

    //add price lines graph
    _snprintf_s(temp, MAX_WARMGUI_NAME_LEN, _TRUNCATE, "graph-%s-price", _name);
    CDataLineGraph* graph = new CDataLineGraph(temp, true);
    _chart->AddGraph(graph);
    graph->SetDrawType(CDataLineGraph::SDATA_GRAPH_LINE_TYPE_LINE);

    _zitg = new CZenInTwiningGraph("zenintwining");
    _zitg->SetDownSample(_chart->_rtcset._down_intval);
    _chart->AddGraph(_zitg, false);

    return S_OK;
}


void CZenInTwiningCanvas::SetGlyphRect()
{
	if (_blind)
		_blind->SetRect(_rect);

    RECT tmp = {0, 0, RectWidth(_rect), RectHeight(_rect)};
    //MYTRACE(L"CZenInTwiningCanvas::price %d %d %d %d\n", tmp.left, tmp.top, tmp.right, tmp.bottom);
    _chart->SetRect(tmp);

}

void CZenInTwiningCanvas::setDataOffset(int pxo, int pyo)
{
    _chart->GetGraph()->SetDataOffset(pxo, pyo);
}

GLYPH_CHANGED_TYPE CZenInTwiningCanvas::NewData(DataObjectPtr data)
{
    GLYPH_CHANGED_TYPE changed = GLYPH_CHANGED_TYPE_NONE;
    changed = (GLYPH_CHANGED_TYPE)((int)changed | (int)_chart->NewData(data));
    return changed;
}


void CZenInTwiningCanvas::SetChatToDataContanier()
{
}

