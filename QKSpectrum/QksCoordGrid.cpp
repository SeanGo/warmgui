#include "StdAfx.h"
#include "qks_incs.h"


CQksCoordGrid::CQksCoordGrid(const char* name)
    : CCoordGrid(name)
    , predict_len(0)
{
}


CQksCoordGrid::~CQksCoordGrid(void)
{
}


inline HRESULT CQksCoordGrid::Init()
{
    CCoordGrid::Init();

    //char key[MAX_PATH];
    //_snprintf_s(key, MAX_PATH, _TRUNCATE, "%s.increase", _strconf);
    //bool b = _config->getValueIncrease(_vi, key);

    return S_OK;
}

inline  bool CQksCoordGrid::AddNewData(DataObjectPtr dop)
{
    return true;
}

