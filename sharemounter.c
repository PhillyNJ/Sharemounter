
#include <stdio.h>
#include <unistd.h>
#include <ctype.h> 
#include <stdlib.h>
#include <string.h>
#include <termios.h>

int loadConfig();
int getch();
char* cfg;
char ipAddress[256];
char folder[256];
char cmd[256];
char username[256];
char type[256];

int main(int argc, char *argv[])
{	
        if ( argc != 2 ){
		printf("\nusage: %s configFile\n", argv[0]);
		return(1);
	}
	cfg = argv[1];
	
	int success = loadConfig();
	int ch;
	int i = 0;	
	char pword[BUFSIZ];
		
	printf ("\nEnter your network password:");
	
	fflush(stdout);  

	while ((ch = getch()) != EOF 
			&& ch != '\n' 
			&& ch != '\r' 
			&& i < sizeof(pword) - 1)
	{
		if (ch == '\b' && i > 0) 
		{
			printf("\b \b");
			fflush(stdout);
			i--;
			pword[i] = '\0';
		}
		else if (isalnum(ch))
		{
			putchar('*');
			pword[i++] = (char)ch;
		}
	}

	pword[i] = '\0';  

	/* Config File*/
	if(success == 0){
                int len = strlen(ipAddress);
		if( ipAddress[len-1] == '\n' ){
			ipAddress[len-1] = 0;
		}
		int userlen = strlen(username);
		if( username[userlen-1] == '\n' ){
                        username[userlen-1] = 0;
                }
		int typelen = strlen(type);
		if( type[typelen-1] == '\n' ){
                        type[typelen-1] = 0;
                }
                

		int typeResultMac = strncmp(type, "mac", 20);
		int typeResultWin = strncmp(type, "windows", 20);
		int pswordResult = strncmp(pword, "", 20);

		if (typeResultMac == 0){
			printf("mounting for mac\n");
			strcpy (cmd, "sudo mount_afp afp://");
	                strcat (cmd, username);
        	        strcat (cmd, ":");
                	strcat (cmd, pword);
               		strcat (cmd, "@");
                	strcat (cmd, ipAddress);
                	strcat (cmd, " ");
                	strcat (cmd, folder);
                
		}else if(typeResultWin == 0 && pswordResult == 0 ){
			printf("mounting for windows\n");
			strcpy (cmd, "sudo mount -t cifs -o guest //");
			strcat (cmd,  ipAddress);
			strcat (cmd, " ");
			strcat (cmd, folder);
		}else if (typeResultWin == 0) {
			printf("mounting for windows\n");
                        strcpy (cmd, "sudo mount -t cifs -o username=");
			strcat (cmd, username);
			strcat (cmd, ",password=");
			strcat (cmd, pword);
			strcat (cmd, " //");
                        strcat (cmd,  ipAddress);
                        strcat (cmd, " ");
                        strcat (cmd, folder);
		}
		/* printf("%s\n",cmd);*/
		system(cmd);
		
	}
	return 0;
}

int loadConfig(){

	FILE *fp;
	char buffer[256];
	char tempStr[256];

	fp = fopen(cfg, "r" );

	if ( fp == NULL ){
	
		printf("Unable to open %s", cfg);
		return -1;	
	}
	
	while(  fgets( buffer, 256, fp ) != NULL ){

		if (strstr(buffer, "ip=") != NULL) {
			strncpy(tempStr, buffer + 3  ,sizeof(buffer));
			tempStr[sizeof(buffer) - 3] = '\0';
			printf("IP/Share: %s", tempStr);
			strncpy(ipAddress, tempStr  ,sizeof(tempStr));
		}
		if (strstr(buffer, "folder=") != NULL) {
			strncpy(tempStr, buffer + 7  ,sizeof(buffer));
			tempStr[sizeof(buffer) - 7] = '\0';
			printf("\nMount Point: %s", tempStr);
			strncpy(folder, tempStr  ,sizeof(tempStr));
		}
		if (strstr(buffer, "username=") != NULL) {
                        strncpy(tempStr, buffer + 9  ,sizeof(buffer));
                        tempStr[sizeof(buffer) - 9] = '\0';
                        printf("\nUsername: %s", tempStr);
                        strncpy(username, tempStr  ,sizeof(tempStr));
                }

		if (strstr(buffer, "type=") != NULL) {
                        strncpy(tempStr, buffer + 5  ,sizeof(buffer));
                        tempStr[sizeof(buffer) - 5] = '\0';
                        printf("\nSystem:s", tempStr);
                        strncpy(type, tempStr  ,sizeof(tempStr));
                }

	}
	return 0;
}

int getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
