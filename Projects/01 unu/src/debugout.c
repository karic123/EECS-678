#include "debugout.h"

const bool RACH_DEBUG = false;

void DEBUG( const char* text )
{
  if ( !RACH_DEBUG ) { return; }
  //printf( "%s", text );
}

void DEBUGN( int num )
{
  if ( !RACH_DEBUG ) { return; }
  //printf( "%d", num );
}
