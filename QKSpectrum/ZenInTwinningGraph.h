#ifndef __zen_in_twining_graph_h__
#define __zen_in_twining_graph_h__

class CZenInTwiningGraph : public IGlyph
{
public:
    CZenInTwiningGraph(const TCHAR* name);
    ~CZenInTwiningGraph(void);

    virtual HRESULT DrawGraph(bool /*redraw = false*/);
    virtual GLYPH_CHANGED_TYPE AppendData(DataObjectPtr dopNewData);

    void SetDownSample(int ds) { _down_sample = ds; }

private:
    HRESULT DrawPoint(double x, float y);
    HRESULT DrawInflLine(INFLEXION& infl);

private:
   	EXTRINFO    extCentral;
	INFLGRP       inflBest;
	CENTRAL_SERIES central;
	LHAREA             lha;

    int       _down_sample;
    int           _data_ds;
};

#endif //__zen_in_twining_graph_h__