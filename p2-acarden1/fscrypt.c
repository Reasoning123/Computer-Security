#include "openssl/blowfish.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "fscrypt.h"

BF_KEY *key; 
unsigned char *out;
unsigned char *out2;


char *substring(char *string, int position, int length) 
{
   char *pointer;
   int c;
 
   pointer = malloc(length+1);
 
   if (pointer == NULL)
   {
      printf("Unable to allocate memory.\n");
      exit(EXIT_FAILURE);
   }
 
   for (c = 0 ; c < position -1 ; c++) 
      string++; 
 
   for (c = 0 ; c < length ; c++)
   {
      *(pointer+c) = *string;      
      string++;   
   }
 
  
   return pointer;
}

unsigned char *fs_encrypt(unsigned char *plaintext, int bufsize, char *keystr, int *resultlen)
{
	int i=9;
	unsigned char *a = calloc(8,sizeof(char));
	unsigned char *b = malloc(1);
	a = substring(plaintext,0,8);
	while(i < bufsize){
		BF_ecb_encrypt(a, out, key, BF_ENCRYPT);
		b = realloc(b,sizeof(b) + sizeof(out));
		strcat(b,out);
		*resultlen += strlen(out);
		strcpy(out,"");
		a = substring(plaintext,i,8);
		i+=8;
	}
	BF_ecb_encrypt(a, out, key, BF_ENCRYPT);
	b = realloc(b,sizeof(b) + sizeof(out));
	strcat(b,out);
	*resultlen += strlen(out);
	return b;

}
unsigned char *fs_decrypt(void *ciphertext, int bufsize, char *keystr, int *resultlen)
{
	int i=9;
	unsigned char *a = calloc(8,sizeof(char));
	unsigned char *b = malloc(1);
	a = substring(ciphertext,0,8);
	while(i < bufsize){
		BF_ecb_encrypt(a, out, key, BF_DECRYPT);
		b = realloc(b,sizeof(b) + sizeof(out));
		strcat(b,out);
		*resultlen += strlen(out);
		strcpy(out,"");
		a = substring(ciphertext,i,8);
		i+=8;
	}
	BF_ecb_encrypt(a, out, key, BF_DECRYPT);
	b = realloc(b,sizeof(b) + sizeof(out));
	strcat(b,out);
	*resultlen += strlen(out);
	return b;
}

int main()
{

  unsigned char s[] = "012345670123456";
  char *outbuf, *recvbuf;
  char pass[] = "top secret";
  int len = 0;
  int recvlen = 0;
  key = calloc(1, sizeof(BF_KEY));
  out = calloc(BLOCKSIZE+1, sizeof(char));
  out2 = calloc(BLOCKSIZE+1, sizeof(char));

	BF_set_key(key,BLOCKSIZE,(const unsigned char *) pass);
	
  outbuf = (char *) fs_encrypt(s, strlen(s)+1, pass, &len);

  int i = 0;
  printf("ciphertext = ");
  printf("%02x\n",outbuf);

  recvbuf  = (char *) fs_decrypt((void *) outbuf, len, pass, &recvlen);
  assert(memcmp(s, recvbuf, recvlen) == 0);
  assert(recvlen == (strlen(s)));
  printf("plaintext = %s\n", recvbuf);

}

