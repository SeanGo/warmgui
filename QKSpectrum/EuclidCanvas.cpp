#include "StdAfx.h"
#include "qks_incs.h"


CEuclidCanvas::CEuclidCanvas(const char* name)
    : IDataCanvas(name)
    , _bHasBackground(true)
    , _blind(0)
{
    setClass();
}


CEuclidCanvas::~CEuclidCanvas(void)
{
}

const HRESULT CEuclidCanvas::Init(const char* name/* = 0*/)
{
    IGlyph* g = 0;
    if (_bHasBackground) {
        _blind = new CBlind(_name, BGR(0, 0, 0), 0.5);
        g = AppendChild(_gt.begin(), _blind);
    }

    return S_OK;
}

void CEuclidCanvas::SetGlyphRect()
{
    if (_blind) _blind->SetRect(_rect);
}


void CEuclidCanvas::setDataOffset(int pxo, int pyo)
{
}

GLYPH_CHANGED_TYPE CEuclidCanvas::NewData(DataObjectPtr data)
{
    return GLYPH_CHANGED_TYPE_NONE;
}

GLYPH_CHANGED_TYPE CEuclidCanvas::NewData(dataptr data, DataObject::MARKET_DATA_TYPE datatype)
{
    return GLYPH_CHANGED_TYPE_NONE;
}

GLYPH_CHANGED_TYPE CEuclidCanvas::NewData(IDataContainer* data_cont, DataObject::MARKET_DATA_TYPE datatype)
{
    return GLYPH_CHANGED_TYPE_NONE;
}



