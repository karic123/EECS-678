#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>

#define R_FILE "/proc/meminfo"
#define BSIZE 256

const int READ_INPUT_PIPE = 0;
const int WRITE_OUTPUT_PIPE = 1;

int main()
{
	int status;
	pid_t pid_1, pid_2;

	/* XXX - need to declare pipes and make the pipe() system call before
	 * forking any children
	 */
	int pid12Pipe[2];
	pipe( pid12Pipe );

	pid_1 = fork();
	if (pid_1 == 0) {
		/* process a */

		int rfd;
		size_t rsize;
		char buf[BSIZE];

		if ((rfd = open(R_FILE, O_RDONLY)) < 0) {
			fprintf(stderr, "\nError opening file: %s. ERROR#%d\n", R_FILE, errno);
			return EXIT_FAILURE;
		}

		/* read contents of file and write it out to a pipe */
		while ((rsize = read(rfd, buf, BSIZE)) > 0) {
			/* XXX - this should write to a pipe - not to stdout */
			close( pid12Pipe[ READ_INPUT_PIPE ] );
			//write(STDOUT_FILENO, buf, rsize);
			write( pid12Pipe[ WRITE_OUTPUT_PIPE ], buf, rsize );
		}

		close(rfd);
		return 0;
	}

	pid_2 = fork();
	if (pid_2 == 0) {
		/* process b */

		size_t rsize;
		char buf[BSIZE];

		/* read from pipe and write out contents to the terminal */

		/* XXX - this should read from a pipe - not from stdin */
//		while ((rsize = read(STDIN_FILENO, buf, BSIZE)) > 0) {
//			write(STDOUT_FILENO, buf, rsize);
//		}

        close( pid12Pipe[ WRITE_OUTPUT_PIPE ] );
		while ((rsize = read( pid12Pipe[ READ_INPUT_PIPE ], buf, BSIZE)) > 0 ) {
			write(STDOUT_FILENO, buf, rsize);
		}

		return 0;
	}

	/* shell process */
	if ((waitpid(pid_1, &status, 0)) == -1) {
		fprintf(stderr, "Process 1 encountered an error. ERROR%d", errno);
		return EXIT_FAILURE;
	}

	if ((waitpid(pid_2, &status, 0)) == -1) {
		fprintf(stderr, "Process 2 encountered an error. ERROR%d", errno);
		return EXIT_FAILURE;
	}

	return 0;
}
