#ifndef __warmgui_graph_data_h_include__
#define __warmgui_graph_data_h_include__

namespace WARMGUI {

class WARMGUI_API IGraphData
{
public:
    enum DATA_SOURCE_TYPE {
        DATA_SOURCE_TYPE_MYSELF,
        DATA_SOURCE_TYPE_MATLAB,
        DATA_SOURCE_TYPE_R,
        DATA_SOURCE_TYPE_PYTHON,
        DATA_SOURCE_TYPE_TA_LIB,
    };

    IGraphData(void);
    IGraphData(const TCHAR* name, const TCHAR* lable, const DATA_SOURCE_TYPE source = DATA_SOURCE_TYPE_MYSELF, const dataptr pdata = 0, const size_t datasize = 0);
    virtual ~IGraphData(void);

    void SetLable(const TCHAR* lable) {_tcscpy_s(_lable, MAX_WARMGUI_NAME_LEN, lable);}
    void SetName (const TCHAR*  name) {_tcscpy_s(_name , MAX_WARMGUI_NAME_LEN, name );}
    void SetSource(const DATA_SOURCE_TYPE source) {_source = source;}
    void SetData(const dataptr data, size_t size) {  _pdata = data, _sizeof = size; }

    const TCHAR* GetLable(){ return _lable;}
    const TCHAR* GetName() { return  _name;}
    const DATA_SOURCE_TYPE GetSourceType() { return _source; }
    const dataptr GetData() { return _pdata; }
    const size_t GetDataSize() { return _sizeof; }

protected:
    TCHAR _lable[MAX_WARMGUI_NAME_LEN];     //the text to show
    TCHAR  _name[MAX_WARMGUI_NAME_LEN];     //the alpahbeta name of dataobject, lick A, B, alpha, beta
    DATA_SOURCE_TYPE           _source;     //where come from the data, default is myself
    size_t                     _sizeof;
    dataptr                     _pdata;      //pointer to the real data
};

}

#endif //__warmgui_graph_data_h_include__ 