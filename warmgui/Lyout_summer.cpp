#include "StdAfx.h"
#include "warmgui_summer.h"


namespace WARMGUI {

COneLayout_summer::COneLayout_summer()
{
}
COneLayout_summer::~COneLayout_summer()
{
}

void COneLayout_summer::Disposal(IAtelier_summer* atelier, RECT& rect)
{
    atelier->set_rect(rect);
}

}//namespace WARMGUI
