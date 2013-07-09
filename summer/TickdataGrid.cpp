#include "StdAfx.h"
#include "summer.h"


CTickdataGrid::CTickdataGrid(const char* name)
    : WARMGUI::CGridCtrl(name)
    , _count(0)
{
    SetClass();
}


CTickdataGrid::~CTickdataGrid(void)
{
}


///the item.row must be seted before call this function
inline void CTickdataGrid::SetCtpData(GV_ITEM& item)
{
    TCHAR buf[MAX_PATH];
    //<head>time, price, vol, iter, open, close, ask1, avol1, bid1, bvol1</head>

    item.col = 0;
    //time
    _sntprintf_s(buf, MAX_PATH, _TRUNCATE, L"%d", (int)(_ctpmmd.m_time));
    item.strText = buf; SetItem(&item); item.col++;
    //price
    _sntprintf_s(buf, MAX_PATH, _TRUNCATE, L"%.02f", _ctpmmd.LastPrice);
    item.strText = buf; SetItem(&item); item.col++;
    //rel-volume
    _sntprintf_s(buf, MAX_PATH, _TRUNCATE, L"%d", (int)(_ctpmmd.relVolume));
    item.strText = buf; SetItem(&item); item.col++;
    //rel-interest
    _sntprintf_s(buf, MAX_PATH, _TRUNCATE, L"%d", (int)(_ctpmmd.relInterest));
    item.strText = buf; SetItem(&item); item.col++;
    //open_vol = (each_vol + each_int)/2 = 10
    _sntprintf_s(buf, MAX_PATH, _TRUNCATE, L"%d", (int)((_ctpmmd.relVolume + _ctpmmd.relInterest) / 2));
    item.strText = buf; SetItem(&item); item.col++;
    //close_vol = each_int - open_vol
    _sntprintf_s(buf, MAX_PATH, _TRUNCATE, L"%d", (int)((_ctpmmd.relVolume - _ctpmmd.relInterest) / 2));
    item.strText = buf; SetItem(&item); item.col++;
    //ask1
    _sntprintf_s(buf, MAX_PATH, _TRUNCATE, L"%.02f", _ctpmmd.AskPrice1);
    item.strText = buf; SetItem(&item); item.col++;
    //ask1 volume
    _sntprintf_s(buf, MAX_PATH, _TRUNCATE, L"%d", (int)_ctpmmd.AskVolume1);
    item.strText = buf; SetItem(&item); item.col++;
    //bid1
    _sntprintf_s(buf, MAX_PATH, _TRUNCATE, L"%.02f", _ctpmmd.BidPrice1);
    item.strText = buf; SetItem(&item); item.col++;
    //bid1 volume
    _sntprintf_s(buf, MAX_PATH, _TRUNCATE, L"%d", (int)_ctpmmd.BidVolume1);
    item.strText = buf; SetItem(&item);
}


GLYPH_CHANGED_TYPE CTickdataGrid::update(dataptr data)
{
    memcpy(&_ctpmmd, data, sizeof(CTPMMD));

#   ifdef _DEBUG
    //MYTRACE(L"tick %.02f %.02f %.02f %.02f %.02f %.02f %.02f\n",
    //    _ctpmmd.fIndex,
      //  _ctpmmd.m_time,
        //_ctpmmd.LastPrice,
    //    _ctpmmd.Volume,
      //  _ctpmmd.relVolume,
        //_ctpmmd.OpenInterest,
    //    _ctpmmd.relInterest);
#   endif //_DEBUG
    GV_ITEM item;
    item.mask = GVIF_TEXT;  
    if (_count < _nRows) {
        item.row = _count;
    } else {
        item.mask = GVIF_TEXT;
        for (int j = 1; j < _nRows - 1; j++) {
            item.row = j;
            for (int i = 0; i < _nCols; i++) {
                item.col = i;
                //SetCtpData(ctpmmmd_ptr, item);
                item.strText = GetItemText(j + 1, i);
                SetItem(&item); 
            }
        }
        item.row += 1;
    }
    SetCtpData(item);
    _count++;
    return 0;
}


HRESULT CTickdataGrid::init()
{
    HRESULT hr = WARMGUI::CGridCtrl::init();
    if (SUCCEEDED(hr)) {
        try {
            char key[MAX_PATH];
            _snprintf_s(key, MAX_PATH, "%s.head", _str_conf);
            //const std::string tmp = _config->getString(key).c_str();
            //CChineseCodeLib::Gb2312ToUnicode(head, MAX_PATH, tmp.c_str());
            WStringArray wsa;
            _config->getStringVector(wsa, key);
            SetColumnCount(wsa.size());
            SetFixedRowCount(1);
            {//test first cell
                GV_ITEM item;
                item.mask = GVIF_TEXT;
    
                item.col = item.row = 0;
                for (int i = 0; i < wsa.size(); i++) {
                    item.col = i;
                    item.strText = wsa[i];
                    SetItem(&item);
                }
            }
       }catch( ... ) {
            hr = -1;
        }
    }
    _count = 1;
    return hr;
}
