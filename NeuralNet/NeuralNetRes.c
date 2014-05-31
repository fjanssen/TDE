#include <stdio.h>
#include "floatfann.h"
#include <string.h>
#include <stdlib.h>
#include "NeuralNetRes.h"


int testNet(struct featureList* fList,int testlen, int inCount){

	struct fann *ann;
	int i,truehit=0;
	struct classified* tempclass;

	ann = fann_create_from_file("tde.net");

	init(ann);
printf("---ann created---\n");
	for(i=0;i<testlen;i++){
		tempclass = classify(ann,fList->feature,inCount);
		printf("Found: %i with confidence %f -- True: %i",tempclass->class,tempclass->confidence,fList->class);
		if(tempclass->class == fList->class){
			truehit++;
			printf(" -- HIT \n");
		}else{
			printf("\n");
		}
		fList = fList->nextFeature;
	}
		
    printf("\n%i\n",truehit);
    double percres = ((double)truehit/(double)testlen)*100.0;
	printf("%.3f \n",percres);

	closeNN(ann);

    return 0;
}

void init(struct fann *ann){
	ann = fann_create_from_file("tde.net");
}

void closeNN(struct fann *ann){
	fann_destroy(ann);
}

/**
 * classifies the given input feature vector using the given ANN
 * returns classified struct
 */
struct classified* classify(struct fann *ann,int * invec, int inCount){
	fann_type *calc_out;
	fann_type input[inCount];
	struct classified* returnStruct;
	int i,j, maxpos=0;
	double max=0.0;
	returnStruct = malloc(sizeof(struct classified));
	for(i=0;i<inCount;i++){
		input[i] = invec[i];
	}

	calc_out = fann_run(ann, input);

	for (j = 0; j < inCount; j++){
		if (calc_out[j] > max){
			max  = calc_out[j];
			maxpos = j;
		}
	}
	returnStruct->class = maxpos;
	returnStruct->confidence = max;

	return returnStruct;
}




