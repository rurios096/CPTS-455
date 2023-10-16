#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>

#define PORT 8000
#define MAX 256
#define MAXLEN 4096

int main()
{
    int option, serverSocket;
    char *serverIP = "127.0.0.0", line[MAX];
    struct sockaddr_in serverAddress;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0); // new socket

    serverAddress.sin_family = AF_INET; // fill in info
    serverAddress.sin_addr.s_addr = inet_addr(serverIP);
    serverAddress.sin_port = htons(PORT);

    connect(serverSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress)); // connects socket with server

    while (1)
    {
        read(serverSocket, line, MAX);
        printf("server: %s\n", line);
        

        printf("'1' to share a file, '2' to send a message\n");
        
        memset(line, 0, sizeof(line));
        fgets(line, MAX, stdin);
	line[strlen(line) - 1] = '\0';
	
        if (strcmp(line, "1") == 0)
        {   
            printf("file name: ");
            
            memset(line, 0, sizeof(line));
            fgets(line, MAX, stdin);
            line[strlen(line) - 1] = '\0';

            int file, src, dest;
            char buf[1024], line2[MAX], line3[MAX];
            

            src = open(line, O_RDONLY);
            
            strcpy(line2, "../server/");
            strcat(line2, line);
            dest = open(line2, O_WRONLY | O_CREAT, 0666);
            
            while(1)
            {
                file = read(src, buf, MAXLEN);
                if (file == 0) break;
                write(dest, buf, file);
            }
            
            close(src);
            close(dest);
            
            strcpy(line3, "shared file - ");
            strcat(line3, line);
            write(serverSocket, line3, MAX);
            printf("client: %s\n", line3);
        }
        
        else
        {
            printf("message to server: ");
        
            memset(line, 0, sizeof(line));
            fgets(line, MAX, stdin);
            line[strlen(line) - 1] = '\0';

            printf("client: %s\n", line);
            write(serverSocket, line, MAX);
        }
    }
}

