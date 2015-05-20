#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <fstream> 
#include <sstream> 
#include <vector>

using namespace std;

string phrase;
vector<string> Mykey;
vector <unsigned int> WholeKey;
int key_length = 0;
int numWords = 0;
int r = 20; 
int w = 32;
unsigned int  S[43];
unsigned int A=0;	
unsigned int B=0;
unsigned int C=0;
unsigned int D=0;
//unsigned int mod = (1 << 31);

unsigned int rol(unsigned int x, unsigned int shift){
	return (x << shift) | (x >> (w - shift));	
}
unsigned int  ror(unsigned int x, unsigned int shift){
	return (x >> shift) | (x << (w - shift));
}

string little_endian(string x){
	string endian,news; 
	for(int i=x.length();i >= 0;i = i -2){
		endian.append(x.substr(i,2));
	}
	return endian;
}
unsigned int toint(string x){
	unsigned int a;
	stringstream ss;
	ss << hex << x; 
	ss >> a;
	return a;
}
void keyMaker(){
	unsigned int j,i,a,b,v;
	S[0] = 0xB7E15163;
	for(int i=1;i < 44;i++){
		S[i] = S[i-1] + 0x9E3779B9;
	}
	a=b=i=j=0;
	if(numWords > ((2 * r)+4)){
		v = (3 * numWords) + 1;
	}else{
		v = 133;
	}	
	for(int s=1;s<v;s++){
		a =rol((S[i] + a + b) ,3);
		b =rol(((WholeKey[j] + a + b)),((a+b) & 0x1f));
		S[i] = a;WholeKey[j] = b;
		i = (i + 1) % 44;
		j = (j + 1) % numWords;
	}
}

void encrypt(){
	unsigned int hold;
	unsigned int t,u;

	B += S[0];
	D += S[1];
	for(int i = 1; i <= r;i++){
		t = rol((B * (2*B + 1)),5);
		u = rol((D * (2*D + 1)),5);
		A = rol((A ^ t),(u & 0x1f)) + S[2*i];
		C = rol((C ^ u),(t & 0x1f)) + S[(2*i)+1];
		hold = A;
		A = B;
		B = C;
		C = D;
		D = hold;
	}
	
	A += S[42];
	C += S[43];
}

void decrypt(){
	unsigned int hold; 
	unsigned int u,t; 

	C -= S[43];
	A -= S[42];
	for(int i=r;i > 0;i--){
		hold = A;
		A = D;
		D = C; 
		C = B;
		B = hold;
		u = rol((D * (2*D + 1)),5);
		t = rol((B * (2*B + 1)),5);
		C = ror((C - S[2*i+1]),(t & 0x1f)) ^ u;
		A = ror((A - S[2*i]),(u & 0x1f)) ^ t;
	}
	D -= S[1];
	B -= S[0];
}

int main( int argc, const char* argv[]){
	ifstream input; 
	ofstream output;
	string line,blah,blah1,choice,number,aKey,completekey;
	const char* p; 
	unsigned int x;
	stringstream ss;

	input.open(argv[1]);
	output.open(argv[2]);
	getline(input,choice);
	getline(input,line);
	istringstream num(line);
	num >> blah;
	while(num >> number){
		phrase.append(number);
	}
	getline(input,line);
	istringstream num1(line);
	num1 >> blah1;
	while(num1 >> aKey){
		completekey.append(aKey);
		key_length++;
	}
	numWords = key_length / (w / 8);
	for(int i=0;i < numWords;i++){
		WholeKey.push_back(toint(little_endian(completekey.substr(2*(w/8)*i,2 * (w/8)))));
		
	}
	keyMaker();

	int y = phrase.length()/4;
	A = toint(little_endian(phrase.substr(0,y)));
	B = toint(little_endian(phrase.substr(y,y)));
	C = toint(little_endian(phrase.substr(2*y,y)));
	D = toint(little_endian(phrase.substr(3*y,y)));
	if(choice.compare("Encryption") == 0){
		encrypt();
		output << "Cyphertext: ";
	}else{
		decrypt();
		output << "Plaintext: ";
	}
	string a,b,c,d;
	ss  << hex << A;
	ss >> a;
	ss.clear();
	ss  << hex << B;
	ss >> b;
	ss.clear();
	ss  << hex << C;
	ss >> c;
	ss.clear();
	ss  << hex << D;
	ss >> d;
	ss.clear();

	a = little_endian(a);
	b = little_endian(b);
	c = little_endian(c);
	d = little_endian(d);
	
	output << a << b << c << d;
	input.close();
	output.close();
	
}
