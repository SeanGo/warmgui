#ifndef __warmgui_summer_h_include__
#define __warmgui_summer_h_include__

#ifndef WARMGUI_API
#   ifdef WARMGUI_EXPORTS
#       define WARMGUI_API __declspec(dllexport)
#   else
//#     define WARMGUI_API __declspec(dllimport)
#       define WARMGUI_API
#   endif //WARMGUI_EXPORTS
#endif //WARMGUI_API

#ifdef _WINDOWS
#	pragma warning (disable : 4251)
#   pragma warning (disable : 4100)
#endif //_WINDOWS


#include "utility.h"
#include "datatype.h"
#include "ChineseCodeLib.h"
#include "DataPtr.h"

#include <Poco/Util/PropertyFileConfiguration.h>

#include "warmgui_type_summer.h"
#include "WarmguiConfig.h"
#include "DxFactorys.h"
#include "WndApp.h"
#include "Window.h"
#include "WindowManager.h"

#include "tree.hpp"

#include "IObject.h"
#include "Artist.h"
#include "World.h"

#include "IInputResponse.h"
#include "Glyph_summer.h"
#include "DataGraph_summer.h"
#include "CoordFrame_summer.h"
#include "Points_summer.h"

#include "ScrollBar.h"
#include "CellRange.h"
#include "GridCellBase.h"
#include "GridCell.h"
#include "GridCtrl.h"

#include "Canvas_summer.h"
#include "Toolbar_summer.h"
#include "Atelier_summer.h"

#include "Calculator_summer.h"
#include "Dispatcher_summer.h"

#endif //__warmgui_summer_h_include__