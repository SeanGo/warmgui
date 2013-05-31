#include "StdAfx.h"
#include "qks_incs.h"


CTickDataGridCtrl::CTickDataGridCtrl(const char* name, int nRows/* = 0*/, int nCols/* = 0*/, int nFixedRows/* = 0*/, int nFixedCols/* = 0*/)
    : CGridCtrl(name, nRows, nCols, nFixedRows, nFixedCols)
{
}


CTickDataGridCtrl::~CTickDataGridCtrl(void)
{
}

///the item.row must be seted before call this function
inline void CTickDataGridCtrl::SetCtpData(const CTPMMD* pctpmmd, GV_ITEM& item)
{
    TCHAR buf[MAX_PATH];
    //<head>time, price, vol, iter, open, close, ask1, avol1, bid1, bvol1</head>

    item.col = 0;
    //time
    _sntprintf_s(buf, MAX_PATH, _TRUNCATE, L"%d", (int)(pctpmmd->m_time));
    item.strText = buf; SetItem(&item); item.col++;
    //price
    _sntprintf_s(buf, MAX_PATH, _TRUNCATE, L"%.02f", pctpmmd->LastPrice);
    item.strText = buf; SetItem(&item); item.col++;
    //rel-volume
    _sntprintf_s(buf, MAX_PATH, _TRUNCATE, L"%d", (int)(pctpmmd->relVolume));
    item.strText = buf; SetItem(&item); item.col++;
    //rel-interest
    _sntprintf_s(buf, MAX_PATH, _TRUNCATE, L"%d", (int)(pctpmmd->relInterest));
    item.strText = buf; SetItem(&item); item.col++;
    //open_vol = (each_vol + each_int)/2 = 10
    _sntprintf_s(buf, MAX_PATH, _TRUNCATE, L"%d", (int)((pctpmmd->relVolume + pctpmmd->relInterest) / 2));
    item.strText = buf; SetItem(&item); item.col++;
    //close_vol = each_int - open_vol
    _sntprintf_s(buf, MAX_PATH, _TRUNCATE, L"%d", (int)((pctpmmd->relVolume - pctpmmd->relInterest) / 2));
    item.strText = buf; SetItem(&item); item.col++;
    //ask1
    _sntprintf_s(buf, MAX_PATH, _TRUNCATE, L"%.02f", pctpmmd->AskPrice1);
    item.strText = buf; SetItem(&item); item.col++;
    //ask1 volume
    _sntprintf_s(buf, MAX_PATH, _TRUNCATE, L"%d", (int)pctpmmd->AskVolume1);
    item.strText = buf; SetItem(&item); item.col++;
    //bid1
    _sntprintf_s(buf, MAX_PATH, _TRUNCATE, L"%.02f", pctpmmd->BidPrice1);
    item.strText = buf; SetItem(&item); item.col++;
    //bid1 volume
    _sntprintf_s(buf, MAX_PATH, _TRUNCATE, L"%d", (int)pctpmmd->BidVolume1);
    item.strText = buf; SetItem(&item);
}


GLYPH_CHANGED_TYPE CTickDataGridCtrl::NewData(IDataContainer* data_cont, DataObject::MARKET_DATA_TYPE datatype)
{
    CCtpmmdContainer* cc = (CCtpmmdContainer*)data_cont;
    const CTPMMD* pctpmmd = cc->getCurrentData();
#   ifdef _DEBUG
    //MYTRACE(L"tick %.02f %.02f %.02f %.02f %.02f %.02f %.02f\n",
    //    pctpmmd->fIndex,
    //    pctpmmd->m_time,
    //    pctpmmd->LastPrice,
    //    pctpmmd->Volume,
    //    pctpmmd->relVolume,
    //    pctpmmd->OpenInterest,
    //    pctpmmd->relInterest);
#   endif //_DEBUG
    size_t count = cc->getCount();
    GV_ITEM item;
    item.mask = GVIF_TEXT;  
    if (count < _nRows) {
        item.row = count;
    } else {
        item.mask = GVIF_TEXT;
        CTPMMD* ctpmmmd_ptr = (CTPMMD*)cc->getDataPtr() + count - 1 - _nRows - 1;
        for (int j = 1; j < _nRows - 1; j++) {
            item.row = j;
            for (int i = 0; i < _nCols; i++) {
                item.col = i;
                //SetCtpData(ctpmmmd_ptr, item);
                item.strText = GetItemText(j + 1, i);
                SetItem(&item); 
            }
            ++ctpmmmd_ptr;
        }
        item.row += 1;
    }
    SetCtpData(pctpmmd, item);
    return GLYPH_CHANGED_TYPE_CHANGED;
}


HRESULT CTickDataGridCtrl::Init()
{
    HRESULT hr = CGridCtrl::Init();
    if (SUCCEEDED(hr)) {
        try {
            char key[MAX_PATH];
            _snprintf_s(key, MAX_PATH, "%s.head", _strconf);
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
    return hr;
}
