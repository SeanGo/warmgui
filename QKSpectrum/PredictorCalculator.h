#ifndef __predictor_calculator_h__
#define __predictor_calculator_h__


class CPredictorCalculator : public WARMGUI::ICalculator
{
public:
    CPredictorCalculator(const char* name);
    ~CPredictorCalculator(void);


    inline virtual int  calculate(MSG msg);
    inline virtual void stop();
    inline virtual bool init();
    inline virtual void start();

    bool                zitdata(DataObject* dataobj);
    //virtual GLYPH_CHANGED_TYPE NewData(IDataContainer* data_cont, DataObject::MARKET_DATA_TYPE datatype);
    void SetWindow(HWND hwnd) { _hwnd = hwnd; }

    GLYPH_CHANGED_TYPE AddData(dataptr pdata, size_t datalen, DataObject::MARKET_DATA_TYPE datatype)
    {
        GLYPH_CHANGED_TYPE change = GLYPH_CHANGED_TYPE_NONE;

        //canvases
        for (DataCanvasArray::const_iterator iter = _canvases.begin(); iter != _canvases.end(); iter++) {
            GLYPH_CHANGED_TYPE c = (*iter)->NewData(pdata, datalen, datatype);
            change = (GLYPH_CHANGED_TYPE)((int)change | (int)c);
        }

        //calculators
        for (CalConstIter iter = _cals.begin(); iter != _cals.end(); iter++) {
            (*iter)->NewData(pdata, datatype);
        }

        //data graphs
        for (DataGraphConstIter iter = _dgraph.begin(); iter != _dgraph.end(); iter++) {
            GLYPH_CHANGED_TYPE c = (*iter)->NewData(pdata, datatype);
            change = (GLYPH_CHANGED_TYPE)((int)change | (int)c);
        }

        return change;
    }

protected:
    //EUCLID::RtCalculate_ctpmmd _rtc;
    EUCLID::CDoubleFilterPredictor _prdtor;
    CTPMMD*                  _ctpmmd;
    HWND                       _hwnd;
    char             _name[MAX_PATH];
};

#endif //__predictor_calculator_h__