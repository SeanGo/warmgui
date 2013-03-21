#include "StdAfx.h"
#include "qks_incs.h"

CZenInTwiningGraph::CZenInTwiningGraph(const TCHAR* name)
    : IGlyph(name)
    , _down_sample(0)
{
    memset(&extCentral, 0, sizeof(extCentral));
    memset(&inflBest  , 0, sizeof(inflBest  ));
    memset(&central   , 0, sizeof(central   ));
    memset(&lha       , 0, sizeof(lha       ));


    extCentral.extremum = new EXTREMUM[200];
    inflBest.infl       = new INFLEXION[100];
    central.central     = new CENTRAL[200];
    lha.lha             = new L_H_AREA[200];
}


CZenInTwiningGraph::~CZenInTwiningGraph(void)
{
    delete extCentral.extremum;
    delete inflBest.infl;
    delete central.central;
    delete lha.lha;
}


HRESULT CZenInTwiningGraph::DrawGraph(bool /*redraw*/)
{
    if (_data_ds != _down_sample) return S_OK;

    MATRIX_2D backtrans;
    _artist->GetTransform(&backtrans);

    D2D1_ANTIALIAS_MODE am = _artist->GetHwndRT()->GetAntialiasMode();
	_artist->GetUsingRT()->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

    if (extCentral.nAllNum) {
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

        for (int i = 0; i < extCentral.nAllNum; i++) {
            DrawPoint(extCentral.extremum[i].ftime, extCentral.extremum[i].fOrngValue);
        }
        
    }

    MATRIX_2D newtrans = *(_referframe->GetTransform());
    newtrans._31 += static_cast<float>(_canvas->GetRect().left + _rect.left),
    newtrans._32 += static_cast<float>(_canvas->GetRect().top + _rect.top);
    _artist->SetTransform(&newtrans);

    if (inflBest.nNum) {

    	_artist->SetSolidColorBrush(D2D1::ColorF(BGR(255, 255, 0), 1.0f));

        for (int i = 0; i < inflBest.nNum; i++) {
            //MYTRACE(L"inf %d: %.02f %.02f %.02f - %.02f %.02f %.02f\n",
            //    i,
            //    Time2Index(((CZenInTwiningAtelier*)_atelier)->GetTimeSec(), inflBest.infl[i].fstime),
            //    inflBest.infl[i].fsValue,
            //    inflBest.infl[i].fstime,
            //    Time2Index(((CZenInTwiningAtelier*)_atelier)->GetTimeSec(), inflBest.infl[i].fetime),
            //    inflBest.infl[i].feValue,
            //    inflBest.infl[i].fetime);
            _artist->DrawLine(
                Time2Index(((CZenInTwiningAtelier*)_atelier)->GetTimeSec(), inflBest.infl[i].fstime),
                inflBest.infl[i].fsValue,
                Time2Index(((CZenInTwiningAtelier*)_atelier)->GetTimeSec(), inflBest.infl[i].fetime),
                inflBest.infl[i].feValue,
                0.1f);

        }
        //MYTRACE(L"\n");
    }

    if (central.nNum) {
        //MYTRACE(L"cnt %.02f %.02f %.02f - %.02f %.02f %.02f\n",
        //Time2Index(((CZenInTwiningAtelier*)_atelier)->GetTimeSec(), central.central[0].fstime),
        //central.central[0].fstime,
        //lha.lha[0].fArea,
        //Time2Index(((CZenInTwiningAtelier*)_atelier)->GetTimeSec(), central.central[central.nNum - 1].fetime),
        //central.central[central.nNum - 1].fetime,
        //lha.lha[lha.nNum - 1].fArea);
        //MYTRACE(L"\n");

        _artist->SetSolidColorBrush(D2D1::ColorF(BGR(0, 255, 255), 1.0f));

        for (int i = 0; i < central.nNum; i++)
            _artist->DrawRectangle(
                Time2Index(((CZenInTwiningAtelier*)_atelier)->GetTimeSec(), central.central[i].fstime),
                central.central[i].stat.fMax,
                Time2Index(((CZenInTwiningAtelier*)_atelier)->GetTimeSec(), central.central[i].fetime),
                central.central[i].stat.fMin,
                0.1f);
    }
    _artist->GetUsingRT()->SetAntialiasMode(am);
    _artist->SetTransform(&backtrans);

    return S_OK;
}

#define move_to_next_data(l) { len -= (l); if (len < 1) return result; buf += (l); }

GLYPH_CHANGED_TYPE CZenInTwiningGraph::AppendData(DataObjectPtr dopNewData)
{
    size_t len = dopNewData->GetDataLength();
    char*  buf = (char*)(dopNewData->GetData());

    GLYPH_CHANGED_TYPE result = GLYPH_CHANGED_TYPE_CHANGED;
    //pointer to ZIT data
    move_to_next_data(sizeof(CTPMMD));

    //down sampler
    _data_ds = *(int*)buf;
    if (_data_ds != _down_sample) return GLYPH_CHANGED_TYPE_NONE;
    move_to_next_data(sizeof(int));

    //get the number of extreme
    extCentral.nAllNum = *(int*)buf;
	if (extCentral.nAllNum < 0 || extCentral.nAllNum > 200)
        return result;

    //get the extremes
    move_to_next_data(sizeof(int));
    memcpy(extCentral.extremum, buf, sizeof(EXTREMUM) * extCentral.nAllNum);

    //get the number of inflexion
    move_to_next_data(sizeof(EXTREMUM) * extCentral.nAllNum);
    inflBest.nNum = *(int*) buf;
    if (inflBest.nNum < 0 || inflBest.nNum > 100)
        return result;

    //get the inflexions
    move_to_next_data(sizeof(int));
    memcpy(inflBest.infl, buf, sizeof(INFLEXION) * inflBest.nNum);
    
    //get the number of central 
    move_to_next_data(sizeof(INFLEXION) * inflBest.nNum);
    central.nNum = *(int*)buf;
    if (central.nNum < 0 || central.nNum > 200)
        return result;
    
    //get centrals
    move_to_next_data(sizeof(int));
    memcpy(central.central, buf, sizeof(CENTRAL) * central.nNum);
    
    //get number of the area
    move_to_next_data(sizeof(CENTRAL) * central.nNum);
    lha.nNum = *(int*)buf;
    if (lha.nNum < 0 || lha.nNum > 200)
        return result;

    //get the areas
    move_to_next_data(sizeof(int));
    memcpy(lha.lha, buf, sizeof(L_H_AREA) * lha.nNum);
    
    //buf += sizeof(L_H_AREA) * lha.nNum; len -= sizeof(L_H_AREA) * lha.nNum;

#   ifdef _DEBUG
    //MYTRACE(L"extremum %d, infl %d, central %d, area %d\n",
    //    extCentral.nAllNum, 
    //    inflBest.nNum,
    //    central.nNum,
    //    lha.nNum);
#   endif //_DEBUG

    return result;
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

