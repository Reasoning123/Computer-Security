//Alexander Cardenuto
const int BLOCKSIZE = 8;           // Block size for blowfish
unsigned char *fs_encrypt(unsigned char *plaintext, int bufsize, char *keystr, int *resultlen);	// encrypt plaintext of length bufsize. Use keystr as the key.
unsigned char *fs_decrypt(void *ciphertext, int bufsize, char *keystr, int *resultlen);// decrypt ciphertext of length bufsize. Use keystr as the key.

