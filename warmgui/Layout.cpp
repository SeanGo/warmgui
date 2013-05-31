#include "StdAfx.h"
#include "warmgui.h"

namespace WARMGUI {

COneLayout::COneLayout()
{
}
COneLayout::~COneLayout()
{
}

void COneLayout::Disposal(IAtelier* atelier, RECT& rect)
{
	COneAtelier* oneAtelier = (COneAtelier*)atelier;
	ConstCanvasIter iter = oneAtelier->_canvasses.begin();
	for (; iter != oneAtelier->_canvasses.end(); iter++)
		(*iter)->SetRect(oneAtelier->_rect);
}

}//namespace WARMGUI
