#ifndef __warmgui_h_include__
#define __warmgui_h_include__

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the WARMGUI_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// WARMGUI_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef WARMGUI_EXPORTS
#define WARMGUI_API __declspec(dllexport)
#else
//#define WARMGUI_API __declspec(dllimport)
#define WARMGUI_API 
#endif

#ifdef _WINDOWS
#	pragma warning (disable : 4251)
#   pragma warning (disable : 4100)
#endif //_WINDOWS


#include "warmgui_type.h"
#include "warmgui_incs.h"


#endif //__warmgui_h_include__