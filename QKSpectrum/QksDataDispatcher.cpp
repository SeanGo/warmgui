#include "StdAfx.h"
#include "qks_incs.h"

void CQksDataDispatcher::SetConfig(CWarmguiConfig* config, const char* cnf_pos)
{
    IDataDispatcher::SetConfig(config, cnf_pos);
    //set other 
}


void CQksDataDispatcher::stop()
{
    _stop = 1;
    PostThreadMessage(_tid, MSG_EXIT_DISPATCHER, 0, 0);
    try {
        _thread.join(1000);
    }catch(...){
        MYTRACE(L"CQksDataDispatcher::stop()\n");
    }
}

int CQksDataDispatcher::MessageProcess(MSG msg)
{
	switch(msg.message) {
	case WM_REALTIME_CTPMMD:
		{
			DataObjectPtr dop = DataObjectPtr((DataObject*)msg.wParam);
			//const char* name = (const char*)msg.lParam;
			CTPMMD* ctpmmd = (CTPMMD*)dop->GetData();
            MYTRACE(L"CTPMMD %d %.02f %.02f %.02f %.02f\n",
				dop->GetDataLength(),
                ctpmmd->fIndex,
                ctpmmd->Volume,
                ctpmmd->LastPrice,
                ctpmmd->OpenInterest);
		}
		break;
		/////////////////////////////////////////////////////////////////////////////////
		// Market Data Interface Message
		//
	case WM_RECEIVED_CTPMMD_PACKAGEDATA:
		{
			DataObjectPtr dop = DataObjectPtr((DataObject*)msg.wParam);
			//const char* name = (const char*)msg.lParam;
            MYTRACE(L"i got CTPMMD %d\n", dop->GetDataLength());
		}
		break;
	case WM_CALUC_FINISHED:
		{
			DataObjectPtr dop = DataObjectPtr((DataObject*)msg.wParam);
			//const char* name = (const char*)msg.lParam;
            //MYTRACE(L"i got Caluc result %d\n", dataptr->GetDataLength());
            DCArray::const_iterator iter = _dcs.begin();
            IDataContainer* dc = *iter;
            dc->AddData(dop);
            //dc = *(++iter);
            //dc->AddData(dop);
			break;
		}
		break;
	case MSG_EXIT_DISPATCHER:
		return (-1);
	default:
		return (0);
	}

    return (0);
}


void CQksDataDispatcher::go()
{
	MSG msg;
	BOOL bRet = 1;

	while(bRet) {
		bRet = GetMessage( &msg, (HWND)-1, 0, 0);
		if (bRet == -1) {
			exit(-10000);
		} else {
            if (_stop || MessageProcess(msg) < 0)
                return;
			DispatchMessage(&msg);
		}
	}
}
