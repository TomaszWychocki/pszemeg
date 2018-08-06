#include "header.h"

void receive_init_broadcast_msg(int * flag, struct MIB_MESSAGE * return_MIB)
{
	int broadcast_socket = socket(AF_INET, SOCK_DGRAM, 0);

    assert(broadcast_socket != -1);

	int broadcast = 1;
    unsigned int ca_len;
	int retval = setsockopt(broadcast_socket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
    if(retval == -1)
	{
		perror("Failed on setsockopt\n");
	}

	struct sockaddr_in socketConfig, clientConfig;
    memset(&socketConfig, 0, sizeof(socketConfig));

    socketConfig.sin_family = AF_INET;
	socketConfig.sin_port = htons(BROADCAST_PORT);
	socketConfig.sin_addr.s_addr = htonl(INADDR_ANY);

    retval = bind(broadcast_socket, (struct sockaddr *)&socketConfig, sizeof(struct sockaddr));
    if(retval == -1)
	{
		perror("Failed on bind\n");
	}

	int recvbytes;

	ca_len = sizeof(clientConfig);
    recvbytes = recvfrom(broadcast_socket, return_MIB, sizeof(struct MIB_MESSAGE), 0,
						(struct sockaddr *)&clientConfig, &ca_len);

	if(recvbytes > 0)
	{
		if (recvbytes > sizeof(struct MIB_MESSAGE))
		{
			return;
		}
		else
		{
			*flag = broadcast_socket;
			return;
		}
	}
	if(recvbytes == 0)
    {
        perror("Server dropped connection.\n");
        exit(EXIT_SUCCESS);
    }
    return;
}

void receive_broadcast(int fd, int * new_enb)
{
	struct MIB_MESSAGE killme;
	struct sockaddr_in clientConfig;
	int recvbytes;

	unsigned int ca_len = sizeof(clientConfig);
	recvbytes = recvfrom(fd, &killme, sizeof(struct MIB_MESSAGE), 0,
						(struct sockaddr *)&clientConfig, &ca_len);

	if(recvbytes > 0)
	{
		if (recvbytes > sizeof(struct MIB_MESSAGE))
		{
			return;
		}
		else
		{
			printf("\nBROADCAST = %d\nPRACH = %d\nDL_SCH = %d\nUL_SCH = %d\nPDDCH = %d\nPUCCH = %d\n",
					killme.broadcast_port, killme.prach_port, killme.dl_sch_port,
					killme.ul_sch_port, killme.pdcch_port, killme.pucch_port);
		}
	}
}

void receive_dci(int fd, int * state)
{
	struct DCI_MESSAGE killme;

	struct sockaddr_in clientConfig;
	int recvbytes;

	unsigned int ca_len = sizeof(clientConfig);
	recvbytes = recvfrom(fd, &killme, sizeof(struct DCI_MESSAGE), 0,
				(struct sockaddr *)&clientConfig, &ca_len);

	int calc_check_sum = 0;

	calc_check_sum += killme.format0_a_flag;
	calc_check_sum += killme.freqency_hooping;
	calc_check_sum += killme.riv;
	calc_check_sum += killme.mcs;
	calc_check_sum += killme.ndi;
	calc_check_sum += killme.tpc;
	calc_check_sum += killme.cyclic_shift;
	calc_check_sum += killme.cqi_request;//tbi

	if(recvbytes > 0)
	{
		if (recvbytes > sizeof(struct DCI_MESSAGE))
		{
			return;
		}
		else
		{
			printf("\nformat_0 = %u\nfreq_hop = %u\nriv = %d\nmcs = %d\nndi = %u\n"
					"tpc = %d\ncyclic shift = %d\ncqi_request = %u\n",
					killme.format0_a_flag, killme.freqency_hooping, killme.riv, killme.mcs,
					killme.ndi, killme.tpc, killme.cyclic_shift, killme.cqi_request);
		}
	}
}

void send_random_access_preamble(int fd, struct UE_INFO * info)
{
    struct RANDOM_ACCESS_PREAMBLE message;
    const short int preamble_identifier = 1337;

    info->UE_state = 1; // sending rap all the time

    message.preamble = preamble_identifier;
    message.RA_RNTI = info->RNTI;
    message.checksum = preamble_identifier + info->RNTI;

    if(send(fd, &message, sizeof(struct RANDOM_ACCESS_PREAMBLE), 0) == -1)
    {
        perror("Random access preamble error: ");
    };
}

void receive_random_access_response()
{

}