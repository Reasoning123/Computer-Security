#include <resolv.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/bio.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

int main(int argc, char * argv[])
{
	char* cert = argv[1];
	char* key = argv[2];
    const SSL_METHOD *method = NULL;
    SSL_CTX *ctx = NULL;
    OpenSSL_add_all_algorithms();
   	SSL_load_error_strings();
	SSL_library_init();
    method = SSLv3_server_method();
    ctx = SSL_CTX_new(method);

    SSL_CTX_use_certificate_file(ctx, cert, SSL_FILETYPE_PEM);
    SSL_CTX_use_PrivateKey_file(ctx, key, SSL_FILETYPE_PEM);
	if ( !SSL_CTX_check_private_key(ctx) )
	 	abort();

	int sd, client;
	int bytes,b = 2;
	char* buf;
	char* a;
	const char* reply;
	SSL *ssl;
	struct sockaddr_in addr;
	sd = socket(PF_INET, SOCK_STREAM, 0); /* create stream socket */
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[3])); /* select ‘port’ */
	addr.sin_addr.s_addr = INADDR_ANY; /* any available addr */
	bind(sd, (struct sockaddr*)&addr, sizeof(addr)); /* bind it */
	listen(sd, 10); /* make into listening socket */
	

	while(b > 1){
		client = accept(sd, 0, 0); /* await and accept connections */
		ssl = SSL_new(ctx); /* get new SSL state with context */
		SSL_set_fd(ssl, client); /* set connection to SSL state */
		SSL_accept(ssl); /* start the handshaking */
	 	/* now you can read/write */
		bytes = SSL_read(ssl, buf, sizeof(buf)); /* get HTTP request */
		/*...process request */
		b = atoi(buf) + 1;
		sprintf(a,"%d",b);
		reply = a;
		SSL_write(ssl, reply, strlen(reply)); /* send reply */
	}
	
	 /* close connection & clean up */
	client = SSL_get_fd(ssl); /* get the raw connection */
	SSL_free(ssl); /* release SSL state */
	close(sd); /* close connection */
    return EXIT_SUCCESS;
}

