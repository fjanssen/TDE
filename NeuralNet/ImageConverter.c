#include <ppm.h>

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

struct featureList * appendFeature(int * feature, int class, struct featureList * currentPosition);
void printList(struct featureList * fList);
int * imageToFeature_netpbm(char * filename);
char * toChar(int n, int mode);
struct dirList * listDirContent(char * dirName);

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

int main()
{
	struct featureList * flRoot;
  	flRoot = malloc(sizeof(struct featureList));
	flRoot->nextFeature = NULL;
	int * curFeature;
	struct featureList * curPosition = flRoot;
	char * fFolder = "../images/";
	char * fName = malloc(sizeof(char) * 1024);
	char * fNameFinal = malloc(sizeof(char) * 1024);
	struct dirList * curDirList;
	struct dirList * curDir;
	int i, first = 1;
	for(i = 0; i < 43; i++)
	{
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
			printf("%s\n", fNameFinal);
			if(curFeature == NULL)
			{
				continue;
			}
			if(first == 1)
			{
				curPosition->feature = curFeature;
				curPosition->class = i;
				first = 0;
			}
			else
			{
				curPosition = appendFeature(curFeature, i, curPosition);
			}
			curDir = curDir->nextDir;
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

	return 0;
}


struct featureList * appendFeature(int * feature, int class, struct featureList * currentPosition)
{
	if(currentPosition != 0)
	{
		struct featureList * newFeature;
		newFeature = malloc(sizeof(struct featureList));

		currentPosition->nextFeature = newFeature;
		newFeature->feature = feature;
		newFeature->class = class;
		newFeature->nextFeature = NULL;
		return newFeature;
	}

	return 0;
}


int * imageToFeature_netpbm(char * filename)
{
	char * mode = "rb";
	FILE * fp = 0;
	int colsP = 0;
	int rowsP = 0;
	pixval maxvalP = 0;
	pixel ** pixArray;
	pixel ** oneArray;
	pixel * onePixel;
	int i, j;

	fp = fopen(filename, mode);
	if(fp == NULL)
	{
		printf("Filename \"%s\" could not be found.\n", filename);
		return NULL;
	}
	pixArray =  ppm_readppm(fp, &colsP, &rowsP, &maxvalP);
	int * feature = 0;
	feature = malloc(rowsP * colsP * 3 * sizeof(int) + sizeof(int));
	int * curFeature = feature;
	oneArray = pixArray;
	for(i = 0; i < rowsP; i++)
	{
		onePixel = (*oneArray);
		oneArray++;

		for(j = 0; j < colsP; j++)
		{
			*curFeature = onePixel->r;
			curFeature++;
			*curFeature = onePixel->g;
			curFeature++;
			*curFeature = onePixel->b;
			curFeature++;
			onePixel++;
		}
	}
	// Determines end of sequence
	*curFeature = -1;

	ppm_freearray(pixArray, rowsP);
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
