#pragma once

namespace WARMGUI {

	typedef map <HWND, CWindow*> CWindowMap;
	typedef pair<HWND, CWindow*> WindowPair;
	typedef CWindowMap::iterator WndIterator;
	typedef CWindowMap::const_iterator ConstWndIterator;
	typedef pair<WndIterator, bool> IterBool;

	class CWindowManager : private CWindowMap
	{
	private:
		CWindowManager(void);
		~CWindowManager(void);
	public:
		bool Add(CWindow * pwnd);
		bool Remove(CWindow * pwnd);
		bool Remove(HWND hwnd);
		void Clear();
		CWindow * Find(HWND hwnd) const;

	public:
		static CWindowManager * GetInstance();

	};


}


