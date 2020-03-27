#include <stdio.h>
#include <math.h>
#include <unistd.h>

#ifndef libshah
#define libshah
#include "libsha.h"
#endif

//on ne hash que des fichiers pour le moment
int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printf("usage : %s [file name]\n",argv[0]);
        exit(-1);
    }
    //H0 H1 H2 H3 H4 defini dans libsha.h
    WORD_t A, B, C, D, E, TEMP;
    size_t nb = 0;
    unsigned char* buffer = malloc(sizeof(char)*512/8);

    WORD_t* W = malloc(sizeof(WORD_t) * 80);

    char* hash = malloc(sizeof(char) * 8 * 5);// 32 bits s'écrivent avec 8 characteres hexadecimaux
    WORD_t* RES = malloc(5 * sizeof(WORD_t));


    FILE* fp = fopen(argv[1],"ab+");
    unsigned long long file_size = GetSize(argv[1]);
    MsgPadding(fp, file_size);
    FILE* fptr = fopen(argv[1],"rb");

    while( nb = fread(buffer, 1, 512/8, fptr) )
    {
        Divide_M_InWord(buffer, W);
        for(int t = 16 ; t < 80 ; t++)
        {
            W[t] = CircularLeftShift( W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16],1) ;
        }
        A = H0;   B = H1;   C = H2;   D = H3;   E = H4;

        for(int t = 0 ; t < 80 ; t++)
        {
         TEMP = CircularLeftShift(A, 5) + f(t, B, C, D) + E + W[t] + K(t);
         E = D;   D = C;   C = CircularLeftShift(B,30);   B = A;   A = TEMP;
        }

        H0 += A;   H1 += B;   H2 += C;   H3 += D; H4 += E;
    }
    RES[0] = H0; RES[1] = H1; RES[2] = H2; RES[3] = H3; RES[4] = H4;

    BinToHexString(RES,hash);
    fclose(fptr);
    FILE* fpointer = fopen(argv[1],"wb");
    RemoveAddedBytes(fpointer,file_size);
    fclose(fpointer);

    return 1;
}