#ifndef __tickdata_canvas_h__
#define __tickdata_canvas_h__

class CTickdataCanvas : public WARMGUI::IDataCanvas
{
public:
                          CTickdataCanvas(const char* name);
                         ~CTickdataCanvas(void);

	virtual void          SetGlyphRect();   ///set children's rect
	virtual const HRESULT Init(const char* name = 0);  ///initial canvas, must set the name before init

    virtual GLYPH_CHANGED_TYPE NewData(DataObjectPtr data);
    virtual GLYPH_CHANGED_TYPE NewData(IDataContainer* data_cont, DataObject::MARKET_DATA_TYPE datatype);

protected:
    CBlind*                    _blind;
    CTickDataGridCtrl* _grid_tickdata;

    virtual void setClass() { SetMyClass("CTickdataCanvas"); }
};

#endif //__tickdata_canvas_h__