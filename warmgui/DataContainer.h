#ifndef __warmgui_data_receiver_h_include__
#define __warmgui_data_receiver_h_include__

namespace WARMGUI {

EXPORT_STL_VECTOR(WARMGUI_API, CSeriesDataChart*)
typedef vector <CSeriesDataChart*> WARMGUI_API CChartArray;
typedef CChartArray::iterator ChartIter;
typedef CChartArray::const_iterator ConstChartIter;

class WARMGUI_API IDataContainer
{
public:
    IDataContainer(void);
    ~IDataContainer(void);

    dataptr getData() { return _pdata; }
    size_t getCount() { return _count; }
    size_t getDataSize() { return _sizeofdata; }

    virtual void AddData(DataObjectPtr dop);
    void SetContainerSize(size_t sizeofdata, size_t len);
    void SetConfig(CWarmguiConfig* config) {_config = config;}

    virtual void dispatch(DataObjectPtr dop);
    void RegisterChart(CSeriesDataChart* chart);

    void SetGeometryData();

    inline dataptr GetData(size_t nSpos);

protected:
    dataptr _pdata;
    size_t  _count, _sizeofdata, _size;

    CChartArray _chart_array;

    CWarmguiConfig* _config;
};

} //namespace WARMGUI

#endif //__warmgui_data_receiver_h_include__