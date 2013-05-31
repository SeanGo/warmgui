#ifndef __data_line_chart_h__
#define __data_line_chart_h__

namespace WARMGUI {
class WARMGUI_API CDataLineChart : public IDataGraph
{
public:
	CDataLineChart(const char* name, bool world_own_type = false, bool data_own_type  = false, bool own_artist = false);
    virtual ~CDataLineChart(void);

    virtual GLYPH_CHANGED_TYPE NewData(IDataContainer* data_cont, DataObject::MARKET_DATA_TYPE datatype)
                               { return GLYPH_CHANGED_TYPE_NONE; }
    virtual HRESULT            DrawGraph(bool redraw = false);
    //virtual HRESULT            Draw(bool redraw = false);     ///draw the graph
    //virtual HRESULT            DrawBkg(bool redraw = false);  ///draw background
    //virtual HRESULT            Renew();                       ///renew by increasement
    virtual HRESULT            Init();
    virtual HRESULT            AddGraphs() = 0;
    void                       SetRect(RECT& rect);

    void                       SetGeometryData(dataptr pdata, int count, int datasize);
    virtual void               AddData(dataptr data);

    inline virtual void        BeginSetData(dataptr data);
    inline virtual void        AddDataToPathGeometry(dataptr data);
    inline virtual void        EndSetData();

    virtual bool               RenewData()    { return true; }
    virtual bool               process_data() { return true; }

    inline  void               SetCoordGrid(CCoordFrame* coord) {_coord = coord;}
    virtual HRESULT            RenewGraph(GLYPH_CHANGED_TYPE changed_type);
    virtual void               ResetWorldRect(IDataContainer* data_cont) {}
    void                       MoveBitmapToLeft();
    CWorld*                    getCoordWorld() { return _coord->getWorld(); }
    CCoordFrame*               getCoordFrame() { return _coord; } 

protected:
    CCoordFrame*         _coord;
    GlyphTreeIter _iter__lines;
    float         _stroke_width;

private:
    //set class name, by IObject
    virtual void setClass() { SetMyClass("CDataLineChart"); }
};

}//namespace WARMGUI

#endif //__data_line_chart_h__

