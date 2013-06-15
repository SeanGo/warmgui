// mo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "mo.h"


static int set_config(XMLConfig* config, EUCLID::CEuclidAnalyst* euclid)
{
    try {
        char temp[MAX_PATH];
        strcpy_s(temp, MAX_PATH, "histroy-data.rt-path");
        strcpy_s(euclid->get_ctpdata()->rt_path, MAX_PATH, config->getString(temp).c_str());
        strcpy_s(temp, MAX_PATH, "histroy-data.m1-path");
        strcpy_s(euclid->get_ctpdata()->m1_path, MAX_PATH, config->getString(temp).c_str());
        strcpy_s(temp, MAX_PATH, "histroy-data.m5-path");
        strcpy_s(euclid->get_ctpdata()->m5_path, MAX_PATH, config->getString(temp).c_str());
        strcpy_s(temp, MAX_PATH, "histroy-data.m15-path");
        strcpy_s(euclid->get_ctpdata()->m15_path, MAX_PATH, config->getString(temp).c_str());

        EUCLID::ANALYST_PARAM* ap = euclid->get_param();

        ap->_zp.len                 = config->getInt("zit.len"               );
        ap->_zp.down_sample         = config->getInt("zit.down-sample"       );
   		ap->_zp.wavelet_level       = config->getInt("zit.wavelet-level"     );
		ap->_zp.hilvl_filter_type   = config->getInt("zit.hilvl-filter-type" );
		ap->_zp.low_filter_level    = config->getInt("zit.low-filter-level"  );
		ap->_zp.hi_filter_level     = config->getInt("zit.hi-filter-level"   );
        ap->_zp.extr_len            = config->getInt("zit.extr-len"          );
        ap->_zp.price_slope_scaling = config->getDouble("zit.price-slope-scaling");

        ap->_short_pp.appx_len            = config->getInt("short-term.appx.len");
        ap->_short_pp.appx_down_sample    = config->getInt("short-term.appx.down-sample");
        ap->_short_pp.appx_low_appx       = config->getInt("short-term.appx.low-appx");
        ap->_short_pp.appx_high_appx      = config->getInt("short-term.appx.high-appx");
        ap->_short_pp.prdt_feture_len     = config->getInt("short-term.predict.feture-len");
        ap->_short_pp.prdt_coff_len       = config->getInt("short-term.predict.coff-len");
        ap->_short_pp.extr_len            = config->getInt("short-term.appx.extr-len"   );
        ap->_short_pp.wavelet             = config->getWaveletType("short-term.appx.wavelet");
        ap->_short_pp.price_slope_scaling = config->getDouble("short-term.appx.price-slope-scaling");

        ap->_long_pp.appx_len            = config->getInt("long-term.appx.len");
        ap->_long_pp.appx_down_sample    = config->getInt("long-term.appx.down-sample");
        ap->_long_pp.appx_low_appx       = config->getInt("long-term.appx.low-appx");
        ap->_long_pp.appx_high_appx      = config->getInt("long-term.appx.high-appx");
        ap->_long_pp.prdt_feture_len     = config->getInt("long-term.predict.feture-len");
        ap->_long_pp.prdt_coff_len       = config->getInt("long-term.predict.coff-len");
        ap->_long_pp.wavelet             = config->getWaveletType("long-term.appx.wavelet");
        ap->_long_pp.extr_len            = config->getInt("long-term.appx.extr-len"   );
        ap->_long_pp.price_slope_scaling = config->getDouble("long-term.appx.price-slope-scaling");

        ap->_volume_pp.min_len             = config->getInt("volume.min-len");
        ap->_volume_pp.max_len             = config->getInt("volume.max-len");
        ap->_volume_pp.level               = config->getInt("volume.level");
        ap->_volume_pp.wavelet             = config->getWaveletType("volume.wavelet");
        ap->_volume_pp.extr_len            = config->getInt("volume.extr-len"          );
        ap->_volume_pp.price_slope_scaling = config->getDouble("volume.price-slope-scaling");

        ap->_interest_pp.min_len             = config->getInt("interest.min-len");
        ap->_interest_pp.max_len             = config->getInt("interest.max-len");
        ap->_interest_pp.level               = config->getInt("interest.level");
        ap->_interest_pp.wavelet             = config->getWaveletType("interest.wavelet");
        ap->_interest_pp.extr_len            = config->getInt("interest.extr-len"          );
        ap->_interest_pp.price_slope_scaling = config->getDouble("interest.price-slope-scaling");

        euclid->init_time_series();
        euclid->set_params();

        return (0);
    }catch(...) {
        return (-1);
    }
}

#ifdef _DEBUG
void print_result(int npos, HRESULT hr, int analysis)
{
    printf("%d ", npos);
    if (hr)
        printf("error code %d\n", hr);
    else {
        if (analysis) {
            if (analysis & HAS_ZIT_ANALYSIS)
                printf("zit ");
            if (analysis & HAS_SHORT_ANALYSIS)
                printf("short ");
            if (analysis & HAS_LONG_ANALYSIS)
                printf("long ");
            if (analysis & HAS_VOLUME_ANALYSIS)
                printf("volume ");
            if (analysis & HAS_INTEREST_ANALYSIS)
                printf("interest ");
        } else {
            printf("no error but no analysis");
        }
        printf("\n");
    }
}

int test_analyse(EUCLID::CEuclidAnalyst* euclid)
{
    HRESULT hr = S_OK;

    //test zit
    if (SUCCEEDED(hr))
        hr = euclid->analysis(128);
    print_result(128, hr, euclid->get_analysis_result());
    if (SUCCEEDED(hr))
        hr = euclid->analysis(511);
    print_result(511, hr, euclid->get_analysis_result());
    if (SUCCEEDED(hr))
        hr = euclid->analysis(512);
    print_result(512, hr, euclid->get_analysis_result());
    if (SUCCEEDED(hr))
        hr = euclid->analysis(560);
    print_result(560, hr, euclid->get_analysis_result());

    //test short
    if (SUCCEEDED(hr))
        hr = euclid->analysis(1022);
    print_result(1022, hr, euclid->get_analysis_result());
    if (SUCCEEDED(hr))
        hr = euclid->analysis(1023);
    print_result(1023, hr, euclid->get_analysis_result());
    if (SUCCEEDED(hr))
        hr = euclid->analysis(1024);
    print_result(1024, hr, euclid->get_analysis_result());
    if (SUCCEEDED(hr))
        hr = euclid->analysis(1188);
    print_result(1188, hr, euclid->get_analysis_result());

    //test long
    if (SUCCEEDED(hr))
        hr = euclid->analysis(2045);
    print_result(2045, hr, euclid->get_analysis_result());
    if (SUCCEEDED(hr))
        hr = euclid->analysis(2046);
    print_result(2046, hr, euclid->get_analysis_result());
    if (SUCCEEDED(hr))
        hr = euclid->analysis(2047);
    print_result(2047, hr, euclid->get_analysis_result());
    if (SUCCEEDED(hr))
        hr = euclid->analysis(2048);
    print_result(2048, hr, euclid->get_analysis_result());
    if (SUCCEEDED(hr))
        hr = euclid->analysis(2168);
    print_result(2168, hr, euclid->get_analysis_result());

    return hr;
}
#endif //_DEBUG
int mo(XMLConfig* config, const char* code, const char* date)
{
    EUCLID::CEuclidAnalyst euclid;
    if (set_config(config, &euclid))
        return (-1);

    HRESULT hr = S_OK;
    hr = euclid.open_data(code, date);

#ifdef _DEBUG
    if (SUCCEEDED(hr))
        hr = test_analyse(&euclid);
#endif

    return (hr);
}


int main(int argc, char* argv[])
{
#ifdef _DEBUG
	CheckMemoryLeak;
#endif //_DEBUGB

    int c, errorno = 0;
	char config_path[MAX_PATH], code[MAX_PATH], date[MAX_PATH];
	*config_path = *code = *date = 0;

	for(;;) {
		//int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		static struct option long_options[] = {
			{"config",   1, 0, 'c'},
            {"code"  ,   1, 0, 'o'},
            {"date"  ,   1, 0, 'd'},
			{"help"  ,   0, 0, 'h'},
			//{0, 0, 0, 0},
		};

		c = getopt_long (argc, argv, "c:o:d:h", long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
			case 'c':
				strcpy_s(config_path, MAX_PATH, optarg);
				break;
            case 'o':
                strcpy_s(code, MAX_PATH, optarg);
                break;
            case 'd':
                strcpy_s(date, MAX_PATH, optarg);
                break;
			case 'h':
				goto usage_exit;
				break;
			case '?':
				printf("Unknown option: %c\n",(char)optopt);
				errorno = 1;
				break;
			default:
				break;
		}
	}

	if (!*config_path) {
		printf("the configuration file path is required\n");
		errorno = 2;
	}

    if (!*code) {
		printf("the code is required\n");
		errorno = 3;
    }

    if (!*date) {
		printf("the date is required\n");
		errorno = 4;
    }

    if (errorno)
		goto usage_exit;


    try {
        XMLConfig* config = new XMLConfig(config_path);
        int result = mo(config, code, date);
        delete config;
        return result;
    }catch(...){
        return (-1);
    }

usage_exit:
    printf ("\nUsage: mo -c <configuration path> [-h]\n");
    printf ("-c or --config=<configuration path> indicates the path of configuration file in xml\n");
    printf ("h or --help to show this help\n");
    return (0);
}

