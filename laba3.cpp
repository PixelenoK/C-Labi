#include <stdio.h>
#include <string>
#define MAX 13

void initialize_massive(int arr[][MAX])
{
	for (int i = 0; i < MAX; ++i)
		for (int j = 0; j < MAX; ++j)
			arr[i][j] = 0;
}

void initialize_massive(int arr[][MAX][MAX])
{
	for (int i = 0; i < MAX; ++i)
		for (int j = 0; j < MAX; ++j)
			for (int k = 0; k < MAX; ++k)
				arr[i][j][k] = 0;
}

int main()
{
	int dot_x = 0;
	int dot_y = 0;
	int dot_z = 0;
	int array[MAX][MAX][MAX];
	int proection[MAX][MAX];
	int amount_of_dots = 0;

	initialize_massive(proection);
	initialize_massive(array);

	printf("How many dots you want to add? ");
	scanf_s("%d", &amount_of_dots);

	for (int i = 0; i < amount_of_dots; ++i)
	{
		printf("\nx-coord: ");
		scanf_s("%d", &dot_x);

		printf("y-coord: ");
		scanf_s("%d", &dot_y);

		printf("z-coord: ");
		scanf_s("%d", &dot_z);

		array[dot_x][dot_y][dot_z] = 1;
	}

	for (int i = 0; i < MAX; ++i) // getting proection on xy
		for (int j = 0; j < MAX; ++j)
			for (int k = 0; k < MAX; ++k)
				if (array[i][j][k] == 1)
					proection[i][j] = 1;

	printf("proection on XY:\n--------------------------------> Y\n");

	for (int i = 0; i < MAX; ++i)
	{
		printf("| ");
		for (int j = 0; j < MAX; ++j)
		{
			printf("%d ", proection[i][j]);
		}
		printf("\n");
	}
	printf("|\nX\n\n");


	initialize_massive(proection);

	for (int i = 0; i < MAX; ++i) // getting proection on xz
		for (int j = 0; j < MAX; ++j)
			for (int k = 0; k < MAX; ++k)
				if (array[i][j][k] == 1)
					proection[i][k] = 1;

	printf("proection on XZ:\n--------------------------------> Z\n");

	for (int i = 0; i < MAX; ++i)
	{
		printf("| ");
		for (int j = 0; j < MAX; ++j)
		{
			printf("%d ", proection[i][j]);
		}
		printf("\n");
	}
	printf("|\nX\n\n");

	initialize_massive(proection);

	for (int i = 0; i < MAX; ++i) // getting proection on yz
		for (int j = 0; j < MAX; ++j)
			for (int k = 0; k < MAX; ++k)
				if (array[i][j][k] == 1)
					proection[j][k] = 1;

	printf("proection on YZ:\n--------------------------------> Z\n");

	for (int i = 0; i < MAX; ++i)
	{
		printf("| ");
		for (int j = 0; j < MAX; ++j)
		{
			printf("%d ", proection[i][j]);
		}
		printf("\n");
	}
	printf("|\nY\n\n");

}