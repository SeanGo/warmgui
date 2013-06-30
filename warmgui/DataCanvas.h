#ifndef __warmgui_realtime_data_h_include__
#define __warmgui_realtime_data_h_include__

namespace WARMGUI {

class IDataContainer;

class WARMGUI_API IDataCanvas :  public ICanvas
{
public:
    inline                     IDataCanvas(const char* name);
    inline                     ~IDataCanvas(void);

    virtual HRESULT            Init();
    virtual void               SetGlyphRect();

    virtual GLYPH_CHANGED_TYPE NewData(IDataContainer* data_cont, MARKET_DATA_TYPE datatype) = 0;
    virtual GLYPH_CHANGED_TYPE NewData(DataObjectPtr dop) { return GLYPH_CHANGED_TYPE_NONE;}
    virtual GLYPH_CHANGED_TYPE NewData(dataptr pdata, size_t datalen, MARKET_DATA_TYPE datatype) { return GLYPH_CHANGED_TYPE_NONE;}

  	inline  void               SetWorldRect(WORLD_RECT& limit);
	inline  void               SetWorldRect(float minx, float maxx, float miny, float maxy, float x0, float y0);
    const   char*              getContainerName() {return _my_cont;}
    const   char*              getName() {return _name;}

	//inline  CWorld*            GetCartesian() { return &_cart; }

    //inline  virtual void       setDataContainer(IDataContainer* dc);
private:
    virtual void               setClass() { SetMyClass("IDataCanvas"); } //set class name, by IObject

protected:
    CBlind*            _blind;
	//CWorld            _cart;
    //CCoordFrame*      _coord;
    GlyphTreeIter _glyph_iter;      //the data glyph start here
    //IDataContainer*       _dc;
    char   _my_cont[MAX_PATH];
};


EXPORT_STL_VECTOR(WARMGUI_API, IDataCanvas*)

typedef vector <IDataCanvas*> WARMGUI_API DataCanvasArray;
typedef DataCanvasArray::iterator DataCanvasIter;
typedef DataCanvasArray::const_iterator DataCanvasConstIter;


}

#endif //__warmgui_realtime_data_h_include__

