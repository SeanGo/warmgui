// mo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "mo.h"

static TCHAR rt_path[MAX_PATH];

static int set_config(XMLConfig* config, EUCLID::CEuclidAnalyst* euclid)
{
    try {
        char temp[MAX_PATH];
        _snprintf_s(temp, MAX_PATH, _TRUNCATE, "histroy-data.rt-path");
        CChineseCodeLib::Gb2312ToUnicode(rt_path, MAX_PATH, config->getString(temp).c_str());

        EUCLID::ANALYST_PARAM* ap = euclid->get_param();

        ap->zit_len                = config->getInt("zit.len");
        ap->zit_down_sample        = config->getInt("zit.down-sample");
        ap->short_appx_len         = config->getInt("short-term.appx.len");
        ap->short_appx_down_sample = config->getInt("short-term.appx.down-sample");
        ap->short_appx_low_appx    = config->getInt("short-term.appx.low-appx");
        ap->short_appx_high_appx   = config->getInt("short-term.appx.high-appx");
        ap->short_prdt_feture_len  = config->getInt("short-term.predict.feture-len");
        ap->short_prdt_coff_len    = config->getInt("short-term.predict.coff-len");


        ap->short_wavelet          = config->getWaveletType("short-term.appx.wavelet");

        ap->long_appx_len          = config->getInt("long-term.appx.len");
        ap->long_appx_down_sample  = config->getInt("long-term.appx.down-sample");
        ap->long_appx_low_appx     = config->getInt("long-term.appx.low-appx");
        ap->long_appx_high_appx    = config->getInt("long-term.appx.high-appx");
        ap->long_prdt_feture_len   = config->getInt("long-term.predict.feture-len");
        ap->long_prdt_coff_len     = config->getInt("long-term.predict.coff-len");
        ap->long_wavelet           = config->getWaveletType("long-term.appx.wavelet");

        ap->volume_min_len         = config->getInt("volume.min-len");
        ap->volume_max_len         = config->getInt("volume.max-len");
        ap->volume_level           = config->getInt("volume.level");
        ap->volume_wavelet         = config->getWaveletType("volume.wavelet");

        ap->interest_min_len       = config->getInt("interest.min-len");
        ap->interest_max_len       = config->getInt("interest.max-len");
        ap->interest_level         = config->getInt("interest.level");
        ap->interest_wavelet       = config->getWaveletType("interest.wavelet");

        return (0);
    }catch(...) {
        return (-1);
    }

}

int mo(XMLConfig* config)
{
    EUCLID::CEuclidAnalyst euclid;
    if (set_config(config, &euclid))
        return (-1);

    return (0);
}


int main(int argc, char* argv[])
{
#ifdef _DEBUG
	CheckMemoryLeak;
#endif //_DEBUG

    int c, errorno = 0;
	char config_path[MAX_PATH];
	*config_path = 0;

	for(;;) {
		//int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		static struct option long_options[] = {
			{"config" ,   1, 0, 'c'},
			{"help"     ,   0, 0, 'h'},
			//{0, 0, 0, 0},
		};

		c = getopt_long (argc, argv, "c:", long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
			case 'c':
				strcpy_s(config_path, MAX_PATH, optarg);
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

	if (errorno)
		goto usage_exit;


    try {
        XMLConfig* config = new XMLConfig(config_path);
        int result = mo(config);
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

