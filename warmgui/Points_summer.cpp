#include "StdAfx.h"
#include "warmgui_summer.h"

namespace WARMGUI {
CPoints_summer::CPoints_summer(const char* name)
    : IDataGraph_summer(name)
{
    SetClass();
}


CPoints_summer::~CPoints_summer(void)
{
}

static D2D1_POINT_2F pnt;
static float x1, y1, x2, y2;
HRESULT CPoints_summer::draw(float x, float y, char c, POINT_TEXT_POSISION pos, COLORALPHA clr)
{
    pnt = _world->get_trans()->TransformPoint(D2D1::Point2F(x, y));
    if (c == 'x')
        x1 = pnt.x - 2, y1 = pnt.y - 2, x2 = pnt.x + 2, y2 = pnt.y + 2;
    else //'+'
        x1 = pnt.x - 2, x2 = pnt.x + 2, y1 = y2 = pnt.y;

    //MYTRACE(L"%.02f, %.02f ---> %.02f, %.02f : translation : %d %d\n", x, y, pnt.x, pnt.y, _abs_rect.left, _abs_rect.top);
    _artist->SetSolidColorBrush(clr);
    _artist->DrawLine(x1, y1, x2, y2);

    return S_OK;
}

}// namespace WARMGUI