#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#define TEXT 200


int main()
{
	int slov_size = 0;
	char *slovar, text[TEXT], mark[] = " ,.?!;:\n";
	FILE* fp;

	fopen_s(&fp ,"Slovar.txt", "r");
	
	if (!fp) exit(1);
	while (fgets(text, 50, fp) != NULL) slov_size++;
	rewind(fp);
	int I = 0;
	slovar = (char*)malloc(50*slov_size*sizeof(char));
	while (fgets(slovar + I * 50, 50, fp) != NULL) I++;

	fclose(fp);
	FILE* fp2;

	fopen_s(&fp2, "Text.txt", "r");
	if (!fp2) exit(1);

	for (int i = 0; i < slov_size; i++) {
		int col = 0;
		while (fgets(text, TEXT, fp) != NULL)
		{
			int k = 0, l = 0, equal = 0, marka = 0;

			while (text[k] != '\0')
			{
				for (int j = 0; j < 8; j++) 
				{
					if (text[k] != mark[j])
						marka = 0;
					else {
						marka = 1;
						break;
					}
				}
				if (marka == 1) {
					if (equal == 1 && (*(slovar + i * 50 + l) == '\n'|| *(slovar + i * 50 + l + 1) == '\0')) col++;
					equal = 0;
					l = 0;
				}
				else {
					if (l == 0 && (text[k] == *(slovar + i * 50 + l) || text[k] == *(slovar + i * 50 + l) - 32)) {
						equal = 1;
						l++;
					}
					else if (text[k] == *(slovar + i * 50 + l) && equal == 1) {
						l++;
					}
					else {
						equal = 0;
						l = 0;
					}
				}
				k++;
			}
		}
		
		int letter = 0;
		while (*(slovar + i * 50 + letter) != '\n')
		{
			printf("%c", *(slovar + i * 50 + letter));
			letter++;
		}
	 
		printf("\t%d", col);
		puts("\n");

		rewind(fp2);
	}
	
	fclose(fp2);

	free(slovar);
	return 0;
}

