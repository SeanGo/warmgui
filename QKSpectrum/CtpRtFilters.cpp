#include "StdAfx.h"
#include "qks_incs.h"
#include "CtpRtFilters.h"


CtpRtFilters::CtpRtFilters(void)
/*
    :       tsShortest(0)
    ,       tpfCentral(0)
    ,          tsShort(0)
    ,    tpfPriceShort(0)
    ,    vfVolumeShort(0)
    , cifInterestShort(0)
    ,           tsLong(0)
    ,     tpfPriceLong(0)
    ,     vfVolumeLong(0)
    ,  cifInterestLong(0)
    ,          sampler(0)
    ,             _mmd(0)
    ,          _curpos(0)
*/
{
    memset(this, 0, sizeof(CtpRtFilters));
}


CtpRtFilters::~CtpRtFilters(void)
{
    SafeDelete(      tsShortest);
    SafeDelete(      tpfCentral);
    SafeDelete(         tsShort);
    SafeDelete(   tpfPriceShort);
    SafeDelete(   vfVolumeShort);
    SafeDelete(cifInterestShort);
    SafeDelete(          tsLong);
    SafeDelete(    tpfPriceLong);
    SafeDelete(    vfVolumeLong);
    SafeDelete( cifInterestLong);
    SafeDelete(         sampler);
    SafeDelete(            _mmd);
}


void CtpRtFilters::InitObjects()
{
    tsShortest       = new CCtpRealtimeSeries(); 
    tpfCentral       = new CTriplePriceFilter();  
    tpfPriceShort    = new CTriplePriceFilter(); 
    tsShort          = new CCtpRealtimeSeries(); 
    tsLong           = new CCtpRealtimeSeries(); 
    vfVolumeShort    = new      CVolumeFilter();
    cifInterestShort = new    CInterestFilter();
    tpfPriceLong     = new CTriplePriceFilter(); 
    vfVolumeLong     = new      CVolumeFilter();
    cifInterestLong  = new    CInterestFilter();
	sampler          = new         CTsSampler();
    
	tpfCentral->SetFilter(
		CEuclidConfig::GetInstance()->GetSng1Config()->Triple_Filter_WvltLevel,
		CEuclidConfig::GetInstance()->GetSng1Config()->Triple_Filter_HLTYPE,
		CEuclidConfig::GetInstance()->GetSng1Config()->Triple_Filter_LowLevel,
		CEuclidConfig::GetInstance()->GetSng1Config()->Triple_Filter_HiLevel);
	tpfCentral->SetSignal(tsShortest->GetTimeSeries());
	tsShortest->SetLength(CEuclidConfig::GetInstance()->GetSng1Config()->m_nWinLen1);

	tsShort->SetLength(CEuclidConfig::GetInstance()->GetSng1Config()->m_nWinLen2);
	tsLong->SetLength(CEuclidConfig::GetInstance()->GetSng1Config()->m_nWinLen3);

	sampler->SetSignal(tsLong->GetTimeSeries());
	sampler->SetInterval(2, 2.0);

	tpfPriceShort->SetFilter(
		CEuclidConfig::GetInstance()->GetSng1Config()->m_nApproxLevel,
		1,
		CEuclidConfig::GetInstance()->GetSng1Config()->m_nWvLowLevel,
		CEuclidConfig::GetInstance()->GetSng1Config()->m_nWvHighLevel);
	tpfPriceShort->SetSignal(tsShort->GetTimeSeries());

	vfVolumeShort->SetLevel(CEuclidConfig::GetInstance()->GetSng1Config()->m_nApproxLevel);
	cifInterestShort->SetLevel(CEuclidConfig::GetInstance()->GetSng1Config()->m_nApproxLevel);

	tpfPriceLong->SetFilter(
		CEuclidConfig::GetInstance()->GetSng1Config()->m_nApproxLevel,
		1,
		CEuclidConfig::GetInstance()->GetSng1Config()->m_nWvLowLevel,
		CEuclidConfig::GetInstance()->GetSng1Config()->m_nWvHighLevel);
	tpfPriceLong->SetSignal(tsLong->GetTimeSeries());

	vfVolumeLong->SetLevel(CEuclidConfig::GetInstance()->GetSng1Config()->m_nApproxLevel);

	cifInterestLong->SetLevel(CEuclidConfig::GetInstance()->GetSng1Config()->m_nApproxLevel);
}

