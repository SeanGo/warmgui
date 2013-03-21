#ifndef __warmgui_data_object_h_include__
#define __warmgui_data_object_h_include__

namespace WARMGUI {

typedef void* DataPointer;

class WARMGUI_API CData
{
public:
    enum DATA_SOURCE_TYPE {
        DATA_SOURCE_TYPE_MYSELF,
        DATA_SOURCE_TYPE_MATLAB,
        DATA_SOURCE_TYPE_R,
        DATA_SOURCE_TYPE_PYTHON,
        DATA_SOURCE_TYPE_TA_LIB,
    };

    CData(void);
    CData(const TCHAR* name, const TCHAR* lable, const DATA_SOURCE_TYPE source = DATA_SOURCE_TYPE_MYSELF, const DataPointer data = 0);
    ~CData(void);

    void SetLable(const TCHAR* lable) {_tcscpy_s(_lable, MAX_WARMGUI_NAME_LEN, lable);}
    void SetName (const TCHAR*  name) {_tcscpy_s(_name , MAX_WARMGUI_NAME_LEN, name );}
    void SetSource(const DATA_SOURCE_TYPE source) {_source = source;}
    void SetData(const DataPointer data) {  _data = data  ;}

    const TCHAR* GetLable(){ return _lable;}
    const TCHAR* GetName() { return  _name;}
    const DATA_SOURCE_TYPE GetSourceType() { return _source; }
    const DataPointer GetData() { return _data; }

protected:
    TCHAR _lable[MAX_WARMGUI_NAME_LEN];     //the text to show
    TCHAR  _name[MAX_WARMGUI_NAME_LEN];     //the alpahbeta name of dataobject, lick A, B, alpha, beta
    DATA_SOURCE_TYPE           _source;     //where come from the data, default is myself

    DataPointer _data;  //pointer to the real data
};

}

#endif //__warmgui_data_object_h_include__