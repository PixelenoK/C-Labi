#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 120

int main()
{
	int i = 0, num;
	char *str, ch, c, mark[] = " ,.?!;:\n";
	
	puts("Enter the number of strings:");
	do {
		scanf_s("%d", &num);;
		c = getchar();
		rewind(stdin);

		if (c != '\n') {
			printf("\nSomething is wrong with input\nTry again:\n");
		}
		else break;
	} while (1);

	str = (char*)malloc(sizeof(char) * MAX_SIZE * num);
	
	puts("\nEnter your text:");
	for (int j = 0; j < num; j++) {
		do{
			ch = getchar();
			str[i++] = ch;
			if (i > num * MAX_SIZE - 2) { 
				puts("The size of your text is larger than you can enter.");
				return 0; 
			}
		} while (ch != '\n');
	}
	str[i] = '\0'; 

	system("cls");
	puts("Your text:");
	puts(str);

	int lenght = i;
	i = 0;
	char* beg = nullptr, * end = nullptr;

	//Переносим последнюю букву в начало слова
	for (int i = 0; i <= lenght; i++) {
		if (beg == nullptr)
			for (int j = 0; j < 8; j++) {
				if (str[i] != mark[j])
					beg = &str[i];
				else {
					beg = nullptr;
					break;
				}
			}
		else {
			for (int j = 0; j < 8; j++) {
				if (str[i] == mark[j]) {
					end = &str[i - 1];
					break;
				}				
				else end = nullptr;			
				
			}
		}

		if (beg != nullptr && end != nullptr) {
			char ech = *end;
			while (end != beg) {
				*end = *(end - 1);
				end--;
			}
			*beg = ech;
			beg = nullptr;
			end = nullptr;
		}
	}

	puts(str);

	int word = 0, size = 0;
	char mas[50];
	for (int i = 0; i < lenght; i++) {
		if (word == 0) {
			for (int j = 0; j < 8; j++) {
				if (str[i] != mark[j])
					word = 1;
				else {
					printf("%c", str[i]);
					word = 0;
					break;
				}
			}
		}
		if (word == 1) {
			for (int j = 0; j < 8; j++) {
				if (str[i] == mark[j]) {
					printf("%c", str[i]);
					word = 0;
					size = 0;
					break;
				}
				else {
					word = 1;					
				}
			}

			if (word == 0) continue;

			char ch = str[i];
			if (str[i] >= 65 && str[i] <= 90) ch += 32;
			if (size == 0) {
				mas[size] = ch;
				size++;
				printf("%c", str[i]);
			}
			else {
				int really = 0;
				for (int j = 0; j < size; j++) {
					if (ch == mas[j]) {
						really = 0;
						break;
					}
					else really = 1;
				}

				if (really == 1) {
					mas[size] = ch;
					size++;
					printf("%c", str[i]);
				}
			}
		}


	}


	free(str);	
	return 0;
}

