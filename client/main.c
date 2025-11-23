#include "../utils/socketUtils.h"

void *listenAndPrint(void *arg)
{

    int socketFD = *(int *)arg;
    free(arg);
    char buffer[1024];

    while (true)
    {

        ssize_t bytes = recv(socketFD, buffer, 1023, 0);
        buffer[bytes] = '\0';
        if (bytes > 0)
        {
            printf("%s\n", buffer);
        }

        if (bytes == 0)
            break;
    }
    close(socketFD);
}
void receiveAndPrintIncomingDataOnNewThread(int socketFD)
{
    pthread_t id;
    int *fd = malloc(sizeof(int));
    *fd = socketFD;
    pthread_create(&id, NULL, listenAndPrint, fd);
    pthread_detach(id);
}

int main()
{
    int socketFD = createTCPIpv4Socket();

    char *ip = "127.0.0.1";

    struct sockaddr_in *address = createIpv4Address(ip, 2000);
    int result = connect(socketFD, address, sizeof(*address));

    if (result == 0)
        printf("connection successful \n");

    receiveAndPrintIncomingDataOnNewThread(socketFD);


char *name = NULL;
    size_t nameSize = 0;
    printf("Please enter your name: ");
    ssize_t nameCount = getline(&name, &nameSize, stdin);
    name[nameCount - 1] = 0;

    char *message = NULL;
    size_t lineSize = 0;
    printf("Type and we will send (type exit to quit)..\n");

    char buffer[1024];
    while (true)
    {
        ssize_t charCount = getline(&message, &lineSize, stdin);
        message[charCount - 1] =0;

        sprintf(buffer, "%s:%s",name,message);

        if (charCount > 0)
        {
            if (strcmp(message, "exit") == 0)
                break;
            ssize_t charSizeSent = send(socketFD, buffer, strlen(buffer), 0);
        }
    }

    close(socketFD);

    return 0;
}