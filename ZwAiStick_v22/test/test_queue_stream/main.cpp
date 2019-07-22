#include <string>
#include <fstream>

#include "zw_usbpipe_util.h"
#include "zw_usbpipe_uptp_api.h"
#include "zw_aistick_sys.h"
#include "zw_aistick_api.h"


int main(int argc, char *argv[])
{
	AS_SYS_INFO info;
	
	memset(&info, 0, sizeof(info));
	AS_SYS_Init(&info);

	//test_xml();
	//test_form_data();
	//faceinfo_test_basic();
	//query_all_group();
	//test_sys();

	getchar();
	info("\n");

	AS_SYS_UnInit();
}
