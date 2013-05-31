#include "StdAfx.h"
#include "qks_incs.h"

CZenInTwiningGraph::CZenInTwiningGraph(const char* name)
    : IDataGraph(name)
    , _down_sample(0)
{
    setClass();
}


CZenInTwiningGraph::~CZenInTwiningGraph(void)
{
}


HRESULT CZenInTwiningGraph::DrawGraph(bool /*redraw*/)
{
    if (_zit_data.data_ds != _down_sample) return S_OK;

    MATRIX_2D backtrans;
    _artist->GetTransform(&backtrans);


    D2D1_ANTIALIAS_MODE am = _artist->GetHwndRT()->GetAntialiasMode();
	_artist->GetUsingRT()->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

    if (_zit_data.extCentral.nAllNum) {
        //MYTRACE(L"ext %.02f %.02f %.02f - %.02f %.02f %.02f\n",
        //Time2Index(((CZenInTwiningAtelier*)_atelier)->GetTimeSec(), extCentral.extremum[0].ftime),
        //    extCentral.extremum[0].fValue,
        //    extCentral.extremum[0].ftime,
        //    Time2Index(((CZenInTwiningAtelier*)_atelier)->GetTimeSec(), extCentral.extremum[extCentral.nAllNum - 1].ftime),
        //    extCentral.extremum[extCentral.nAllNum - 1].fValue,
        //    extCentral.extremum[extCentral.nAllNum - 1].ftime);

        //MATRIX_2D* m = _referframe->GetTransform();
	    //MYTRACE(L"ReferFrame %s %.02f %.02f %.02f %.02f %.02f %.02f - ", 
		//    _name,
		//    m->_11, m->_12, m->_21, m->_22, m->_31, m->_32);

        MATRIX_2D newtrans = D2D1::Matrix3x2F::Identity();
        newtrans._31 = static_cast<float>(_canvas->GetRect().left + _rect.left),
        newtrans._32 = static_cast<float>(_canvas->GetRect().top + _rect.top);
        _artist->SetTransform(&newtrans);

    	_artist->SetSolidColorBrush(D2D1::ColorF(BGR(0, 255, 0), 1.0f));

        for (int i = 0; i < _zit_data.extCentral.nAllNum; i++) {
            DrawPoint(_zit_data.extCentral.extremum[i].ftime, _zit_data.extCentral.extremum[i].fOrngValue);
        }
        
    }

    MATRIX_2D newtrans = *(_referframe->GetTransform());
    newtrans._31 += static_cast<float>(_canvas->GetRect().left + _rect.left),
    newtrans._32 += static_cast<float>(_canvas->GetRect().top + _rect.top);
    _artist->SetTransform(&newtrans);

    if (_zit_data.inflBest.nNum) {

    	_artist->SetSolidColorBrush(D2D1::ColorF(BGR(255, 255, 0), 1.0f));

        for (int i = 0; i < _zit_data.inflBest.nNum; i++) {
            //MYTRACE(L"inf %d: %.02f %.02f %.02f - %.02f %.02f %.02f\n",
            //    i,
            //    Time2Index(((CZenInTwiningAtelier*)_atelier)->GetTimeSec(), inflBest.infl[i].fstime),
            //    inflBest.infl[i].fsValue,
            //    inflBest.infl[i].fstime,
            //    Time2Index(((CZenInTwiningAtelier*)_atelier)->GetTimeSec(), inflBest.infl[i].fetime),
            //    inflBest.infl[i].feValue,
            //    inflBest.infl[i].fetime);
            _artist->DrawLine(
                Time2Index(((CZenInTwiningAtelier*)_atelier)->GetTimeSec(), _zit_data.inflBest.infl[i].fstime),
                _zit_data.inflBest.infl[i].fsValue,
                Time2Index(((CZenInTwiningAtelier*)_atelier)->GetTimeSec(), _zit_data.inflBest.infl[i].fetime),
                _zit_data.inflBest.infl[i].feValue,
                0.1f);

        }
        //MYTRACE(L"\n");
    }

    if (_zit_data.central.nNum) {
        //MYTRACE(L"cnt %.02f %.02f %.02f - %.02f %.02f %.02f\n",
        //Time2Index(((CZenInTwiningAtelier*)_atelier)->GetTimeSec(), central.central[0].fstime),
        //central.central[0].fstime,
        //lha.lha[0].fArea,
        //Time2Index(((CZenInTwiningAtelier*)_atelier)->GetTimeSec(), central.central[central.nNum - 1].fetime),
        //central.central[central.nNum - 1].fetime,
        //lha.lha[lha.nNum - 1].fArea);
        //MYTRACE(L"\n");

        _artist->SetSolidColorBrush(D2D1::ColorF(BGR(0, 255, 255), 1.0f));

        for (int i = 0; i < _zit_data.central.nNum; i++)
            _artist->DrawRectangle(
                Time2Index(((CZenInTwiningAtelier*)_atelier)->GetTimeSec(), _zit_data.central.central[i].fstime),
                _zit_data.central.central[i].stat.fMax,
                Time2Index(((CZenInTwiningAtelier*)_atelier)->GetTimeSec(), _zit_data.central.central[i].fetime),
                _zit_data.central.central[i].stat.fMin,
                0.1f);
    }
    _artist->GetUsingRT()->SetAntialiasMode(am);
    _artist->SetTransform(&backtrans);

    return S_OK;
}


GLYPH_CHANGED_TYPE CZenInTwiningGraph::AppendData(DataObjectPtr dopNewData)
{
    //if (
    //return result;
    return GLYPH_CHANGED_TYPE_NONE;
}



HRESULT CZenInTwiningGraph::DrawPoint(double x, float y)
{
    HRESULT hr = S_OK;
    float xx = Time2Index(((CZenInTwiningAtelier*)_atelier)->GetTimeSec(), x);
	
    /*MATRIX_2D m;
	_artist->GetTransform(&m);
	MYTRACE(L"ReferFrame %s %.02f %.02f %.02f %.02f %.02f %.02f\n", 
		_name,
		m._11, m._12, m._21, m._22, m._31, m._32);
        */

    _artist->GetStrokeStyle();
    if (_referframe) {
        POINT pnt = _referframe->Transform(xx, y);

        _artist->DrawLine(
            static_cast<float>(pnt.x - 2),
            static_cast<float>(pnt.y - 2),
            static_cast<float>(pnt.x + 2),
            static_cast<float>(pnt.y + 2),
            0.3f);
        _artist->DrawLine(
            static_cast<float>(pnt.x + 2),
            static_cast<float>(pnt.y - 2),
            static_cast<float>(pnt.x - 2),
            static_cast<float>(pnt.y + 2),
            0.3f);

        //MYTRACE(L"%.02f %.02f %d %d ", x, xx, pnt.x, pnt.y);
    }
    return hr;
}

