#include "zw_usbpipe_base.h"
#include "zw_usbpipe_uptp.h"

int zw_uptp_send(int fd, struct zw_uptp_message *msg)
{
	int dataLen = msg->dataLen + 16;
	msg->identify[0] = 'P';
	msg->identify[1] = 'I';
	msg->identify[2] = 'P';
	msg->identify[3] = 'E';

	as_usbpipe_send(fd, (char*)msg, dataLen);

	return 0;
}
