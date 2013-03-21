#ifndef __qiankun_spectrum_application_h__
#define __qiankun_spectrum_application_h__

class qksapp : public CDxWinApp
{
public:
    qksapp(void);
    ~qksapp(void);

	virtual int InitialApp(HINSTANCE hInstance, int nCmdShow);
	virtual void CleanupApp();

};

#endif //__qiankun_spectrum_application_h__