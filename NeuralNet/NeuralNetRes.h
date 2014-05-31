/*
 * NeuralNetRes.h
 *
 *  Created on: May 30, 2014
 *      Author: race
 */

#ifndef NEURALNETRES_H_
#define NEURALNETRES_H_

struct featureList
{
	int * feature;
	int class;
	struct featureList * nextFeature;
};

struct classified
{
	int class;
	double confidence;
};

int testNet(struct featureList* fList,int testlen, int inCount);
void init(struct fann *ann);
void closeNN(struct fann *ann);
struct classified* classify(struct fann *ann,int * invec, int inCount);

#endif /* NEURALNETRES_H_ */
