#ifndef __approximate_line_graph_h__
#define __approximate_line_graph_h__

class CApproxLineGraph :public WARMGUI::CDataLineGraph
{
public:
	CApproxLineGraph(const char* name, bool world_own_type = false, bool data_own_type  = false, bool own_artist = false);

    ~CApproxLineGraph(void);
};

#endif //__approximate_line_graph_h__