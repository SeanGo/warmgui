#ifndef __ctp_realtime_filters_h__
#define __ctp_realtime_filters_h__

class CtpRtFilters
{
public:
    CtpRtFilters(void);
    ~CtpRtFilters(void);

    void InitObjects();

protected:
	CTPTIMESEC              ctpdatasec;
	unsigned int               _curpos;

	// for Central
	CCtpRealtimeSeries*     tsShortest;
	CTriplePriceFilter*     tpfCentral;
	// short term
	CCtpRealtimeSeries*        tsShort;
	CTriplePriceFilter*  tpfPriceShort;
	CVolumeFilter*       vfVolumeShort;
	CInterestFilter*  cifInterestShort;
	// long  term
	CCtpRealtimeSeries*         tsLong;
	CTriplePriceFilter*   tpfPriceLong;
	CVolumeFilter*        vfVolumeLong;
	CInterestFilter*   cifInterestLong;
	// long term
	CTsSampler*                sampler;
private:
	MyMarketData*                 _mmd;
};

#endif //__ctp_realtime_filters_h__
