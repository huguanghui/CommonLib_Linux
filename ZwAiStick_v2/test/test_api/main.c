#include "zw_usbpipe_util.h"
#include "zw_aistick_sys.h"
#include "zw_aistick_api.h"

static void test_task_run()
{
    FaceGroupInfo_t faceGroup[MAX_FACE_GROUP_COUNT];
    int groupSize;

    int rc = 0;
    for (int i = 0; i < 10; i++)
    {
        rc = AS_FaceGroup_GetAll(faceGroup, &groupSize);
        sleep(1);
        for (int i=0; i<groupSize; i++)
        {
            info("ID[%lld] GroupName[%s] CreateTime[%s] enrolment[%d] Valid[%d]\n", faceGroup[i].faceGroupId, 
                                faceGroup[i].groupName, faceGroup[i].createTime, faceGroup[i].enrolment, faceGroup[i].nValidState);
        }
    }

    return;
}



int main(int argc, char const *argv[])
{
    info("[%s:%d]\n", __FUNCTION__, __LINE__);

    AS_SYS_INFO info;
    AS_SYS_Init(&info);

    test_task_run();

    getchar();

    AS_SYS_UnInit();

    return 0;
}
