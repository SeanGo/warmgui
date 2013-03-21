#include "StdAfx.h"
#include "warmgui_incs.h"

namespace WARMGUI {
CWarmGridCtrl::CWarmGridCtrl(void)
    : ncol(0)
    , nrow(0)
    , nrhead(0)
    , nchead(0)
{
}

CWarmGridCtrl::CWarmGridCtrl(const TCHAR * name, bool own_artist/* = false*/)
    : IGlyph(name, own_artist)
{
}

CWarmGridCtrl::~CWarmGridCtrl(void)
{
}


HRESULT CWarmGridCtrl::DrawGraph(bool redraw/* = false*/)
{
    HRESULT hr = S_OK;
    return hr;
}

}

