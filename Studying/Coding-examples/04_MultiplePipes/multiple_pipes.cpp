// Adapted from twopipes.c
// http://www.cs.loyola.edu/~jglenn/702/S2005/Examples/dup2.html

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

const int READ_INPUT_PIPE = 0;
const int WRITE_OUTPUT_PIPE = 1;

int main(int argc, char **argv)
{
    int status;
    int i;

    // catToGrep[0] = read end    of cat->grep pipe   (read by grep)
    // catToGrep[1] = write end   of cat->grep pipe   (written by cat)
    // grepToCut[2] = read end    of grep->cut pipe   (read by cut)
    // grepToCut[3] = write end   of grep->cut pipe   (written by grep)
    int catToGrep[2];
    int grepToCut[2];
    pipe( catToGrep );
    pipe( grepToCut );

    char *cat_args[] = {"/bin/echo", "abcd", NULL};
    char *grep_args[] = {"/bin/grep", "a", NULL};
    char *cut_args[] = {"/usr/bin/cut", "-b", "1-10", NULL};

    if ( fork() == 0 )      // cat
    {
        // Replace process's stdout with the write of the 1st pipe
        dup2( catToGrep[ 1 ], STDOUT_FILENO );

        // Close pipes
        close( catToGrep[0] );
        close( catToGrep[1] );
        close( grepToCut[0] );
        close( grepToCut[1] );

        execv( *cat_args, cat_args );

        return 0;
    }

    if ( fork() == 0 )      // grep
    {
        // replace grep's stdin with read end of 1st pipe
        dup2( catToGrep[0], STDIN_FILENO );

        // replace grep's stdout with write end of 2nd pipe
        dup2( grepToCut[1], STDOUT_FILENO );

        // Close pipes
        close( catToGrep[0] );
        close( catToGrep[1] );
        close( grepToCut[0] );
        close( grepToCut[1] );

        execvp(*grep_args, grep_args);

        return 0;
    }

    if ( fork() == 0 )      // cut
    {
        // replace cut's stdin with input read of 2nd pipe
        dup2( grepToCut[0], STDIN_FILENO );

        // Close pipes
        close( catToGrep[0] );
        close( catToGrep[1] );
        close( grepToCut[0] );
        close( grepToCut[1] );

        execvp( *cut_args, cut_args );

        return 0;
    }


  // only the parent gets here and waits for 3 children to finish
  close( catToGrep[0] );
  close( catToGrep[1] );
  close( grepToCut[0] );
  close( grepToCut[1] );

  for (i = 0; i < 3; i++)
    wait(&status);
}
