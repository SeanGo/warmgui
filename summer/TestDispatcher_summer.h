#ifndef __warmgui_dispatcher_summer__
#define __warmgui_dispatcher_summer__

class CTestDispatcher_summer : public WARMGUI::IDispatcher_summer
{
public:
    CTestDispatcher_summer(const char* name);
    ~CTestDispatcher_summer(void);

    virtual bool init();
    virtual void go();
    virtual void stop();

private:
    void generate_dataset();

private:
    DOUBLE_DATA_POINTER pset1;
    DOUBLE_DATA_POINTER pset2;
};


#endif