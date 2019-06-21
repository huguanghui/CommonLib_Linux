#include "zw_usbpipe_util.h"
#include "zw_aistick_sys.h"
#include "zw_aistick_api.h"

// static void async_call(char *result, int len)
// {
//     info("[%s:%s:%d] result[%p] len[%d]\n", __FILE__, __FUNCTION__, __LINE__, result, len);

//     return;
// }

int main(int argc, char const *argv[])
{
    info("[%s:%d]\n", __FUNCTION__, __LINE__);

    AS_SYS_INFO info;
    AS_SYS_Init(&info);

    FaceGroupInfo_t faceGroup[MAX_FACE_GROUP_COUNT];
    int groupSize;

    int rc = 0;
    for (int i = 0; i < 10; i++)
    {
        rc = AS_FaceGroup_GetAll(faceGroup, &groupSize);

        for (int i=0; i<groupSize; i++)
        {
            info("ID[%lld] GroupName[%s] CreateTime[%s] enrolment[%d] Valid[%d]\n", faceGroup[i].faceGroupId, 
                                faceGroup[i].groupName, faceGroup[i].createTime, faceGroup[i].enrolment, faceGroup[i].nValidState);
        }
    }

    getchar();

    AS_SYS_UnInit();

    return 0;
}
