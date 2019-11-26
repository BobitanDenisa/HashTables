/* Bobitan Denisa-Ioana

   Task: 

   Implement correctly and efficiently the insert and search operations in a hash table using open addressing and quadratic probing.

   Observation: 

   The complexity in all cases is O(1), except for the worst case, with O(n), so we only display the results, without using the graphs.

   Conclusion:

   Hash tables are very useful and quick when search is required.
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "Profiler.h"

#pragma warning(disable : 4996)

#define N 9973 //the length of the hash table

FILE *f = fopen("report.txt", "w");
FILE *g = fopen("test.txt", "w");

int op;

typedef struct
{
	int id;
	char name[30];
} Entry;
 
int hash(int x, int i, int size)
{
	return (x + i + i * i) % size; //this function was chosen for simplicity, for the sake of the test function
	//a better function would be f(x)=(x+c1*i+c2*i*i)%size, c1, c2 being constants
}

int insertInTable(Entry *table, int key, int size)
{
	int i = 0;
	int h;
	do
	{
		h = hash(key, i, size);
		if (table[h].id == 0 || table[h].id == key) //if the same element is already there, overwrite it
		{
			table[h].id = key;
			return h;
		}
		else
		{
			i++;
			fprintf(g, "Collision: %d on %d\n", key, h); //used only with the test function to see if there were any collisions, otherwise, it will be commented
		}
	} while (i < size);
	return -1;
}

int searchInTable(Entry *table, int key, int size)
{
	int i = 0;
	int h;
	do
	{
		h = hash(key, i, size);
		op++;
		if (table[h].id == key)
		{
			return h;
		}
		i++;
	} while (table[h].id != 0 && i < size);
	return -1;
}

void initTable(Entry *table, int size)
{
	for (int i = 0; i < size; i++)
	{
		table[i].id = 0;
		strcpy(table[i].name, "");
	}
}

Entry *fillFactor(int percentage)
{
	int x = N * percentage / 100; //x is the number of elements inserted in the hash table
	Entry *table = (Entry*)calloc(N, sizeof(Entry));
	initTable(table, N);
	int *a = (int*)malloc(x * sizeof(int));
	FillRandomArray(a, x, 10, 50000, true, 0);
	for (int i = 0; i < x; i++)
	{
		insertInTable(table, a[i], N);
	}
	return table;
}

void evaluateSearch()
{
	int fill[] = { 80, 85, 90, 95, 99 };
	int maxF, maxNotF;
	float avgF, avgNotF;
	int *a = (int*)malloc(1500 * sizeof(int));
	
	Entry *table = (Entry*)calloc(N, sizeof(Entry));
	fprintf(f, "Filling Factor     Avg Effort (hit)     Max Effort (hit)     Avg Effort (miss)     Max Effort (miss)\n");
	for (int i = 0; i < 5; i++) //all elements of fill array, representing the fill factors
	{
		fprintf(f, "    %.2f", (float)fill[i] / 100);
		maxF = maxNotF = 0;
		avgF = avgNotF = 0;
		for (int run = 0; run < 5; run++) //5 runs for average
		{
			table = fillFactor(fill[i]);
			for (int j = 0; j < 1500; j++) //found (hit) - search for arrays that are in the table for sure (table[j])
			{
				op = 0;
				searchInTable(table, table[j].id, N);
				if (maxF < op)
				{
					maxF = op;
				}
				avgF += op;
			}
			FillRandomArray(a, 1500, 50001, 60000, true, 0);
			for (int j = 0; j < 1500; j++) //not found (miss) - search for elements that are out of the range of the table's elements
			{
				op = 0;
				searchInTable(table, a[j], N);
				if (maxNotF < op)
				{
					maxNotF = op;
				}
				avgNotF += op;
			}
		}
		fprintf(f, "                 %5.2f                 %d                  %5.2f                  %d\n", avgF / (N * fill[i] / 100), maxF, avgNotF / (N * fill[i] / 100), maxNotF);
	}
	fclose(f);
}

void displayArray(Entry *a, int n)
{
	for (int i = 0; i < n; i++)
	{
		fprintf(g, "Position%d: %d (name: %s)\n", i, a[i].id, a[i].name);
	}
	fprintf(g, "\n");
}

void displayResult(Entry *table, int pos, int n)
{
	if (pos == -1)
	{
		fprintf(g, "Not found: %d\n", n);
	}
	else
	{
		fprintf(g, "Found: %s\n", table[pos].name);
	}
}

void testHashing()
{
	Entry *table = (Entry*)calloc(7, sizeof(Entry));
	initTable(table, 7);
	
	int n, nr, pos;
	char *name=(char*)malloc(10*sizeof(char));
	printf("Enter number of elements to insert in table: ");
	scanf("%d", &n);
	for (int i = 0; i < n; i++)
	{
		scanf("%d %s", &nr, name);		
		pos = insertInTable(table, nr, 7); //6 is maxsize of the table for this test function
		fprintf(g, "%d on pos: %d\n", nr, pos);
		strcpy(table[pos].name, name);
	}
	fprintf(g, "\n");
	int el;
	do
	{
		printf("Enter element to search: ");
		scanf("%d", &el);
		if (el != 0) //0 is used just to exit the readig loop - it reads elements to search until it finds 0
		{
			pos = searchInTable(table, el, 7);
			displayResult(table, pos, el);
		}
	} while (el != 0);
	
	fclose(g);
}

int main()
{
	//testHashing();

	evaluateSearch();

	return 0;
}