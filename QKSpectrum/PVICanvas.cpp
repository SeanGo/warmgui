#include "StdAfx.h"
#include "qks_incs.h"


CPVICanvas::CPVICanvas(const char* name)
    : IDataCanvas(name)
    ,    _chart_price(0)
    ,   _chart_volume(0)
    , _chart_interest(0)
{
    setClass();
}

CPVICanvas::~CPVICanvas(void)
{
}

const HRESULT CPVICanvas::Init(const char* name/*=0*/)
{
    char temp[MAX_PATH];
    _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.blind", _strconf);
    _bHasBackground = _config->getBool(temp);

    if (_bHasBackground) {
        _blind = new CBlind(_name, BGR(0, 0, 0), 0.5);
        AppendChild(_gt.begin(), _blind);
    }

    // add data chart "price"
    AddDataChart("price", &_chart_price);
    AddDataChart("volume", &_chart_volume);
    AddDataChart("interest", &_chart_interest);


	CTPMMD ctpdata;
	setDataOffset(
        (int)(&ctpdata.fIndex) - (int)&ctpdata, (int)(&ctpdata.LastPrice) - (int)&ctpdata,
	    (int)(&ctpdata.fIndex) - (int)&ctpdata, (int)(&ctpdata.Volume) - (int)&ctpdata,
	    (int)(&ctpdata.fIndex) - (int)&ctpdata, (int)(&ctpdata.OpenInterest) - (int)&ctpdata);

    return S_OK;
}

void CPVICanvas::setDataOffset(int pxo, int pyo, int vxo, int vyo, int ixo, int iyo)
{
    _chart_price->GetGraph()->SetDataOffset(pxo, pyo);
    _chart_volume->GetGraph()->SetDataOffset(vxo, vyo);
    _chart_interest->GetGraph()->SetDataOffset(ixo, iyo);
}


void CPVICanvas::SetGlyphRect()
{
	if (_blind)
		_blind->SetRect(_rect);

    RECT tmp = {0, 0, RectWidth(_rect), (long)(fRectHeight(_rect) * .618)};
    //MYTRACE("CPVICanvas::price %d %d %d %d\n", tmp.left, tmp.top, tmp.right, tmp.bottom);
    _chart_price->SetRect(tmp);

    tmp.top = (long)(fRectHeight(_rect) * .618), tmp.bottom = (long)(fRectHeight(_rect) * (.618 + (1 - .618) * .5));
    //MYTRACE("CPVICanvas::volume %d %d %d %d\n", tmp.left, tmp.top, tmp.right, tmp.bottom);
    _chart_volume->SetRect(tmp);

    tmp.top = (long)(fRectHeight(_rect) * (.618 + (1 - .618) * .5)), tmp.bottom = RectHeight(_rect);
    //MYTRACE("CPVICanvas::interest %d %d %d %d\n", tmp.left, tmp.top, tmp.right, tmp.bottom);
    _chart_interest->SetRect(tmp);
}

GLYPH_CHANGED_TYPE CPVICanvas::NewData(DataObjectPtr data)
{
    GLYPH_CHANGED_TYPE changed = GLYPH_CHANGED_TYPE_NONE;
    changed = (GLYPH_CHANGED_TYPE)((int)changed | (int)_chart_price->NewData(data));
    changed = (GLYPH_CHANGED_TYPE)((int)changed | (int)_chart_volume->NewData(data));
    changed = (GLYPH_CHANGED_TYPE)((int)changed | (int)_chart_interest->NewData(data));
    return changed;
}

void CPVICanvas::SetGeometryData(dataptr pdata, int count, int datasize)
{
    if (count < 2) return;

    int ndown = (_chart_price->GetRtcSet()->_down_intval < 2) ? 1 : _chart_price->GetRtcSet()->_down_intval;
    char* p = (char*)pdata;
    _chart_price   ->GetGraph()->BeginSetData(p);
    _chart_volume  ->GetGraph()->BeginSetData(p);
    _chart_interest->GetGraph()->BeginSetData(p);


    p += datasize;
    for (int i = 1; i < count; i += ndown, p += datasize) {
        _chart_price   ->GetGraph()->AddDataToPathGeometry(p);
        _chart_volume  ->GetGraph()->AddDataToPathGeometry(p);
        _chart_interest->GetGraph()->AddDataToPathGeometry(p);
    }

    _chart_price   ->GetGraph()->EndSetData();
    _chart_volume  ->GetGraph()->EndSetData();
    _chart_interest->GetGraph()->EndSetData();
}


void CPVICanvas::AddDataChart(const char* name, CSeriesDataChart** chart)
{
    *chart = new CSeriesDataChart(name);
    InsertNext(_blind->GetGlyphTreeIter(), *chart);

    char chartconf[MAX_PATH];
    _snprintf_s(chartconf, MAX_PATH, _TRUNCATE, "%s.rtchart-price", _strconf);
    (*chart)->setConfig(_config, chartconf);
    (*chart)->Init();

    //add price lines graph
    char temp[MAX_WARMGUI_NAME_LEN];
    _snprintf_s(temp, MAX_WARMGUI_NAME_LEN, _TRUNCATE, "graph-%s-price", _name);
    CDataLineGraph* graph = new CDataLineGraph(temp, true);
    (*chart)->AddGraph(graph);
    graph->SetDrawType(CDataLineGraph::SDATA_GRAPH_LINE_TYPE_LINE);


/*
    _chart_volume = new CSeriesDataChart("volume");
    InsertNext(_chart_price->GetGlyphTreeIter(), _chart_volume);
    _chart_volume->Init();

    //add volume lines graph
    _snprintf_s(name, MAX_WARMGUI_NAME_LEN, _TRUNCATE, "graph-%s-vol", _name);
    graph = new CDataLineGraph(name, true, _chart_volume);
    _chart_volume->AddGraph(graph);
    graph->SetDrawType(CDataLineGraph::SDATA_GRAPH_LINE_TYPE_DIFF_LINEBAR);

    _chart_interest = new CSeriesDataChart("intr");
    InsertNext(_chart_volume->GetGlyphTreeIter(), _chart_interest);
    _chart_interest->Init();

    //add interest lines graph
    _snprintf_s(name, MAX_WARMGUI_NAME_LEN, _TRUNCATE, "graph-%s-intr", _name);
    graph = new CDataLineGraph(name, true, _chart_interest);
    _chart_interest->AddGraph(graph);
    graph->SetDrawType(CDataLineGraph::SDATA_GRAPH_LINE_TYPE_LINE);
*/
}
