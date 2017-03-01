/* Lab 2 - Rachel J Morris, Spring 2017 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct Storage {
  intptr_t num_bugs_on_mars;
  const char* scary_bug;
  char* sentence[7];
  const char* colorful_bug;
  intptr_t num_bugs_on_earth;
  intptr_t num_bugs_on_venus;
  char* useless_bug;
};

void echo(char** strs);
void echoohce(char** strs);
void InitStuff( struct Storage* bug_info );
void DisplayStuff( struct Storage* bug_info );
void FreeStuff( struct Storage* bug_info );

int main(int argc, char** argv) {
  struct Storage bug_info;

  InitStuff( &bug_info );
  DisplayStuff( &bug_info );
  FreeStuff( &bug_info );

  printf("The current bug adjective is: %s\n", bug_info.sentence[2]);

  /*
  I don't know *why* this causes a crash -- this pointer is
  still pointing to a memory address, so it seems like freeing it
  would be OK.
  My guess is that free() should only be used opposite to malloc(),
  but this string was assigned directly.
  (I have a lot of exp in C++, but not C.)
  */
  // free(bug_info.useless_bug);

  printf("Bugs didn't cause me to crash!\n");

  return EXIT_SUCCESS;
}


void echo(char** strs) {
  if (*strs != NULL) {
    printf("%s ", *strs);
    fflush(stdout);

    echo(strs + 1);
  }
}

void echoohce(char** strs) {
  char** iter;
  char** stop_beginning = strs - 1;

  for (iter = strs; *iter != NULL; ++iter)
    printf("%s ", *iter);

  fflush(stdout);

  --iter;

  for (; iter != stop_beginning; --iter)
    printf("%s ", *iter);

  printf("\n");
  fflush(stdout);
}


void DisplayStuff( struct Storage* bug_info )
{
  printf("The current bug population of Earth is about: %zu\n",
         (*bug_info).num_bugs_on_earth);
  printf("The current bug population of Mars is about: %zu\n",
         (*bug_info).num_bugs_on_mars);
  printf("The current bug population of Venus is about: %zu\n",
         (*bug_info).num_bugs_on_venus);


  printf("The total bug population of the solar system is: %zu\n",
         (*bug_info).num_bugs_on_mars +
         (*bug_info).num_bugs_on_venus +
         (*bug_info).num_bugs_on_earth);

  echo((*bug_info).sentence);
  printf("%s\n", (*bug_info).useless_bug);

  (*bug_info).sentence[2] = strdup("colorful");

  echo((*bug_info).sentence);
  printf("%s\n", (*bug_info).colorful_bug);

  echoohce((*bug_info).sentence);
}

void InitStuff( struct Storage* bug_info )
{
  (*bug_info).num_bugs_on_earth = 7400000000 * 200000000;
  (*bug_info).num_bugs_on_venus = 0;
  (*bug_info).colorful_bug = "butterfly";
  (*bug_info).useless_bug = "mosquito";
  (*bug_info).scary_bug = "~~~~~~~~ SPIDER!!! ~~~~~~~~";

  (*bug_info).num_bugs_on_mars = 0;

  (*bug_info).sentence[0] = strdup("The");
  (*bug_info).sentence[1] = strdup("most");
  (*bug_info).sentence[2] = strdup("useless");
  (*bug_info).sentence[3] = strdup("bug");
  (*bug_info).sentence[4] = strdup("is");
  (*bug_info).sentence[5] = strdup("a");
  (*bug_info).sentence[6] = NULL;
}

void FreeStuff( struct Storage* bug_info )
{
  for ( int i = 0; i < 6; i++ )
  {
    free( (*bug_info).sentence[i] );
    (*bug_info).sentence[i] = NULL;
  }
}
