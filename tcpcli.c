#include "unp.h" 

int 
main(int argc, char **argv)
{   
    /* idenfities the socket */
    int sockfd, n; 

    /* a char array used to store data received from the server */
    char recvline[MAXLINE + 1];

    /* sockaddr_in stories info about IPv4 */
    struct sockaddr_in servaddr;

    /* program takes exactly 1 IPaddress so argc should be exactly 2 (<executable><IPaddress>)*/
    if (argc != 2)
        err_quit("usage: a.out <IPaddres>");
    
    /* Create a new Socket with: 
    1. IPv4 (AF_INET)
    2. TCP (SOCK_STREAM)
    3. Default TCP protocol(0)

    If successful returns the socket descriptor
    else -1.
     */
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_sys("socket error"); 

    /* Cleans servaddr and fills it with 0 */
    memset(&servaddr, 0, sizeof(servaddr));

    /* specifies the address family and IPv4 address */
    servaddr.sinfamily = AF_INET;
    servaddr.sin_port = htons(13); /* Daytime server on Port 13 */ 

    /* inet_pton converts IPaddress to binary. 

    Syntax: inet_pton(family,source destination, final destination)

    return values: 
    1 - Success 
    2 - Invalid IP address format
    -1 - Func error 
    */
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        err_quit("inet_pton error for &s", argv[1]);

    /* Establishes a connection b/w clinet socket and Server...
    
    Syntax: connect(socket_fd, server_address, address_size);

    0 - connection successful
    -1 - connection failed 
    */
    if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
        err_sys("connect error"); 

        /* Loop receives data from the server and prints it
         
        n > 0 - Datr was received thus continue
        c == 0 - Server closed the connection, stop...
        n < 0 - read error 
        */
    while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0; 
        /* Tries to print the data */
        if (fputs(recvline, stdout) == EOF)
            err_sys("fputs error"); 
    }
    if (n < 0)
        err_sys("read error");

    exit(0);
}