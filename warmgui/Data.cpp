#include "StdAfx.h"
#include "warmgui.h"


namespace WARMGUI {

CData::CData(void)
    : _source(DATA_SOURCE_TYPE_MYSELF)
    , _data(0)
{
    *_lable = L'\0';
    *_name  = L'\0';
}


CData::~CData(void)
{
}

CData::CData(const TCHAR* name, const TCHAR* lable, const DATA_SOURCE_TYPE source /*= DATA_SOURCE_TYPE_MYSELF*/, const DataPointer data /*= 0*/)
{
    _tcscpy_s(_name , MAX_WARMGUI_NAME_LEN, name );
    _tcscpy_s(_lable, MAX_WARMGUI_NAME_LEN, lable);
    _source = source, _data = data;
}

}
