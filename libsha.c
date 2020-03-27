/*
 *   Librairie pour SHA-1 : RFC 3174 Méthode 1
 *
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>

#ifndef libshah
#define libshah
#include "libsha.h"
#endif



unsigned int MsgPadding(FILE* fptr,
                        unsigned long long file_size)
{
    //fptr est deja ouvert ici
    unsigned long long* r = malloc(sizeof(unsigned long long));
    *r = file_size * 8;

    //reflechir a tous les cas possibles lors du padding
    if( (file_size * 8)%512 == 0 )
    {
        unsigned char* buffer = malloc(sizeof(unsigned char) * 448 / 8);
        buffer[0] = 1 << 7;
        for(int i = 1 ; i < 448/8; i++)
            buffer[i] = 0;

        fwrite(buffer, 1, 448/8,fptr);
        for(int i = 0 ; i < 8 ; i++)
            fwrite( (char*)r + 7 - i, 1, 1, fptr);
        fclose(fptr);
        return 448/8;

    }
    else if ( (file_size * 8 )%512 >= 448 ) //chevauchement de bloc
    {
        //on padd avec 1, puis des 0 afin de finir le bloc de 512. Ensuite on crée un nouveau bloc de 512 avec des 0, puis on padd avec file_size en binaire.
        // a ecrire
        unsigned char* buffer = malloc(sizeof(unsigned char) * (512/8 - (file_size * 8)%512 / 8) );
        buffer[0] = 1 << 7;
        for(int i = 1 ; i < (512/8 -  (file_size * 8)%512 / 8 ); i++ )
            buffer[i] = 0;

        fwrite(buffer, 1, ( (512/8 - (file_size * 8)%512 / 8 )),fptr);

        unsigned char* buff = malloc(sizeof(unsigned char) * 448 / 8);
        buff[0] = 0;
        for(int i = 1 ; i < 448/8; i++)
            buff[i] = 0;

        fwrite(buff, 1, 448/8,fptr);
        for(int i = 0 ; i < 8 ; i++)
            fwrite( (char*)r + 7 - i, 1, 1, fptr);
        fclose(fptr);
        return 448/8;

    }
    //ajouter un bit à 1
    //ajouter des zeros jusqu'a ce qu'il ne reste plus que 64 bits pour faire un bloc de 512 bits complet
    //ajouter file_size en tant que u64
    else if ( (file_size * 8 )%512 < 448 )
    {
        //on rempli le bloc avec 0x80 puis on ecrit file_size
        unsigned char* buffer = malloc(sizeof(unsigned char) * (512/8 - (file_size * 8)%512 / 8) - 8);
        buffer[0] = 1 << 7;
        for(int i = 1 ; i < (512/8 -  (file_size * 8)%512 / 8 ) - 8 ; i++ )
            buffer[i] = 0;

        fwrite(buffer, 1, ( (512/8 - (file_size * 8)%512 / 8 ) - 8 ),fptr);
        for(int i = 0 ; i < 8 ; i++)
            fwrite( (char*)r + 7 - i, 1, 1, fptr);
        fclose(fptr);
        return 0;
    }
    return 0;
}

WORD_t add_words( WORD_t word_x,
                  WORD_t word_y)
{

    unsigned long long t = (unsigned long long)2 << (unsigned long long)31;
    unsigned long long z = ( (unsigned long long)word_x + (unsigned long long)word_y ) % t;
    return (WORD_t)z;
}


WORD_t CircularLeftShift(WORD_t word_x,
                         unsigned int n)
{
    if( (n > 32) || (n < 0) )
    {
        perror("circulat shift, mauvaise valeur de n"); abort();
    }

    return ( (word_x << n) | (word_x >> (32 - n)) );
}

void BinToHexString(WORD_t* res_word,
                    char* hash)
{
    // res_word contient 5 mots de 32 bits, soit 160 bits
    // hash est une chaine de 5 mots, soit 5 * 8 chars

    // il y a un problème à l'affichage sinon tout va bien !
    // il faut directement fait printf( " %x " ), mais 4 bits par 4 bits ou un truc de ce style

    for(int i = 0 ; i < 5 ; i++)
    {
        sprintf(hash + i * 8 * sizeof(char),"%x", res_word[i]);
    }
    for(int i = 0; i < 40 ; i++)
        printf("%c",hash[i]); //n'affiche pas le 0 s'il est en premiere position (hash[0]) !! incomprehension totale
    printf("\n");
    return (void)0;
}

unsigned long long GetSize(char* filename)
{
    //lstat pour avoir la taille en octet
    struct stat* stat_buf = malloc(sizeof(struct stat));
    stat(filename, stat_buf);
    return  (stat_buf->st_size);
}

WORD_t f(unsigned int t, WORD_t B, WORD_t C, WORD_t D)
{
    if(t < 0 )
    {
        perror("f : t < 0");
        abort();
    }
    if( t <= 19)
    {
        return ( (B & C) | ((~B) & D) );
    }
    else if ( t <= 39)
    {
        return (B ^ C ^ D);
    }
    else if (t <= 59)
    {
        return ( (B & C) | (B & D) | (C & D)  );
    }
    else if( t <= 79)
    {
        return (B ^ C ^ D);
    }
    else
    {
        perror("f : t > 79");
        abort();
    }
}

WORD_t K(unsigned int t)
{
    if (t < 0) {
        perror("K : t < 0");
        abort();
    } else if (t <= 19) {
        return K0;
    } else if (t <= 39) {
        return K20;
    } else if (t <= 59) {
        return K40;
    } else if (t <= 79) {
        return K60;
    } else {
        perror("K : t > 79");
        abort();
    }

}
void Divide_M_InWord(unsigned char* buffer,
                     WORD_t* W)
{
    for(int i = 0,j = 0; i < 16 * 4 ; j++)
    {
        W[j] = (unsigned int)(buffer[i] << 24) ^ (unsigned int)(buffer[i + 1] << 16) ^ (unsigned int)(buffer[i + 2] << 8) ^ (unsigned int)(buffer[i + 3]);
        i += 4;
    }
    return (void)0;
}

void RemoveAddedBytes(FILE* fptr,
                      unsigned long long size)
{
    int fd = fileno(fptr);
    if(fd == -1)
    {
        perror("error fileno()\n");
        exit(-1);
    }
    int code = ftruncate(fd, size);
    //ftruncate echoue et renvoie -1
    if(code == -1)
    {
        perror("error ftruncate\n");
        exit(-1);
    }
}