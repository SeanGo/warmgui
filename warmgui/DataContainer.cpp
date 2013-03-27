#include "StdAfx.h"
#include "warmgui.h"

namespace WARMGUI {
IDataContainer::IDataContainer(void)
    : _pdata(0)
    , _count(0)
    , _sizeofdata(0)
    , _size(0)
{
}


IDataContainer::~IDataContainer(void)
{
    if (_pdata) delete _pdata;
}

void IDataContainer::SetContainerSize(size_t sizeofdata, size_t len)
{
    if (_pdata) delete _pdata;
    _count = 0, _sizeofdata = sizeofdata, _size = len;
    _pdata = (void*)(new char[sizeofdata * len]);
}


void IDataContainer::AddData(DataObjectPtr dop)
{
    if (_count < _size) {
        memcpy((char*)_pdata + _sizeofdata * _count++, dop->GetData(), _sizeofdata);
    }
}



inline dataptr IDataContainer::GetData(size_t nSpos)
{
    if (_count && nSpos < _count)
        return ((char*)_pdata + nSpos * _sizeofdata);
    else
        return (0);
}



}
