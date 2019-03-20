/*!
 *\file		gui_filesearch.c
 *\brief	文件检索页面-Test
 *\author	Huguanghui
 *\date		2019/03/15
 *\version	v.2.1.1 [2019/03/15]
 *\pusrpose	创建
 */

// ------------------------------------ include standard header and custom head ------------------------------------ //
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#include "gui_filesearch.h"
#include "gui_common.h"
#include "gui_login.h"
#include "gui_osd.h"

// ------------------------------------ definition of constant value ------------------------------------ //
// ------------------------------------ local macro define ------------------------------------ //
#define dbg() printf("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__)

#define PAGE_NUM 10

#define IDC_LISTVIEW  10
#define IDC_CTRL1     20
#define IDC_CTRL2     30

#define IDC_TEXT_CURPAGE 50

#define IDC_FILE_PATH  100
#define IDC_FILE_IMPORT		0x130

#define IDC_SUB_INCLUDE 200
#define IDC_SUB_EXCLUDE 201

#define IDC_OPEN_FILE 300

#define IDC_MC 400

#define IDC_ST_FIRSTPAGE    500
#define IDC_ST_PREPAGE      501
#define IDC_ST_NEXTPAGE     502
#define IDC_ST_LASTPAGE     503

#define IDC_EDIT1                       1011
#define IDC_EDIT2                       1012

// ------------------------------------ local type define ------------------------------------ //
// ------------------------------------ global variable ------------------------------------ //
T_FILESEARCHSTATE g_tFileSearchState;
int g_iSearchFilterType;
DLGTEMPLATE DlgFileSearch = 
{
    WS_BORDER | WS_CAPTION, 
    WS_EX_NONE,
    70, 80, 720, 520, 
    "Testing...",
    0, 0,
    3, NULL,
    0
};

CTRLDATA CtrlDataFileSearchDlg[] = 
{
     {
        CTRL_STATIC,
        WS_VISIBLE, 
        10, 10, 280, 18,
        IDC_STATIC,
        "Select a directory:",
        0
    },
    {
        CTRL_SLEDIT,
        WS_VISIBLE | WS_BORDER | ES_LEFT | ES_VCENTER | ES_NEEDSKB | ES_AUTOCHANGE,
        10, 32, 300, 25,
        IDC_FILE_PATH,
        "/root",
        0
    },
    {
        CTRL_BUTTON,
        WS_TABSTOP | WS_VISIBLE | BS_DEFPUSHBUTTON,
        320, 32, 50, 25,
        IDC_OPEN_FILE,
        "Open",
        0
    },
    {
        CTRL_LISTVIEW,
        WS_BORDER | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_HSCROLL,
        10, 60, 425, 310,
        IDC_LISTVIEW,
        "file list",
        0
    },
    {
        CTRL_STATIC,
        WS_VISIBLE, 
        10, 380, 80, 25,
        IDC_TEXT_CURPAGE,
        "Page 1/1",
        0
    },
    {
        CTRL_BUTTON,
        WS_TABSTOP | WS_VISIBLE | BS_DEFPUSHBUTTON,
        95, 380, 60, 25,
        IDC_ST_FIRSTPAGE,
        "First",
        0
    },
    {
        CTRL_BUTTON,
        WS_TABSTOP | WS_VISIBLE | BS_DEFPUSHBUTTON,
        160, 380, 75, 25,
        IDC_ST_PREPAGE,
        "Previous",
        0
    },
    {
        CTRL_BUTTON,
        WS_TABSTOP | WS_VISIBLE | BS_DEFPUSHBUTTON,
        240, 380, 60, 25,
        IDC_ST_NEXTPAGE,
        "Next",
        0
    },
    {
        CTRL_BUTTON,
        WS_TABSTOP | WS_VISIBLE | BS_DEFPUSHBUTTON,
        305, 380, 60, 25,
        IDC_ST_LASTPAGE,
        "Last",
        0
    },
    {
        CTRL_BUTTON,
        WS_TABSTOP | WS_VISIBLE | BS_DEFPUSHBUTTON,
        370, 380, 60, 25,
        IDOK,
        "Close",
        0
    },
    {
        CTRL_BUTTON,
        WS_VISIBLE | BS_AUTORADIOBUTTON | BS_CHECKED | WS_TABSTOP | WS_GROUP,
        450, 10, 250, 25,
        IDC_SUB_INCLUDE,
        "Include Sub Directory",
        0
    },
    {
        CTRL_BUTTON,
        WS_VISIBLE | BS_AUTORADIOBUTTON,
        450, 45, 250, 25,
        IDC_SUB_EXCLUDE,
        "Exclude Sub Directory",
        0
    },
    {
        CTRL_MONTHCALENDAR,
        WS_VISIBLE | MCS_NOTIFY | MCS_CHN | WS_TABSTOP,
        450, 120, 250, 220,
        IDC_MC,
        "",
        0
    }
};

// ------------------------------------ static variable ------------------------------------ //
static int g_iCurPage = 0;
static int g_iTotalPage = 1;
static int g_iTotalNum = 0;
static HWND g_hWnd = 0;
static H_FILESEARCHSTATE s_hFileSearchState = NULL;
static char* caption [] =
{
    "No.", "FileName", "Create Time", "Size(KB)"
};

// ------------------------------------ static function prototype ------------------------------------ //

// 事件驱动
int FileSearch_Init(HWND hWnd);
static void FileSearch_OnCommand(HWND hWnd, WORD wID, WORD wCode);
static void my_notif_proc (HWND hwnd, int id, int nc, DWORD add_data);
int DlgFileSearchWndProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam);

// 链表操作
int List_File_Length(struct listFile *list);
int List_File_copy(struct listFile *dst, struct listFile *src, int type, T_TIMEDURATION *duration);
int List_File_Destroy(struct listFile *list);
int List_File_Echo(struct listFile *list);
struct elFile* List_File_GetItemByIndex(struct listFile *list, int idx);

void FileSearch_ForeachPath(char *dir_path, struct listFile *list);
int FileSearch_GetFileList(char *dirname, struct listFile *list);
int FileSearch_GenEchoList(int hasSub, T_TIMEDURATION *duration);
int FileSearch_DestroyFileList();
int FileSearch_SearchToPage(int iPage);
void FileSearch_RefreshPageStatus();

// ------------------------------------ class realization ------------------------------------ //
// ------------------------------------ global function realization ------------------------------------ //
int FileSearchWnd_Create(HWND hParentWnd, H_FILESEARCHSTATE hFileSearchState)
{
    GUI_ASSERT(hParentWnd && hFileSearchState);
    if (!(hParentWnd && hFileSearchState))
    {
        return 1;
    }

    s_hFileSearchState = hFileSearchState;
    LIST_INIT(&s_hFileSearchState->all_list);
    LIST_INIT(&s_hFileSearchState->show_list);
    pthread_mutex_init(&s_hFileSearchState->all_list.mtxLock, NULL);
    pthread_mutex_init(&s_hFileSearchState->show_list.mtxLock, NULL);

    //GetCurrentResolve(&s_iScrW, &s_iScrH);

	DlgFileSearch.controls = CtrlDataFileSearchDlg;
	DlgFileSearch.controlnr = sizeof (CtrlDataFileSearchDlg) / sizeof (CTRLDATA);

    g_tGuiWnd.hFileSearchWnd = DialogBoxIndirectParam(&DlgFileSearch, hParentWnd, (WNDPROC)DlgFileSearchWndProc, 0L);
	if (!g_tGuiWnd.hFileSearchWnd || HWND_INVALID == g_tGuiWnd.hFileSearchWnd)
	{
		printf("[%s, %s] CreateMainWindowIndirect Failed !!!\n", __FILE__, __FUNCTION__);
		free(hFileSearchState);
		return -1;
	}
    
    return 0;
}


int FileSearchWnd_Quit()
{
    int iRet = 0;

    // 清理链表
    FileSearch_DestroyFileList();
    // 销毁退出窗口
	EndDialog(g_hWnd, IDCANCEL);
    
    return iRet;
}

// ------------------------------------ static function realization ------------------------------------ //

int FileSearch_Init(HWND hWnd)
{
    g_iTotalNum = 0;
    g_iCurPage = 0;
    
    // RadioButton
    {
        g_iSearchFilterType = IDC_SUB_INCLUDE;
        int i;
        for (i = IDC_SUB_INCLUDE; i <= IDC_SUB_EXCLUDE; i++)
        SetNotificationCallback (GetDlgItem(hWnd, i), my_notif_proc);
    }
    
    // ListView
    {
        int i, j;
        LVCOLUMN lvcol;
        LVITEM item;
        GHANDLE hitem;
    
        {
            // No
            lvcol.nCols = 1;
            lvcol.pszHeadText = caption[0];
            lvcol.width = 40;
            lvcol.pfnCompare = NULL;
            lvcol.colFlags = LVCF_CENTERALIGN;
            SendMessage(GetDlgItem(hWnd, IDC_LISTVIEW), LVM_ADDCOLUMN, 0, (LPARAM) &lvcol);
    
            // FileName
            lvcol.nCols = 2;
            lvcol.pszHeadText = caption[1];
            lvcol.width = 300;
            lvcol.pfnCompare = NULL;
            lvcol.colFlags = LVCF_CENTERALIGN | LVHF_CENTERALIGN;
            SendMessage(GetDlgItem(hWnd, IDC_LISTVIEW), LVM_ADDCOLUMN, 0, (LPARAM) &lvcol);
    
            // Create Time
            lvcol.nCols = 3;
            lvcol.pszHeadText = caption[2];
            lvcol.width = 200;
            lvcol.pfnCompare = NULL;
            lvcol.colFlags = LVCF_CENTERALIGN | LVHF_CENTERALIGN;
            SendMessage(GetDlgItem(hWnd, IDC_LISTVIEW), LVM_ADDCOLUMN, 0, (LPARAM) &lvcol);
    
            // Size
            lvcol.nCols = 4;
            lvcol.pszHeadText = caption[3];
            lvcol.width = 75;
            lvcol.pfnCompare = NULL;
            lvcol.colFlags = LVCF_CENTERALIGN | LVHF_CENTERALIGN;
            SendMessage(GetDlgItem(hWnd, IDC_LISTVIEW), LVM_ADDCOLUMN, 0, (LPARAM) &lvcol);    
        }
    }
    
    // Monthcalendar
    {
        MCCOLORINFO color;
        SendMessage (GetDlgItem(hWnd, IDC_MC), MCM_GETCOLOR, 0, (LPARAM)&color);
        color.clr_titlebk = COLOR_black;
        color.clr_titletext = PIXEL_lightwhite;
        color.clr_weekcaptbk = COLOR_black;
        color.clr_weekcapttext = PIXEL_lightwhite;
        color.clr_daybk = COLOR_black;
        color.clr_daytext = PIXEL_lightwhite;
        color.clr_trailingtext = PIXEL_black;
        color.clr_arrow = PIXEL_lightwhite;
        color.clr_arrowHibk = PIXEL_lightwhite;
        SendMessage (GetDlgItem(hWnd, IDC_MC), MCM_SETCOLOR, 0, (LPARAM)&color) ;
    }

    return 0;
}

void* Fxn_OpenFile(void *arg)  
{
    pthread_detach(pthread_self());

    FileSearch_DestroyFileList();

    char fileName[256] = {0};
    GetWindowText(GetDlgItem(g_hWnd, IDC_FILE_PATH), fileName, sizeof(fileName));
    FileSearch_GetFileList(fileName, &s_hFileSearchState->all_list);

    FileSearch_GenEchoList((g_iSearchFilterType==IDC_SUB_INCLUDE), NULL);
    FileSearch_RefreshPageStatus();
    FileSearch_SearchToPage(0);
    
    return NULL;  
}

static void FileSearch_OnCommand(HWND hWnd, WORD wID, WORD wCode)
{
    switch (wID)
    {
    case IDOK: 
    case IDCANCEL:
        {
            FileSearchWnd_Quit();
        }
        break;
    case IDC_OPEN_FILE:
        {
            // 清理链表
            pthread_create(&s_hFileSearchState->tid_openfile, NULL, Fxn_OpenFile, NULL); 
        }
        break;
    case IDC_MC:
        {
            if (MCN_DATECHANGE == wCode)
            {
                SYSTEMTIME timeInfo;
                struct tm tmTime; 
                T_TIMEDURATION durationTime;
    
                SendMessage(GetDlgItem(hWnd, IDC_MC), MCM_GETCURDATE, 0, (LPARAM)&timeInfo);
                tmTime.tm_year = timeInfo.year - 1900;   
                tmTime.tm_mon  = timeInfo.month - 1;   
                tmTime.tm_mday = timeInfo.day;   
                tmTime.tm_hour = 0;
                tmTime.tm_min = 0;
                tmTime.tm_sec = 0;    
                durationTime.beginTime = mktime(&tmTime);
                tmTime.tm_hour = 23;
                tmTime.tm_min = 59;
                tmTime.tm_sec = 59;    
                durationTime.endTime = mktime(&tmTime);

                FileSearch_GenEchoList((g_iSearchFilterType==IDC_SUB_INCLUDE), &durationTime);
                FileSearch_RefreshPageStatus();
                FileSearch_SearchToPage(0);
            }
        }
        break;
    case IDC_ST_FIRSTPAGE:
        {
            FileSearch_SearchToPage(0);    
        }
        break;
    case IDC_ST_PREPAGE:
        {
            if (g_iCurPage > 0)
            {
                g_iCurPage--;
                FileSearch_SearchToPage(g_iCurPage);    
            }
        }
        break;
    case IDC_ST_NEXTPAGE:
        {
            if (g_iCurPage < (g_iTotalPage-1))
            {
                g_iCurPage++;
                FileSearch_SearchToPage(g_iCurPage);
            }
        }
        break;
    case IDC_ST_LASTPAGE:
        {
            FileSearch_SearchToPage(g_iTotalPage-1);
        }
        break;
    }

    return ;
}

static void my_notif_proc (HWND hwnd, int id, int nc, DWORD add_data)
{
    if (nc == BN_CLICKED) {
        g_iSearchFilterType = id;

        FileSearch_GenEchoList((g_iSearchFilterType==IDC_SUB_INCLUDE), NULL);
        FileSearch_RefreshPageStatus();
        FileSearch_SearchToPage(0);
    }

    return;
}

int DlgFileSearchWndProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    H_FILESEARCHSTATE hFileSearchState = s_hFileSearchState;
    
    switch(message)
    {
    case MSG_INITDIALOG:
        {
            g_hWnd = hWnd;
            FileSearch_Init(hWnd);
        }
        break;
	case MSG_COMMAND:
		{
            int id   = LOWORD(wParam);
            int code = HIWORD(wParam);
            FileSearch_OnCommand(hWnd, id, code);            
        }
	    break;  
	case MSG_CLOSE:
	    {
            FileSearchWnd_Quit();
            break;
	    }
    }

    return DefaultDialogProc(hWnd, message, wParam, lParam);
}

int List_File_Echo(struct listFile *list)
{
    if ((NULL == list))
    {
        return -1;
    }

    struct elFile *np;
    LIST_FOREACH(np, list, elements)
    {
        if (0 == np->isDir)
        {
            printf("$$$$$$$$$$ Path: %s\n", np->name);
        }
        else
        {
            List_File_Echo(np->sub);
        }
    }

    return 0;

}

int List_File_Length(struct listFile *list)
{
    if ((NULL == list))
    {
        return 0;
    }

    int lens = 0;

    struct elFile *np;

    pthread_mutex_lock(&list->mtxLock);
    LIST_FOREACH(np, list, elements)    
    {
        lens++;        
    }
    pthread_mutex_unlock(&list->mtxLock);
    
    return lens;
}

// type 1 - 含有子目录 0 - 不含子目录
// duration 非NULL, 过滤时间段
int List_File_copy(struct listFile *dst, struct listFile *src, int type, T_TIMEDURATION *duration)
{
    if ((NULL == src) || (NULL == dst))
    {
        return -1;
    }

    struct elFile *np;
    struct elFile *node;

    LIST_FOREACH(np, src, elements)
    {
        if (0 == np->isDir)
        {
            if (duration)
            {
                if ((np->createdTime >= duration->beginTime) && (np->createdTime <= duration->endTime))
                {
                    node = (struct elFile *)malloc(sizeof(struct elFile));
                    memset(node, 0, sizeof(struct elFile));
                    snprintf(node->name, sizeof(node->name), "%s", np->name);
                    node->createdTime = np->createdTime;
                    node->size = np->size;
                    LIST_INSERT_HEAD(dst, node, elements);
                }
            }
            else
            {
                node = (struct elFile *)malloc(sizeof(struct elFile));
                memset(node, 0, sizeof(struct elFile));
                snprintf(node->name, sizeof(node->name), "%s", np->name);
                node->createdTime = np->createdTime;
                node->size = np->size;
                LIST_INSERT_HEAD(dst, node, elements);
            }    
        }
        else
        {
            if (1 == type)
            {
                List_File_copy(dst, np->sub, type, duration);
            }
        }
    }   

    return 0;
}

int List_File_Destroy(struct listFile *list)
{
    if (NULL == list)
    {
        return 0;
    }

    struct elFile *np;
    struct elFile *tmp;
    
    LIST_FOREACH(np, list, elements)
    {
        tmp = np;
        LIST_REMOVE(np, elements);

        if ((1 == tmp->isDir) && (tmp->sub != NULL))
        {
            List_File_Destroy(tmp->sub);
            free(tmp->sub);
            tmp->sub = NULL;
        }
        
        free(tmp);
    }

    return 0;
}

struct elFile* List_File_GetItemByIndex(struct listFile *list, int idx)
{
    if (NULL == list)
    {
        return NULL;
    }

    int capLen = 0;
    int iPos = 0;
    struct elFile *np;
    struct elFile *elRes = NULL;

    iPos = 0;
    LIST_FOREACH(np, list, elements)
    {
        if (iPos == idx)
        {
            elRes = np;  
        }        
        iPos++;
    }   
    
    return elRes;    
}

void FileSearch_ForeachPath(char *dir_path, struct listFile *list)
{
    if (NULL == list)
    {
        return;
    }
    
	if( strcmp( dir_path, "." ) == 0 || strcmp( dir_path, ".." )  == 0 )
	{
		return;
	}
 
	DIR *dir_ptr;
	struct dirent *file_info;
	struct elFile *node;
 
	dir_ptr = opendir( dir_path );
	if( dir_ptr == NULL )
	{
			return ;
	}
 
	int dir_len = strlen( dir_path );
	if( dir_path[dir_len-1] == '/')
	{
		dir_path[dir_len-1] = '\0';
	}
 
	char sub_dir[512] = {0};
 
	while( file_info = readdir(dir_ptr))
	{
		switch( file_info->d_type )
		{
			case DT_DIR:
			    {
				    if( strcmp( file_info->d_name, ".") != 0 && strcmp(file_info->d_name, "..")  != 0 )
    			    {
				     	sprintf( sub_dir, "%s/%s", dir_path, file_info->d_name );
				     	
                        node = (struct elFile *)malloc(sizeof(struct elFile));
                        memset(node, 0, sizeof(struct elFile));
                        snprintf(node->name, sizeof(node->name), "%s/%s", dir_path, file_info->d_name);
                        node->isDir = 1;
                        node->sub = (struct listFile *)malloc(sizeof(struct listFile));
                        LIST_INIT(node->sub);
                        LIST_INSERT_HEAD(list, node, elements);
                        
				     	FileSearch_ForeachPath(sub_dir, node->sub);
				    }
			    }
				break; 
			case DT_BLK: 
			case DT_CHR:
			case DT_FIFO:
			case DT_LNK:
			case DT_REG:
			case DT_SOCK:
			case DT_UNKNOWN:
			    {
                    struct stat statInfo;
                    
                    node = (struct elFile *)malloc(sizeof(struct elFile));
                    memset(node, 0, sizeof(struct elFile));
                    snprintf(node->name, sizeof(node->name), "%s/%s", dir_path, file_info->d_name);
                    stat(node->name, &statInfo);
                    node->createdTime = statInfo.st_mtime;
                    node->size = statInfo.st_size/1024;
                    node->isDir = 0;
                    LIST_INSERT_HEAD(list, node, elements);
			    }
				break;
			default:
				break;
		}
 
	}
	
	closedir( dir_ptr );

	return ;
}

// 获取dirname目录下的文件链表
// 限制目录的路径长度要小于256字节
// Return 0 - 成功 1 - 失败
int FileSearch_GetFileList(char *dirname, struct listFile *list)
{
    int fd;
    struct stat st;
    DIR *dirPtr;
    struct dirent *file_info;

    // 1.检测dirname的合法性
    if (NULL == list)
    {
        return -1;
    }
    
    if ((NULL == dirname) ||
            (strlen(dirname) > 256) ||
            (strlen(dirname) == 0))
    {
        return -1;
    }

    if (strcmp(dirname, ".") == 0 || strcmp(dirname, "..") == 0)
    {
        return -1;
    }

    pthread_mutex_lock(&list->mtxLock);
    FileSearch_ForeachPath(dirname, list);
    pthread_mutex_unlock(&list->mtxLock);
    
    return 0;
}

int FileSearch_GenEchoList(int hasSub, T_TIMEDURATION *duration)
{
    pthread_mutex_lock(&s_hFileSearchState->all_list.mtxLock);
    pthread_mutex_lock(&s_hFileSearchState->show_list.mtxLock);
    List_File_Destroy(&s_hFileSearchState->show_list);
    List_File_copy(&s_hFileSearchState->show_list, &s_hFileSearchState->all_list, hasSub, duration);
    pthread_mutex_unlock(&s_hFileSearchState->show_list.mtxLock);
    pthread_mutex_unlock(&s_hFileSearchState->all_list.mtxLock);
    
    return 0;
}

int FileSearch_DestroyFileList()
{   
    pthread_mutex_lock(&s_hFileSearchState->all_list.mtxLock);
    List_File_Destroy(&s_hFileSearchState->all_list);
    pthread_mutex_unlock(&s_hFileSearchState->all_list.mtxLock);
    pthread_mutex_destroy(&s_hFileSearchState->all_list.mtxLock);

    pthread_mutex_lock(&s_hFileSearchState->show_list.mtxLock);
    List_File_Destroy(&s_hFileSearchState->show_list);
    pthread_mutex_unlock(&s_hFileSearchState->show_list.mtxLock);
    pthread_mutex_destroy(&s_hFileSearchState->show_list.mtxLock);
    
    return 0;
}

int FileSearch_SearchToPage(int iPage)
{
    HWND hListView;
    char buf[64];
    LVITEM lvItem;
    struct elFile *np;
    struct tm tm1;
    int idx = 0;
    int iCol = 0;
    char aacRowText[4][128];
    LVSUBITEM subdata;
    int i;
    int iCurlItemNum = 0;
    int iLeftItemNum = g_iTotalNum - iPage*PAGE_NUM;

    g_iCurPage = iPage;
    hListView = GetDlgItem(g_hWnd, IDC_LISTVIEW);;

    snprintf(buf, sizeof(buf), "Page %d/%d", iPage + 1, g_iTotalPage);
    SetDlgItemText(g_hWnd, IDC_TEXT_CURPAGE, buf);
    
    SendMessage (hListView, LVM_DELALLITEM, 0, 0);

    if (iLeftItemNum > PAGE_NUM)
    {
        iCurlItemNum = PAGE_NUM;       
    }
    else
    {
        iCurlItemNum = iLeftItemNum;
    }

    idx = iPage*PAGE_NUM;
    pthread_mutex_lock(&s_hFileSearchState->show_list.mtxLock);
    np = List_File_GetItemByIndex(&s_hFileSearchState->show_list, idx);
    for (i = 0; i < iCurlItemNum; i ++)
    {
        if (NULL == np)
        {
            break;
        }
        
        idx++;
        lvItem.nItem = i + 1;
        GHANDLE item = SendMessage(hListView, LVM_ADDITEM, 0, (LPARAM)&lvItem);

        subdata.nItem = lvItem.nItem;

        snprintf(aacRowText[0], sizeof(aacRowText[0]), "%d", idx);
        snprintf(aacRowText[1], sizeof(aacRowText[0]), "%s", np->name);
        localtime_r(&np->createdTime, &tm1);
        snprintf(aacRowText[2], sizeof(aacRowText[2]),  "%4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d",  
           tm1.tm_year+1900, tm1.tm_mon+1, tm1.tm_mday,  
             tm1.tm_hour, tm1.tm_min, tm1.tm_sec);
        snprintf(aacRowText[3], sizeof(aacRowText[3]), "%d", np->size);

        for (iCol = 1; iCol <= 4; iCol++)
        {
            subdata.subItem = iCol;
            subdata.nTextColor = PIXEL_red;
            subdata.pszText = aacRowText[iCol - 1];
            subdata.flags = 0;
            subdata.image = 0;
            SendMessage(hListView, LVM_SETSUBITEM, 0, (LPARAM) & subdata);
        }
        
        np=np->elements.le_next;
    }
    pthread_mutex_unlock(&s_hFileSearchState->show_list.mtxLock);
    
    return 0;
}

void FileSearch_RefreshPageStatus()
{
    g_iTotalNum = List_File_Length(&s_hFileSearchState->show_list);
    if (0 == g_iTotalNum)
    {
        g_iTotalPage = 1;   
    }
    else
    {
        g_iTotalPage = (g_iTotalNum + PAGE_NUM - 1)/PAGE_NUM;
    }

    return ;
}

