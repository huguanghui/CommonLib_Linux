#include <stdio.h>  
#include <string.h> 
#include <stdlib.h>  
#include <dirent.h>  
#include <sys/stat.h>  
#include <unistd.h>  
#include <sys/types.h> 

void listDir(char *path)  
{  
        DIR              *pDir ;  
        struct dirent    *ent  ;  
        int               i=0  ;  
        char              childpath[512];  

        pDir=opendir(path);  
        memset(childpath,0,sizeof(childpath));  
  
        while((ent=readdir(pDir))!=NULL)  
        {  

                if(ent->d_type & DT_DIR)  
                {  
                    if(strcmp(ent->d_name,".")==0 || strcmp(ent->d_name,"..")==0)  
                            continue;  

                    sprintf(childpath,"%s/%s",path,ent->d_name);  
                    printf("path:%s/n",childpath);  

                    listDir(childpath);  
                }  
                else
                {
                    printf("ent->d_name:%s\n", ent->d_name);
                }
        }  

}  

int main(int argc,char *argv[])  
{  
        listDir("/root");  
        return 0;  
}