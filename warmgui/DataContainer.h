#ifndef __warmgui_data_receiver_h_include__
#define __warmgui_data_receiver_h_include__

namespace WARMGUI {

    
class WARMGUI_API IDataContainer
{
public:
    IDataContainer(void);
    ~IDataContainer(void);

    dataptr        getData() { return _pdata; }
    size_t         getCount() { return _count; }
    size_t         getDataSize() { return _sizeofdata; }

    virtual void   AddData(DataObjectPtr dop);
    void           SetContainerSize(size_t sizeofdata, size_t len);
    void           SetConfig(CWarmguiConfig* config) {_config = config;}


    inline dataptr GetData(size_t nSpos);

protected:
    dataptr          _pdata;
    size_t           _count;
    size_t      _sizeofdata;
    size_t            _size;

    CalculatorArray   _cals;
    CCanvasArray    _canvas;

    CWarmguiConfig* _config;
};


EXPORT_STL_VECTOR(WARMGUI_API, IDataContainer*)

typedef std::vector<IDataContainer*> DCArray;
typedef DCArray::iterator DCIter;
typedef DCArray::const_iterator DCConstIter;

} //namespace WARMGUI

#endif //__warmgui_data_receiver_h_include__