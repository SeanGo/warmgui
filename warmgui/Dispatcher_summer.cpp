#include "StdAfx.h"
#include "pocoinc.h"
#include "warmgui_summer.h"

namespace WARMGUI {

IDispatcher_summer::IDispatcher_summer(const char* name)
{
    strcpy_s(_name, MAX_PATH, name);
}


IDispatcher_summer::~IDispatcher_summer(void)
{
}


inline void IDispatcher_summer::dispatch_data(dataptr data)
{
    //calculators
    for (int i = 0; i < _cals.size(); i++)
        _cals[i]->update(data);

    //data graphs
    for (int i = 0; i < _dgraph.size(); i++)
        _dgraph[i]->update(data);

    for (int i = 0; i < _atelier_array.size(); i++)
        _atelier_array[i]->redraw_window(false);
}

inline void IDispatcher_summer::dispatch_data(DataObjectPtr dop)
{
    //calculators
    for (int i = 0; i < _cals.size(); i++)
        _cals[i]->update(dop);

    //data graphs
    for (int i = 0; i < _dgraph.size(); i++)
        _dgraph[i]->update(dop);

    for (int i = 0; i < _atelier_array.size(); i++)
        _atelier_array[i]->redraw_window(false);
}


inline void IDispatcher_summer::register_atelier(IAtelier_summer* as)
{
    bool found = false;
    for (int i = 0; i < _atelier_array.size(); i++) {
        if (as == _atelier_array[i]) {
            found = true;
            break;
        }
    }
    if (!found)
        _atelier_array.push_back(as);
}

} //namespace WARMGUI {