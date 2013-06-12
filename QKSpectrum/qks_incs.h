#ifndef __qiankun_spectrum_h_include__
#define __qiankun_spectrum_h_include__

#include "pocoinc.h"

#include "utility.h"
#include "datatype.h"
#include "datastructio.h"
#include "ChineseCodeLib.h"
#include "MemShareFile.h"

#include "network_inc.h"
#include "clicomm.h"


#define   __euclid_calculator__ 1
#if (__euclid_calculator__)
#include "TimeSeries.h"
#include "Sampler.h"
#include "wavelet1d.h"
#include "DspFilter.h"
#include "MarketDataFilter.h"

#include "euclid.h"
#include "Extremum.h"
#include "VolumeTurn.h"
#include "Inflexion.h"

#include "sas.h"
#include "nr.h"
#include "Predict.h"
#include "Central.h"
#include "caluc_in_time.h"
#include "RealtimeCalc.h"
#include "CalCentral.h"
#include "DoublePredict.h"
#endif



#include "warmgui.h"
using namespace WARMGUI;

typedef struct ZitData {
   	EXTRINFO    extCentral;
	INFLGRP       inflBest;
	CENTRAL_SERIES central;
	LHAREA             lha;
    int            data_ds;

    double _short_prdt[180];
    double  _long_prdt[180];
    bool    _has_short_prdt;
    bool     _has_long_prdt;

    ZitData()
        :data_ds(0)
        , _has_short_prdt(false)
        ,  _has_long_prdt(false)
    {
        memset(&extCentral, 0, sizeof(extCentral));
        memset(&inflBest  , 0, sizeof(inflBest  ));
        memset(&central   , 0, sizeof(central   ));
        memset(&lha       , 0, sizeof(lha       ));


        extCentral.extremum = new EXTREMUM[200];
        inflBest.infl       = new INFLEXION[100];
        central.central     = new CENTRAL[200];
        lha.lha             = new L_H_AREA[200];

        memset(extCentral.extremum, 0, sizeof(EXTREMUM) * 200);
        memset(inflBest.infl, 0, sizeof(EXTREMUM) * 100);
        memset(central.central, 0, sizeof(CENTRAL) * 200);
        memset(lha.lha, 0, sizeof(L_H_AREA) * 200);
    }

    void reset()
    {
        dataptr dpr = extCentral.extremum;
        memset(&extCentral, 0, sizeof(extCentral));
        extCentral.extremum = (EXTREMUM*)dpr;
        memset(extCentral.extremum, 0, sizeof(EXTREMUM) * 200);

        dpr = inflBest.infl;
        memset(&inflBest  , 0, sizeof(inflBest  ));
        inflBest.infl = (INFLEXION*)dpr;
        memset(inflBest.infl, 0, sizeof(EXTREMUM) * 100);


        dpr = central.central;
        memset(&central   , 0, sizeof(central   ));
        central.central = (CENTRAL*)dpr;
        memset(central.central, 0, sizeof(CENTRAL) * 200);

        dpr = lha.lha;
        memset(&lha       , 0, sizeof(lha       ));
        lha.lha = (L_H_AREA*)dpr;
        memset(lha.lha, 0, sizeof(L_H_AREA) * 200);

        _has_short_prdt = false;
         _has_long_prdt = false;

    }

    ~ZitData()
    {
        SafeDelete(extCentral.extremum);
        SafeDelete(inflBest.infl);
        SafeDelete(central.central);
        SafeDelete(lha.lha);
    }

    bool AppendData(dataptr zitdata, size_t length);
} ZitData;



#include "CtpmmdContainer.h"
#include "ZitContainer.h"
#include "QksDataDispatcher.h"
#include "CtpDispatcher.h"

#include "EuclidCalculator.h"
#include "PredictorCalculator.h"
#include "HistoryDataContainer.h"
#include "QksFactories.h"


#include "TickDataChart.h"
#include "PVI_Canvas.h"
#include "TickDataGridCtrl.h"
#include "TickdataCanvas.h"
#include "TickDataLayout.h"
#include "TickDataAtelier.h"

#include "ZITGraph.h"
#include "ZitCanvas.h"
#include "EuclidCanvas.h"
#include "EuclidLayout.h"
#include "EuclidAtelier.h"

#include "PVIAnalysisCanvas.h"
#include "AnalystCanvas.h"
#include "AnalystLayout.h"
#include "AnalystAtelier.h"


#include "AnalystView.h"
#include "TickDataView.h"
#include "EuclidView.h"


#include "qksapp.h"
extern qksapp the_app;

#endif //__qiankun_spectrum_h_include__