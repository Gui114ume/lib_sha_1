#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <math.h>


#define _FILE_OFFSET_BITS 64

typedef unsigned int WORD_t;

typedef struct BLOCK
{
    WORD_t word[16];
} BLOCK_t;

WORD_t K0  = 0x5A827999;
WORD_t K20 = 0x6ED9EBA1;
WORD_t K40 = 0x8F1BBCDC;
WORD_t K60 = 0xCA62C1D6;

WORD_t H0  = 0x67452301;
WORD_t H1  = 0xEFCDAB89;
WORD_t H2  = 0x98BADCFE;
WORD_t H3  = 0x10325476;
WORD_t H4  = 0xC3D2E1F0;

unsigned int MsgPadding(FILE* fptr,
                        unsigned long long size);

WORD_t add_words( WORD_t word_x,
                  WORD_t word_y);

WORD_t CircularLeftShift(WORD_t word_x,
                         unsigned int n);

void BinToHexString(WORD_t* res_word,
                    char* hash);

unsigned long long GetSize(char* filename);

WORD_t f(unsigned int t,
         WORD_t B,
         WORD_t C,
         WORD_t D);

WORD_t K(unsigned int t);

void Divide_M_InWord(unsigned char* buffer,
                     WORD_t* W);

void RemoveAddedBytes(FILE* fptr,
                      unsigned long long size);
