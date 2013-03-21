#include "StdAfx.h"
#include "qks_incs.h"
#include "resource.h"

//////////////////////////////////////////////////////////////////////////////////////////
// class CZenInTwiningAtelier

CZenInTwiningAtelier::CZenInTwiningAtelier(const TCHAR* name)
    :   _canvasImgbkg(0)
    ,      _pvicanvas(0)
    ,      _layoutOne(0)
    ,       _rtcanvas(0)
    ,       _1mcanvas(0)
    ,       _5mcanvas(0)
    ,      _15mcanvas(0)
    ,        _message(0)
    ,       _trademsg(0)
	,        _toolbar(0)
{
    _tcscpy_s(_name, MAX_WARMGUI_NAME_LEN, name);
}

void CZenInTwiningAtelier::SetLayout()
{
	_layoutOne = new CZenInTwiningLayout();
	_layout = _layoutOne;
}

HRESULT CZenInTwiningAtelier::InitAtelier(HWND hwnd, WARMGUI::CWarmguiConfig* config)
{
	HRESULT hr = CAtelier::InitAtelier(hwnd, config);

	if (SUCCEEDED(hr) && GetBitmapMaterial()) {
		_canvasImgbkg = new WARMGUI::CBkgCanvas(L"bkgcanvas");
		_canvasImgbkg->SetBitmap(&_appbmp);
		Insert(_canvasImgbkg);
	}

    _pvicanvas = new CPVICanvas(L"canvas-main-pvi");
    Insert(_pvicanvas);

	_rtcanvas = new CZenInTwiningCanvas(L"canvas-rt-twinig");
	Insert(_rtcanvas);
	CTPMMD ctpdata;
    int x0 = (int)(&ctpdata.fIndex) - (int)&ctpdata;
    int y0 = (int)(&ctpdata.LastPrice) - (int)&ctpdata; 
    _rtcanvas->setDataOffset(x0, y0);

	_1mcanvas = new CZenInTwiningCanvas(L"canvas-twining-1m");
	Insert(_1mcanvas);
    _1mcanvas->setDataOffset(x0, y0);

	_5mcanvas = new CZenInTwiningCanvas(L"canvas-twining-5m");
	Insert(_5mcanvas);
    _5mcanvas->setDataOffset(x0, y0);

	_15mcanvas = new CZenInTwiningCanvas(L"canvas-twining-15m");
	Insert(_15mcanvas);
    _15mcanvas->setDataOffset(x0, y0);

    _message  = new CGridCanvas(L"analyse-info");
	Insert(_message);

    _trademsg = new CGridCanvas(L"trand-info");
	Insert(_trademsg);

	_toolbar = new WARMGUI::CToolbar(L"toolbar", ID_NETWORK_LOGIN);
	Insert(_toolbar);

    SetLayout();
	return hr;
}

void CZenInTwiningAtelier::SetChatToDataContanier()
{
	if (_data_container)
    {
		_pvicanvas->SetChatToDataContanier();
		_rtcanvas->SetChatToDataContanier();
		_1mcanvas->SetChatToDataContanier();
		_5mcanvas->SetChatToDataContanier();
		_15mcanvas->SetChatToDataContanier();
	}
}


void CZenInTwiningAtelier::ToggleToolbar()
{
	if (_toolbar->IsVisible())
		_toolbar->SetVisible(false);
	else
		_toolbar->SetVisible(true);

	_layout->Disposal(this, _rect);

	ClearChanged();
	Changed(GLYPH_CHANGED_TYPE_ATELIER_RESIZE);

	InvalidateRect(_hwnd, 0, FALSE);
}


void CZenInTwiningAtelier::ReceiveData(DataObjectPtr dop, const char* name)
{
	//TCHAR wname[128];
	//CChineseCodeLib::Gb2312ToUnicode(wname, 128, name);
	//MYTRACE(_T("i got Caluc result %d from %s\n"), dataptr->GetDataLength(), wname);
	//_atelier->NewRtCal(dataptr);

    //CTPMMD* ctpmmd = (CTPMMD*)dop->GetData();
    //char*  buf = (char*)dop->GetData();
	//size_t len = dop->GetDataLength();
    
    if (!_ctptimesec.st1)
        _ctptimesec.set_time_sec(((CTPMMD*)(dop->GetData()))->m_time);
    _data_container->AddData(dop);

    //buf += sizeof(CTPMMD), len -= sizeof(CTPMMD);

    /*
    //High-level filter
	int nHigh = *(int*)buf; buf += sizeof(int); len -= sizeof(int);
	if (nHigh != shortestSize) return;
    //get High-level data
	double* maHigh = (double*)buf; buf += nHigh * sizeof(double); len -= nHigh * sizeof(double);

    //Low-level filter
    int nLow = *(int*)  buf; buf += sizeof(int); len -= sizeof(int);
	if (nLow != shortestSize) return;
    //get Low-level data
	double* maLow = (double*)buf; buf += nLow * sizeof(double);  len -= nLow * sizeof(double);
    */

    /*
    if (len > 0) {
	    //const size_t shortestSize =  512;
	    const size_t  samplerSize = 1024;
	    const size_t  predictSize =  120;

	    EXTRINFO    extCentral;
	    INFLGRP       inflBest;
	    CENTRAL_SERIES central;
	    LHAREA             lha;
	    //double*  shortApprox = 0;
	    double*   shortPredict = 0;
	    //double*   longApprox = 0;
	    double*    longPredict = 0;

        extCentral.nAllNum  = *(int*)    buf; buf += sizeof(int) ; len -= sizeof(int);
	    if (extCentral.nAllNum < 0 || extCentral.nAllNum > 200) return;
	    extCentral.extremum = (EXTREMUM*)buf; buf += sizeof(EXTREMUM) * extCentral.nAllNum; len -= sizeof(EXTREMUM) * extCentral.nAllNum;

        inflBest.nNum       = *(int*)    buf; buf += sizeof(int) ; len -= sizeof(int);
	    if (inflBest.nNum < 0 || inflBest.nNum > 100) return;
	    inflBest.infl       = (INFLEXION*)buf;buf += sizeof(INFLEXION) * inflBest.nNum; len -= sizeof(INFLEXION) * inflBest.nNum;

        central.nNum        = *(int*)    buf; buf += sizeof(int) ; len -= sizeof(int);
	    if (central.nNum < 0 || central.nNum > 200) return;
	    central.central     = (CENTRAL*) buf; buf += sizeof(CENTRAL  ) * central.nNum; len -= sizeof(CENTRAL) * central.nNum;

        lha.nNum            = *(int*)    buf; buf += sizeof(int) ; len -= sizeof(int);
	    if (lha.nNum < 0 || lha.nNum > 200) return;
	    lha.lha             = (L_H_AREA*)buf; buf += sizeof(L_H_AREA) * lha.nNum; len -= sizeof(L_H_AREA) * lha.nNum;

        //draw Low-pass, HiPass, extremum, inflexions, central, area, shortest_size

	    if (len > 0) {
            ///////the short term approx
		    //int nShortApprox = *(int*)buf; buf += sizeof(int); len -= sizeof(int);
		    //if (nShortApprox != samplerSize) return;
		    //shortApprox = (double*)buf; buf += sizeof(double) * samplerSize; len -= sizeof(double) * samplerSize;
            ///////the short term approx
		    int nPredict =  *(int*)buf; buf += sizeof(int); len -= sizeof(int);
		    if (nPredict != predictSize) return;
		    shortPredict = (double*)buf; buf += sizeof(double) * predictSize; len -= sizeof(double) * predictSize;
            //draw shortApprox(samplerSize), predict(predictSize)
	    }

	    if (len > 0) {
            ///////the long term approx
		    //int nLongApprox = *(int*)buf; buf += sizeof(int); len -= sizeof(int);
		    //if (nLongApprox != samplerSize) return;
		    //longApprox = (double*)buf; buf += sizeof(double) * samplerSize; len -= sizeof(double) * samplerSize;
            ///////the log term approx
		    int nPredict =  *(int*)buf; buf += sizeof(int); len -= sizeof(int);
		    if (nPredict != predictSize) return;
		    longPredict = (double*)buf; buf += sizeof(double) * predictSize; len -= sizeof(double) * predictSize;

            //draw longApprox(samplerSize), longPredict(predictSize)
	    }
    }
    */
}
