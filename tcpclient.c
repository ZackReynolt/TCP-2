#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define LENGTH 512 // Buffer length

int main() {
	int s, s2;
	char c=' ';
	s = socket (AF_INET, SOCK_STREAM, 0);
	char revbuf[LENGTH]; //Buffer de recepción

	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port   = 50789;
	server.sin_addr.s_addr = inet_addr ("141.57.9.14");

	int rc;
	rc = connect(s,(struct sockaddr*)&server,sizeof(server));
	if (rc < 0) {
		perror("connect");
		exit (2);
	}

	//Data transfer
	int l, flag=0;
	char buff[100];
	char aux = 'n';
	strcpy(buff, "ping");

	//Senden - Simple, sólo se hace una vez
	//while(c != EOF){
		
		l = write ( s, buff, strlen(buff)+1);
		if(l < 0){
			perror("send");
			exit(1);
		}
		l = read (s, buff, sizeof(buff));
		//printf ("Empfang: %d Bytes, %s\n", l, buff);
		

		printf("[client]reception begins\n");
		//Recepción del archivo
		
		char* f_name = "receive.txt";

		FILE *check = fopen(f_name,"r");
		if(check == NULL){
			//File does not exist
			//Create it the normal way
			printf("[client]file does not exist, creating new file...\n");
	    		FILE *fp = fopen(f_name, "w"); 
				//w crea o si existe, borra contenido y empieza
	    		if(fp == NULL) printf("[client error]file %s cannot be opened.\n", f_name);
	    		else
	    		{
				l = LENGTH;
				while(l == LENGTH){
					bzero(revbuf, LENGTH);
					l = read(s, revbuf, LENGTH);
					fprintf(fp, revbuf);
				}
	            		fclose(fp);
	    		}

		}else{
			//File exists, ask first
			printf("[client]file already exists, do you want to replace it? [y/n]: ");
			char res[1];
			scanf("%s", res);
			if(strcmp(res,"y") == 0){
				//Replace
				printf("[client]replacing file...\n");
				FILE *fp = fopen(f_name, "w"); 
				//w crea o si existe, borra contenido y empieza
	    			if(fp == NULL) printf("[client error]file %s cannot be opened.\n", f_name);
	    			else
	    			{
					l = LENGTH;
					while(l == LENGTH){
						bzero(revbuf, LENGTH);
						l = read(s, revbuf, LENGTH);
						fprintf(fp, revbuf);
					}
	            			fclose(fp);
	    			}
			}else{
				printf("[client]file was not replaced\n");
			}
		}
	//}
	close(s);
	printf("[client]kommunikation beendet\n");
return 0;
}
