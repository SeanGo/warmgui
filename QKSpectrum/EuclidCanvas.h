#ifndef __euclid_canvas_h__
#define __euclid_canvas_h__


class CEuclidCanvas : public WARMGUI::IDataCanvas
{
public:
    CEuclidCanvas(const char* name);
    ~CEuclidCanvas(void);

    virtual const HRESULT      Init(const char* name = 0);
	virtual void               SetGlyphRect();

    void                       setDataOffset(int pxo, int pyo);
    virtual GLYPH_CHANGED_TYPE NewData(DataObjectPtr data);
    virtual GLYPH_CHANGED_TYPE NewData(dataptr data, DataObject::MARKET_DATA_TYPE datatype);
    virtual GLYPH_CHANGED_TYPE NewData(IDataContainer* data_cont, DataObject::MARKET_DATA_TYPE datatype);

protected:
    bool _bHasBackground;
    CBlind*       _blind;

private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CEuclidCanvas"); }
};



#endif //__euclid_canvas_h__