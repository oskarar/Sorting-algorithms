#define _CRT_SECURE_NO_WARNINGS
#include "SortingAlgorithms.h"
#include "Statistics.h"
#include "Array.h"
#include <assert.h>
#include <string.h>

void merge_Sort(ElementType arrayToSort[], int first, int last, Statistics *statistics);
void merge_Sorted(ElementType *arrayToSort, int first, int mid, int last, Statistics *statistics);
int isImplemented(SortingAlgorithm algorithm)
{
	switch (algorithm)
	{
     case BUBBLE_SORT:
//     case INSERTION_SORT:
     case SELECTION_SORT:
//      case QUICK_SORT:
      case MERGE_SORT:
            return 1;
        default:
            return 0;
	}
}


/******************************************************************************************/
/* Era algoritmer har: */

static void bubbleSort(ElementType* arrayToSort, size_t size, Statistics* statistics)
{
	int i, j;

	for (j=0; lessThan(j, size, statistics); j++) {
		for (i=0; lessThan(i, size, statistics); i++) {

			if (greaterThan(arrayToSort[i], arrayToSort[i+1], statistics)) {

				swapElements(&(arrayToSort[i]), &(arrayToSort[i+1]), statistics);

			}
		}
	}
}

static void insertionSort(ElementType* arrayToSort, size_t size, Statistics* statistics)
{

	int i, j, border;
	for (j = 2; lessThan(j, size, statistics); j++) { 
		border = arrayToSort[j]; 
		i = j - 1; 
		while (greaterThan(i, 0, statistics) && greaterThan(arrayToSort[i], border, statistics)) {
			swapElements(&arrayToSort[i + 1], &arrayToSort[i], statistics);
			i = i - 1;

		}
		swapElements(&arrayToSort[i], &border, statistics);
	}

}

static void selectionSort(ElementType* arrayToSort, size_t size, Statistics* statistics)
{
	int i = 0, j = 0;
	int tempMin;

	for (i = 0; lessThan(i, size, statistics); i++) {
		tempMin = i;
		for (j = i; lessThan(j, size, statistics); j++) {

			if (lessThan(arrayToSort[j], arrayToSort[i], statistics)) {

				if (lessThan(arrayToSort[j], arrayToSort[tempMin], statistics)) {
					tempMin = j;

				}
			}		

		}
		swapElements(&(arrayToSort[i]), &(arrayToSort[tempMin]), statistics);
		
	}

}

static void mergeSort(ElementType* arrayToSort, size_t size, Statistics* statistics)
{
	int last = size - 1;
	merge_Sort(arrayToSort, 0, last, statistics);

}

void merge_Sort(ElementType arrayToSort[], int first, int last, Statistics *statistics)
{
	int mid;

	if (lessThan(first, last, statistics))
	{
		mid = (first + last) / 2;
		merge_Sort(arrayToSort, first, mid, statistics);                  //Vänstra rekustionen
		merge_Sort(arrayToSort, mid + 1, last, statistics);                 //Högra rekursionen
		merge_Sorted(arrayToSort, first, mid, last, statistics);     //Sammanslagningen av höger och vänster array.
	}
}

void merge_Sorted(ElementType *arrayToSort, int first, int mid, int last, Statistics *statistics)
{
	unsigned int *tempArr = calloc(first + last + 1, sizeof(int));
	int iL = first, iR = mid + 1, i = first; // Iteratorerna

	while (lessThanOrEqualTo(iL, mid, statistics) && lessThanOrEqualTo(iR, last, statistics)) {
		if (lessThan(arrayToSort[iL], arrayToSort[iR], statistics)) {
			swapElements(&(tempArr[i++]), &(arrayToSort[iL++]), statistics);
		}
		else {
			swapElements(&(tempArr[i++]), &(arrayToSort[iR++]), statistics);
		}
	} // sorterar vänster och höger delarr och stoppar in i temparrayen
	while (lessThanOrEqualTo(iR, last, statistics)) {
		swapElements(&(tempArr[i++]), &(arrayToSort[iR++]), statistics);
	}
	while (lessThanOrEqualTo(iL, mid, statistics)) {
		swapElements(&(tempArr[i++]), &(arrayToSort[iL++]), statistics);
	}
	for (i = first; lessThanOrEqualTo(i, last, statistics); i++) {
		swapElements(&(arrayToSort[i]), &(tempArr[i]), statistics);
	}
	free(tempArr);

}



static void quickSort(ElementType* arrayToSort, size_t size, Statistics* statistics)
{



}

/******************************************************************************************/


char* getAlgorithmName(SortingAlgorithm algorithm)
{
	/* Ar inte strangen vi allokerar lokal for funktionen?
	   Nej, inte i detta fall. Strangkonstanter ar ett speciallfall i C */
	switch (algorithm)
	{
        case BUBBLE_SORT:	 return "  Bubble sort ";
        case SELECTION_SORT: return "Selection sort";
        case INSERTION_SORT: return "Insertion sort";
        case MERGE_SORT:	 return "  Merge sort  ";
        case QUICK_SORT:	 return "  Quick sort  ";
        default: assert(0 && "Ogiltig algoritm!"); return "";
	}
}

// Sorterar 'arrayToSort' med 'algorithmToUse'. Statistik for antal byten och jamforelser hamnar i *statistics
static void sortArray(ElementType* arrayToSort, size_t size, SortingAlgorithm algorithmToUse, Statistics* statistics)
{
	if(statistics != NULL)
		resetStatistics(statistics);

	switch (algorithmToUse)
	{
	case BUBBLE_SORT:	 bubbleSort(arrayToSort, size, statistics); break;
	case SELECTION_SORT: selectionSort(arrayToSort, size, statistics); break;
	case INSERTION_SORT: insertionSort(arrayToSort, size, statistics); break;
	case MERGE_SORT:	 mergeSort(arrayToSort, size, statistics); break;
	case QUICK_SORT:	 quickSort(arrayToSort, size, statistics); break;
	default:
		assert(0 && "Ogiltig algoritm!");
	}
}

// Forbereder arrayer for sortering genom att allokera minne for dem, samt intialisera dem
static void prepareArrays(SortingArray sortingArray[], SortingAlgorithm algorithm, const ElementType* arrays[], const unsigned int sizes[])
{
	assert(isImplemented(algorithm));

	int i;
	int totalArraySize;

	for (i = 0; i < NUMBER_OF_SIZES; i++)
	{
		totalArraySize = sizeof(ElementType)*sizes[i];
		sortingArray[i].arrayToSort = malloc(totalArraySize);
		memcpy(sortingArray[i].arrayToSort, arrays[i], totalArraySize);

		sortingArray[i].algorithm = algorithm;
		sortingArray[i].arraySize = sizes[i];
		resetStatistics(&sortingArray[i].statistics);
	}
}

// Sorterar arrayerna
static void sortArrays(SortingArray toBeSorted[])
{
	int i;
	for (i = 0; i < NUMBER_OF_SIZES; i++)
	{
		SortingArray* current = &toBeSorted[i];
		sortArray(current->arrayToSort, current->arraySize, current->algorithm, &current->statistics);
	
		if (!isSorted(current->arrayToSort, current->arraySize))
		{
			printf("Fel! Algoritmen %s har inte sorterat korrekt!\n", getAlgorithmName(current->algorithm));
			printf("Resultatet är: \n");
			printArray(current->arrayToSort, current->arraySize, stdout);
			assert(0); // Aktiveras alltid
		}
	}
}

void printResult(SortingArray sortedArrays[], FILE* file)
{
	assert(file != NULL);

	int i;
	for (i = 0; i < NUMBER_OF_SIZES; i++)
	{
		fprintf(file, "%4d element: ", sortedArrays[i].arraySize);
		printStatistics(&sortedArrays[i].statistics, file);
		fprintf(file, "\n");
	}
	fprintf(file, "\n");
}

void sortAndPrint(SortingArray sortingArray[], SortingAlgorithm algorithm, const ElementType* arrays[], unsigned int sizes[], FILE* file)
{
	assert(file != NULL);

	prepareArrays(sortingArray, algorithm, arrays, sizes);
	sortArrays(sortingArray);
	printResult(sortingArray, file);
}

void freeArray(SortingArray sortingArray[])
{
	int i;
	for (i = 0; i < NUMBER_OF_SIZES; i++)
	{
		if (sortingArray[i].arrayToSort != NULL)
			free(sortingArray[i].arrayToSort);
		sortingArray[i].arrayToSort = NULL;
		sortingArray[i].arraySize = 0;
		resetStatistics(&sortingArray[i].statistics);
	}
}
