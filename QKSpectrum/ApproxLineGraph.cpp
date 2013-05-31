#include "StdAfx.h"
#include "qks_incs.h"

CApproxLineGraph::CApproxLineGraph(const char* name, bool world_own_type/* = false*/, bool data_own_type/*  = false*/, bool own_artist/* = false*/)
    : CDataLineGraph(name, world_own_type, data_own_type, own_artist)
{
}


CApproxLineGraph::~CApproxLineGraph(void)
{
}
