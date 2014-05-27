#include <ppm.h>

#include<stdio.h>
#include<stdlib.h>

struct featureList * appendFeature(int * feature, int class, struct featureList * currentPosition);
void printList(struct featureList * fList);
int* imageToFeature_netpbm();
char * toChar(int n);

struct featureList {
	int * feature;
	int class;
	struct featureList * nextFeature;
};

int main()
{
	struct featureList * flRoot;
  	flRoot = malloc(sizeof(struct featureList));
	flRoot->nextFeature = 0;
	int * rootFeature = imageToFeature_netpbm("../images/00000.ppm");
	int * curFeature;
	flRoot->feature = rootFeature;
	flRoot->class = 0;
	struct featureList * curPosition = flRoot;
	char * fFolder = "../images/";
	char * fExtension = ".ppm";
	char * fName = malloc(sizeof(char) * 1024);
	int i;
	for(i = 1; i < 4; i++)
	{
		strcpy(fName, "");
		strcat(fName, fFolder);
		strcat(fName, toChar(i));
		strcat(fName, fExtension);
		curFeature = imageToFeature_netpbm(fName);
		curPosition = appendFeature(curFeature, 0, curPosition);
	}

	printList(flRoot);
	return 0;
}

char * toChar(int n)
{
	int rem;
	char buf = '0';
	char * result = malloc(sizeof(char) * 10);
	strncpy(result, "00000", 10);
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

struct featureList * appendFeature(int * feature, int class, struct featureList * currentPosition)
{
	printf("%d\n", currentPosition->class);
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
	pixArray =  ppm_readppm(fp, &colsP, &rowsP, &maxvalP);
	printf("%d\n%d\n", rowsP, colsP);
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
