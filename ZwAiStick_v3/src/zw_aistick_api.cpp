#include "zw_usbpipe_util.h"
#include "zw_aistick_task.h"
#include "zw_aistick_api.h"

int AS_FaceGroup_GetAll(FaceGroupInfo_t* pFg, int* FgSize)
{
	int ret = 0;
	AS_TASK_REQUEST req;
	AS_TASK_RESULT result;

	memset(&req, 0, sizeof(AS_TASK_REQUEST));
	memset(&result, 0, sizeof(AS_TASK_RESULT));
	*FgSize = 0;

	req.cmd = AS_TASK_FACEGROUP_GET_ALL;
	int rc = AS_TASK_Run(&req, &result);
	if (0 == rc)
	{
		int sz = sizeof(FaceGroupInfo_t);
		if (0 == result.paramLen%sz)
		{
			*FgSize = result.paramLen/sz;
			memcpy(pFg, result.param, result.paramLen);
		}
	}

	return ret;
}
