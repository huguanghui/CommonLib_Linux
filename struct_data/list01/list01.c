#include "list01.h"

struct listFile g_listFile;

int list01_init()
{
    LIST_INIT(&g_listFile);
    pthread_mutex_init(&g_listFile.mtxLock, NULL);

    return 0;
}

static void list_foreachpath(char *dir_path, struct listFile *list)
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
                        
				     	list_foreachpath(sub_dir, node->sub);
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

int list01_genList(char *dirname, struct listFile *list)
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
    list_foreachpath(dirname, list);
    pthread_mutex_unlock(&list->mtxLock);
    
    return 0;
}