#include <string>
#include <sys/stat.h>
#include <fstream>

#include "zw_usbpipe_util.h"
#include "zw_usbpipe_uptp_api.h"
#include "zw_aistick_sys.h"
#include "zw_aistick_api.h"

#define USBPIPE_FILE_NAME_HOST	"/dev/usbpipe0"

int main(int argc, char const *argv[])
{
	// 检测文件是否存在
	// 方法一
	// std::ifstream f(USBPIPE_FILE_NAME_HOST);
	// info("F[%d]\n", f.good());
	// 方法二
	// FILE *file = fopen(USBPIPE_FILE_NAME_HOST, "r");
	// if (file)
	// {
	// 	info("file[%d]\n", file!=NULL?1:0);
	// 	fclose(file);
	// }
	// else
	// {
	// 	info("file[%d]\n", file!=NULL?1:0);
	// }
	// 方法三
	// int status = access(USBPIPE_FILE_NAME_HOST, F_OK);
	// info("status[%d]\n", status!=-1?1:0);
	// 方法四
	struct stat buffer;
	int ss = stat(USBPIPE_FILE_NAME_HOST, &buffer);
	int status = (ss==0)?1:0;
	if (0 == status) {
		return -1;
	}

	AS_SYS_Init();
	
	// while (1) 
	// {
	// 	int value = -1;
	// 	int rc = AS_FaceReco_Check_Online(&value);
	// 	info("rc[%d] status[%d]\n", rc, value);
	// 	sleep(10);
	// }

	getchar();
	info("\n");
	AS_SYS_UnInit();

	return 0;
}
