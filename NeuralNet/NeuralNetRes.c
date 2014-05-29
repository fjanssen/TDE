#include <stdio.h>
#include "floatfann.h"
#include <string.h>
#include <stdlib.h>
#include <NeuralNetRes.h>

int stringToInt(char a[]) {
  int c, n;
 
  n = 0;
 
  for (c = 0; a[c] != '\0'; c++) {
    n = (int)(n * 10 + (int)a[c] - '0');
  }
 
  return n;
}

int testNet()
{
	const char * test_file_name = "../dataset/val.txt";
	const char * net_file_name = "tde.net";
	const char lineSeperator = ' ';
	int inlen=3072;
	int testlen=4000;

	int maxpos=0,c = 0,i=0,j=0,truehit=0,curpos=1,curstrlen=0, resSign, resSignTrue[testlen];
	double max=0;
    fann_type *calc_out;
    fann_type input[inlen];

	FILE *fp;

	char line[128], tempnum[2], curchar;
	int testdata[testlen][inlen];
	for(i=0;i<testlen;i++){
		for(j=0;j<inlen;j++){
			testdata[i][j] = 0;}}
		
	
	i=0;j=0;
	fp = fopen(test_file_name,"r");

	//##################build test set and res####################
	while ( fgets ( line, sizeof line, fp ) != NULL )
	{
		curpos = 1;
		
		resSignTrue[i] = line[0];
		curchar = line[curpos];
		for(j=0;j<inlen;j++){
			curstrlen = 0;
			tempnum[0]='\0';
			tempnum[1]='\0';
			while(curchar != lineSeperator && curchar != NULL){
				tempnum[curstrlen] = curchar;
				curpos++;
				curchar = line[curpos];
				curstrlen++;
			}
			curpos++;
			curchar = line[curpos];
			testdata[i][j] = stringToInt(tempnum);
		}
//		printf("Sign ----> %c ", resSignTrue[i]);
//		printf("data ----> ");
//		for(int k = 1; k < inlen; k++) {
//			printf("%i ", testdata[i][k]);
//        }
//        printf("\n");
//		i++;
//		for(int k = 0; k < 128; k++) {
//			line[k] = '\0';
//        }
	}
	
	fclose(fp);

	//##################Network create and res ##################
    struct fann *ann = fann_create_from_file(net_file_name);
    int l,m;
	for(l=0;l<testlen;l++){
//		for(int q = 0; q < inlen; q++) {
//				printf("%i ", testdata[l][q]);
//			}
//			printf("\n");
		for(m = 0;m<inlen;m++){
			input[m] = testdata[l][m+1];
		}
//		for(int q = 0; q < inlen; q++) {
//				printf("%.0f ", input[q]);
//			}
//			printf("\n");
		max=0;
		maxpos=0;
		calc_out = fann_run(ann, input);
		for (c = 0; c < inlen-1; c++){
			if (calc_out[c] > max){
				max  = calc_out[c];
				maxpos = c;
			}
		}
		resSign = maxpos;
		printf("Found: %c  -- True: %c \n",resSign,resSignTrue[l]);
		if(resSign == resSignTrue[l]){
			truehit++;
		}
	}
		
		
//	for(int i = 0; i < 26; i++) {
//    //    printf("%.2f ", calc_out[i]);
//    }
    printf("\n%i\n",truehit);
    double percres = ((double)truehit/(double)testlen)*100.0;
	printf("%.3f \n",percres);
    fann_destroy(ann);
    return 0;
}
