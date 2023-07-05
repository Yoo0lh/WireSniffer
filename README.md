## WireSniffer

**WireSniffer** is a simple packet sniffer that can be used to capture and analyze network traffic. It can be used to identify the different types of traffic that is flowing through a network, as well as the source and destination of that traffic.

## Usage :

To use WireSniffer, you will need to have **root privileges**. Once you have root privileges, you can run the program by typing the following command:

```
./compile.sh run
```
This will start the program and it will begin capturing packets. the output will be show up on the standard output, to see the packet information in details you can check the log file **log_file.txt .**

you can also use the following command to delete the binary and log file.
```
./compile.sh clean
``` 
## Example

The following example shows how to use the socket function to print out information about a TCP packet :
create a socket row for capturing the TCP packets using the socket function .
```c
int socketfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)
```
AF_INET : specifies the address family IPV4.
SOCK_RAW : create a raw socket .
PPROTO_TCP: specifies the protocol to filter in this case tcp.

> you need a root privilege to run this code as raw socket special permission.

```c
int main(){
	char *buff;
	struct sockaddr sock;
	int sock_size = 0;
	int socketfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
	char *buff = (char *) malloc (SIZE);
	if (buff == NULL)
		Error(1, "malloc");
	int data_size = recvfrom(socketfd, buff, 7000, 0, (struct sockaddr *)&sock, (socklen_t *)&soc_size);
	if (data_size < 0) 
		Error(2, "recfrom");
	PrintPacketInfo(buff);
	return 0;
}
```
This code will print out information about the TCP packet that was captured, including the **source** and **destination** IP addresses, the source and destination **ports**, and the **TCP flags**.

## Output

The output of the `PrintPacketInfo()` function will vary depending on the type of packet that is being captured. However, the following is an example of the output that would be produced for a TCP packet:

```sh
Source IP: 10.10.10.10
Destination IP: 10.10.10.11
Source Port: 80
Destination Port: 443
TCP Flags: [SYN ACK]

```
The output of the `PrintTcp() ` and `PrintUdp` functions will be written to the `log_file.txt` file. This file will contain more detailed output about the packets that are captured. This includes information about the **IP header**, the **TCP header**, and the **data payload**.
```sh
---------------------------------------------- [ TCP ] ------------------------------------------
--> IP HEADER
	-> IP VERSION               : 4
	-> IP HEADER LEN            : 5 D
	-> IP TOTLA LEN             : 13312 B
	-> TTL                      : 64
	-> PROTOCOL                 : 6 [TCP]
	-> IP VERSION               : 28395
	-> IP SOURCE                : 10.10.10.10
	-> IP DESTINATION           : 10.10.10.11
--> TCP HEADER
	-> SHOURCE PORT             : 49375
	-> DESTINATION  PORT        : 22
	-> SEQUENCE NUMBER          : 47202
	-> HEADER LENGTH            : 32
	-> FLAG                     :
	    -> URGENT               : 0
	    -> ACK                  : 1
	    -> SYN                  : 0
	    -> PUSH                 : 0
	    -> FINISH               : 0
                                             [DATA DUMP]
IP HEADER
    45 48 00 34 00 00 40 00 40 06 6E EB 0A 0C 08 01         EH.4..@.@.n.....
    0A 0C AF 78                                             ...x
TCP HEADER
    C0 DF 00 16 B8 62 3B 0B 68 29 1B 19 80 10 07 FC         .....b;.h)..�...
    4E 2F 00 00 01 01 08 0A 23 53 4A 97 C7 BA E7 B5         N/......#SJ.....
DATA PAYLOAD
    C6 CB 8A B8 4B 28 29 AA 8D 46 4D F4 64 4F 28 6F         ....K()..FM.dO(o
    B8 42 D4 96 3B 84 8D 45 3B 3B AF 1C 79 43 EE B1         .B..;..E;;..yC..
    CB 50 75 D1 BE BE BE BE BE BE BE BE BE BE BE BE         .Pu.............
    BE BE BE BE BE BE BE BE BE BE BE BE BE BE BE BE         ................
-------------------------------------------------------------------------------------------------
```

## Limitations

**WireSniffer** is a simple packet sniffer and it does not have all of the features of a more comprehensive network analysis tool. However, it can be used to quickly identify the different types of traffic that are flowing through a network.

## Contact

If you have any questions or feedback, please contact me on **twitter** .
