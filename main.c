#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <fcntl.h>
#include <arpa/inet.h>

//deinetion
#define DOMAIN_NAME 0
#define SOCK_TYPE 0
#define PROTOCL_TCP 0
#define SIZE 70000

int tcp=0,udp=0,cmp=0;
int gmp=0, xxx=0;
int count = 0;
FILE * log_fd;
struct sockaddr_in source, dest;

void Error (int exitcode, const char * message){
  perror (message);
  exit(exitcode);
}

void Maclloc (unsigned char **buffer){
  *buffer  = (unsigned char *) malloc (SIZE);
  if (*buffer == NULL)
    Error(2, "malloc");
}


void ManageThePacket (char *buff, int size){
	(void)size;
	struct iphdr *ip;
	ip = (struct iphdr *) buff;
	int tmp_ip = ip->protocol ;
	if (tmp_ip == 1){
		//print the icmp protocl
		++cmp;
	}
	else if (tmp_ip == 2){
		++gmp;
		//print the igmp protocol ;
	}
	else if (tmp_ip == 6){
		++tcp;
		//you should print the tcp packet ;
	}
	else if (tmp_ip == 17){
		++udp;
		//print the udp protocl ;
	}
	else
		++xxx;
	++count;
	printf ("TCP : %d \nUDP : %d\nICMP : %d\nIGMP : %d\nOTHERS : %d\nTOTALE : %d\n",tcp, udp, cmp, gmp, xxx, count);
}

void PrintIpheader (const char * buff, int size){
	(void)size;
	size_t ip_len;
	struct iphdr *ip = (struct iphdr *)buff;
	ip_len = ip->ihl * 4 ;
	memset(&source, 0, sizeof(source));
	source.sin_addr.s_addr = ip->saddr;
	memset (&dest, 0, sizeof (dest));
	dest.sin_addr.s_addr = ip->daddr;
	(void)ip_len;
	fprintf (log_fd, "\n");
	fprintf (log_fd, "-->>IP HEADER\n");
	fprintf (log_fd, "           -> IP VERSION               : %d\n", (unsigned int)ip->version);
	fprintf (log_fd, "           -> IP HEADER LEN            : %d\n D", (unsigned int)ip->ihl);
	fprintf (log_fd, "           -> IP TOTLA LEN             : %d\n B", (unsigned int)ip->tot_len);
	fprintf (log_fd, "           -> TTL                      : %d\n", (unsigned int)ip->ttl);
	fprintf (log_fd, "           -> PROTOCOL                 : %d\n", (unsigned int)ip->protocol);
	fprintf (log_fd, "           -> IP VERSION               : %d\n", ntohs(ip->check));
	fprintf (log_fd, "           -> IP SOURCE                : %s\n", inet_ntoa(source.sin_addr));
	fprintf (log_fd, "           -> IP DESTINATION           : %s\n", inet_ntoa(dest.sin_addr));
}

void PrintTcp(const char *buff, int size){
	size_t ip_len;
	struct iphdr *ip = (struct iphdr *) buff;
	ip_len = ip->ihl * 4;
	struct tcphdr *tcph = (struct tcphdr *) (buff + ip_len);

	fprintf (log_fd, "------------------------ TCP --------------------- \n");
	PrintIpheader(buff,size);
	fprintf (log_fd, "-->>TCP HEADER\n");
	fprintf (log_fd, "           -> SHOURCE PORT             : %u\n", ntohs(tcph->source));
	fprintf (log_fd, "           -> DESTINATION  PORT        : %u\n", ntohs(tcph->dest));
	fprintf (log_fd, "           -> SEQUENCE NUMBER          : %u\n", ntohs(tcph->seq));
	fprintf (log_fd, "           -> HEADER LENGTH            : %d\n B",tcph->doff * 4);
	fprintf (log_fd, "           -> FLAG                     :\n");
	fprintf (log_fd, "           	-> URGENT                : %u\n",(unsigned int)tcph->urg);
	fprintf (log_fd, "           	-> ACK                   : %u\n",(unsigned int)tcph->ack);
	fprintf (log_fd, "           	-> SYN                   : %u\n",(unsigned int)tcph->syn);
	fprintf (log_fd, "           	-> PUSH                  : %u\n",(unsigned int)tcph->psh);
	fprintf (log_fd, "           	-> FINISH                : %u\n",(unsigned int)tcph->fin);
	//window
	//checksum
	//pointer
	fprintf (log_fd, "*********** DATA DUMP ************** \n");
	//print data dump for ip
	//printf data dump for tcp
	//print data dump for payloead
}
int main (){
	int i = 0;
	int j = 0;
	(void)i;
	(void)j;
	int soc_size = 0;
	int data_size = 0;
	struct sockaddr sock;
	int socketfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
	char *buff = (char *) malloc (SIZE);
	if (buff == NULL)
		Error(1, "malloc");
	if (socketfd < 0)
      		Error(1, "socket");
	//open the file for logs;
  	log_fd = fopen ("log_file.txt", "w");
	if (log_fd == NULL)
		Error (1, "open");
	printf ("START .............\n");
  	while (1){
		soc_size = sizeof (sock);
		data_size = recvfrom (socketfd, buff, SIZE, 0 , (struct  sockaddr *)&sock, (socklen_t *)&soc_size);
		if (data_size < 0)
			Error (2, "recfrom");
		ManageThePacket (buff, data_size);
  	}
	// after this step you need to procces the packet ;
  	fclose (log_fd);
	printf ("FINISH ...............\n");
  	return 0;
}

