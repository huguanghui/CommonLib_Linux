#include <stdio.h>
#include <stdlib.h>

#define HGH_DBG(fmt, args...) printf("\033[40;33m HGH_DBG(%s %s %d):\t\033[0m"fmt, __FILE__, __func__, __LINE__, ## args)

static hgh_get_ifreq_speed()
{
    struct ifreq ifr;  
    int fd;

    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ);
    
    fd = socket(AF_INET, SOCK_DGRAM, 0);  
    if (fd < 0) 
    {  
        perror("Cannot get control socket");  
        return -1;  
    }  

    int err;  
    struct ethtool_cmd ep;    
    ep.cmd = ETHTOOL_GSET;                  // 0x00000001 /* Get settings. */  
    ifr.ifr_data = (caddr_t)&ep;
    err = ioctl(fd, SIOCETHTOOL, &ifr);
    if (err != 0) 
    {
        printf(" ioctl is erro .\n"); 
        close(fd);
        return -1;  
    }
    
    close(fd);

    return ep.speed;
}

int main(int argc, char const *argv[])
{
    /* code */
    HGH_DBG("Hello ifreq\n");

    return 0;
}
