#pragma once


namespace WARMGUI {
	class WARMGUI_API CWindow
	{
	public:
		CWindow(void);
		virtual ~CWindow(void);

		HWND GetSafeHwnd();
		HINSTANCE GetInstance();

		void Attach(HWND hwnd);
		void Dettach();
		virtual BOOL InitInstance(  HINSTANCE hInstance,
									HINSTANCE hPrevInstance,
									LPTSTR lpCmdLine,
									int nCmdShow,
									TCHAR * szClassName,
									LPTSTR szTitle);

		virtual void  RunMessageLoop();
		virtual BOOL PreCreateWindow(LPCREATESTRUCT cs);

		bool Create(TCHAR *  szClassName,
					TCHAR * szWindowName,
						   DWORD dwStyle,
							  RECT& rect,
					CWindow * pParentWnd,
					  LPVOID lpParam = 0,
					   int nShowState = SW_SHOW);

		virtual BOOL MoveTo(int x, int y, int cx, int cy);
		void WindowPaint();
		void SetParentWindow(CWindow * pParent) { _pParent = pParent; }

		HWND SafeHwnd() { return _hwnd; }
		bool LoadAccelerators(UINT acceid);

        void Close() {SendMessage(_hwnd, WM_CLOSE, 0, 0);}

	protected:
		HWND         _hwnd;
		HINSTANCE   _hInst;
		CWindow * _pParent;
		HACCEL     _hAccel;

	protected:
		///OnCreate must return 0 to continue the creation of the CWnd object. If the application returns ¨C1, the window will be destroyed. 
		msgfun int  OnCreate                    (LPCREATESTRUCT cs);
		msgfun void OnDestroy                   ();
		msgfun int  OnMove                      (int x, int y);
		msgfun void OnSize                      (UINT nType, int cx, int cy);
		msgfun int OnActivate                   (WPARAM wParam, LPARAM lParam);
		msgfun int OnSetfocus                   (HWND hLostFocus);
		msgfun int OnKillfocus                  (WPARAM wParam, LPARAM lParam);
		msgfun int OnEnable                     (WPARAM wParam, LPARAM lParam);
		msgfun int OnSetredraw                  (WPARAM wParam, LPARAM lParam);
		msgfun int OnSettext                    (WPARAM wParam, LPARAM lParam);
		msgfun int OnGettext                    (WPARAM wParam, LPARAM lParam);
		msgfun int OnGettextlength              (WPARAM wParam, LPARAM lParam);
		//msgfun void OnPaint                     (HDC hdc, LPPAINTSTRUCT ps);
		msgfun void OnDraw();
		msgfun int OnClose                      (WPARAM wParam, LPARAM lParam);
		msgfun int OnQueryendsession            (WPARAM wParam, LPARAM lParam);
		msgfun int OnQueryopen                  (WPARAM wParam, LPARAM lParam);
		msgfun int OnEndsession                 (WPARAM wParam, LPARAM lParam);
		msgfun int OnQuit                       (WPARAM wParam, LPARAM lParam);
		///An application should return nonzero if it erases the background; otherwise, it should return zero.
		msgfun int OnEraseBkgnd                 (HDC hdc);
		msgfun int OnSyscolorchange             (WPARAM wParam, LPARAM lParam);
		msgfun int OnShowwindow                 (WPARAM wParam, LPARAM lParam);
		msgfun int OnWininichange               (WPARAM wParam, LPARAM lParam);
		msgfun int OnSettingchange              (WPARAM wParam, LPARAM lParam);
		msgfun int OnDevmodechange              (WPARAM wParam, LPARAM lParam);
		msgfun int OnActivateapp                (WPARAM wParam, LPARAM lParam);
		msgfun int OnFontchange                 (WPARAM wParam, LPARAM lParam);
		msgfun int OnTimechange                 (WPARAM wParam, LPARAM lParam);
		msgfun int OnCancelmode                 (WPARAM wParam, LPARAM lParam);
		msgfun int OnSetcursor                  (WPARAM wParam, LPARAM lParam);
		msgfun int OnMouseactivate              (WPARAM wParam, LPARAM lParam);
		msgfun int OnChildactivate              (WPARAM wParam, LPARAM lParam);
		msgfun int OnQueuesync                  (WPARAM wParam, LPARAM lParam);
		msgfun int OnGetminmaxinfo              (WPARAM wParam, LPARAM lParam);
		msgfun int OnPainticon                  (WPARAM wParam, LPARAM lParam);
		msgfun int OnIconerasebkgnd             (WPARAM wParam, LPARAM lParam);
		msgfun int OnNextdlgctl                 (WPARAM wParam, LPARAM lParam);
		msgfun int OnSpoolerstatus              (WPARAM wParam, LPARAM lParam);
		msgfun int OnDrawItem                   (WPARAM wParam, LPARAM lParam);
		msgfun int OnMeasureItem                (WPARAM wParam, LPARAM lParam);
		msgfun int OnDeleteItem                 (WPARAM wParam, LPARAM lParam);
		msgfun int OnVkeyToItem                 (WPARAM wParam, LPARAM lParam);
		msgfun int OnCharToItem                 (WPARAM wParam, LPARAM lParam);
		msgfun int OnSetFont                    (WPARAM wParam, LPARAM lParam);
		msgfun int OnGetFont                    (WPARAM wParam, LPARAM lParam);
		msgfun int OnSetHotKey                  (WPARAM wParam, LPARAM lParam);
		msgfun int OnGetHotKey                  (WPARAM wParam, LPARAM lParam);
		msgfun int OnQuerydragicon              (WPARAM wParam, LPARAM lParam);
		msgfun int OnCompareitem                (WPARAM wParam, LPARAM lParam);
		msgfun int OnGetobject                  (WPARAM wParam, LPARAM lParam);
		msgfun int OnCompacting                 (WPARAM wParam, LPARAM lParam);
		msgfun int OnCommnotify                 (WPARAM wParam, LPARAM lParam);
		msgfun int OnWindowposchanging          (WPARAM wParam, LPARAM lParam);
		msgfun int OnWindowposchanged           (WPARAM wParam, LPARAM lParam);
		msgfun int OnPower                      (WPARAM wParam, LPARAM lParam);
		msgfun int OnCopydata                   (WPARAM wParam, LPARAM lParam);
		msgfun int OnCanceljournal              (WPARAM wParam, LPARAM lParam);
		msgfun int OnNotify                     (WPARAM wParam, LPARAM lParam);
		msgfun int OnInputlangchangerequest     (WPARAM wParam, LPARAM lParam);
		msgfun int OnInputlangchange            (WPARAM wParam, LPARAM lParam);
		msgfun int OnTcard                      (WPARAM wParam, LPARAM lParam);
		msgfun int OnHelp                       (WPARAM wParam, LPARAM lParam);
		msgfun int OnUserchanged                (WPARAM wParam, LPARAM lParam);
		msgfun int OnNotifyformat               (WPARAM wParam, LPARAM lParam);
		msgfun int OnContextmenu                (WPARAM wParam, LPARAM lParam);
		msgfun int OnStylechanging              (WPARAM wParam, LPARAM lParam);
		msgfun int OnStylechanged               (WPARAM wParam, LPARAM lParam);
		msgfun int OnDisplayChange              (WPARAM wParam, LPARAM lParam);
		msgfun int OnGeticon                    (WPARAM wParam, LPARAM lParam);
		msgfun int OnSeticon                    (WPARAM wParam, LPARAM lParam);
		msgfun int OnNccreate                   (WPARAM wParam, LPARAM lParam);
		msgfun int OnNcdestroy                  (WPARAM wParam, LPARAM lParam);
		msgfun int OnNccalcsize                 (WPARAM wParam, LPARAM lParam);
		msgfun int OnNchittest                  (WPARAM wParam, LPARAM lParam);
		msgfun int OnNcpaint                    (WPARAM wParam, LPARAM lParam);
		msgfun int OnNcactivate                 (WPARAM wParam, LPARAM lParam);
		msgfun int OnGetdlgcode                 (WPARAM wParam, LPARAM lParam);
		msgfun int OnSyncpaint                  (WPARAM wParam, LPARAM lParam);
		msgfun int OnNcmousemove                (WPARAM wParam, LPARAM lParam);
		msgfun int OnNclbuttondown              (WPARAM wParam, LPARAM lParam);
		msgfun int OnNclbuttonup                (WPARAM wParam, LPARAM lParam);
		msgfun int OnNclbuttondblclk            (WPARAM wParam, LPARAM lParam);
		msgfun int OnNcrbuttondown              (WPARAM wParam, LPARAM lParam);
		msgfun int OnNcrbuttonup                (WPARAM wParam, LPARAM lParam);
		msgfun int OnNcrbuttondblclk            (WPARAM wParam, LPARAM lParam);
		msgfun int OnNcmbuttondown              (WPARAM wParam, LPARAM lParam);
		msgfun int OnNcmbuttonup                (WPARAM wParam, LPARAM lParam);
		msgfun int OnNcmbuttondblclk            (WPARAM wParam, LPARAM lParam);
		msgfun int OnNcxbuttondown              (WPARAM wParam, LPARAM lParam);
		msgfun int OnNcxbuttonup                (WPARAM wParam, LPARAM lParam);
		msgfun int OnNcxbuttondblclk            (WPARAM wParam, LPARAM lParam);
		msgfun int OnInput_Device_Change        (WPARAM wParam, LPARAM lParam);
		msgfun int OnInput                      (WPARAM wParam, LPARAM lParam);
		
		//msgfun int OnKeyfirst                   (WPARAM wParam, LPARAM lParam);
		msgfun int OnKeydown                    (UINT nChar, UINT nRepCnt, UINT nFlags);
		msgfun int OnKeyup                      (UINT nChar, UINT nRepCnt, UINT nFlags);
		msgfun int OnChar                       (UINT nChar, UINT nRepCnt, UINT nFlags);
		msgfun int OnDeadchar                   (UINT nChar, UINT nRepCnt, UINT nFlags);
		msgfun int OnSyskeydown                 (UINT nChar, UINT nRepCnt, UINT nFlags);
		msgfun int OnSyskeyup                   (UINT nChar, UINT nRepCnt, UINT nFlags);
		msgfun int OnSyschar                    (UINT nChar, UINT nRepCnt, UINT nFlags);
		msgfun int OnSysdeadchar                (UINT nChar, UINT nRepCnt, UINT nFlags);
		msgfun int OnUnichar                    (UINT nChar, UINT nRepCnt, UINT nFlags);
		
		//msgfun int OnKeylast                    (WPARAM wParam, LPARAM lParam);
		msgfun int OnIme_Startcomposition       (WPARAM wParam, LPARAM lParam);
		msgfun int OnIme_Endcomposition         (WPARAM wParam, LPARAM lParam);
		msgfun int OnIme_Composition            (WPARAM wParam, LPARAM lParam);
		msgfun int OnIme_Keylast                (WPARAM wParam, LPARAM lParam);
		msgfun int OnInitdialog                 (WPARAM wParam, LPARAM lParam);
		msgfun int OnCommand                    (WORD nCmdId, WORD nSource, HWND hwnd);
		msgfun int OnSyscommand                 (WPARAM wParam, LPARAM lParam);
		msgfun int OnTimer                      (WPARAM wParam, LPARAM lParam);
		msgfun int OnHscroll                    (WPARAM wParam, LPARAM lParam);
		msgfun int OnVscroll                    (WPARAM wParam, LPARAM lParam);
		msgfun int OnInitmenu                   (WPARAM wParam, LPARAM lParam);
		msgfun int OnInitmenupopup              (WPARAM wParam, LPARAM lParam);
		msgfun int OnMenuselect                 (WPARAM wParam, LPARAM lParam);
		msgfun int OnMenuchar                   (WPARAM wParam, LPARAM lParam);
		msgfun int OnEnteridle                  (WPARAM wParam, LPARAM lParam);
		msgfun int OnMenurbuttonup              (WPARAM wParam, LPARAM lParam);
		msgfun int OnMenudrag                   (WPARAM wParam, LPARAM lParam);
		msgfun int OnMenugetobject              (WPARAM wParam, LPARAM lParam);
		msgfun int OnUninitmenupopup            (WPARAM wParam, LPARAM lParam);
		msgfun int OnMenucommand                (WPARAM wParam, LPARAM lParam);
		msgfun int OnChangeuistate              (WPARAM wParam, LPARAM lParam);
		msgfun int OnUpdateuistate              (WPARAM wParam, LPARAM lParam);
		msgfun int OnQueryuistate               (WPARAM wParam, LPARAM lParam);
		msgfun int OnCtlcolormsgbox             (WPARAM wParam, LPARAM lParam);
		msgfun int OnCtlcoloredit               (WPARAM wParam, LPARAM lParam);
		msgfun int OnCtlcolorlistbox            (WPARAM wParam, LPARAM lParam);
		msgfun int OnCtlcolorbtn                (WPARAM wParam, LPARAM lParam);
		msgfun int OnCtlcolordlg                (WPARAM wParam, LPARAM lParam);
		msgfun int OnCtlcolorscrollbar          (WPARAM wParam, LPARAM lParam);
		msgfun int OnCtlcolorstatic             (WPARAM wParam, LPARAM lParam);
		msgfun int OnGetHMenu                   (WPARAM wParam, LPARAM lParam);
		msgfun int OnMousefirst                 (WPARAM wParam, LPARAM lParam);
		msgfun void OnMouseMove    (UINT nFlags, int x, int y);
		msgfun void OnLButtonDown  (UINT nFlags, int x, int y);
		msgfun void OnLButtonUp    (UINT nFlags, int x, int y);
		msgfun void OnLButtonDblclk(UINT nFlags, int x, int y);
		msgfun void OnRButtonDown  (UINT nFlags, int x, int y);
		msgfun void OnRButtonUp    (UINT nFlags, int x, int y);
		msgfun void OnRButtonDblclk(UINT nFlags, int x, int y);
		msgfun void OnMButtonDown  (UINT nFlags, int x, int y);
		msgfun void OnMButtonUp    (UINT nFlags, int x, int y);
		msgfun void OnMButtonDblclk(UINT nFlags, int x, int y);
		msgfun BOOL OnMouseWheel   (UINT nFlags, short zDelta, int x, int y);
		msgfun int OnXbuttondown                (WPARAM wParam, LPARAM lParam);
		msgfun int OnXbuttonup                  (WPARAM wParam, LPARAM lParam);
		msgfun int OnXbuttondblclk              (WPARAM wParam, LPARAM lParam);
		msgfun int OnMousehwheel                (WPARAM wParam, LPARAM lParam);
		msgfun int OnMouselast                  (WPARAM wParam, LPARAM lParam);
		msgfun int OnParentnotify               (WPARAM wParam, LPARAM lParam);
		msgfun int OnEntermenuloop              (WPARAM wParam, LPARAM lParam);
		msgfun int OnExitmenuloop               (WPARAM wParam, LPARAM lParam);
		msgfun int OnNextmenu                   (WPARAM wParam, LPARAM lParam);
		msgfun int OnSizing                     (WPARAM wParam, LPARAM lParam);
		msgfun int OnCapturechanged             (WPARAM wParam, LPARAM lParam);
		msgfun int OnMoving                     (WPARAM wParam, LPARAM lParam);
		msgfun int OnPowerbroadcast             (WPARAM wParam, LPARAM lParam);
		msgfun int OnDevicechange               (WPARAM wParam, LPARAM lParam);
		msgfun int OnMdicreate                  (WPARAM wParam, LPARAM lParam);
		msgfun int OnMdidestroy                 (WPARAM wParam, LPARAM lParam);
		msgfun int OnMdiactivate                (WPARAM wParam, LPARAM lParam);
		msgfun int OnMdirestore                 (WPARAM wParam, LPARAM lParam);
		msgfun int OnMdinext                    (WPARAM wParam, LPARAM lParam);
		msgfun int OnMdimaximize                (WPARAM wParam, LPARAM lParam);
		msgfun int OnMditile                    (WPARAM wParam, LPARAM lParam);
		msgfun int OnMdicascade                 (WPARAM wParam, LPARAM lParam);
		msgfun int OnMdiiconarrange             (WPARAM wParam, LPARAM lParam);
		msgfun int OnMdigetactive               (WPARAM wParam, LPARAM lParam);
		msgfun int OnMdisetmenu                 (WPARAM wParam, LPARAM lParam);
		msgfun int OnEntersizemove              (WPARAM wParam, LPARAM lParam);
		msgfun int OnExitsizemove               (WPARAM wParam, LPARAM lParam);
		msgfun int OnDropfiles                  (WPARAM wParam, LPARAM lParam);
		msgfun int OnMdirefreshmenu             (WPARAM wParam, LPARAM lParam);
		msgfun int OnIme_Setcontext             (WPARAM wParam, LPARAM lParam);
		msgfun int OnIme_Notify                 (WPARAM wParam, LPARAM lParam);
		msgfun int OnIme_Control                (WPARAM wParam, LPARAM lParam);
		msgfun int OnIme_Compositionfull        (WPARAM wParam, LPARAM lParam);
		msgfun int OnIme_Select                 (WPARAM wParam, LPARAM lParam);
		msgfun int OnIme_Char                   (WPARAM wParam, LPARAM lParam);
		msgfun int OnIme_Request                (WPARAM wParam, LPARAM lParam);
		msgfun int OnIme_Keydown                (WPARAM wParam, LPARAM lParam);
		msgfun int OnIme_Keyup                  (WPARAM wParam, LPARAM lParam);
		msgfun int OnMousehover                 (WPARAM wParam, LPARAM lParam);
		msgfun int OnMouseleave                 (WPARAM wParam, LPARAM lParam);
		msgfun int OnNcmousehover               (WPARAM wParam, LPARAM lParam);
		msgfun int OnNcmouseleave               (WPARAM wParam, LPARAM lParam);
		msgfun int OnWtssession_Change          (WPARAM wParam, LPARAM lParam);
		msgfun int OnTablet_First               (WPARAM wParam, LPARAM lParam);
		msgfun int OnTablet_Last                (WPARAM wParam, LPARAM lParam);
		msgfun int OnCut                        (WPARAM wParam, LPARAM lParam);
		msgfun int OnCopy                       (WPARAM wParam, LPARAM lParam);
		msgfun int OnPaste                      (WPARAM wParam, LPARAM lParam);
		msgfun int OnClear                      (WPARAM wParam, LPARAM lParam);
		msgfun int OnUndo                       (WPARAM wParam, LPARAM lParam);
		msgfun int OnRenderformat               (WPARAM wParam, LPARAM lParam);
		msgfun int OnRenderallformats           (WPARAM wParam, LPARAM lParam);
		msgfun int OnDestroyclipboard           (WPARAM wParam, LPARAM lParam);
		msgfun int OnDrawclipboard              (WPARAM wParam, LPARAM lParam);
		msgfun int OnPaintclipboard             (WPARAM wParam, LPARAM lParam);
		msgfun int OnVscrollclipboard           (WPARAM wParam, LPARAM lParam);
		msgfun int OnSizeclipboard              (WPARAM wParam, LPARAM lParam);
		msgfun int OnAskcbformatname            (WPARAM wParam, LPARAM lParam);
		msgfun int OnChangecbchain              (WPARAM wParam, LPARAM lParam);
		msgfun int OnHscrollclipboard           (WPARAM wParam, LPARAM lParam);
		msgfun int OnQuerynewpalette            (WPARAM wParam, LPARAM lParam);
		msgfun int OnPaletteischanging          (WPARAM wParam, LPARAM lParam);
		msgfun int OnPalettechanged             (WPARAM wParam, LPARAM lParam);
		msgfun int OnHotKey                     (WPARAM wParam, LPARAM lParam);
		msgfun int OnPrint                      (WPARAM wParam, LPARAM lParam);
		msgfun int OnPrintclient                (WPARAM wParam, LPARAM lParam);
		msgfun int OnAppcommand                 (WPARAM wParam, LPARAM lParam);
		msgfun int OnThemechanged               (WPARAM wParam, LPARAM lParam);
		msgfun int OnClipboardupdate            (WPARAM wParam, LPARAM lParam);
		msgfun int OnDwmcompositionchanged      (WPARAM wParam, LPARAM lParam);
		msgfun int OnDwmncrenderingchanged      (WPARAM wParam, LPARAM lParam);
		msgfun int OnDwmcolorizationcolorchanged(WPARAM wParam, LPARAM lParam);
		msgfun int OnDwmwindowmaximizedchange   (WPARAM wParam, LPARAM lParam);
		msgfun int OnGettitlebarinfoex          (WPARAM wParam, LPARAM lParam);
		msgfun int OnHandheldfirst              (WPARAM wParam, LPARAM lParam);
		msgfun int OnHandheldlast               (WPARAM wParam, LPARAM lParam);
		msgfun int OnAfxfirst                   (WPARAM wParam, LPARAM lParam);
		msgfun int OnAfxlast                    (WPARAM wParam, LPARAM lParam);
		msgfun int OnPenwinfirst                (WPARAM wParam, LPARAM lParam);
		msgfun int OnPenwinlast                 (WPARAM wParam, LPARAM lParam);
		msgfun int OnApp                        (WPARAM wParam, LPARAM lParam);
		msgfun int OnMessage      (UINT message, WPARAM wParam, LPARAM lParam);

	protected:
		static CWindow * FromHandlePermanent(HWND hWnd);
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		virtual ATOM MyRegisterClass(HINSTANCE hInstance, TCHAR * szClassName);
	};
}
