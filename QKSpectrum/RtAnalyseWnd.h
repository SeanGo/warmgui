#ifndef _qkspectrum_realtime_analyse_window_h_
#define _qkspectrum_realtime_analyse_window_h_

class RtAnalyseWnd : public WARMGUI::CDxWindow
{
public:
    RtAnalyseWnd(void);
    ~RtAnalyseWnd(void);

    msgfun int OnCreate(LPCREATESTRUCT cs);
	msgfun int OnCommand(WORD nCmdId, WORD nSource, HWND hwnd);
};

#endif //_qkspectrum_realtime_analyse_window_h_