#ifndef __zit_calculator_summer__
#define __zit_calculator_summer__

class CZitCalculator_summer : public WARMGUI::ICalculator_summer
{
public:
    CZitCalculator_summer(const char* name);
    ~CZitCalculator_summer(void);

    inline virtual int  calculate(MSG msg);
    inline virtual bool init();

private:
    EUCLID::CCalCentral        _rtc;
    ZitData               _zit_data;
};


#endif //__zit_calculator_summer__