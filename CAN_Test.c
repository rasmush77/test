#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>


int main()
{
	int sock;
	struct sockaddr_can addr;
	struct ifreq ifr;
	struct can_frame frame;

	printf("CAN Send Test\n");

	// create socket
	if ((sock = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
	{
		perror("Socket Error");
		return 1;
	}

	strcpy(ifr.ifr_name, "can0");
	ioctl(sock, SIOCGIFINDEX, &ifr);

	memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	// bind socket
	if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		perror("Bind Error");
		return 1;
	}

	frame.can_id = 0x555;	// CAN frame ID
	frame.can_dlc = 8;		// data length of frame
	sprintf(frame.data, "au617942");

	// write socket
	if (write(sock, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame))
	{
		perror("Write Error");
		return 1;
	}

	if (close(sock) < 0)
	{
		perror("Close Error");
		return 1;
	}

	return 0;
}
