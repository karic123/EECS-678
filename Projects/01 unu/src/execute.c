/*
    SPRING 2017 EECS 678
    Rachel Morris
    Aishwarya Bhatnagar

    Repository: https://github.com/Rachels-studies/EECS-678-Project-1
*/

/**
 * @file execute.c
 *
 * @brief Implements interface functions between Quash and the environment and
 * functions that interpret an execute commands.
 *
 * @note As you add things to this file you may want to change the method signature
 */

#include "execute.h"

#include <stdio.h>
#include <sys/wait.h>

#include "quash.h"

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include "debugout.h"

#include "deque.h"

// pid queue
IMPLEMENT_DEQUE_STRUCT( PidQueue, int );
IMPLEMENT_DEQUE( PidQueue, int );

struct Job
{
  struct PidQueue pidQueue;
  int id;
  pid_t firstPid;
  char* command;
};


// job queue
IMPLEMENT_DEQUE_STRUCT( JobQueue, struct Job );
IMPLEMENT_DEQUE( JobQueue, struct Job );
bool jobQueueInitialized = false;

// Global Job list
struct JobQueue jobList;

void FreeJobAndProcesses(struct Job s_job) {
	struct Job *job = &s_job;
	if ( job != NULL )
	{
		destroy_PidQueue( &job->pidQueue );
		free( job->command );
	}

	return;
}



#define READ_END 0
#define WRITE_END 1


//Creating a global tracker for pipes
static int p1[2][2];
static int prev_pipe = -1;
static int next_pipe = 0;

static int outfile;
static int infile;


// Remove this and all expansion calls to it
/**
 * @brief Note calls to any function that requires implementation
 */
#define IMPLEMENT_ME()                                                  \
  fprintf(stderr, "IMPLEMENT ME: %s(line %d): %s()\n", __FILE__, __LINE__, __FUNCTION__)

const int PATH_MAX = 1024;

int PrintError()
{
    printf( "\n ERROR! \t %s \n", strerror( errno ) );
    return errno;
}

bool IsChild( pid_t pid )
{
  return ( pid == 0 );
}

/***************************************************************************
 * Interface Functions
 ***************************************************************************/

// Return a string containing the current working directory.
// https://linux.die.net/man/3/get_current_dir_name
/*
As an extension to the POSIX.1-2001 standard, Linux (libc4, libc5, glibc) getcwd()
allocates the buffer dynamically using malloc(3) if buf is NULL.
In this case, the allocated buffer has the length size unless size is zero,
when buf is allocated as big as necessary.
The caller should free(3) the returned buffer.
*/
char* get_current_directory(bool* should_free) {    // DONE
	char* directory = getcwd (NULL, 0);
  *should_free = true;
  return directory;
}

// Returns the value of an environment variable env_var
/*
You should not have to search for environment variables ($) in your functions.
The parser uses your implementation of lookup_env() function to expand the environment
variables for you.
*/
// https://linux.die.net/man/3/getenv
const char* lookup_env(const char* env_var) {    // DONE
  char* env = getenv( env_var );
  return env;
}

// Check the status of background jobs
// wait should be non-blocking!
// https://linux.die.net/man/2/waitpid

// TODO: Check on the statuses of all processes belonging to all background
// jobs. This function should remove jobs from the jobs queue once all
// processes belonging to a job have completed.
void check_jobs_bg_status() {

  size_t length = length_JobQueue( &jobList );
  int counter = 0;

  while ( counter < length )
  {
    // Access current item...:
    struct Job currentJob = pop_front_JobQueue( &jobList );



    if ( !is_empty_PidQueue( &currentJob.pidQueue ) )
    {
      pid_t currentPid = pop_front_PidQueue (&currentJob.pidQueue);


      int status;
      pid_t result = waitpid(
        currentPid,
        &status,
        WNOHANG );

      if ( result == 0 )
      {
        // Done with this job
        // Pop the process
        push_back_PidQueue( &currentJob.pidQueue, currentPid );
        break;
      }
    }

    if ( is_empty_PidQueue( &currentJob.pidQueue ) )
    {
      print_job_bg_complete(
        currentJob.id,
        currentJob.firstPid,
        currentJob.command );
        FreeJobAndProcesses( currentJob );
    }
    else
    {
      push_back_JobQueue( &jobList, currentJob );
    }

    counter++;
  }
}

// Prints the job id number, the process id of the first process belonging to
// the Job, and the command string associated with this job
void print_job(int job_id, pid_t pid, const char* cmd) {    // DONE?
  printf("[%d]\t%8d\t%s\n", job_id, pid, cmd);
  fflush(stdout);
}

// Prints a start up message for background processes
void print_job_bg_start(int job_id, pid_t pid, const char* cmd) {    // DONE?
  printf("Background job started: ");
  print_job(job_id, pid, cmd);
}

// Prints a completion message followed by the print job
void print_job_bg_complete(int job_id, pid_t pid, const char* cmd) {    // DONE?
  printf("Completed: \t");
  print_job(job_id, pid, cmd);
}

/***************************************************************************
 * Functions to process commands
 ***************************************************************************/
 /*
 When implementing the run_<command type> functions in src/execute.c,
 the command structures usually hold everything needed to pass to the
 corresponding function calls.

 For example, the GenericCommand structure
 contains an args field fully formatted and ready to pass into the argv
 argument of the execvp(3) library function. The one exception to this rule
 is run_cd(). In the CDCommand structure, the field dir is the path that the user typed.
 This needs to be expanded to an absolute path with realpath(3) before you use it.
 */


// Run a program reachable by the path environment variable, relative path, or
// absolute path
void run_generic(GenericCommand cmd) {
  
  // Execute a program with a list of arguments. The `args` array is a NULL
  // terminated (last string is always NULL) list of strings. The first element
  // in the array is the executable
  char* exec = cmd.args[0];
  char** args = cmd.args;

  if ( execvp( exec, args ) == -1 )
  {
    perror("ERROR: Failed to execute program");
  }
}

void run_echo(EchoCommand cmd) {  // DONE

  // Print an array of strings. The args array is a NULL terminated (last
  // string is always NULL) list of strings.
  char** str = cmd.args;

  int a=0;
  while(cmd.args[a] != NULL)
  {
    printf("%s ", str[a]);
    a++;
  }
  printf( "\n" );
  fflush( stdout );
}

// https://linux.die.net/man/3/setenv
// The setenv() function returns zero on success, or -1 on error,
// with errno set to indicate the cause of the error.
// int setenv(const char *name, const char *value, int overwrite);
void write_env(const char* env_var, const char* val)  // DONE
{
  // TODO: Remove debug output
 
  int result = setenv( env_var, val, 1 );

  if ( result == -1 )
  {
    PrintError(); // prints errno message
  }
}

// Sets an environment variable
void run_export(ExportCommand cmd) {  // DONE
 
  // Write an environment variable
  const char* env_var = cmd.env_var;
  const char* val = cmd.val;
  write_env( env_var, val );
}

// Changes the current working directory
void run_cd(CDCommand cmd) {  // DONE


  // Get the directory name
  const char* dir = cmd.dir;


  // Check if the directory is valid
char buff[PATH_MAX];
realpath(dir,buff);

  // Check if the directory is valid
  	if (dir == NULL) {
    perror("ERROR: Failed to resolve path");
    return;
  }

  bool shouldFree;
char* current_dir=get_current_directory( &shouldFree );



write_env("OLD_PWD",current_dir);

  if(chdir(buff)== -1)
{
printf("Error in cha‌nging directory\n");
free(buff);
exit(EXIT_FAILURE);
}
free(current_dir);
write_env("PWD",buff);
}

// Sends a signal to all processes contained in a job
void run_kill(KillCommand cmd) {

  int signal = cmd.sig;
  int job_id = cmd.job;

  size_t length = length_JobQueue( &jobList );
  int counter = 0;
  struct Job* currentJob;
  bool found = false;

  while ( counter < length )
  {
    // Access current item...:
    currentJob = &jobList.data[ counter ];

    if ( currentJob->id == job_id )
    {
      found = true;
      break;
    }
    counter++;
  }

  if ( !found )
  {
    // Job not found

    return;
  }

  // Iterate through all the processes and kill them
  int processLength = length_PidQueue( &currentJob->pidQueue );
  while ( processLength > 0 )
  {
    pid_t currentPid = currentJob->pidQueue.data[ 0 ]; // get front item


    kill( currentPid, signal );
    pop_front_PidQueue( &currentJob->pidQueue );

    processLength = length_PidQueue( &currentJob->pidQueue );
  }
}


// Prints the current working directory to stdout
void run_pwd() {  // DONE

  bool shouldFree;
  char* currentDirectory = get_current_directory( &shouldFree );

  printf( "%s \n", currentDirectory );

  if ( shouldFree )
  {
    free( currentDirectory );
  }

  // Flush the buffer before returning
  fflush(stdout);
}

// Prints all background jobs currently in the job list to stdout
void run_jobs()
{

  size_t length = length_JobQueue( &jobList );
  int counter = 0;
  struct Job currentJob;

  while ( counter < length )
  {
    currentJob = pop_front_JobQueue( &jobList );
    // Access current item...:
    print_job( currentJob.id, currentJob.firstPid, currentJob.command );
    push_back_JobQueue( &jobList, currentJob );
    counter++;
  }

  // Flush the buffer before returning
  fflush(stdout);
}

/***************************************************************************
 * Functions for command resolution and process setup
 ***************************************************************************/

char* debug_get_command_type(CommandType type)
{
  if      ( type == GENERIC)  { return "[GENERIC]"; }
  else if ( type == ECHO)     { return "[ECHO]"; }
  else if ( type == PWD)      { return "[PWD]"; }
  else if ( type == JOBS)     { return "[JOBS]"; }
  else if ( type == EXPORT)   { return "[EXPORT]"; }
  else if ( type == CD)       { return "[CD]"; }
  else if ( type == KILL)     { return "[KILL]"; }
  else if ( type == EXIT)     { return "[EXIT]"; }
  else if ( type == EOC)      { return "[EOC]"; }
  else if ( type == CD)       { return "[CD]"; }
  return "";
}

/**
 * @brief A dispatch function to resolve the correct @a Command variant
 * function for child processes.
 *
 * This version of the function is tailored to commands that should be run in
 * the child process of a fork.
 *
 * @param cmd The Command to try to run
 *
 * @sa Command
 */
void child_run_command(Command cmd) {   // DONE

  CommandType type = get_command_type(cmd);

  switch (type) {
  case GENERIC:
    run_generic(cmd.generic);
    break;

  case ECHO:
    run_echo(cmd.echo);
    break;

  case PWD:
    run_pwd();
    break;

  case JOBS:
    run_jobs();
    break;

  case EXPORT:
  case CD:
  case KILL:
  case EXIT:
  case EOC:
    break;

  default:
    fprintf(stderr, "Unknown command type: %d\n", type);
  }
}

/**
 * @brief A dispatch function to resolve the correct @a Command variant
 * function for the quash process.
 *
 * This version of the function is tailored to commands that should be run in
 * the parent process (quash).
 *
 * @param cmd The Command to try to run
 *
 * @sa Command
 */
void parent_run_command(Command cmd) {   // DONE

  CommandType type = get_command_type(cmd);

  switch (type) {
  case EXPORT:
    run_export(cmd.export);
    break;

  case CD:
    run_cd(cmd.cd);
    break;

  case KILL:
    run_kill(cmd.kill);
    break;

  case GENERIC:
  case ECHO:
  case PWD:
  case JOBS:
  case EXIT:
  case EOC:
    break;

  default:
    fprintf(stderr, "Unknown command type: %d\n", type);
  }
}

/**
 * @brief Creates one new process centered around the @a Command in the @a
 * CommandHolder setting up redirects and pipes where needed
 *
 * @note Processes are not the same as jobs. A single job can have multiple
 * processes running under it. This function creates a process that is part of a
 * larger job.
 *
 * @note Not all commands should be run in the child process. A few need to
 * change the quash process in some way
 *
 * @param holder The CommandHolder to try to run
 *
 * @sa Command CommandHolder
 */
 /*
 The create_process() function is intended to be the place where you [fork processes],
 handle pipe creation, and file redirection. You should not call execvp(3) from this function.
 Instead you should call derivatives of the example_run_command() function.

 Also you can determine whether you should use the boolean variables at the top
 of this function to determine if pipes and redirects should be setup. It may be
 necessary to keep a global execution state structure so that different calls to
 create process can view important information created in previous invocations of
 create_process() (i.e. the file descriptors for open pipes of previous processes).
 */

 /*
                     The current job
      Process A         Process B         Process C
         | ---------------- | --------------- |
         stdout       stdin   stdout     stdin
              prePipes           postPipes
 */
// http://codewiki.wikidot.com/c:system-calls:open
void create_process(CommandHolder holder, struct Job* job) {

  // Read the flags field from the parser
  bool p_in  = holder.flags & PIPE_IN;
  bool p_out = holder.flags & PIPE_OUT;
  bool r_in  = holder.flags & REDIRECT_IN;
  bool r_out = holder.flags & REDIRECT_OUT;
  bool r_app = holder.flags & REDIRECT_APPEND; // This can only be true if r_out is true

/*
The pipe the previous child process has written to
(so that we may read from it in the new child process)

The pipe the new child process is going to write to
(so that the next child may read from it)
*/

  outfile = -1;
  infile = -1;

  // Setup pipes, redirects, and new process
  if (p_out)
  {
    pipe(p1[next_pipe]);
  }
//Creating a child by forking a process
  int pid = fork();
  if (pid == 0)
  {

    if (p_in)
    {
      dup2 (p1[prev_pipe][READ_END], STDIN_FILENO);

      /*We are never going to write to the previous pipe so we can safely close it */
      close (p1[prev_pipe][WRITE_END]);
    }

    if (p_out)
    {
      dup2 (p1[next_pipe][WRITE_END], STDOUT_FILENO);
      /* We are never going to read from our own pipe so we can safely close it */
      close(p1[next_pipe][READ_END]);
    }

    if ( r_in ) // ???
    {
      infile = open( holder.redirect_in, O_RDONLY );
      dup2( infile, STDIN_FILENO );
      close(infile);
    }

    if ( r_out ) // ???
    {
      if ( r_app )
      {
        outfile = open( holder.redirect_out, O_RDWR | O_APPEND );

        dup2( outfile ,STDOUT_FILENO );
      }
      else
      {
      	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
        outfile = open( holder.redirect_out, O_WRONLY | O_CREAT |  O_TRUNC ,mode  );

        dup2( outfile, STDOUT_FILENO );
      }
close(outfile);

    }

    child_run_command(holder.cmd);

    exit(EXIT_SUCCESS);

  }
  else
  {
    /*Closing the remaining pipes in the parent*/

    if(p_out)
    {
      close (p1[next_pipe][WRITE_END]);
    }

    /*Pipe trackers updation for the next iteration cycle*/
    next_pipe = (next_pipe + 1) % 2;
    prev_pipe = (prev_pipe + 1) % 2;

    push_back_PidQueue( &job->pidQueue, pid );
    parent_run_command(holder.cmd);

    if ( outfile != -1 )
    {
      close( outfile );
    }
    if ( infile != -1 )
    {
      close( infile );
    }
  }



  return;
}

// Run a list of commands
/*
The entry level function for execution in quash is run_script().
This function is responsible for calling create_process() on an array of CommandHolders.

After all of the processes have been created for a job,
run_script() should either wait on all processes inside a foreground job to complete
or add it to the background job list without waiting if it is a background job.

After returning from process creation, and if the job is foreground,
pop processes one by one from the queue and wait for each of them to exit
*/
/*
CommandHolder has
    char* redirect_in, char* redirect_out,
    char flags, and Command cmd.

Command has
    SimpleCommand simple, GenericCommand generic, EchoCommand echo,
    ExportCommand export, CDCommand cd, KillCommand kill, PWDCommand pwd,
    JobsCommand jobs, ExitCommand exit, EOCCommand eoc...
    Each command has different types of arguments (e.g., char** args).
*/
void run_script(CommandHolder* holders) {

  if (holders == NULL)
    return;

  if ( jobQueueInitialized == false )
  {
    jobList = new_destructable_JobQueue( 10, FreeJobAndProcesses );
    jobQueueInitialized = true;
  }

  check_jobs_bg_status();

  if (get_command_holder_type(holders[0]) == EXIT &&
      get_command_holder_type(holders[1]) == EOC) {
    end_main_loop();
    return;
  }

  CommandType type;
  struct Job newJob;
  newJob.id = length_JobQueue( &jobList ) +1;
  newJob.command = get_command_string();
  newJob.pidQueue = new_PidQueue( 1 );


  // Run all commands in the `holder` array
  for (int i = 0; (type = get_command_holder_type(holders[i])) != EOC; ++i)
  {
    create_process(holders[i], &newJob);
  }

  bool isBackground = holders[0].flags & BACKGROUND;

  if (isBackground == false) { // FOREGROUND
    // Not a background Job
    // Wait for each job...
    int pidLength = length_PidQueue( &newJob.pidQueue );

    while ( pidLength != 0 )
    {
pid_t currentPid = pop_front_PidQueue (&newJob.pidQueue);
      int status;
      waitpid( currentPid, &status, 0 );
      pidLength = length_PidQueue( &newJob.pidQueue );
    }

    FreeJobAndProcesses( newJob );
  }
  else {  // BACKGROUND
    // Add to job queue
    if ( length_PidQueue( &newJob.pidQueue ) > 0 )
    {
      newJob.firstPid = newJob.pidQueue.data[0];
    }
    push_back_JobQueue( &jobList, newJob );
    print_job_bg_start( newJob.id, newJob.firstPid, newJob.command );
  }

}
