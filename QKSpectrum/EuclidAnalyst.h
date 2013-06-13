#ifndef __euclid_analyst_h__
#define __euclid_analyst_h__

class CEuclid
{
public:
    CEuclid(void);
    ~CEuclid(void);

    void set_history_data_container(CHistoryDataContainer* history_dc) { _history_dc = history_dc; }
    bool set_config();


    /*
    <zit>
		<len>512</len>
	</zit>
	<short-term>
		<appx>
			<len>1024</len>
			<down-sample>0</down-sample>
			<low-appx>7</low-appx>
			<high-appx>9</high-appx>
		</appx>
		<predict>
			<feture-len>180</feture-len>
			<coff-len>40</coff-len>
		</predict>
	</short-term>
	<long-term>
		<appx>
			<len>2048</len>
			<down-sample>2</down-sample>
			<low-appx>7</low-appx>
			<high-appx>9</high-appx>
		</appx>
		<predict>
			<feture-len>180</feture-len>
			<coff-len>40</coff-len>
		</predict>
	</long-term>
    */
protected:
    CHistoryDataContainer* _history_dc;
    WARMGUI::CWarmguiConfig*   _config;
    EUCLID::CEuclidAnalyst     _eudlid;
};

#endif //__euclid_analyst_h__