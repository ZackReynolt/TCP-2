#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<time.h>

#define LENGTH 512 // Buffer length

int main() {
char c=' ';
int s, s2;
s = socket (AF_INET, SOCK_STREAM, 0);

struct sockaddr_in server;
server.sin_family = AF_INET;
server.sin_port   = 50789;
server.sin_addr.s_addr = INADDR_ANY;
struct sockaddr_in client;
int clen = sizeof(client);

char aux = 'n';
int rc;

rc = bind (s,(struct sockaddr*)&server, sizeof(server));
if (rc < 0) {
	perror("bind");
	exit (1);
}

rc = listen ( s, 5 );
if (rc < 0) {
	perror("listen");
	exit (1);
}

s2 = accept ( s,(struct sockaddr*)&client, &clen);
if (s2 < 0) {
	perror("accept");
	exit (1);
}
	
while(1){

	int l, flag=0;
	char buff[100];
	int noexit = 0;
	char* f_name = "send.txt";
	char sdbuf[LENGTH];
		
	l = read ( s2, buff, 100);
	if (l == 0) break;
	if(l < 0){
		perror("read");
		exit(1);
	}	
	strcpy(buff, "pong");
	write (s2, buff, strlen(buff)+1);
	

	printf("[server]transmision begins\n");
	
	//Transmisión de archivo

	FILE *fp = fopen(f_name, "r");
        if(fp == NULL)
        {
            printf("ERROR: File %s not found.\n", f_name);
            exit(1);
        }
        bzero(sdbuf, LENGTH);
        int f_block_sz;
	printf("[server]sending file");
        while(!feof(fp))//Empieza a leer 0 caracteres
        {
		printf(".");
		f_block_sz = fread(sdbuf, sizeof(char), LENGTH, fp);
		//printf("%s\n",sdbuf);
		if(write(s2, sdbuf, f_block_sz) < 0)
		{
			printf("ERROR: Failed to send file %s.\n", f_name);
			break;
		}
            	bzero(sdbuf, LENGTH);
        }
	printf("file sent!\n");
	fclose(fp);
	printf("[server]file closed\n");
}

//La hora del log!
struct hostent *hp;
hp = gethostbyaddr((char *)&client.sin_addr, sizeof client.sin_addr, client.sin_family);
if(!hp){
	printf("ERROR en get host");
}else{

time_t td;
time(&td);
char dtbuf[128];
int n;
n = (int) strftime(dtbuf, sizeof dtbuf, "%A %b %d %H:%M:%S %Y", localtime(&td));

FILE *log = fopen("log.txt","a");

if(log == NULL) printf("[client error]file %s cannot be opened.\n", "log.txt");
else{
	fprintf(log, "\n[%s]\n", dtbuf);
	fprintf(log, "    ip: %s\n",inet_ntoa(client.sin_addr));
	fprintf(log, "    port: %d\n",ntohs(client.sin_port));
	fprintf(log, "    hostname: %s\n", hp->h_name);
}
fclose(log);
}

close(s2);
printf("[server]komunnikation beendet\n");
close(s);

return 0;
}
