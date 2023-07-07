#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void dbody()
{

}

int main()
{

    int pfd[2]; pid_t fres, cres;

    if ( pipe(pfd) == -1 ) { printf("Pipe Failed : exit"); exit(EXIT_FAILURE); }

    else { printf("-----\n"); fflush(stdout); }

    //for ( int i = 0; i < _NSIG; i++ ) signal( i, SIG_DFL );

    fres = fork();

    if ( !fres ) {

        close( pfd[0] );

        int sid = setsid();

        if ( sid >= 0 ) { printf( "Child SID : %i\n", sid ); fflush(stdout); } else { printf( "EPERM Error : exit" ); exit(EXIT_FAILURE); }

        cres = fork();

        if ( !cres ) {

            pid_t chpid = getpid(); char wmsg = 1;

            printf("Second child PID : %i\n", chpid ); fflush(stdout);

            write( pfd[1], &wmsg, 1 );

            close( pfd[1] );

            while ( 1 ) { sleep(60); }

        }

        else if ( cres > 0 ) { pid_t fcpid = getpid();

            close( pfd[1] ); printf("First child PID : %i - exited\n", fcpid ); fflush(stdout);

            exit(EXIT_SUCCESS);

        }

        else if ( cres < 0 ) { printf("Second fork failed...\n"); fflush(stdout); exit(EXIT_FAILURE); }

    }

    else if ( fres > 0 ) { pid_t ppid = getpid();

        printf( "Parent process PID : %i \n", ppid ); fflush(stdout);

        close( pfd[1] );

        char rmsg = 0;

        while ( !rmsg ) { read( pfd[0], &rmsg, 1 ); }

        close( pfd[0] );

        printf("Parent exited\n");
    }

    else if ( fres < 0 ) { printf("First fork failed...\n"); exit(EXIT_FAILURE); }

    exit(EXIT_SUCCESS);
}
