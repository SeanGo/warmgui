#include "stdafx.h"
#include "qks_incs.h"


bool CZitdataContainer::NewData(DataObjectPtr dop)
{
    char* buf = (char*)dop->GetData();
    size_t len = dop->GetDataLength();

    if ((DataObject::MARKET_DATA_TYPE)(*(int*)buf) == DataObject::EUCLID_CALUC_TO_TRANSMIT) {
        buf += sizeof(int), len -= sizeof(int);

        return _zit_data.AppendData(buf, len);
    }
    return false;
}

