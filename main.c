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
int i;
int j;
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

void PrintTcp(const char *buff, int size);
void PrintIpheader (const char * buff, int size);
void PrintDataDump(unsigned char* data, int Size);

void PrintFirst(const char *protocol){
	fprintf (log_fd, "---------------------------------------------- [ %s ] ------------------------------------------\n", protocol);
}
void PrintEnd (){
	fprintf (log_fd, "-------------------------------------------------------------------------------------------------\n");
}

void PrintPacketInfo(const char *packet) {
	struct iphdr *iph = (struct iphdr *)packet;
	struct tcphdr *tcph = (struct tcphdr *) (packet + sizeof(struct iphdr));

 	 printf("Source IP: %u.%u.%u.%u ",
		(unsigned int)(iph->saddr & 0xFF),
         	(unsigned int)((iph->saddr >> 8) & 0xFF),
         	(unsigned int)((iph->saddr >> 16) & 0xFF),
         	(unsigned int)((iph->saddr >> 24) & 0xFF));
  	printf("Destination IP: %u.%u.%u.%u  ",
        	(unsigned int)(iph->daddr & 0xFF),
         	(unsigned int)((iph->daddr >> 8) & 0xFF),
         	(unsigned int)((iph->daddr >> 16) & 0xFF),
         	(unsigned int)((iph->daddr >> 24) & 0xFF));

  		printf("Source Port: %u ", ntohs(tcph->source));
  	printf("Destination Port: %u  ", ntohs(tcph->dest));

 	 printf("Flags: [.]");
  	if (tcph->ack)
  	  	printf(" ack");
  	if (tcph->psh)
		printf(" psh");
 	 if (tcph->fin)
    		printf(" fin");
  	if (tcph->syn)
    		printf(" syn");
  	if (tcph->urg)
  		  printf(" urg");

 	printf(" Window Size: %u ", ntohs(tcph->window));
  	 if ((tcph->doff * 4) > sizeof(struct tcphdr)) {
    		unsigned int *tsval = (unsigned int *)(packet + sizeof(struct iphdr) + sizeof(struct tcphdr) + 8);
    		unsigned int *tsecr = (unsigned int *)(packet + sizeof(struct iphdr) + sizeof(struct tcphdr) + 12);
    	printf("Options: [nop,nop,TS val %u ecr %u] ", ntohl(*tsval), ntohl(*tsecr));
  	}
	 else {
    	printf("Options: [nop,nop]  ");
  	}

//	printf("Options: [nop,nop,TS val %u ecr %u] ", ntohl(tcph->tsval), ntohl(tcph->tsecr));
  	printf("Length: %d\n", tcph->doff * 4);
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
		PrintTcp (buff, size);
	}
	else if (tmp_ip == 17){
		++udp;
		//print the udp protocl ;
	}
	else
		++xxx;
	++count;
//	printf ("TCP : %d    UDP :  %d     TOTALE : %d\n",tcp, udp, count);
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
	fprintf (log_fd, "--> IP HEADER\n");
	fprintf (log_fd, "	-> IP VERSION               : %d\n", (unsigned int)ip->version);
	fprintf (log_fd, "	-> IP HEADER LEN            : %d D\n", (unsigned int)ip->ihl);
	fprintf (log_fd, "	-> IP TOTLA LEN             : %d B\n", (unsigned int)ip->tot_len);
	fprintf (log_fd, "	-> TTL                      : %d\n", (unsigned int)ip->ttl);
	if ((unsigned int)ip->protocol == 6)
		fprintf (log_fd, "	-> PROTOCOL                 : %d [TCP]\n", (unsigned int)ip->protocol);
	else if ((unsigned int)ip->protocol == 17)
		fprintf (log_fd, "	-> PROTOCOL                 : %d [UDP]\n", (unsigned int)ip->protocol);
	else if ((unsigned int)ip->protocol == 6)
		fprintf (log_fd, "	-> PROTOCOL                 : %d\n", (unsigned int)ip->protocol);
	fprintf (log_fd, "	-> IP VERSION               : %d\n", ntohs(ip->check));
	fprintf (log_fd, "	-> IP SOURCE                : %s\n", inet_ntoa(source.sin_addr));
	fprintf (log_fd, "	-> IP DESTINATION           : %s\n", inet_ntoa(dest.sin_addr));
}

void PrintTcp(const char *buff, int size){
	size_t ip_len;
	struct iphdr *ip = (struct iphdr *) buff;
	ip_len = ip->ihl * 4;
	struct tcphdr *tcph = (struct tcphdr *) (buff + ip_len);

	PrintEnd ("TCP");
	PrintIpheader(buff,size);
	fprintf (log_fd, "--> TCP HEADER\n");
	fprintf (log_fd, "	-> SHOURCE PORT             : %u\n", ntohs(tcph->source));
	fprintf (log_fd, "	-> DESTINATION  PORT        : %u\n", ntohs(tcph->dest));
	fprintf (log_fd, "	-> SEQUENCE NUMBER          : %u\n", ntohs(tcph->seq));
	fprintf (log_fd, "	-> HEADER LENGTH            : %d\n B",tcph->doff * 4);
	fprintf (log_fd, "	-> FLAG                     :\n");
	fprintf (log_fd, "	    -> URGENT               : %u\n",(unsigned int)tcph->urg);
	fprintf (log_fd, "	    -> ACK                  : %u\n",(unsigned int)tcph->ack);
	fprintf (log_fd, "	    -> SYN                  : %u\n",(unsigned int)tcph->syn);
	fprintf (log_fd, "	    -> PUSH                 : %u\n",(unsigned int)tcph->psh);
	fprintf (log_fd, "	    -> FINISH               : %u\n",(unsigned int)tcph->fin);
	//window
	//checksum
	//pointer
	fprintf (log_fd, "\n                                             [DATA DUMP]\n");
	fprintf (log_fd, "IP HEADER\n");
	PrintDataDump  ((unsigned char *)buff, ip_len);
	fprintf(log_fd, "TCP HEADER\n");
	PrintDataDump ((unsigned char *)buff + ip_len, tcph->doff * 4);
	fprintf(log_fd, "DATA PAYLOAD\n");
	PrintDataDump ((unsigned char *)buff+ip_len+tcph->doff * 4, (size + tcph->doff * 4 - ip->ihl * 4));
	PrintEnd();
	//print data dump for ip
	//printf data dump for tcp
	//print data dump for payloead
}

void PrintUdp (const char *buff, int size){
	size_t ip_len;
	struct  iphdr *ip = (struct iphdr *)buff;
	ip_len = ip->ihl * 4;
	struct udphdr *udph = (struct udphdr *)(buff + ip_len);

	PrintFirst ("UDP");
	PrintIpheader(buff, size);
	fprintf (log_fd, "--> UDP HEADER\n");
	fprintf (log_fd, "	-> SHOURCE PORT             : %u\n", ntohs(udph->source));
	fprintf (log_fd, "	-> DESTINATION PORT         : %u\n", ntohs(udph->dest));
	fprintf (log_fd, "	-> UDP LENGHT               : %u\n", ntohs(udph->len));
	fprintf (log_fd, "	-> UDP CHECKSUM             : %u\n", ntohs(udph->check));
	fprintf (log_fd, "\n                                             [DATA DUMP]\n");
	fprintf (log_fd, "IP HEADER\n");
	PrintDataDump ((unsigned char *)buff, ip_len);
	fprintf (log_fd, "UDP HEADER\n");
	PrintDataDump ((unsigned char *)buff + ip_len, sizeof udph);
	fprintf (log_fd, "DATA PAYLOAD\n");
	PrintDataDump ((unsigned char *)buff + ip_len + sizeof udph, (size - sizeof udph - ip->ihl * 4));
	PrintEnd();
}

void  PrintDataDump(unsigned char* data, int Size) {
	for (i = 0; i < Size; i++) {
        if (i != 0 && i % 16 == 0) { // if one line of hex printing is complete...
            fprintf(log_fd, "         ");
            for (j = i - 16; j < i; j++) {
                if (data[j] >= 32 && data[j] <= 128)
                    fprintf(log_fd, "%c", (unsigned char)data[j]); // if it's a number or alphabet
                else
                    fprintf(log_fd, "."); // otherwise, print a dot
            }
            fprintf(log_fd, "\n");
        }

        if (i % 16 == 0)
            fprintf(log_fd, "   ");
        fprintf(log_fd, " %02X", (unsigned int)data[i]);

        if (i == Size - 1) { // print the last spaces
            for (j = 0; j < 15 - i % 16; j++)
                fprintf(log_fd, "   "); // extra spaces

            fprintf(log_fd, "         ");

            for (j = i - i % 16; j <= i; j++) {
                if (data[j] >= 32 && data[j] <= 128)
                    fprintf(log_fd, "%c", (unsigned char)data[j]);
                else
                    fprintf(log_fd, ".");
            }
            fprintf(log_fd, "\n");
        }
    }
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
  		PrintPacketInfo (buff);
	}
	// after this step you need to procces the packet ;
  	close (socketfd);
	printf ("FINISH ...............\n");
  	return 0;
}

