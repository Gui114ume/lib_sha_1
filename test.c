#include <stdlib.h>

#ifndef libshah
#define libshah
#include "libsha.h"

#endif

int main(int argc, char* argv[])
{
    char* hash = malloc(sizeof(char) * 32 * 5);
    WORD_t word[5];
    word[0]=0x67452301;
    word[1]=0xEFCDAB89;
    word[2]=0x98BADCFE;
    word[3]=0x10325476;
    word[4]=0xC3D2E1F0;
    BinToHexString(word,hash);

}