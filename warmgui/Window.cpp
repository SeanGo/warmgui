#include "StdAfx.h"
#include "warmgui.h"

using namespace WARMGUI;

extern HINSTANCE ghDllInstance;

INT_PTR CALLBACK AboutWarmGui(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
static HHOOK ghook = 0;
static WARMGUI::CWindow * gpInitWnd = 0;
static CLockEx   gLockInitWnd;
LRESULT CALLBACK WndProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static TCHAR _gszClassName[MAX_PATH];

static void SetInitWnd(CWindow * pWnd)
{
	CLockEx::Scoped lock(gLockInitWnd);

	gpInitWnd = pWnd;
}

static void UnsetInitWnd()
{
	CLockEx::Scoped lock(gLockInitWnd);

	gpInitWnd = 0;
}

static CWindow * GetInitPwnd()
{
	CLockEx::Scoped lock(gLockInitWnd);

	return gpInitWnd;
}

static LRESULT CALLBACK MyCBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HCBT_CREATEWND) {
		//GetInitPwnd() return gpInitWnd, is the creating window
		CWindow * pwnd = GetInitPwnd();
		if (pwnd) {
			HWND hwnd = pwnd->GetSafeHwnd();
			if (!hwnd) {
				//first time call this proc, the CWindow have no get HWND yet,
				//Attach() will attach the hwnd to pwnd
				pwnd->Attach((HWND)wParam);
				return (0);
			} else {
				//second time call this proc, i donw know why for now.
				//but this is second chance to decide what is the style
				//of the window, or the window should be created or not,
				//if you want create it, return 0, else return non-zero.
				//call the PreCreateWindow before WM_CREATE
				if (!(pwnd->PreCreateWindow((LPCREATESTRUCT)lParam)))
					return (1);
				/*
				HWND hParentWnd = (HWND)GetWindowLong(hwnd, GWL_HWNDPARENT);
				if (hParentWnd) {
					CWindow * pParent = CWindowManager::GetInstance()->Find(hParentWnd);
					if (pParent)
						pwnd->SetParentWindow(pParent);
				}
				*/
				return (0);
			}
		} else {
			return (1);
		}
	} else
		return CallNextHookEx(ghook, nCode, wParam, lParam);
}

static bool HookCrate()
{
   HANDLE hThread = GetCurrentThread();
   DWORD dwThreadId = GetThreadId(hThread);
   if (hThread) {
		ghook = SetWindowsHookEx(
		  WH_CBT,
		  MyCBTProc,
		  0,
		  dwThreadId);
		if (!ghook)
			return false;
   }

   return (0);
}

static void HookDestroy()
{
   if (ghook) {
		UnhookWindowsHookEx(ghook);
		ghook = 0;
   }
}



CWindow::CWindow(void)
	: _hwnd(0)
	, _hInst(0)
	, _pParent(0)
{
}

CWindow::~CWindow(void)
{
}


ATOM CWindow::MyRegisterClass(HINSTANCE hInstance, TCHAR * szClassName)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= &CWindow::WndProc;
 	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= 0;	//LoadIcon(ghDllInstance, MAKEINTRESOURCE(IDI_WARMGUI_WINDOW));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName	= 0;	//MAKEINTRESOURCE(IDR_WARMGUI_MENU);
	wcex.lpszClassName	= szClassName;
	wcex.hIconSm		= 0;	//LoadIcon(ghDllInstance, MAKEINTRESOURCE(IDI_WARMGUI_WINDOW));

	return RegisterClassEx(&wcex);
}


CWindow * CWindow::FromHandlePermanent(HWND hWnd)
{
	if (!hWnd)
		return (0);
	else { 
		CWindow * pwnd = CWindowManager::GetInstance()->Find(hWnd);
		if (pwnd)
			return pwnd;
		else {
			return 0;
		}
	}
}



//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL CWindow::InitInstance(	HINSTANCE hInstance,
    HINSTANCE /*hPrevInstance*/,
    LPTSTR /*lpCmdLine*/,
    int nCmdShow,
	TCHAR * szClassName,
	LPTSTR szTitle)
{
   HWND hWnd;

   _tcscpy_s(_gszClassName, MAX_PATH, szClassName);

   if (!MyRegisterClass(hInstance, szClassName)) {
	   DWORD dwErr = GetLastError();
	   if (dwErr != ERROR_CLASS_ALREADY_EXISTS) //0x00000582
		   return FALSE;
   }

	_hInst = hInstance;
	SetInitWnd(this);
	HookCrate();
	hWnd = CreateWindow(szClassName, szTitle, WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance,
						NULL);
	HookDestroy();
	UnsetInitWnd();

	if (!hWnd)
		return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	ValidateRect(_hwnd, NULL);

	return TRUE;
}

BOOL CWindow::PreCreateWindow(LPCREATESTRUCT /*cs*/)
{
	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK CWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CWindow * pwnd = FromHandlePermanent(hWnd);
	if (!pwnd) return DefWindowProc(hWnd, message, wParam, lParam);

	LRESULT r = 0;
	switch (message)
	{
		case WM_CREATE                      :
			r = pwnd->OnCreate((LPCREATESTRUCT)lParam);
			if (r)
				return (r);
			break;
		case WM_DESTROY                     :
			pwnd->OnDestroy();
			return (0);
		case WM_MOVE                        :
			{
				r = pwnd->OnMove((int)(short) LOWORD(lParam), (int)(short) HIWORD(lParam));   // vertical position 
				if (r)
					return r;
				else
					break;
			}
		case WM_SIZE                        :
			{
				pwnd->OnSize((UINT)wParam, LOWORD(lParam), HIWORD(lParam));
				return (0);
			}
		case WM_ACTIVATE                    : r = pwnd->OnActivate                   (wParam, lParam); if (r) return r; else break;
		case WM_SETFOCUS                    : r = pwnd->OnSetfocus((HWND)wParam); if (r) return r; else break;

		case WM_KILLFOCUS                   : r = pwnd->OnKillfocus                  (wParam, lParam); if (r) return r; else break;
		case WM_ENABLE                      : r = pwnd->OnEnable                     (wParam, lParam); if (r) return r; else break;
		case WM_SETREDRAW                   : r = pwnd->OnSetredraw                  (wParam, lParam); if (r) return r; else break;
		case WM_SETTEXT                     : r = pwnd->OnSettext                    (wParam, lParam); if (r) return r; else break;
		case WM_GETTEXT                     : r = pwnd->OnGettext                    (wParam, lParam); if (r) return r; else break;
		case WM_GETTEXTLENGTH               : r = pwnd->OnGettextlength              (wParam, lParam); if (r) return r; else break;

		case WM_PAINT:
			pwnd->WindowPaint();
			return (0);

		case WM_CLOSE                       : r = pwnd->OnClose                      (wParam, lParam); if (r) return r; else break;
		case WM_QUERYENDSESSION             : r = pwnd->OnQueryendsession            (wParam, lParam); if (r) return r; else break;
		case WM_QUERYOPEN                   : r = pwnd->OnQueryopen                  (wParam, lParam); if (r) return r; else break;
		case WM_ENDSESSION                  : r = pwnd->OnEndsession                 (wParam, lParam); if (r) return r; else break;
		case WM_QUIT                        : r = pwnd->OnQuit                       (wParam, lParam); if (r) return r; else break;

		case WM_ERASEBKGND                  :
			{
				r = pwnd->OnEraseBkgnd((HDC)wParam);
				if (r)
					return 0;
				else
					break;
			}

		case WM_SYSCOLORCHANGE              : r = pwnd->OnSyscolorchange             (wParam, lParam); if (r) return r; else break;
		case WM_SHOWWINDOW                  : r = pwnd->OnShowwindow                 (wParam, lParam); if (r) return r; else break;
		case WM_WININICHANGE                : r = pwnd->OnWininichange               (wParam, lParam); if (r) return r; else break;
		//case WM_SETTINGCHANGE             : r = pwnd->OnSettingchange              (wParam, lParam); if (r) return r; else break;
		case WM_DEVMODECHANGE               : r = pwnd->OnDevmodechange              (wParam, lParam); if (r) return r; else break;
		case WM_ACTIVATEAPP                 : r = pwnd->OnActivateapp                (wParam, lParam); if (r) return r; else break;
		case WM_FONTCHANGE                  : r = pwnd->OnFontchange                 (wParam, lParam); if (r) return r; else break;
		case WM_TIMECHANGE                  : r = pwnd->OnTimechange                 (wParam, lParam); if (r) return r; else break;
		case WM_CANCELMODE                  : r = pwnd->OnCancelmode                 (wParam, lParam); if (r) return r; else break;
		case WM_SETCURSOR                   : r = pwnd->OnSetcursor                  (wParam, lParam); if (r) return r; else break;
		case WM_MOUSEACTIVATE               : r = pwnd->OnMouseactivate              (wParam, lParam); if (r) return r; else break;
		case WM_CHILDACTIVATE               : r = pwnd->OnChildactivate              (wParam, lParam); if (r) return r; else break;
		case WM_QUEUESYNC                   : r = pwnd->OnQueuesync                  (wParam, lParam); if (r) return r; else break;
		case WM_GETMINMAXINFO               : r = pwnd->OnGetminmaxinfo              (wParam, lParam); if (r) return r; else break;
		case WM_PAINTICON                   : r = pwnd->OnPainticon                  (wParam, lParam); if (r) return r; else break;
		case WM_ICONERASEBKGND              : r = pwnd->OnIconerasebkgnd             (wParam, lParam); if (r) return r; else break;
		case WM_NEXTDLGCTL                  : r = pwnd->OnNextdlgctl                 (wParam, lParam); if (r) return r; else break;
		case WM_SPOOLERSTATUS               : r = pwnd->OnSpoolerstatus              (wParam, lParam); if (r) return r; else break;
		case WM_DRAWITEM                    : r = pwnd->OnDrawItem                   (wParam, lParam); if (r) return r; else break;
		case WM_MEASUREITEM                 : r = pwnd->OnMeasureItem                (wParam, lParam); if (r) return r; else break;
		case WM_DELETEITEM                  : r = pwnd->OnDeleteItem                 (wParam, lParam); if (r) return r; else break;

		case WM_VKEYTOITEM                  : r = pwnd->OnVkeyToItem                 (wParam, lParam); if (r) return r; else break;
		case WM_CHARTOITEM                  : r = pwnd->OnCharToItem                 (wParam, lParam); if (r) return r; else break;

		case WM_SETFONT                     : r = pwnd->OnSetFont                    (wParam, lParam); if (r) return r; else break;
		case WM_GETFONT                     : r = pwnd->OnGetFont                    (wParam, lParam); if (r) return r; else break;
		case WM_SETHOTKEY                   : r = pwnd->OnSetHotKey                  (wParam, lParam); if (r) return r; else break;
		case WM_GETHOTKEY                   : r = pwnd->OnGetHotKey                  (wParam, lParam); if (r) return r; else break;
		case WM_QUERYDRAGICON               : r = pwnd->OnQuerydragicon              (wParam, lParam); if (r) return r; else break;
		case WM_COMPAREITEM                 : r = pwnd->OnCompareitem                (wParam, lParam); if (r) return r; else break;
		case WM_GETOBJECT                   : r = pwnd->OnGetobject                  (wParam, lParam); if (r) return r; else break;
		case WM_COMPACTING                  : r = pwnd->OnCompacting                 (wParam, lParam); if (r) return r; else break;
		case WM_COMMNOTIFY                  : r = pwnd->OnCommnotify                 (wParam, lParam); if (r) return r; else break;
		case WM_WINDOWPOSCHANGING           : r = pwnd->OnWindowposchanging          (wParam, lParam); if (r) return r; else break;
		case WM_WINDOWPOSCHANGED            : r = pwnd->OnWindowposchanged           (wParam, lParam); if (r) return r; else break;
		case WM_POWER                       : r = pwnd->OnPower                      (wParam, lParam); if (r) return r; else break;
		case WM_COPYDATA                    : r = pwnd->OnCopydata                   (wParam, lParam); if (r) return r; else break;
		case WM_CANCELJOURNAL               : r = pwnd->OnCanceljournal              (wParam, lParam); if (r) return r; else break;

		case WM_NOTIFY                      : r = pwnd->OnNotify                     (wParam, lParam); if (r) return r; else break;

		case WM_INPUTLANGCHANGEREQUEST      : r = pwnd->OnInputlangchangerequest     (wParam, lParam); if (r) return r; else break;
		case WM_INPUTLANGCHANGE             : r = pwnd->OnInputlangchange            (wParam, lParam); if (r) return r; else break;
		case WM_TCARD                       : r = pwnd->OnTcard                      (wParam, lParam); if (r) return r; else break;
		case WM_HELP                        : r = pwnd->OnHelp                       (wParam, lParam); if (r) return r; else break;
		case WM_USERCHANGED                 : r = pwnd->OnUserchanged                (wParam, lParam); if (r) return r; else break;
		case WM_NOTIFYFORMAT                : r = pwnd->OnNotifyformat               (wParam, lParam); if (r) return r; else break;
		case WM_CONTEXTMENU                 : r = pwnd->OnContextmenu                (wParam, lParam); if (r) return r; else break;
		case WM_STYLECHANGING               : r = pwnd->OnStylechanging              (wParam, lParam); if (r) return r; else break;
		case WM_STYLECHANGED                : r = pwnd->OnStylechanged               (wParam, lParam); if (r) return r; else break;

		case WM_DISPLAYCHANGE               :
			pwnd->WindowPaint();
			return (0);

		case WM_GETICON                     : r = pwnd->OnGeticon                    (wParam, lParam); if (r) return r; else break;
		case WM_SETICON                     : r = pwnd->OnSeticon                    (wParam, lParam); if (r) return r; else break;
		case WM_NCCREATE                    : r = pwnd->OnNccreate                   (wParam, lParam); if (r) return r; else break;
		case WM_NCDESTROY                   : r = pwnd->OnNcdestroy                  (wParam, lParam); if (r) return r; else break;
		case WM_NCCALCSIZE                  : r = pwnd->OnNccalcsize                 (wParam, lParam); if (r) return r; else break;
		case WM_NCHITTEST                   : r = pwnd->OnNchittest                  (wParam, lParam); if (r) return r; else break;
		case WM_NCPAINT                     : r = pwnd->OnNcpaint                    (wParam, lParam); if (r) return r; else break;
		case WM_NCACTIVATE                  : r = pwnd->OnNcactivate                 (wParam, lParam); if (r) return r; else break;
		case WM_GETDLGCODE                  : r = pwnd->OnGetdlgcode                 (wParam, lParam); if (r) return r; else break;
		case WM_SYNCPAINT                   : r = pwnd->OnSyncpaint                  (wParam, lParam); if (r) return r; else break;
		case WM_NCMOUSEMOVE                 : r = pwnd->OnNcmousemove                (wParam, lParam); if (r) return r; else break;
		case WM_NCLBUTTONDOWN               : r = pwnd->OnNclbuttondown              (wParam, lParam); if (r) return r; else break;
		case WM_NCLBUTTONUP                 : r = pwnd->OnNclbuttonup                (wParam, lParam); if (r) return r; else break;
		case WM_NCLBUTTONDBLCLK             : r = pwnd->OnNclbuttondblclk            (wParam, lParam); if (r) return r; else break;
		case WM_NCRBUTTONDOWN               : r = pwnd->OnNcrbuttondown              (wParam, lParam); if (r) return r; else break;
		case WM_NCRBUTTONUP                 : r = pwnd->OnNcrbuttonup                (wParam, lParam); if (r) return r; else break;
		case WM_NCRBUTTONDBLCLK             : r = pwnd->OnNcrbuttondblclk            (wParam, lParam); if (r) return r; else break;
		case WM_NCMBUTTONDOWN               : r = pwnd->OnNcmbuttondown              (wParam, lParam); if (r) return r; else break;
		case WM_NCMBUTTONUP                 : r = pwnd->OnNcmbuttonup                (wParam, lParam); if (r) return r; else break;
		case WM_NCMBUTTONDBLCLK             : r = pwnd->OnNcmbuttondblclk            (wParam, lParam); if (r) return r; else break;
		case WM_NCXBUTTONDOWN               : r = pwnd->OnNcxbuttondown              (wParam, lParam); if (r) return r; else break;
		case WM_NCXBUTTONUP                 : r = pwnd->OnNcxbuttonup                (wParam, lParam); if (r) return r; else break;
		case WM_NCXBUTTONDBLCLK             : r = pwnd->OnNcxbuttondblclk            (wParam, lParam); if (r) return r; else break;
		case WM_INPUT_DEVICE_CHANGE         : r = pwnd->OnInput_Device_Change        (wParam, lParam); if (r) return r; else break;
		case WM_INPUT                       : r = pwnd->OnInput                      (wParam, lParam); if (r) return r; else break;

		//case WM_KEYFIRST                  : r = pwnd->OnKeyfirst                   (wParam, lParam); if (r) return r; else break;
		case WM_KEYDOWN                     : r = pwnd->OnKeydown                    ((UINT)wParam, LOWORD(lParam), HIWORD(lParam)); if (r) return r; else break;
		case WM_KEYUP                       : r = pwnd->OnKeyup                      ((UINT)wParam, LOWORD(lParam), HIWORD(lParam)); if (r) return r; else break;
		case WM_CHAR                        : r = pwnd->OnChar                       ((UINT)wParam, LOWORD(lParam), HIWORD(lParam)); if (r) return r; else break;
		case WM_DEADCHAR                    : r = pwnd->OnDeadchar                   ((UINT)wParam, LOWORD(lParam), HIWORD(lParam)); if (r) return r; else break;
		case WM_SYSKEYDOWN                  : r = pwnd->OnSyskeydown                 ((UINT)wParam, LOWORD(lParam), HIWORD(lParam)); if (r) return r; else break;
		case WM_SYSKEYUP                    : r = pwnd->OnSyskeyup                   ((UINT)wParam, LOWORD(lParam), HIWORD(lParam)); if (r) return r; else break;
		case WM_SYSCHAR                     : r = pwnd->OnSyschar                    ((UINT)wParam, LOWORD(lParam), HIWORD(lParam)); if (r) return r; else break;
		case WM_SYSDEADCHAR                 : r = pwnd->OnSysdeadchar                ((UINT)wParam, LOWORD(lParam), HIWORD(lParam)); if (r) return r; else break;
		case WM_UNICHAR                     : r = pwnd->OnUnichar                    ((UINT)wParam, LOWORD(lParam), HIWORD(lParam)); if (r) return r; else break;
		//case WM_KEYLAST                   : r = pwnd->OnKeylast                    (wParam, lParam); if (r) return r; else break;

		case WM_IME_STARTCOMPOSITION        : r = pwnd->OnIme_Startcomposition       (wParam, lParam); if (r) return r; else break;
		case WM_IME_ENDCOMPOSITION          : r = pwnd->OnIme_Endcomposition         (wParam, lParam); if (r) return r; else break;
		case WM_IME_COMPOSITION             : r = pwnd->OnIme_Composition            (wParam, lParam); if (r) return r; else break;
		//case WM_IME_KEYLAST               : r = pwnd->OnIme_Keylast                (wParam, lParam); if (r) return r; else break;
		case WM_INITDIALOG                  : r = pwnd->OnInitdialog                 (wParam, lParam); if (r) return r; else break;

		case WM_COMMAND                     :
			r = pwnd->OnCommand(LOWORD(wParam), HIWORD(wParam), (HWND)lParam);
			if (r)
				return r;
			else
				break;

		case WM_SYSCOMMAND                  : r = pwnd->OnSyscommand                 (wParam, lParam); if (r) return r; else break;
		case WM_TIMER                       : r = pwnd->OnTimer                      (wParam, lParam); if (r) return r; else break;
		case WM_HSCROLL                     : r = pwnd->OnHscroll                    (wParam, lParam); if (r) return r; else break;
		case WM_VSCROLL                     : r = pwnd->OnVscroll                    (wParam, lParam); if (r) return r; else break;
		case WM_INITMENU                    : r = pwnd->OnInitmenu                   (wParam, lParam); if (r) return r; else break;
		case WM_INITMENUPOPUP               : r = pwnd->OnInitmenupopup              (wParam, lParam); if (r) return r; else break;
		case WM_MENUSELECT                  : r = pwnd->OnMenuselect                 (wParam, lParam); if (r) return r; else break;
		case WM_MENUCHAR                    : r = pwnd->OnMenuchar                   (wParam, lParam); if (r) return r; else break;
		case WM_ENTERIDLE                   : r = pwnd->OnEnteridle                  (wParam, lParam); if (r) return r; else break;
		case WM_MENURBUTTONUP               : r = pwnd->OnMenurbuttonup              (wParam, lParam); if (r) return r; else break;
		case WM_MENUDRAG                    : r = pwnd->OnMenudrag                   (wParam, lParam); if (r) return r; else break;
		case WM_MENUGETOBJECT               : r = pwnd->OnMenugetobject              (wParam, lParam); if (r) return r; else break;
		case WM_UNINITMENUPOPUP             : r = pwnd->OnUninitmenupopup            (wParam, lParam); if (r) return r; else break;
		case WM_MENUCOMMAND                 : r = pwnd->OnMenucommand                (wParam, lParam); if (r) return r; else break;
		case WM_CHANGEUISTATE               : r = pwnd->OnChangeuistate              (wParam, lParam); if (r) return r; else break;
		case WM_UPDATEUISTATE               : r = pwnd->OnUpdateuistate              (wParam, lParam); if (r) return r; else break;
		case WM_QUERYUISTATE                : r = pwnd->OnQueryuistate               (wParam, lParam); if (r) return r; else break;
		case WM_CTLCOLORMSGBOX              : r = pwnd->OnCtlcolormsgbox             (wParam, lParam); if (r) return r; else break;
		case WM_CTLCOLOREDIT                : r = pwnd->OnCtlcoloredit               (wParam, lParam); if (r) return r; else break;
		case WM_CTLCOLORLISTBOX             : r = pwnd->OnCtlcolorlistbox            (wParam, lParam); if (r) return r; else break;
		case WM_CTLCOLORBTN                 : r = pwnd->OnCtlcolorbtn                (wParam, lParam); if (r) return r; else break;
		case WM_CTLCOLORDLG                 : r = pwnd->OnCtlcolordlg                (wParam, lParam); if (r) return r; else break;
		case WM_CTLCOLORSCROLLBAR           : r = pwnd->OnCtlcolorscrollbar          (wParam, lParam); if (r) return r; else break;
		case WM_CTLCOLORSTATIC              : r = pwnd->OnCtlcolorstatic             (wParam, lParam); if (r) return r; else break;
		case MN_GETHMENU                    : r = pwnd->OnGetHMenu                   (wParam, lParam); if (r) return r; else break;
		//case WM_MOUSEFIRST                : r = pwnd->OnMousefirst                 (wParam, lParam); if (r) return r; else break;

		case WM_MOUSEMOVE                   :
			pwnd->OnMouseMove((UINT)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_LBUTTONDOWN                 :
			pwnd->OnLButtonDown((UINT)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_LBUTTONUP                   :
			pwnd->OnLButtonUp((UINT)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_LBUTTONDBLCLK               :
			pwnd->OnLButtonDblclk((UINT)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_RBUTTONDOWN                 :
			pwnd->OnRButtonDown((UINT)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_RBUTTONUP                   :
			pwnd->OnRButtonUp((UINT)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_RBUTTONDBLCLK               :
			pwnd->OnRButtonDblclk((UINT)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_MBUTTONDOWN                 :
			pwnd->OnMButtonDown((UINT)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_MBUTTONUP                   :
			pwnd->OnMButtonUp((UINT)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_MBUTTONDBLCLK               :
			pwnd->OnMButtonDblclk((UINT)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_MOUSEWHEEL                  :
			pwnd->OnMouseWheel(GET_KEYSTATE_WPARAM(wParam), GET_WHEEL_DELTA_WPARAM(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;

		case WM_XBUTTONDOWN                 : r = pwnd->OnXbuttondown                (wParam, lParam); if (r) return r; else break;
		case WM_XBUTTONUP                   : r = pwnd->OnXbuttonup                  (wParam, lParam); if (r) return r; else break;
		case WM_XBUTTONDBLCLK               : r = pwnd->OnXbuttondblclk              (wParam, lParam); if (r) return r; else break;
		case WM_MOUSEHWHEEL                 : r = pwnd->OnMousehwheel                (wParam, lParam); if (r) return r; else break;
		//case WM_MOUSELAST                 : r = pwnd->OnMouselast                  (wParam, lParam); if (r) return r; else break;
		case WM_PARENTNOTIFY                : r = pwnd->OnParentnotify               (wParam, lParam); if (r) return r; else break;
		case WM_ENTERMENULOOP               : r = pwnd->OnEntermenuloop              (wParam, lParam); if (r) return r; else break;
		case WM_EXITMENULOOP                : r = pwnd->OnExitmenuloop               (wParam, lParam); if (r) return r; else break;
		case WM_NEXTMENU                    : r = pwnd->OnNextmenu                   (wParam, lParam); if (r) return r; else break;
		case WM_SIZING                      : r = pwnd->OnSizing                     (wParam, lParam); if (r) return r; else break;
		case WM_CAPTURECHANGED              : r = pwnd->OnCapturechanged             (wParam, lParam); if (r) return r; else break;
		case WM_MOVING                      : r = pwnd->OnMoving                     (wParam, lParam); if (r) return r; else break;
		case WM_POWERBROADCAST              : r = pwnd->OnPowerbroadcast             (wParam, lParam); if (r) return r; else break;
		case WM_DEVICECHANGE                : r = pwnd->OnDevicechange               (wParam, lParam); if (r) return r; else break;
		case WM_MDICREATE                   : r = pwnd->OnMdicreate                  (wParam, lParam); if (r) return r; else break;
		case WM_MDIDESTROY                  : r = pwnd->OnMdidestroy                 (wParam, lParam); if (r) return r; else break;
		case WM_MDIACTIVATE                 : r = pwnd->OnMdiactivate                (wParam, lParam); if (r) return r; else break;
		case WM_MDIRESTORE                  : r = pwnd->OnMdirestore                 (wParam, lParam); if (r) return r; else break;
		case WM_MDINEXT                     : r = pwnd->OnMdinext                    (wParam, lParam); if (r) return r; else break;
		case WM_MDIMAXIMIZE                 : r = pwnd->OnMdimaximize                (wParam, lParam); if (r) return r; else break;
		case WM_MDITILE                     : r = pwnd->OnMditile                    (wParam, lParam); if (r) return r; else break;
		case WM_MDICASCADE                  : r = pwnd->OnMdicascade                 (wParam, lParam); if (r) return r; else break;
		case WM_MDIICONARRANGE              : r = pwnd->OnMdiiconarrange             (wParam, lParam); if (r) return r; else break;
		case WM_MDIGETACTIVE                : r = pwnd->OnMdigetactive               (wParam, lParam); if (r) return r; else break;
		case WM_MDISETMENU                  : r = pwnd->OnMdisetmenu                 (wParam, lParam); if (r) return r; else break;
		case WM_ENTERSIZEMOVE               : r = pwnd->OnEntersizemove              (wParam, lParam); if (r) return r; else break;
		case WM_EXITSIZEMOVE                : r = pwnd->OnExitsizemove               (wParam, lParam); if (r) return r; else break;
		case WM_DROPFILES                   : r = pwnd->OnDropfiles                  (wParam, lParam); if (r) return r; else break;
		case WM_MDIREFRESHMENU              : r = pwnd->OnMdirefreshmenu             (wParam, lParam); if (r) return r; else break;
		case WM_IME_SETCONTEXT              : r = pwnd->OnIme_Setcontext             (wParam, lParam); if (r) return r; else break;
		case WM_IME_NOTIFY                  : r = pwnd->OnIme_Notify                 (wParam, lParam); if (r) return r; else break;
		case WM_IME_CONTROL                 : r = pwnd->OnIme_Control                (wParam, lParam); if (r) return r; else break;
		case WM_IME_COMPOSITIONFULL         : r = pwnd->OnIme_Compositionfull        (wParam, lParam); if (r) return r; else break;
		case WM_IME_SELECT                  : r = pwnd->OnIme_Select                 (wParam, lParam); if (r) return r; else break;
		case WM_IME_CHAR                    : r = pwnd->OnIme_Char                   (wParam, lParam); if (r) return r; else break;
		case WM_IME_REQUEST                 : r = pwnd->OnIme_Request                (wParam, lParam); if (r) return r; else break;
		case WM_IME_KEYDOWN                 : r = pwnd->OnIme_Keydown                (wParam, lParam); if (r) return r; else break;
		case WM_IME_KEYUP                   : r = pwnd->OnIme_Keyup                  (wParam, lParam); if (r) return r; else break;
		case WM_MOUSEHOVER                  : r = pwnd->OnMousehover                 (wParam, lParam); if (r) return r; else break;
		case WM_MOUSELEAVE                  : r = pwnd->OnMouseleave                 (wParam, lParam); if (r) return r; else break;
		case WM_NCMOUSEHOVER                : r = pwnd->OnNcmousehover               (wParam, lParam); if (r) return r; else break;
		case WM_NCMOUSELEAVE                : r = pwnd->OnNcmouseleave               (wParam, lParam); if (r) return r; else break;
		case WM_WTSSESSION_CHANGE           : r = pwnd->OnWtssession_Change          (wParam, lParam); if (r) return r; else break;
		case WM_TABLET_FIRST                : r = pwnd->OnTablet_First               (wParam, lParam); if (r) return r; else break;
		case WM_TABLET_LAST                 : r = pwnd->OnTablet_Last                (wParam, lParam); if (r) return r; else break;
		case WM_CUT                         : r = pwnd->OnCut                        (wParam, lParam); if (r) return r; else break;
		case WM_COPY                        : r = pwnd->OnCopy                       (wParam, lParam); if (r) return r; else break;
		case WM_PASTE                       : r = pwnd->OnPaste                      (wParam, lParam); if (r) return r; else break;
		case WM_CLEAR                       : r = pwnd->OnClear                      (wParam, lParam); if (r) return r; else break;
		case WM_UNDO                        : r = pwnd->OnUndo                       (wParam, lParam); if (r) return r; else break;
		case WM_RENDERFORMAT                : r = pwnd->OnRenderformat               (wParam, lParam); if (r) return r; else break;
		case WM_RENDERALLFORMATS            : r = pwnd->OnRenderallformats           (wParam, lParam); if (r) return r; else break;
		case WM_DESTROYCLIPBOARD            : r = pwnd->OnDestroyclipboard           (wParam, lParam); if (r) return r; else break;
		case WM_DRAWCLIPBOARD               : r = pwnd->OnDrawclipboard              (wParam, lParam); if (r) return r; else break;
		case WM_PAINTCLIPBOARD              : r = pwnd->OnPaintclipboard             (wParam, lParam); if (r) return r; else break;
		case WM_VSCROLLCLIPBOARD            : r = pwnd->OnVscrollclipboard           (wParam, lParam); if (r) return r; else break;
		case WM_SIZECLIPBOARD               : r = pwnd->OnSizeclipboard              (wParam, lParam); if (r) return r; else break;
		case WM_ASKCBFORMATNAME             : r = pwnd->OnAskcbformatname            (wParam, lParam); if (r) return r; else break;
		case WM_CHANGECBCHAIN               : r = pwnd->OnChangecbchain              (wParam, lParam); if (r) return r; else break;
		case WM_HSCROLLCLIPBOARD            : r = pwnd->OnHscrollclipboard           (wParam, lParam); if (r) return r; else break;
		case WM_QUERYNEWPALETTE             : r = pwnd->OnQuerynewpalette            (wParam, lParam); if (r) return r; else break;
		case WM_PALETTEISCHANGING           : r = pwnd->OnPaletteischanging          (wParam, lParam); if (r) return r; else break;
		case WM_PALETTECHANGED              : r = pwnd->OnPalettechanged             (wParam, lParam); if (r) return r; else break;
		case WM_HOTKEY                      : r = pwnd->OnHotKey                     (wParam, lParam); if (r) return r; else break;
		case WM_PRINT                       : r = pwnd->OnPrint                      (wParam, lParam); if (r) return r; else break;
		case WM_PRINTCLIENT                 : r = pwnd->OnPrintclient                (wParam, lParam); if (r) return r; else break;
		case WM_APPCOMMAND                  : r = pwnd->OnAppcommand                 (wParam, lParam); if (r) return r; else break;
		case WM_THEMECHANGED                : r = pwnd->OnThemechanged               (wParam, lParam); if (r) return r; else break;
		case WM_CLIPBOARDUPDATE             : r = pwnd->OnClipboardupdate            (wParam, lParam); if (r) return r; else break;
		case WM_DWMCOMPOSITIONCHANGED       : r = pwnd->OnDwmcompositionchanged      (wParam, lParam); if (r) return r; else break;
		case WM_DWMNCRENDERINGCHANGED       : r = pwnd->OnDwmncrenderingchanged      (wParam, lParam); if (r) return r; else break;
		case WM_DWMCOLORIZATIONCOLORCHANGED : r = pwnd->OnDwmcolorizationcolorchanged(wParam, lParam); if (r) return r; else break;
		case WM_DWMWINDOWMAXIMIZEDCHANGE    : r = pwnd->OnDwmwindowmaximizedchange   (wParam, lParam); if (r) return r; else break;
		case WM_GETTITLEBARINFOEX           : r = pwnd->OnGettitlebarinfoex          (wParam, lParam); if (r) return r; else break;
		case WM_HANDHELDFIRST               : r = pwnd->OnHandheldfirst              (wParam, lParam); if (r) return r; else break;
		case WM_HANDHELDLAST                : r = pwnd->OnHandheldlast               (wParam, lParam); if (r) return r; else break;
		case WM_AFXFIRST                    : r = pwnd->OnAfxfirst                   (wParam, lParam); if (r) return r; else break;
		case WM_AFXLAST                     : r = pwnd->OnAfxlast                    (wParam, lParam); if (r) return r; else break;
		case WM_PENWINFIRST                 : r = pwnd->OnPenwinfirst                (wParam, lParam); if (r) return r; else break;
		case WM_PENWINLAST                  : r = pwnd->OnPenwinlast                 (wParam, lParam); if (r) return r; else break;
		case WM_APP                         : r = pwnd->OnApp                        (wParam, lParam); if (r) return r; else break;

		default:
		{
			if (message > WM_USER)
				return pwnd->OnMessage(message, wParam, lParam);
			else
				return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}



void CWindow::RunMessageLoop()
{
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
		if (_hAccel) {
			if (!TranslateAccelerator(msg.hwnd, _hAccel, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		} else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
    }
}



void CWindow::OnDestroy()
{
	Dettach();
}

bool CWindow::LoadAccelerators(UINT acceid)
{
	_hAccel = ::LoadAccelerators(_hInst, MAKEINTRESOURCE(acceid));
    return (_hAccel != 0);
}

void CWindow::WindowPaint()
{
	PAINTSTRUCT ps;
	//HDC hdc = BeginPaint(_hwnd, &ps);
	BeginPaint(_hwnd, &ps);

	//OnPaint(hdc, &ps);
	//ValidateRect(_hwnd, NULL);
	OnDraw();

	EndPaint(_hwnd, &ps);
}

void CWindow::OnDraw()
{
}

/*
void CWindow::OnPaint(HDC hdc, LPPAINTSTRUCT ps)
{
	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 255));
	
	HBRUSH old_brush = (HBRUSH)SelectObject(hdc, brush);
	TCHAR hello[] = L"Hello Word!";
	RECT rect;
	rect.left = 100, rect.top = 100, rect.right = 200, rect.bottom = 200;
	COLORREF oldBkg = SetBkColor(hdc, RGB(255, 0, 0));
	COLORREF oldClr = SetTextColor(hdc, RGB(0, 0, 255));
	DrawText(hdc, hello, _tcslen(hello), &rect, DT_CENTER);
	SetTextColor(hdc, oldClr);
	SetBkColor(hdc, oldBkg);
	SelectObject(hdc, old_brush);
	DeleteObject(brush);
}
*/

///OnCreate must return 0 to continue the creation of the CWnd object. If the application returns ¨C1, the window will be destroyed. 
int CWindow::OnCreate(LPCREATESTRUCT /*cs*/) { return (0); }
int CWindow::OnMove                       (int /*x*/, int /*y*/) { return (0); }
int CWindow::OnActivate                   (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnSetfocus(HWND /*hLostFocus*/) { return (0); }
int CWindow::OnKillfocus                  (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnEnable                     (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnSetredraw                  (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnSettext                    (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnGettext                    (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnGettextlength              (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnClose                      (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnQueryendsession            (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnQueryopen                  (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnEndsession                 (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnQuit                       (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnEraseBkgnd                 (HDC /*hdc*/) { return (0); }
int CWindow::OnSyscolorchange             (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnShowwindow                 (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnWininichange               (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnSettingchange              (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnDevmodechange              (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnActivateapp                (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnFontchange                 (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnTimechange                 (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnCancelmode                 (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnSetcursor                  (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnMouseactivate              (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnChildactivate              (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnQueuesync                  (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnGetminmaxinfo              (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnPainticon                  (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnIconerasebkgnd             (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnNextdlgctl                 (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnSpoolerstatus              (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnDrawItem                   (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnMeasureItem                (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnDeleteItem                 (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnVkeyToItem                 (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnCharToItem                 (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnSetFont                    (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnGetFont                    (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnSetHotKey                  (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnGetHotKey                  (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnQuerydragicon              (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnCompareitem                (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnGetobject                  (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnCompacting                 (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnCommnotify                 (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnWindowposchanging          (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnWindowposchanged           (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnPower                      (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnCopydata                   (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnCanceljournal              (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnNotify                     (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnInputlangchangerequest     (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnInputlangchange            (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnTcard                      (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnHelp                       (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnUserchanged                (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnNotifyformat               (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnContextmenu                (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnStylechanging              (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnStylechanged               (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnDisplayChange              (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnGeticon                    (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnSeticon                    (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnNccreate                   (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnNcdestroy                  (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnNccalcsize                 (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnNchittest                  (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnNcpaint                    (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnNcactivate                 (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnGetdlgcode                 (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnSyncpaint                  (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnNcmousemove                (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnNclbuttondown              (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnNclbuttonup                (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnNclbuttondblclk            (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnNcrbuttondown              (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnNcrbuttonup                (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnNcrbuttondblclk            (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnNcmbuttondown              (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnNcmbuttonup                (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnNcmbuttondblclk            (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnNcxbuttondown              (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnNcxbuttonup                (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnNcxbuttondblclk            (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnInput_Device_Change        (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnInput                      (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }

int CWindow::OnKeydown                    (UINT /*nChar*/, UINT /*nRepCnt*/, UINT /*nFlags*/) { return (0); }
int CWindow::OnKeyup                      (UINT /*nChar*/, UINT /*nRepCnt*/, UINT /*nFlags*/) { return (0); }
int CWindow::OnChar                       (UINT /*nChar*/, UINT /*nRepCnt*/, UINT /*nFlags*/) { return (0); }
int CWindow::OnDeadchar                   (UINT /*nChar*/, UINT /*nRepCnt*/, UINT /*nFlags*/) { return (0); }
int CWindow::OnSyskeydown                 (UINT /*nChar*/, UINT /*nRepCnt*/, UINT /*nFlags*/) { return (0); }
int CWindow::OnSyskeyup                   (UINT /*nChar*/, UINT /*nRepCnt*/, UINT /*nFlags*/) { return (0); }
int CWindow::OnSyschar                    (UINT /*nChar*/, UINT /*nRepCnt*/, UINT /*nFlags*/) { return (0); }
int CWindow::OnSysdeadchar                (UINT /*nChar*/, UINT /*nRepCnt*/, UINT /*nFlags*/) { return (0); }
int CWindow::OnUnichar                    (UINT /*nChar*/, UINT /*nRepCnt*/, UINT /*nFlags*/) { return (0); }

int CWindow::OnIme_Startcomposition       (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnIme_Endcomposition         (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnIme_Composition            (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnIme_Keylast                (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnInitdialog                 (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnCommand(WORD /*nCmdId*/, WORD /*nSource*/, HWND /*hwnd*/)         { return (0); }
int CWindow::OnSyscommand                 (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnTimer                      (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnHscroll                    (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnVscroll                    (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnInitmenu                   (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnInitmenupopup              (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnMenuselect                 (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnMenuchar                   (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnEnteridle                  (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnMenurbuttonup              (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnMenudrag                   (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnMenugetobject              (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnUninitmenupopup            (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnMenucommand                (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnChangeuistate              (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnUpdateuistate              (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnQueryuistate               (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnCtlcolormsgbox             (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnCtlcoloredit               (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnCtlcolorlistbox            (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnCtlcolorbtn                (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnCtlcolordlg                (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnCtlcolorscrollbar          (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnCtlcolorstatic             (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnGetHMenu                   (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnMousefirst                 (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }

void CWindow::OnMouseMove    (UINT /*nFlags*/, int /*x*/, int /*y*/) {}
void CWindow::OnLButtonDown  (UINT /*nFlags*/, int /*x*/, int /*y*/) {}
void CWindow::OnLButtonUp    (UINT /*nFlags*/, int /*x*/, int /*y*/) {}
void CWindow::OnLButtonDblclk(UINT /*nFlags*/, int /*x*/, int /*y*/) {}
void CWindow::OnRButtonDown  (UINT /*nFlags*/, int /*x*/, int /*y*/) {}
void CWindow::OnRButtonUp    (UINT /*nFlags*/, int /*x*/, int /*y*/) {}
void CWindow::OnRButtonDblclk(UINT /*nFlags*/, int /*x*/, int /*y*/) {}
void CWindow::OnMButtonDown  (UINT /*nFlags*/, int /*x*/, int /*y*/) {}
void CWindow::OnMButtonUp    (UINT /*nFlags*/, int /*x*/, int /*y*/) {}
void CWindow::OnMButtonDblclk(UINT /*nFlags*/, int /*x*/, int /*y*/) {}
BOOL CWindow::OnMouseWheel   (UINT /*nFlags*/, short /*zDelta*/, int /*x*/, int /*y*/) {return TRUE;}

int CWindow::OnXbuttondown                (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnXbuttonup                  (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnXbuttondblclk              (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnMousehwheel                (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnMouselast                  (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnParentnotify               (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnEntermenuloop              (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnExitmenuloop               (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnNextmenu                   (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnSizing                     (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnCapturechanged             (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnMoving                     (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnPowerbroadcast             (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnDevicechange               (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnMdicreate                  (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnMdidestroy                 (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnMdiactivate                (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnMdirestore                 (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnMdinext                    (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnMdimaximize                (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnMditile                    (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnMdicascade                 (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnMdiiconarrange             (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnMdigetactive               (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnMdisetmenu                 (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnEntersizemove              (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnExitsizemove               (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnDropfiles                  (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnMdirefreshmenu             (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnIme_Setcontext             (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnIme_Notify                 (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnIme_Control                (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnIme_Compositionfull        (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnIme_Select                 (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnIme_Char                   (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnIme_Request                (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnIme_Keydown                (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnIme_Keyup                  (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnMousehover                 (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnMouseleave                 (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnNcmousehover               (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnNcmouseleave               (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnWtssession_Change          (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnTablet_First               (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnTablet_Last                (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnCut                        (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnCopy                       (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnPaste                      (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnClear                      (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnUndo                       (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnRenderformat               (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnRenderallformats           (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnDestroyclipboard           (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnDrawclipboard              (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnPaintclipboard             (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnVscrollclipboard           (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnSizeclipboard              (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnAskcbformatname            (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnChangecbchain              (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnHscrollclipboard           (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnQuerynewpalette            (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnPaletteischanging          (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnPalettechanged             (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnHotKey                     (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnPrint                      (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnPrintclient                (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnAppcommand                 (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnThemechanged               (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnClipboardupdate            (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnDwmcompositionchanged      (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnDwmncrenderingchanged      (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnDwmcolorizationcolorchanged(WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnDwmwindowmaximizedchange   (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnGettitlebarinfoex          (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnHandheldfirst              (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnHandheldlast               (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnAfxfirst                   (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnAfxlast                    (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnPenwinfirst                (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnPenwinlast                 (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnApp                        (WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }
int CWindow::OnMessage  (UINT /*message*/, WPARAM /*wParam*/, LPARAM /*lParam*/) { return (0); }


HWND CWindow::GetSafeHwnd()
{
	return _hwnd;
}

HINSTANCE CWindow::GetInstance()
{
	return _hInst;
}

void CWindow::Attach(HWND hwnd)
{
	_hwnd = hwnd;
	CWindowManager::GetInstance()->Add(this);
}

void CWindow::Dettach()
{
	CWindowManager::GetInstance()->Remove(_hwnd);
	_hwnd = 0;
}

void CWindow::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
}

bool CWindow::Create(TCHAR *  szClassName,	//NULL for default class name
					 TCHAR * szWindowName,
							DWORD dwStyle,
							   RECT& rect,
					 CWindow * pParentWnd,
						   LPVOID lpParam, //= 0
					       int nShowState) //= SW_SHOW
{
	if (!pParentWnd)
		return false;

	TCHAR * szcn = (szClassName) ? szClassName : _gszClassName;

	_hInst = pParentWnd->GetInstance();

	if (!MyRegisterClass(_hInst, szcn)) {
		DWORD dwErr = GetLastError();
		if (dwErr != ERROR_CLASS_ALREADY_EXISTS) //0x00000582
			return false;
	}

	SetInitWnd(this);
	HookCrate();	//gpInitWnd = pWnd;
	
	HWND hWnd = CreateWindow(szcn,
						szWindowName,
						dwStyle,
						rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
						pParentWnd->GetSafeHwnd(),
						NULL,	//menu
						_hInst,
						lpParam);
	HookDestroy();
	UnsetInitWnd();	//gpInitWnd = 0;

	if (!hWnd)
		return FALSE;

	_pParent = pParentWnd;

	/*
    if (_hwnd) {
        WNDPROC wpOrigProc = (WNDPROC)SetWindowLong(_hwnd, GWL_WNDPROC, (LONG)WndProc);
        if (!wpOrigProc) {
            DestroyWindow(hWnd);
            return FALSE;
        }
	}
    */

	ShowWindow(hWnd, nShowState);
	UpdateWindow(hWnd);

	return TRUE;
}

BOOL CWindow::MoveTo(int x, int y, int cx, int cy)
{
	return ::MoveWindow(_hwnd, x, y, cx, cy, TRUE);
}

