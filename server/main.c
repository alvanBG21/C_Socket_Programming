#include "../utils/socketUtils.h"

struct AcceptedSocket
{
    int acceptedSocketFD;
    struct sockaddr_in address;
    int error;
    bool acceptedSuccessfully;
};

struct AcceptedSocket *acceptedSocket[10];
int acceptedSocketCount = 0;

struct AcceptedSocket *acceptIncomingConnection(int serverSocketFD);
void *receiveAndPrintIncomingData(void *);
void sendReceivedMessageToOtherSockets(char *, int );
void startAcceptingIncomingConnections(int socketFD);
void receiveAndPrintIncomingDataOnSeparateThread(struct AcceptedSocket *psocket);

void *receiveAndPrintIncomingData(void *arg)
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
            sendReceivedMessageToOtherSockets(buffer, socketFD);
        }

        if (bytes == 0)
            break;
    }
    close(socketFD);
}

void sendReceivedMessageToOtherSockets(char *buffer, int socketFD)
{
    printf("%d \n", acceptedSocketCount);
    for (int i = 0; i < acceptedSocketCount; i++)
    {
        if (acceptedSocket[i]->acceptedSocketFD != socketFD)
        {
            send(acceptedSocket[i]->acceptedSocketFD, buffer, strlen(buffer), 0);
        }
    }
}
void startAcceptingIncomingConnections(int socketFD)
{
    while (true)
    {

        struct AcceptedSocket *clientSocket = acceptIncomingConnection(socketFD);
        if (clientSocket->acceptedSuccessfully)
        {
            acceptedSocket[acceptedSocketCount++] = clientSocket;
            receiveAndPrintIncomingDataOnSeparateThread(clientSocket);
        }
        else
        {

            printf("Accept error: %d\n", clientSocket->error);
        }
    }
}

void receiveAndPrintIncomingDataOnSeparateThread(struct AcceptedSocket *psocket)
{
    pthread_t id;
    int *fd = malloc(sizeof(int));
    *fd = psocket->acceptedSocketFD;
    pthread_create(&id, NULL, receiveAndPrintIncomingData, fd);
    pthread_detach(id);
}

struct AcceptedSocket *acceptIncomingConnection(int serverSocketFD)
{
    struct sockaddr_in clientAddress;
    socklen_t clientAddressSize = sizeof(clientAddress);

    int clientSocketFD =
        accept(serverSocketFD, (struct sockaddr *)&clientAddress, &clientAddressSize);

    struct AcceptedSocket *acceptedSocket = malloc(sizeof(struct AcceptedSocket));
    acceptedSocket->address = clientAddress;
    acceptedSocket->acceptedSocketFD = clientSocketFD;
    acceptedSocket->acceptedSuccessfully = clientSocketFD > 0;

    if (!acceptedSocket->acceptedSuccessfully)
        acceptedSocket->error = clientSocketFD;

    return acceptedSocket;
}

int main()
{
    int serverSocketFD = createTCPIpv4Socket();
    struct sockaddr_in *address = createIpv4Address("0.0.0.0", 2000);

    int res = bind(serverSocketFD, (struct sockaddr *)address, sizeof(*address));
    printf("bind result: %d\n", res);

    if (res == 0)
        printf("Socket was bound successfully\n");

    listen(serverSocketFD, 10);

    startAcceptingIncomingConnections(serverSocketFD);
    shutdown(serverSocketFD, SHUT_RDWR);

    return 0;
}
