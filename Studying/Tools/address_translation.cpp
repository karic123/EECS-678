#include <iostream>
#include <string>
using namespace std;

/*
# ----------------------------------------------------
# Address Translation

#   8 bits      8 bits      8 bits
# ----------------------------------
# | 1st level | 2nd level | offset |
# ----------------------------------

# Virtual address format: 24 bits

#   4 bits    3 bits  1 bit
# --------------------------------
# | frame # | unused | Valid bit |
# --------------------------------

# Page table entry: 8 bit

#   Addr	+0	+1	+2	+3	+4	+5	+6	+7	+8	+9	+A	+B	+C	+D	+E	+F
#   0x000				31
#   0x010
#   0x020				41
#   …
#   0x100	00	01						01	00				01
#   …
#   0x200
*/

struct PageCell
{
	string colHeader;
	string rowHeader;
	string cellValue;
};

int main()
{
	PageCell pageTable[16][4];
	
	pageTable
}
