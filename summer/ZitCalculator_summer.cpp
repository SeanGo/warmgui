#include "StdAfx.h"
#include "warmgui_summer.h"
#include "summer.h"

CZitCalculator_summer::CZitCalculator_summer(const char* name)
    : WARMGUI::ICalculator_summer(name)
{
}


CZitCalculator_summer::~CZitCalculator_summer(void)
{
}

int  CZitCalculator_summer::calculate(MSG msg)
{
    CTPMMD* mmd = (CTPMMD*)msg.wParam;
    int pos = msg.lParam;

    return (0);
}

bool CZitCalculator_summer::init()
{
    return (true);
}
