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
        dispatch(dop);
    }
}


void IDataContainer::dispatch(DataObjectPtr dop)
{
    for (ConstChartIter iter = _chart_array.begin(); iter != _chart_array.end(); iter++) {
        int interval = (*iter)->GetRtcSet()->_down_intval;
        if (!interval || !(_count % interval) || _count == 1)
            (*iter)->NewData(dop);
    }
}

void IDataContainer::RegisterChart(CSeriesDataChart* chart)
{
    _chart_array.push_back(chart);
}

void IDataContainer::SetGeometryData()
{
    for (ConstChartIter iter = _chart_array.begin(); iter != _chart_array.end(); iter++) {
        (*iter)->SetGeometryData(_pdata, _count, _sizeofdata);
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
