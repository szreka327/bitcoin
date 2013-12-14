#include "merkel.h"

//adott számhoz (levelek száma) megmondja mennyi a fa mélysége
int deep;
void deepness(int transnum){
	int d=0;
	while(pow(2,d)<transnum){
		d++;}
  deep=d;}

//kettõ hast összerak egy kifejezéssé
void melt(unsigned char x1[32], unsigned char x2[32], unsigned char y[64]){
	int i;
	for(i=0;i<32;i++){
		y[i]=x1[i];
		y[i+32]=x2[i];}}

//egy adott szintrõl való hasheket fûzi össze, és hasheli meg újra, legyártva ezzel a következõ szintet
void leveljump(unsigned char level0[1024][32], unsigned char level1[1024][32], int db){
	int i=0,j=0,ind=0;
	unsigned char help[1024][64];
	if(db%2==1){
		db=db-1;
		ind=1;
		melt(level0[db], level0[db], help[db/2]);}
    while(i<db){
		melt(level0[i],level0[i+1],help[i/2]);
		i=i+2;}
	for(j=0;j<db/2;j++){
		DSHA256(help[j], level1[j],64);}
	if (ind==1){
		DSHA256(help[db/2], level1[db/2],64);}}

/*
//a tranzakciókat hasheli össze
void transtohash(unsigned char trans[], unsigned char hash[1024][32]){
	int i;
	for(i=0;i<x;i++){
		SHA256(trans[i], hash[i]);}}
*/

//kiszámolja az adott tranzakciókhoz tartozó hashgyökeret
void mroot(unsigned char hashes[1024][32], int db, unsigned char root[32]){
    deepness(db);
    unsigned char help[deep+1][1024][32];
	int i,j;
	int ind=0;
	for(i=0;i<db;i++){
		for(j=0;j<32;j++){
			help[deep][i][j]=hashes[i][j];}}
    while(deep!=0){
		leveljump(help[deep], help[deep-1], db);
        db=db/2;
        deep=deep-1;}
    for(i=0;i<32;i++){
		root[i]=help[0][0][i];}}

/*
void change(unsigned char txt[32]){
	unsigned char help[32];
	int i;
	for(i=0;i<32;i++){
		help[i]=txt[i];}
	for(i=0;i<16;i++){
		txt[2*i]=help[2*i+1];
		txt[2*i+1]=help[2*i];}}
*/

//beolvas fileból két hash értéket
void read(FILE *in, unsigned char x[64], unsigned char y[64], unsigned char z[1024][32]){
	in=fopen("input.txt", "r");
		fscanf(in,"%s\n%s",x , y);
		fscanf(in,"%s\n%s",x, y);
		const char *src0 = x, *src1 = y;
		unsigned char buffer0[32], buffer1[32];
		unsigned char *dst0 = buffer0, *dst1 = buffer1;
		unsigned char *end0 = buffer0 + sizeof(buffer0), *end1 = buffer1 + sizeof(buffer1);
 		unsigned int u0, u1;
		while (dst0 < end0 && sscanf(src0, "%02x", &u0) == 1){
 			*dst0++ = u0;
        	src0 += 2;}
  		while (dst1 < end1 && sscanf(src1, "%02x", &u1) == 1){
 			*dst1++ = u1;
        	src1 += 2;}
		int i=0;
		for (dst0 = buffer0; dst0 < end0; dst0++){
			z[0][i]=*dst0;
        	i++;}
		i=0;
        for (dst1 = buffer1; dst1 < end1; dst1++){
        	z[1][i]=*dst1;
        	i++;}
		fclose(in);}


int main(int argc, char **argv){  		
    unsigned char input[1024][32];
    unsigned char input0[64];
	unsigned char input1[65];
    unsigned char hash[32];
    FILE *input_file;
	read(input_file, input0, input1, input);
	print_hash(input[0]);
	print_hash(input[1]);
/*	unsigned char help[2][32];
	for(i=0;i<32;i++){
		help[0][i]=input[0][i];
		help[1][i]=input[1][i];}
	for(i=0;i<32;i++){
		input[0][i]=help[0][32-i];
		input[1][i]=help[1][32-i];}
*/	mroot(input,2,hash);
    print_hash(hash);
	return 0;}
