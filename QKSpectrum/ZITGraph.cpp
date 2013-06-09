#include "StdAfx.h"
#include "qks_incs.h"

CZITGraph::CZITGraph(const char* name)
    : IDataGraph(name)
    , _down_sample(0)
    , _zit_data(0)
    , _pAreaFont(0)
{
    setClass();
}


CZITGraph::~CZITGraph(void)
{
    SafeRelease(&_pAreaFont);
}


HRESULT CZITGraph::DrawGraph(bool /*redraw*/)
{
    if (!_zit_data || _zit_data->data_ds != _down_sample) return S_OK;

    TCHAR name[MAX_PATH];
    CChineseCodeLib::Gb2312ToUnicode(name, MAX_PATH, _name);
    MATRIX_2D backtrans;
    _artist->GetTransform(&backtrans);

    //MYTRACE(L"zit graph backup trans %s\n", name);
	//MYTRACE(L"backup %.02f %.02f %.02f %.02f %.02f %.02f\n", 
	//    backtrans._11, backtrans._12, backtrans._21, backtrans._22, backtrans._31, backtrans._32);

    MATRIX_2D rect_trans = D2D1::Matrix3x2F::Identity();
    rect_trans._31 = static_cast<float>(_rect.left), rect_trans._32 = static_cast<float>(_rect.top );
    _artist->SetTransform(&rect_trans);
	//MYTRACE(L"rect_trans %.02f %.02f %.02f %.02f %.02f %.02f\n", 
	//    rect_trans._11, rect_trans._12, rect_trans._21, rect_trans._22, rect_trans._31, rect_trans._32);
    _artist->PushLayer(.0f, .0f, fRectWidth(_rect), fRectHeight(_rect));

    //_artist->GetUsingRT()->PushLayer(
    D2D1_ANTIALIAS_MODE am = _artist->GetHwndRT()->GetAntialiasMode();
	_artist->GetUsingRT()->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

    if (_zit_data->extCentral.nAllNum) {
        //MYTRACE(L"ext %d %.02f %.02f %.02f - %.02f %.02f %.02f\n",
        //    _zit_data->extCentral.nAllNum, 
        //    Time2Index(g_ctpsec, _zit_data->extCentral.extremum[0].ftime),
        //    _zit_data->extCentral.extremum[0].fValue,
        //    _zit_data->extCentral.extremum[0].ftime,
        //    Time2Index(g_ctpsec, _zit_data->extCentral.extremum[_zit_data->extCentral.nAllNum - 1].ftime),
        //    _zit_data->extCentral.extremum[_zit_data->extCentral.nAllNum - 1].fValue,
        //    _zit_data->extCentral.extremum[_zit_data->extCentral.nAllNum - 1].ftime);
        //MATRIX_2D* m = _referframe->GetTransform();
	    //MYTRACE(L"ReferFrame %s %.02f %.02f %.02f %.02f %.02f %.02f - ", 
		//    _name,
		//    m->_11, m->_12, m->_21, m->_22, m->_31, m->_32);

        //MYTRACE(L"ext %d %.02f %.02f %.02f\n",
        //    _zit_data->extCentral.nAllNum,
        //    Time2Index(g_ctpsec, _zit_data->extCentral.extremum[_zit_data->extCentral.nAllNum - 1].ftime) - Time2Index(g_ctpsec, _zit_data->extCentral.extremum[0].ftime),
        //    Time2Index(g_ctpsec, _zit_data->extCentral.extremum[0].ftime),
        //    Time2Index(g_ctpsec, _zit_data->extCentral.extremum[_zit_data->extCentral.nAllNum - 1].ftime));

    	_artist->SetSolidColorBrush(D2D1::ColorF(BGR(0, 255, 0), 0.5f));
        for (int i = 0; i < _zit_data->extCentral.nAllNum; i++) {
            DrawPoint(_zit_data->extCentral.extremum[i].ftime,
                      _zit_data->extCentral.extremum[i].fOrngValue,
                      _zit_data->extCentral.extremum[i].nType);
            //MYTRACE(L"ext %d %d %.02f %.02f %.02f\n",
            //    _zit_data->extCentral.extremum[i].nIndex,
            //    _zit_data->extCentral.extremum[i].nType,
            //    _zit_data->extCentral.extremum[i].fOrngValue,
            //    _zit_data->extCentral.extremum[i].fValue,
            //    _zit_data->extCentral.extremum[i].fMagenitude);
        }
    }

    DrawArea();

    MATRIX_2D newtrans = *(_referframe->GetTransform());
    newtrans._31 += static_cast<float>(_rect.left),
    newtrans._32 += static_cast<float>(_rect.top);
    _artist->SetTransform(&newtrans);
	//MYTRACE(L"newtrans %.02f %.02f %.02f %.02f %.02f %.02f\n", 
	//    newtrans._11, newtrans._12, newtrans._21, newtrans._22, newtrans._31, newtrans._32);

    if (_zit_data->inflBest.nNum) {

    	_artist->SetSolidColorBrush(D2D1::ColorF(BGR(0, 255, 255), 0.5f));

        for (int i = 0; i < _zit_data->inflBest.nNum; i++) {
            //MYTRACE(L"inf %d: %.02f %.02f %.02f - %.02f %.02f %.02f\n",
             //   i,
            //    Time2Index(g_ctpsec, _zit_data->inflBest.infl[i].fstime),
            //    _zit_data->inflBest.infl[i].fsValue,
            //    _zit_data->inflBest.infl[i].fstime,
            //    Time2Index(g_ctpsec, _zit_data->inflBest.infl[i].fetime),
             //   _zit_data->inflBest.infl[i].feValue,
             //   _zit_data->inflBest.infl[i].fetime);
            _artist->DrawLine(
                Time2Index(g_ctpsec, _zit_data->inflBest.infl[i].fstime),
                _zit_data->inflBest.infl[i].fsValue,
                Time2Index(g_ctpsec, _zit_data->inflBest.infl[i].fetime),
                _zit_data->inflBest.infl[i].feValue,
                0.02f);
        }
        //MYTRACE(L"\n");
    }

    if (_zit_data->central.nNum) {
        //MYTRACE(L"cnt %.02f %.02f %.02f - %.02f %.02f %.02f\n",
        //    Time2Index(g_ctpsec, _zit_data->central.central[0].fstime),
        //    _zit_data->central.central[0].fstime,
        //    _zit_data->lha.lha[0].fArea,
        //    Time2Index(g_ctpsec, _zit_data->central.central[_zit_data->central.nNum - 1].fetime),
        //    _zit_data->central.central[_zit_data->central.nNum - 1].fetime,
        //    _zit_data->lha.lha[_zit_data->lha.nNum - 1].fArea);
        //MYTRACE(L"\n");

        _artist->SetSolidColorBrush(D2D1::ColorF(BGR(128, 0, 128), 1.0f));

        for (int i = 0; i < _zit_data->central.nNum; i++) {
            _artist->DrawRectangle(
                Time2Index(g_ctpsec, _zit_data->central.central[i].fstime),
                _zit_data->central.central[i].stat.fMin,
                Time2Index(g_ctpsec, _zit_data->central.central[i].fetime),
                _zit_data->central.central[i].stat.fMax,
                0.02f);

            //MYTRACE(L"Central %.02f, %.02f\n",
            //    Time2Index(g_ctpsec, _zit_data->central.central[i].fetime) - Time2Index(g_ctpsec, _zit_data->central.central[i].fstime),
            //    _zit_data->central.central[i].stat.fMax - _zit_data->central.central[i].stat.fMin);
        }
    }
    _artist->GetUsingRT()->SetAntialiasMode(am);
    _artist->SetTransform(&rect_trans);
    _artist->PopLayer();
    _artist->SetTransform(&backtrans);
    //MYTRACE(L"zit graph restore trans %s\n", name);

    return S_OK;
}


GLYPH_CHANGED_TYPE CZITGraph::AppendData(DataObjectPtr dopNewData)
{
    return GLYPH_CHANGED_TYPE_NONE;
}



HRESULT CZITGraph::DrawPoint(double x, float y, int type)
{
    HRESULT hr = S_OK;
    float xx = Time2Index(g_ctpsec, x);
	
    MATRIX_2D m;
	_artist->GetTransform(&m);
	//MYTRACE(L"ReferFrame %.02f %.02f %.02f %.02f %.02f %.02f =>", 
	//	m._11, m._12, m._21, m._22, m._31, m._32);

    _artist->GetStrokeStyle();
    if (_referframe) {
        POINT pnt = _referframe->Transform(xx, y);

        if (type > 0) {
            _artist->DrawLine(
                static_cast<float>(pnt.x - 2),
                static_cast<float>(pnt.y - 2),
                static_cast<float>(pnt.x + 2),
                static_cast<float>(pnt.y + 2),
                1.0f);
            _artist->DrawLine(
                static_cast<float>(pnt.x + 2),
                static_cast<float>(pnt.y - 2),
                static_cast<float>(pnt.x - 2),
                static_cast<float>(pnt.y + 2),
                1.0f);
        } else {
            _artist->DrawLine(
                static_cast<float>(pnt.x - 2),
                static_cast<float>(pnt.y),
                static_cast<float>(pnt.x + 2),
                static_cast<float>(pnt.y),
                1.0f);
            _artist->DrawLine(
                static_cast<float>(pnt.x),
                static_cast<float>(pnt.y - 2),
                static_cast<float>(pnt.x),
                static_cast<float>(pnt.y + 2),
                1.0f);
        }


        //MYTRACE(L"%.02f (%.02f) %.02f => %d %d\n", x, xx, y, pnt.x, pnt.y);
    }
    return hr;
}

GLYPH_CHANGED_TYPE CZITGraph::NewData(IDataContainer* data_cont, DataObject::MARKET_DATA_TYPE datatype)
{
    return GLYPH_CHANGED_TYPE_NONE;
}

GLYPH_CHANGED_TYPE CZITGraph::NewData(dataptr pdata, bool need_renew/* = true*/)
{
    _zit_data = (ZitData*)pdata;
    //DrawGraph(need_renew);
    return GLYPH_CHANGED_TYPE_CHANGED;
}


HRESULT CZITGraph::Init()
{
    _crAreaUpText = D2D1::ColorF(1.0f, .0f, .0f, 1.0f),
        _crAreaDownText = D2D1::ColorF(.0f, 1.0f, .0f, 1.0f);

    FONT font;
    font.fontSize = 12;
    _tcscpy_s(font.fontName, 64, L"Arial");

    if (!CDxFactorys::GetInstance()->CreateTextFormat(font, &_pAreaFont))
        return (-1);
    return IDataGraph::Init();
}


HRESULT CZITGraph::DrawArea()
{
    if (_zit_data->lha.nNum > 1)
    {
        RECT rect;
        rect.right = RectWidth(_rect) - 5, rect.left = rect.right - 50, rect.top = 5, rect.bottom = rect.top + 20;
        TCHAR text[MAX_PATH];
        _sntprintf_s(text, MAX_PATH, _TRUNCATE, L"%.02f", _zit_data->lha.lha[_zit_data->lha.nNum - 1].fArea);

        if (_zit_data->lha.lha[_zit_data->lha.nNum - 1].fArea > 0)
            _artist->SetSolidColorBrush(_crAreaUpText);
        else
            _artist->SetSolidColorBrush(_crAreaDownText);
        _artist->SetTextFormat(_pAreaFont);
        //MYTRACE(L"area font %x %d %d %d %d\n", _pAreaFont, rect.left, rect.top, rect.right, rect.bottom);
        _artist->DrawTextW(text, rect);

        rect.top = rect.bottom + 5, rect.bottom = rect.top + 20;

        _sntprintf_s(text, MAX_PATH, _TRUNCATE, L"%.02f", _zit_data->lha.lha[_zit_data->lha.nNum - 2].fArea);

        if (_zit_data->lha.lha[_zit_data->lha.nNum - 2].fArea > 0)
            _artist->SetSolidColorBrush(_crAreaUpText);
        else
            _artist->SetSolidColorBrush(_crAreaDownText);
        _artist->DrawTextW(text, rect);
    }

    return S_OK;
}
