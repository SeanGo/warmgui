#include "StdAfx.h"
#include "warmgui_summer.h"
#include "TestDispatcher_summer.h"


CTestDispatcher_summer::CTestDispatcher_summer(const char* name)
    : IDispatcher_summer(name)
{
}


CTestDispatcher_summer::~CTestDispatcher_summer(void)
{
}

bool CTestDispatcher_summer::init()
{
    pset1.init(1024);
    pset2.init(1024);

    srand(time(0));

    *pset1.x = *pset2.x = 0;
    *pset1.y = (float)(rand() % 21 + 10);
    *pset2.y = (float)(rand() % 21 + 10);

    for (int i = 1; i < 1024; i++) {
        //pset1.x[i] = pset1.x[i - 1] + rand() % 2 + 1;
        //pset2.x[i] = pset2.x[i - 1] + rand() % 2 + 1;
        pset1.x[i] = pset1.x[i - 1] + 1;
        pset2.x[i] = pset2.x[i - 1] + rand() % 2 + 1;
        pset1.y[i] = (float)(rand() % 21 + 10);
        pset2.y[i] = (float)(rand() % 21 + 10);
    }

    pset1.count = pset2.count = 1024;

    return true;
}

void CTestDispatcher_summer::go()
{
    for (int i = 0; !_stop && i < 100; i++) // pset1.count; i++)
    {
        DataObject* dataobj = new DataObject(sizeof(POINTF), 1);
        POINTF* point = (POINTF*)dataobj->GetData();
        point->x = pset1.x[i], point->y = pset1.y[i];

        DataObjectPtr dop(dataobj);

        dispatch_data(dop);

        Sleep(100);
    }

    _stop = 1;
    _tid  = 0;
}


void CTestDispatcher_summer::stop()
{
    _stop = 1;
}
