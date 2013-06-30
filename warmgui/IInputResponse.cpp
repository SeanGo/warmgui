#include "StdAfx.h"
#include "warmgui_summer.h"

namespace WARMGUI {
IInputResponse::IInputResponse(void)
{
}


IInputResponse::~IInputResponse(void)
{
}


int  IInputResponse::OnKeydown      (WPARAM wParam, LPARAM lParam) { return (0); }
int  IInputResponse::OnKeyup        (WPARAM wParam, LPARAM lParam) { return (0); }
//void IInputResponse::OnMouseMove    (UINT nFlags, int x, int y) { return (0); }
int  IInputResponse::OnLButtonDown  (UINT nFlags, int x, int y) { return (0); }
int  IInputResponse::OnLButtonUp    (UINT nFlags, int x, int y) { return (0); }
int  IInputResponse::OnLButtonDblclk(UINT nFlags, int x, int y) { return (0); }
int  IInputResponse::OnRButtonDown  (UINT nFlags, int x, int y) { return (0); }
int  IInputResponse::OnRButtonUp    (UINT nFlags, int x, int y) { return (0); }
int  IInputResponse::OnRButtonDblclk(UINT nFlags, int x, int y) { return (0); }
int  IInputResponse::OnMButtonDown  (UINT nFlags, int x, int y) { return (0); }
int  IInputResponse::OnMButtonUp    (UINT nFlags, int x, int y) { return (0); }
int  IInputResponse::OnMButtonDblclk(UINT nFlags, int x, int y) { return (0); }
BOOL IInputResponse::OnMouseWheel   (UINT nFlags, short zDelta, int x, int y) { return (0); }

int  IInputResponse::OnCut          (WPARAM wParam, LPARAM lParam) { return (0); }
int  IInputResponse::OnCopy         (WPARAM wParam, LPARAM lParam) { return (0); }
int  IInputResponse::OnPaste        (WPARAM wParam, LPARAM lParam) { return (0); }
int  IInputResponse::OnClear        (WPARAM wParam, LPARAM lParam) { return (0); }
int  IInputResponse::OnUndo         (WPARAM wParam, LPARAM lParam) { return (0); }

}