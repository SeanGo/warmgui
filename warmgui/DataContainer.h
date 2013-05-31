#ifndef __warmgui_data_receiver_h_include__
#define __warmgui_data_receiver_h_include__

namespace WARMGUI {

class WARMGUI_API IDataContainer
{
public:
                            IDataContainer(const char* name);
    virtual                ~IDataContainer(void);

    ///get the configureation of data-container
    virtual void            SetConfig(CWarmguiConfig* config, const char* cnf_pos)
                            {
                                _config = config;
                                strcpy_s(_cnf_pos, MAX_PATH, cnf_pos);
                                try {
                                    //_my_disp;
                                    char temp[MAX_PATH];
                                    _snprintf_s(temp, MAX_PATH, "%s.dispatchers", cnf_pos);
                                    strcpy_s(_my_disp, MAX_PATH, _config->getString(temp).c_str());
                                } catch(...) {
                                }
                            }

    GLYPH_CHANGED_TYPE      AddData(DataObjectPtr dop);
    inline void             RegisterCalculator(ICalculator* calculator);
    inline void             RegisterCanvas(IDataCanvas* canvas);
    inline void             RegisterDataGraph(IDataGraph* graph);
    inline StringArray&     getDataNames()  { return  _sa_data_names; }
    bool                    isme(const char* nemas) 
                            { return (0 != strstr(nemas, _name)); }
    const char*             getDispatcherName() {return _my_disp;}
    const char*             getName() {return _name;}

private:
    virtual bool            NewData(DataObjectPtr dop) = 0;

protected:
    CalculatorArray       _cals;
    DataCanvasArray   _canvases;
    DataGraphArray      _dgraph;

    CWarmguiConfig*     _config;
    char     _cnf_pos[MAX_PATH];

    StringArray  _sa_data_names;
    char     _my_disp[MAX_PATH];
    char        _name[MAX_PATH];
};


EXPORT_STL_VECTOR(WARMGUI_API, IDataContainer*)

typedef std::vector<IDataContainer*> DCArray;
typedef DCArray::iterator             DCIter;
typedef DCArray::const_iterator  DCConstIter;

/*
class WARMGUI_API CCommonContainer : public IDataContainer
{
public:
    CCommonContainer(size_t sizeofdata, size_t len);

    virtual void    AddData(DataObjectPtr dop);
    void            SetContainerSize(size_t sizeofdata, size_t len);
    virtual dataptr GetData(size_t nSpos);

protected:
    POINT_f _pnt;
};
*/

class WARMGUI_API IDataContainerCreator
{
public:
    IDataContainerCreator () : _config(0) {}
    ~IDataContainerCreator(){}

    void SetConfigure(CWarmguiConfig* config) { _config = config; }
    virtual IDataContainer* CreateContainer(const char* conf_pos) = 0;

protected:
    CWarmguiConfig* _config;
};

} //namespace WARMGUI

#endif //__warmgui_data_receiver_h_include__
