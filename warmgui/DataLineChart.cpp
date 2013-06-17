#include "StdAfx.h"
#include "warmgui.h"


namespace WARMGUI {


CDataLineChart::CDataLineChart(const char* name, bool world_own_type/* = false*/, bool data_own_type/*  = false*/, bool own_artist/* = false*/)
    : IDataGraph(name, world_own_type, data_own_type, own_artist)
    , _coord(0)
    , _stroke_width(1.0f)
{

}


CDataLineChart::~CDataLineChart(void)
{
}


HRESULT CDataLineChart::DrawGraph(bool redraw/* = false*/)
{
    /*
    for (int i = 1; i < _iter.number_of_children(); i++) {
        IGlyph* g = _canvas->getChild(_iter, i);
        if (g)
            g->DrawGraph(redraw);
    }
    */
    return S_OK;
}

/*
HRESULT CDataLineChart::Draw(bool redraw/* = false*//*)
{
    return S_OK;
}

HRESULT CDataLineChart::DrawBkg(bool redraw/* = false*//*)
{
    return S_OK;
}

HRESULT CDataLineChart::Renew()
{
    return S_OK;
}
*/

void CDataLineChart::SetGeometryData(dataptr pdata, int count, int datasize)
{
}

void CDataLineChart::AddData(dataptr data)
{
}

void CDataLineChart::SetRect(RECT& rect)
{
    RECT rect_graph = rect;
    rect_graph.left += _coord->getRulerWidth().left,
        rect_graph.top += _coord->getRulerWidth().top,
        rect_graph.right -= _coord->getRulerWidth().right,
        rect_graph.bottom -= _coord->getRulerWidth().bottom;

    for (int i = 0; i < _iter.number_of_children(); i++) {
        IGlyph* g = _canvas->getChild(_iter, i);
        if (g) {
            if (g->isClass("CCoordFrame"))
                g->SetRect(rect);
            else {
                //TCHAR name[MAX_PATH];
                //CChineseCodeLib::Gb2312ToUnicode(name, MAX_PATH, g->_name);
                //MYTRACE(L"%s reset rect\n", name);

                g->SetRect(rect_graph);
                g->Changed((GLYPH_CHANGED_TYPE)((int)GLYPH_CHANGED_TYPE_CHANGED | (int)GLYPH_CHANGED_TYPE_GLYPH_RESIZE | (int)GLYPH_CHANGED_TYPE_COORDFRAME));
            }
        }
    }
}

HRESULT CDataLineChart::Init()
{
    char temp[MAX_PATH];
    _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.name", _strconf);
    std::string str = _config->getString(temp);
    const char* name = str.c_str();
    strcpy_s(_name, sizeof(_name), name);

    _snprintf_s(temp, MAX_PATH, _TRUNCATE, "%s.stroke-width", _strconf);
    _stroke_width = (float)(_config->getDouble(temp));

    return IDataGraph::Init();
}


void CDataLineChart::BeginSetData(dataptr data)
{
#ifdef _DEBUG
    //TCHAR name[MAX_PATH];
    //CChineseCodeLib::Gb2312ToUnicode(name, MAX_PATH, _name);
    //MYTRACE(L"CDataLineChart::BeginSetData %s\n", name);
#endif 
    for (int i = 0; i < _canvas->getChildNumber(_iter); i++) {
        IGlyph* g = _canvas->getChild(_iter, i);
        if (g->isClass("CDataLineGraph"))
            ((CDataLineGraph*)g)->BeginSetData(data);
    }
}

void CDataLineChart::AddDataToPathGeometry(dataptr data)
{
#ifdef _DEBUG
    /*
    CTPMMD* ctpmmmd = (CTPMMD*)data;
    TCHAR name[MAX_PATH];
    CChineseCodeLib::Gb2312ToUnicode(name, MAX_PATH, _name);
    MYTRACE(
        L"%s %.02f, %.02f, %.02f, %.02f, %.02f, %.02f\n",
        name,
        (ctpmmmd)->fIndex,
        (ctpmmmd)->LastPrice,
        (ctpmmmd)->Volume,
        (ctpmmmd)->relVolume,
        (ctpmmmd)->OpenInterest,
        (ctpmmmd)->relInterest);
    */
#endif //_DEBUG
    for (int i = 0; i < _canvas->getChildNumber(_iter); i++) {
        IGlyph* g = _canvas->getChild(_iter, i);
        if (g->isClass("CDataLineGraph"))
            ((CDataLineGraph*)g)->AddDataToPathGeometry(data);
    }
}

void CDataLineChart::EndSetData()
{
    for (int i = 0; i < _canvas->getChildNumber(_iter); i++) {
        IGlyph* g = _canvas->getChild(_iter, i);
        if (g->isClass("CDataLineGraph"))
            ((CDataLineGraph*)g)->EndSetData();
    }
}

HRESULT CDataLineChart::RenewGraph(GLYPH_CHANGED_TYPE changed_type)
{
    for (int i = 0; i < _canvas->getChildNumber(_iter); i++) {
        IGlyph* g = _canvas->getChild(_iter, i);
        if (g->isClass("CDataLineGraph")) {
            //TCHAR name[MAX_PATH];
            //CChineseCodeLib::Gb2312ToUnicode(name, MAX_PATH, g->GetName());
            //MYTRACE(L"CDataLineChart::RenewGraph %s %d\n", name, g->IsChanged());
            ((CDataLineGraph*)g)->Changed(changed_type);
            ((CDataLineGraph*)g)->Renew();
        }
    }
    return S_OK;
}

void CDataLineChart::MoveBitmapToLeft()
{
    for (int i = 0; i < _canvas->getChildNumber(_iter); i++) {
        IGlyph* g = _canvas->getChild(_iter, i);
        if (g->isClass("CDataLineGraph")) {
            //TCHAR name[MAX_PATH];
            //CChineseCodeLib::Gb2312ToUnicode(name, MAX_PATH, g->GetName());
            //MYTRACE(L"CDataLineChart::RenewGraph %s %d\n", name, g->IsChanged());
            ((CDataLineGraph*)g)->MoveBitmapToLeft();
        }
    }
}

}//namespace WARMGUI