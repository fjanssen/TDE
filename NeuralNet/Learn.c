#include<opencv2/core/core_c.h>
#include<opencv2/imgproc/imgproc_c.h>
#include<opencv2/highgui/highgui_c.h>
#include <ppm.h>

#include<stdio.h>
#include<stdlib.h>

struct featureList * appendFeature(int * feature, int class, struct featureList * currentPosition);
int* imageToFeature_netpbm();
void imageToFeature_opencv();
char * toChar(int n);

struct featureList {
	int * feature;
	int class;
	struct featureList * nextFeature;
};

int main()
{
	printf("%s\n", toChar(15));

	struct featureList * flRoot;
  	flRoot = malloc(sizeof(struct featureList));
	flRoot->nextFeature = 0;
	int * rootFeature = imageToFeature_netpbm("../images/00000.ppm");
	int * curFeature;
	flRoot->feature = rootFeature;
	flRoot->class = 0;
	struct featureList * curPosition = flRoot;
	char * fName = malloc(1024 * sizeof(char));
	int i;
	for(i = 1; i < 4; i++)
	{
		strncpy(fName, "../images/00000.ppm", 1023);
		//fName[11] = itoc(i);
		printf("%s\n", fName);
		curFeature = imageToFeature_netpbm(fName);
		printf("%d\n", *curFeature);
		curPosition = appendFeature(curFeature, 0, curPosition);
	}
	return 0;
}

char * toChar(int n)
{
	int rem, i;
	char buf = '0';
	char * result = malloc(sizeof(char) * 10);
	strncpy(result, "00000", 10);
	char * pResult = result;
	for(i = 4; i >= 0; i--)
	{
		rem = n % 10;
		buf = (char)(rem + (int)('0'));
		for(; *pResult; ++pResult)
		{
			printf("%c\n", *pResult);
			*pResult = buf;
			printf("%c\n", *pResult);
		}
		n /= 10;
		if(n == 0)
		{
			break;
		}
	}
	printf("%s\n", result);
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
		newFeature->nextFeature = 0;
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
	pixArray =  ppm_readppm(fp, &colsP, &rowsP, &maxvalP);
	printf("%d\n%d\n", rowsP, colsP);
	int * feature = 0;
	feature = malloc(rowsP * colsP * 3 * sizeof (int));
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

	ppm_freearray(pixArray, rowsP);
	return feature;
}

void imageToFeature_opencv()
{
	IplImage* img = cvLoadImage("images/00001.ppm", -1);

	if (!img)
	{
		printf("Image could not be loaded.\n");
		return;
	}


	printf("%d", img->height);
	printf("%d", img->width);

	cvReleaseImage(&img);
}
