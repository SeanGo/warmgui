#ifndef __warmgui_summer_ctpmdif_dispatcher__
#define __warmgui_summer_ctpmdif_dispatcher__


class CCtpmdDispatcher_summer : public WARMGUI::IDispatcher_summer
{
public:
    CCtpmdDispatcher_summer(const char* name);
    ~CCtpmdDispatcher_summer(void);

    virtual void go()  ;
    virtual void stop();
    virtual bool init();

    int get_code_num();
    char* get_code(int index)   { return _codes[index] ; }
    CTPMMD* get_data(int index, int* count)
    { 
        if (_ctpmmd && _ctpmmd[index] && _count && _count[index])
        {
            *count = _count[index] + 1;
            return _ctpmmd[index];
        } else {
            return (0);
        }
    }
private:
    CCtpmdReader _reader;
    int*         _curpos;
    int*         _count;
    CTPTIMESEC*  _ctp_time_sec;
    char**       _codes;
    float*       _first_interest;
    CTPMMD**     _ctpmmd;
    int          _max_len;

    void clear();
};

#endif //__warmgui_summer_ctpmdif_dispatcher__