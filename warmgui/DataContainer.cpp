#include "StdAfx.h"
#include "warmgui.h"

namespace WARMGUI {
IDataContainer::IDataContainer(const char* name)
{
    strcpy_s(_name, MAX_PATH, name);
    *_my_disp = 0;
}



IDataContainer::~IDataContainer(void)
{
    //SafeDelete(_pdata);

    //do not delete canvas here, do it in ateler
    //do not delete graph  here, do it in canvas
}

/*
void IDataContainer::SetContainerSize(size_t sizeofdata, size_t len)
{
    if (_pdata) delete _pdata;
    _count = 0, _sizeofdata = sizeofdata, _data_len = len;
    _pdata = (void*)(new char[sizeofdata * len]);

    //x must be first pos and flot type
    _x = (float*)_pdata;
}

void IDataContainer::SetContainerSize(size_t len, const char* datatype)
{
    if (strcmp(datatype, "ctpmmd")) {
        SetContainerSize(sizeof(CTPMMD), len);
    }

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
*/

inline void IDataContainer::RegisterCalculator(ICalculator* calculator)
{
#   ifdef _DEBUG
    TCHAR name1[MAX_PATH], name2[MAX_PATH];
    CChineseCodeLib::Gb2312ToUnicode(name1, MAX_PATH, calculator->getName());
    CChineseCodeLib::Gb2312ToUnicode(name2, MAX_PATH, _name);
    MYTRACE(L"ICalculator %s regestered to data-container %s\n", name1, name2);
#   endif
    _cals.push_back(calculator);
}

inline void IDataContainer::RegisterCanvas(IDataCanvas* canvas)
{
#   ifdef _DEBUG
    TCHAR name1[MAX_PATH], name2[MAX_PATH];
    CChineseCodeLib::Gb2312ToUnicode(name1, MAX_PATH, canvas->getName());
    CChineseCodeLib::Gb2312ToUnicode(name2, MAX_PATH, _name);
    MYTRACE(L"IDataCanvas %s regestered to data-container %s\n", name1, name2);
#   endif

    _canvases.push_back(canvas);
}

inline void IDataContainer::RegisterDataGraph(IDataGraph* graph)
{
#   ifdef _DEBUG
    TCHAR name1[MAX_PATH], name2[MAX_PATH];
    CChineseCodeLib::Gb2312ToUnicode(name1, MAX_PATH, graph->getName());
    CChineseCodeLib::Gb2312ToUnicode(name2, MAX_PATH, _name);
    MYTRACE(L"IDataGraph %s regestered to data-container %s\n", name1, name2);
#   endif

    _dgraph.push_back(graph);
}


GLYPH_CHANGED_TYPE IDataContainer::AddData(DataObjectPtr dop)
{
    GLYPH_CHANGED_TYPE change = GLYPH_CHANGED_TYPE_NONE;
    MARKET_DATA_TYPE datatype = (MARKET_DATA_TYPE)*(int*)(dop->GetData());

    if (NewData(dop)) {
        //TCHAR name[MAX_PATH];
        //CChineseCodeLib::Gb2312ToUnicode(name, MAX_PATH, _name);
        //MYTRACE(L"container name is %s\n", name);

        //calculators
        for (CalConstIter iter = _cals.begin(); iter != _cals.end(); iter++) {
            (*iter)->NewData(this, datatype);
        }

        //canvases
        for (DataCanvasArray::const_iterator iter = _canvases.begin(); iter != _canvases.end(); iter++) {
            GLYPH_CHANGED_TYPE c = (*iter)->NewData(this, datatype);
            change = (GLYPH_CHANGED_TYPE)((int)change | (int)c);
        }

        //data graphs
        for (DataGraphConstIter iter = _dgraph.begin(); iter != _dgraph.end(); iter++) {
            GLYPH_CHANGED_TYPE c = (*iter)->NewData(this, datatype);
            change = (GLYPH_CHANGED_TYPE)((int)change | (int)c);
        }
    }

    return change;
}

/*
//////////////////////////////////////////////////////////////////////////////////////
//
// class CCommonContainer
//
CCommonContainer::CCommonContainer(size_t sizeofdata, size_t len)
{
    SetContainerSize(sizeofdata, len);
}


void CCommonContainer::AddData(DataObjectPtr dop)
{
    if (_count < _data_len) {
        *((float*)_pdata + _count) = *(float*)(dop->GetData());
    }
}

void CCommonContainer::SetContainerSize(size_t sizeofdata, size_t len)
{
    if (sizeofdata == sizeof(float)) {
        if (_pdata) delete _pdata;

        _pdata = new float[len];

        _count = 0, _sizeofdata = sizeof(float), _data_len = len, _own_xval = true;
    }
}

inline dataptr CCommonContainer::GetData(size_t nSpos)
{
    if (nSpos < _data_len)
        _pnt.x = _x[nSpos], _pnt.y = *(((float*)_pdata) + nSpos);
    else
        _pnt.x = _pnt.y = 0;

    return &_pnt;
}
*/


void             RegisterCalculator(ICalculator* calculator);
void             RegisterCanvas(IDataCanvas* canvas);
void             RegisterDataGraph(IDataGraph* graph);

}
