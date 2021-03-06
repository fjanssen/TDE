/*
 * ImageConverter.c
 *
 *  Created on: May 25, 2014
 *      Author: Cordt Voigt
 */

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>

#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>

struct featureList * appendFeature(int * feature, int class, struct featureList * currentPosition);
struct featureList* createDataset();
void printList(struct featureList * fList);
int * imageToFeature_netpbm(char * filename);
char * toChar(int n, int mode);
struct dirList * listDirContent(char * dirName);
void writeFeatureSet(struct featureList * fList, int mode);

const int TRAFFIC_SIGN_X = 32;
const int TRAFFIC_SIGN_Y = 32;
const int COLOUR_MODEL_DIMENSION = 3;
const int FEATURE_VECTOR_SIZE = 3072;
const int TRAFFIC_SIGN_CLASSES = 43;

struct featureList
{
	int * feature;
	int class;
	struct featureList * nextFeature;
};

struct dirList
{
	char dir[254];
	struct dirList * nextDir;
};


struct featureList * appendFeature(int * feature, int class, struct featureList * currentPosition)
{
	if(currentPosition != 0)
	{
		if(currentPosition->feature == NULL)
		{
			currentPosition->feature = feature;
			currentPosition->class = class;
			return currentPosition;
		}
		else
		{
			struct featureList * newFeature;
			newFeature = malloc(sizeof(struct featureList));

			currentPosition->nextFeature = newFeature;
			newFeature->feature = feature;
			newFeature->class = class;
			newFeature->nextFeature = NULL;
			return newFeature;
		}
	}

	return 0;
}


struct featureList* createDataset()
{
	srand(time(NULL));
	int i, r, selectTest, cFile;

	struct featureList * flRoot;
  	flRoot = malloc(sizeof(struct featureList));
  	flRoot->feature = NULL;
	flRoot->nextFeature = NULL;
	int * curFeature;
	struct featureList * curPosition = flRoot;
	struct featureList * flTestRoot;
	flTestRoot = malloc(sizeof(struct featureList));
	flTestRoot->feature = NULL;
	flTestRoot->nextFeature = NULL;
	struct featureList * curTestPosition = flTestRoot;

	char * fFolder = "../images/";
	char * fName = malloc(sizeof(char) * 1024);
	char * fNameFinal = malloc(sizeof(char) * 1024);
	struct dirList * curDirList;
	struct dirList * curDir;

	for(i = 0; i < TRAFFIC_SIGN_CLASSES; i++)
	{
		r = rand() % 20;
		selectTest = 0;
		cFile = 0;
		strcpy(fName, fFolder);
		strcat(fName, toChar(i, 1));
		curDirList = listDirContent(fName);
		curDir = curDirList;
		strcat(fName, "/");
		while(curDir != NULL)
		{
			strcpy(fNameFinal, fName);
			strcat(fNameFinal, curDir->dir);
			curFeature = imageToFeature_netpbm(fNameFinal);
			if(curFeature == NULL)
			{
				continue;
			}
			if((r == cFile) || (curDir->nextDir == NULL && selectTest != 1))
			{
				curTestPosition = appendFeature(curFeature, i, curTestPosition);
				selectTest = 1;
			}
			else
			{
				curPosition = appendFeature(curFeature, i, curPosition);
			}

			curDir = curDir->nextDir;
			cFile++;
		}
		curDir = curDirList;
		struct dirList * lastDir = curDir;
		while(curDir->nextDir != NULL)
		{
			curDir = curDir->nextDir;
			free(lastDir);
			lastDir = curDir;
		}
	}

	writeFeatureSet(flRoot, 0);
	writeFeatureSet(flTestRoot, 1);

	free(fName);
	free(fNameFinal);

	curPosition = flRoot;
	struct featureList * lastPosition = curPosition;
	while(curPosition->nextFeature != NULL)
	{
		curPosition = curPosition->nextFeature;
		free(lastPosition);
		lastPosition = curPosition;
	}

//	curTestPosition = flTestRoot;
//	lastPosition = curTestPosition;
//	while(curTestPosition->nextFeature != NULL)
//	{
//		curTestPosition = curTestPosition->nextFeature;
//		free(lastPosition);
//		lastPosition = curTestPosition;
//	}
	return flTestRoot;
}


int * imageToFeature_netpbm(char * filename)
{
	int * feature = 0;
	IplImage* img = 0;
	int height,width,step,channels;
	uchar *data;
	int i,j,k;

	// load an image
	img = cvLoadImage(filename, CV_LOAD_IMAGE_COLOR);
	if(!img){
		printf("Could not load image file: %s\n", filename);
		exit(0);
	}

	// get the image data
	height    = img->height;
	width     = img->width;
	step      = img->widthStep;
	channels  = img->nChannels;
	data      = (uchar *)img->imageData;

	feature = malloc(height * width * channels * sizeof(int) + sizeof(int));
	int * curFeature = feature;

	for(i = 0;i < height; i++) for(j = 0; j < width; j++) for(k = 0; k < channels; k++) {
		*curFeature = data[i*step+j*channels+k];
		curFeature++;
	}

	// Determines end of sequence
	*curFeature = -1;

	// release the image
	cvReleaseImage(&img );

	return feature;
}


struct dirList * listDirContent(char * dirName)
{
	DIR * d;
	struct dirList * dRoot;
	struct dirList * dCurrent;
	dRoot = malloc(sizeof(struct dirList));
	dRoot->nextDir = NULL;
	dCurrent = dRoot;

	d = opendir(dirName);
	if (! d)
	{
		fprintf (stderr, "Cannot open directory '%s': %s\n", dirName, strerror(errno));
		exit(EXIT_FAILURE);
	}

	int first = 1;
	while (1)
	{
		struct dirent * entry;

		entry = readdir(d);
		if (!entry)
		{
			break;
		}
		if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
		{
			if(first == 1)
			{
				strcpy(dRoot->dir, entry->d_name);
				first = 0;
			}
			else
			{
				struct dirList * newDir;
				newDir = malloc(sizeof(struct dirList));
				strcpy(newDir->dir, entry->d_name);
				newDir->nextDir = NULL;
				dCurrent->nextDir = newDir;
				dCurrent = newDir;
			}
		}
	}

	if (closedir (d))
	{
		fprintf (stderr, "Could not close '%s': %s\n", dirName, strerror(errno));
		exit (EXIT_FAILURE);
	}

	return dRoot;
}


void printList(struct featureList * fList)
{
	while(fList != NULL)
	{
		int * element = fList->feature;
		while(*element != -1)
		{
			printf("%d ", *element);
			++element;
		}
		fList = fList->nextFeature;
		printf("\n");
	}
}


// parameter mode: 0 -> file, 1 -> folder
char * toChar(int n, int mode)
{
	int rem;
	char buf = '0';
	char * result = malloc(sizeof(char) * 10);
	if(mode == 0)
	{
		strncpy(result, "00000", 10);
	}
	else
	{
		strncpy(result, "00", 10);
	}
	char * pResult = result;
	while(*pResult != '\0')
		++pResult;
	--pResult;

	for(; *pResult; --pResult)
	{
		rem = n % 10;
		buf = (char)(rem + (int)('0'));
		*pResult = buf;
		n /= 10;
		if(n == 0)
		{
			break;
		}
	}
	return result;
}


// parameter mode: 0 -> training set, 1 -> test set
void writeFeatureSet(struct featureList * fList, int mode)
{
	char * fileName;
	int nFeatures, i, tmp;
	struct featureList * curFeature = fList;
	int * curValue;

	if(mode == 0)
	{
		fileName = "../dataset/train.txt";
	}
	else
	{
		fileName = "../dataset/val.txt";
	}
	FILE * f = fopen(fileName, "w");
	if(f == NULL)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}

	while(curFeature != NULL)
	{
		nFeatures++;
		curFeature = curFeature->nextFeature;
	}

	curFeature = fList;
	fprintf(f, "%d %d %d\n", nFeatures-1, FEATURE_VECTOR_SIZE, TRAFFIC_SIGN_CLASSES);

	while(curFeature != NULL)
	{
		curValue = curFeature->feature;
		while(*curValue != -1)
		{
			if(*(curValue + 1) == -1)
			{
				fprintf(f, "%d\n", *curValue);
			}
			else
			{
				fprintf(f, "%d ", *curValue);
			}
			++curValue;
		}

		for(i = 0; i < TRAFFIC_SIGN_CLASSES; i++)
		{
			if(curFeature->class == i)
			{
				tmp = 1;
			}
			else
			{
				tmp = 0;
			}
			if(i == (TRAFFIC_SIGN_CLASSES - 1))
			{
				fprintf(f, "%d\n", tmp);
			}
			else
			{
				fprintf(f, "%d ", tmp);
			}
		}
		curFeature = curFeature->nextFeature;
	}

	nFeatures = 0;
	fclose(f);
}
