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

#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

using namespace std;
int main(int argc, char** argv){
	const SSL_METHOD *method;
	SSL_CTX *ctx;
	OpenSSL_add_all_algorithms(); /* load & register cryptos */
	SSL_load_error_strings(); /* load all error messages */
	method = SSLv3_client_method(); /* create client instance */
	ctx = SSL_CTX_new(method); /* create context */

	/*---Standard TCP Client---*/
	int sd,server,x = 1,y,z;
	char* hold;
	const char* sending;
	struct hostent *host;
	struct sockaddr_in addr;
	host = gethostbyname(argv[1]); /* convert hostname IP addr */
	sd = socket(PF_INET, SOCK_STREAM, 0); /* create TCP socket */
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2])); /* set the desired port */
	addr.sin_addr.s_addr = *(long*)(host->h_addr); /* and address */
	server = connect(sd, (struct sockaddr*)&addr, sizeof(addr));/* connect */

	while( x > 0){
		cout << "enter a number: ";
		cin >> x;
		y = x + 1;
		SSL *ssl;
		ssl = SSL_new(ctx); /* create new SSL connection state */
		SSL_set_fd(ssl, server); /* attach the socket descriptor */
		SSL_connect(ssl); /* perform the connection */
		sprintf(hold,"%d",x);
		sending = hold;
		SSL_write(ssl, sending, strlen(sending));
		SSL_read(ssl,hold,strlen(hold));
		z = atoi(hold);
		if( y == z){
			cout << "success" << endl;
		}else{
			cout << "fail" << endl;
		}
		SSL_free(ssl); /* release SSL state */
	}
}
