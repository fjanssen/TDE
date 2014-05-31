/*
 * ImageConverter.h
 *
 *  Created on: May 29, 2014
 *      Author: Cordt Voigt
 */

#ifndef IMAGECONVERTER_H_
#define IMAGECONVERTER_H_

struct featureList * appendFeature(int * feature, int class, struct featureList * currentPosition);
struct featureList* createDataset();
void printList(struct featureList * fList);
int * imageToFeature_netpbm(char * filename);
char * toChar(int n, int mode);
struct dirList * listDirContent(char * dirName);
void writeFeatureSet(struct featureList * fList, int mode);

#endif /* IMAGECONVERTER_H_ */
