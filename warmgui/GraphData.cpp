#include "StdAfx.h"
#include "warmgui.h"


namespace WARMGUI {

IGraphData::IGraphData(void)
    : _source(DATA_SOURCE_TYPE_MYSELF)
    , _pdata(0)
{
    *_lable = L'\0';
    *_name  = L'\0';
}


IGraphData::~IGraphData(void)
{
}

IGraphData::IGraphData(const TCHAR* name, const TCHAR* lable, const DATA_SOURCE_TYPE source /*= DATA_SOURCE_TYPE_MYSELF*/, const dataptr data /*= 0*/, const size_t datasize/* = 0 */)
{
    _tcscpy_s(_name , MAX_WARMGUI_NAME_LEN, name );
    _tcscpy_s(_lable, MAX_WARMGUI_NAME_LEN, lable);
    _source = source, _pdata = data, _sizeof = datasize;
}

}
