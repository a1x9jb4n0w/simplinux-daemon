#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

struct addrinfo *res; int sockfd;

void dbody()
{

}

void sigthandler()
{
        close( sockfd ); freeaddrinfo( res );
}

int main()
{
    int pfd[2]; pid_t pid1, pid2;

    if ( pipe(pfd) == -1 ) { printf("pipe() error : exit\n"); exit(EXIT_FAILURE); }

    for ( int i = 0; i < _NSIG; i++ ) signal( i, SIG_DFL );

    pid1 = fork();

    if ( !pid1 ) {

        close( pfd[0] );

        if ( setsid() == -1 ) { printf( "EPERM error : exit\n" ); fflush(stdout); exit(EXIT_FAILURE); }

        pid2 = fork();

        if ( !pid2 ) {

            pid_t chpid = getpid(); char wmsg = 1;

            printf("PID = %i : Ok\n", chpid ); fflush(stdout);

            struct addrinfo hints, *res; int sockfd; unsigned char *hp = (unsigned char*)&hints;

            for ( size_t n = 0; n <= sizeof hints; n++ ) hp[n] = 0;

            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_flags = AI_PASSIVE;

            if ( getaddrinfo( NULL, "31254", &hints, &res ) != 0 ) wmsg = 2;

            else { sockfd = socket( res->ai_family, res->ai_socktype, res->ai_protocol );

              if ( sockfd == -1 ||
                   bind( sockfd, res->ai_addr, res->ai_addrlen ) == -1 ||
                   listen( sockfd, 5 ) == -1 ) wmsg = 2; }

            if ( signal( SIGTERM, sigthandler ) == SIG_ERR ) wmsg = 3;

            write( pfd[1], &wmsg, 1 );

            close( pfd[1] );

            while ( wmsg == 3 || wmsg == 0 ) { sleep(60); }

        }

        else if ( pid2 > 0 ) { close( pfd[1] ); exit(EXIT_SUCCESS); }

        else if ( pid2 == -1 ) { close( pfd[1] );

            printf("fork() #2 error : exit\n"); fflush(stdout); exit(EXIT_FAILURE); }

    }

    else if ( pid1 > 0 ) { close( pfd[1] ); char rmsg = 0;

        while ( !rmsg ) { read( pfd[0], &rmsg, 1 ); }

        close( pfd[0] );

        if ( rmsg == 1 ) printf("Socket : Ok\n"); fflush(stdout);
        if ( rmsg == 2 ) printf("Socket error : exit\n"); fflush(stdout);
        if ( rmsg == 3 ) printf("Socket : Ok\nSIGTERM handler error : well, Ok anyway...\n"); fflush(stdout);
    }

    else if ( pid1 == -1 ) { close( pfd[1] ); close( pfd[0] );

        printf("fork() #1 error : exit\n"); fflush(stdout); exit(EXIT_FAILURE); }

    exit(EXIT_SUCCESS);
}
