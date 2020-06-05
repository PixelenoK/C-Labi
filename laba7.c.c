#define CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <string.h>

typedef char bool;
#define true 1;
#define false 0;

double SALARY = 67.65; //in $,in month

typedef struct capital
{
	char* name;
	double oilPrice; //in $
	double fruitPrice; //in $
	double toBorderBack; //way to Minsk
	double toBorder; //way from Minsk
} capital;

typedef struct truck
{
	double avaiblePlace; //in tonnes
	double oilConsumption; //12 l. per 60 km (20 per 100) //  will be 0.2/60 per s
	double haveOil; //will be 500 l. from start
	char speed; //speed
	double spent; //spent money
	double spentOnFruits;
	bool active; //false - truck is waiting for a signal to leave
	double toNextPoint; //km to next point
	char path;
	bool onBorder;
	bool done; //if true - truck driving in the way to Minsk, false - truck driving from Minsk
	char point;
} truck;

/*
point:
0 - Minsk
1 - Warsaw
2 - Prague
etc...
6 - Kiyv
etc...
*/

void ChangePrices(capital*[]);
void Trading(truck *, capital*);
void MissionCompleted(truck*, capital*);
void PrintTrucks(truck*, capital*[]);
void PrintFruitPrices(capital* []);
void oilRefill(truck*, capital*);

/*

6.2 (29). FruitImport. Белорусская компания импортирует фрукты из ряда
европейских стран. Компания арендует торговые площадки,
находящиеся на двух главных маршрутах: 1) в Варшаве, Праге,
Вене, Милане, Марселе, Барселоне и 2) в Киеве, Кишиневе,
Бухаресте, Софии, Афинах. FruitImport располагает 6 грузовиками,
каждый из которых способен перевезти максимум 25 тонн груза. В
каждой машине едет 2 водителя, таким образом, машина находится
в пути круглосуточно. Средняя скорость – 60 км/ч, среднее время на
пересечение границы – 4 часа. Стоимость бензина и зарплата
водителям – стандартные. В Минске компания сразу реализует
привезенные фрукты оптовику, ее прибыль составляет 5-15% от
выручки. Каждый час, в течение суток, партнеры передают
компании информацию о поставках фруктов (объем (тонн) и цена
партии) на торговых площадках. Разработать программу
оптимального движения грузовиков FruitImport.
Проиллюстрировать движение автомобилей и совершаемые
деловые операции в режиме реального времени.

*/

int main()
{
	capital* capitals[12];

#pragma region capitals

	capital Minsk;
	{
		Minsk.oilPrice = 0.7;
		Minsk.toBorder = 301.5; //to Poland
		Minsk.toBorderBack = 369; //to Ukraine

		Minsk.fruitPrice = 0;
		Minsk.name = "Minsk";

		capitals[0] = &Minsk;
	}


	//path 1
	capital Warsaw;
	{
		Warsaw.fruitPrice = 1.5;
		Warsaw.oilPrice = 0.95;
		Warsaw.toBorderBack = 251.7;
		Warsaw.toBorder = 555;

		Warsaw.name = "Warsaw";

		capitals[1] = &Warsaw;
	}

	capital Prague;
	{
		Prague.fruitPrice = 1.5;
		Prague.oilPrice = 1;
		Prague.toBorderBack = 134;
		Prague.toBorder = 222;

		Prague.name = "Prague";

		capitals[2] = &Prague;
	}

	capital Vein;
	{
		Vein.fruitPrice = 1.5;
		Vein.oilPrice = 1.115;
		Vein.toBorderBack = 72.7;
		Vein.toBorder = 391;

		Vein.name = "Vein";

		capitals[3] = &Vein;
	}

	capital Milan;
	{
		Milan.fruitPrice = 1.5;
		Milan.oilPrice = 1.5;
		Milan.toBorderBack = 494;
		Milan.toBorder = 287;

		Milan.name = "Milan";

		capitals[4] = &Milan;
	}

	capital Marseilles;
	{
		Marseilles.fruitPrice = 1.5;
		Marseilles.oilPrice = 1.35;
		Marseilles.toBorderBack = 228;
		Marseilles.toBorder = 346;

		Marseilles.name = "Marseilles";

		capitals[5] = &Marseilles;
	}

	capital Barcelona;
	{
		Barcelona.fruitPrice = 1.5;
		Barcelona.oilPrice = 1.17;
		Barcelona.toBorderBack = 166;
		Barcelona.toBorder = 0;

		Barcelona.name = "Barcelona";

		capitals[6] = &Barcelona;
	}


	//path 2
	capital Kiev;
	{
		Kiev.fruitPrice = 1.5;
		Kiev.oilPrice = 0.813;
		Kiev.toBorderBack = 219;
		Kiev.toBorder = 407;

		Kiev.name = "Kiev";

		capitals[7] = &Kiev;
	}

	capital Kishinev;
	{
		Kishinev.fruitPrice = 1.5;
		Kishinev.oilPrice = 0.871;
		Kishinev.toBorderBack = 64.6;
		Kishinev.toBorder = 80;

		Kishinev.name = "Kishinev";

		capitals[8] = &Kishinev;
	}

	capital Bucharest;
	{
		Bucharest.fruitPrice = 1.5;
		Bucharest.oilPrice = 4.5;
		Bucharest.toBorderBack = 357;
		Bucharest.toBorder = 143;

		Bucharest.name = "Bucharest";

		capitals[9] = &Bucharest;
	}

	capital Sofia;
	{
		Sofia.fruitPrice = 1.5;
		Sofia.oilPrice = 0.948;
		Sofia.toBorderBack = 215;
		Sofia.toBorder = 182;

		Sofia.name = "Sofia";

		capitals[10] = &Sofia;
	}

	capital Athens;
	{
		Athens.fruitPrice = 1.5;
		Athens.oilPrice = 1.438;
		Athens.toBorderBack = 609;
		Athens.toBorder = 0;

		Athens.name = "Athens";

		capitals[11] = &Athens;
	}
#pragma endregion capitals

	unsigned long long whenOnBorder[6];
	truck trucks[6];
	bool visited[6][6];
	unsigned long long lastPriceChange = 0;

	bool timeBoost = false;

	for (char i = 0; i < 6; i++)
	{
		for (char j = 0; j < 6; j++)
		{
			visited[i][j] = false;
		}
	}

	for (int i = 0; i < 6; i++)
	{
		trucks[i].avaiblePlace = 25;
		trucks[i].haveOil = 500;
		trucks[i].oilConsumption = 0.003;
		trucks[i].speed = 60; // 1/60 km in s ~ 0,167
		trucks[i].spent = 0;
		trucks[i].active = false;
		trucks[i].toNextPoint = 0;
		trucks[i].onBorder = false;
		trucks[i].point = 0;
		trucks[i].done = false;
		trucks[i].spentOnFruits = 0;
	}

	double path1price = 0;
	double path2price = 0;

	for (char i = 1; i < 7; i++)
	{
		path1price += capitals[i]->fruitPrice;
	}

	for (char i = 7; i < 12; i++)
	{
		path2price += capitals[i]->fruitPrice;
	}


	for (unsigned long long sec = 0; !GetAsyncKeyState(0x1B); sec++) //main cycle
	{
		system("cls");

		if (GetAsyncKeyState(0x54))  // 't' pressed, boost time
		{
			while (GetAsyncKeyState(0x54)) {};

			if (!timeBoost)
			{
				timeBoost = true;
			}
			else
			{
				timeBoost = false;
			}
		}				

		if (sec - lastPriceChange >= 3600) //changing prices
		{
			ChangePrices(capitals);
			lastPriceChange = sec;

			path1price = 0;
			path2price = 0;

			for (char i = 1; i < 7; i++)
			{
				path1price += capitals[i]->fruitPrice;
			}

			for (char i = 7; i < 12; i++)
			{
				path2price += capitals[i]->fruitPrice;
			}
		}

		if (GetAsyncKeyState(0x31)) // '1' pressed, path 1
		{
			while (GetAsyncKeyState(0x31)) {};
			for (char i = 0; i < 6; i++)
			{
				if (!trucks[i].active)
				{
					trucks[i].path = 1;
					trucks[i].active = true;
					trucks[i].toNextPoint = Minsk.toBorder;
					break;
				}
			}
		}
		if (GetAsyncKeyState(0x32)) //'2' pressed, path 2
		{
			while (GetAsyncKeyState(0x32)) {};
			for (char i = 0; i < 6; i++)
			{
				if (!trucks[i].active)
				{
					trucks[i].path = 2;
					trucks[i].active = true;
					trucks[i].toNextPoint = Minsk.toBorderBack;
					break;
				}
			}
		}	
		if (GetAsyncKeyState(0x61)) //'numpad 1' pressed
		{
			while (GetAsyncKeyState(0x61)) {};
			for (char i = 0; i < 6; i++)
			{
				if (!trucks[i].active)
				{
					trucks[i].path = 1;
					trucks[i].active = true;
					trucks[i].toNextPoint = 0;
					trucks[i].point = 6;
					break;
				}
			}
		}
		if (GetAsyncKeyState(0x62)) //'numpad 2' pressed
		{
			while (GetAsyncKeyState(0x61)) {};
			for (char i = 0; i < 6; i++)
			{
				if (!trucks[i].active)
				{
					trucks[i].path = 2;
					trucks[i].active = true;
					trucks[i].toNextPoint = 0;
					trucks[i].point = 5;
					break;
				}
			}
		}

		if (!timeBoost) //prints
		{
			//prices of fruits on pathes
			{
				puts("'t' - time boost, depends on computing power\n'h' - an hour ahead\n'1' - send truck on the 1st path\n'2' - send truck on the 2nd path\n'numpad 1' - send truck on the 1st path to Minsk\n'numpad 2' - send truck on the 2nd path to Minsk\n");

				printf("Path 1 fruit price: %.2f\nPath 2 fruit price: %.2f\n", path1price, path2price);

				if (path1price * 5 < path2price * 6)
				{
					puts("Path 1 is more profitable.");
				}
				else if (path1price * 5 > path2price * 6)
				{
					puts("Path 2 is more profitable.");
				}
				else
				{
					puts("Pathes are equal");
				}

				puts("\n");
			}

			PrintTrucks(trucks, capitals);
			puts("\n\n");
			PrintFruitPrices(capitals);
		}

		for (char i = 0; i < 6; i++)
		{
			if (trucks[i].active)
			{
				if (trucks[i].onBorder)
				{
					if ((sec - whenOnBorder[i]) >= 3600 * 4)
					{
						trucks[i].onBorder = false;
					}
				}
				else //slill driving
				{				
					trucks[i].toNextPoint -= (double)trucks[i].speed / 3600;
					trucks[i].haveOil -= trucks[i].oilConsumption;

					if (trucks[i].haveOil <= 100) //instant oil refilling, yep
					{
						oilRefill(&trucks[i], (capitals[trucks[i].point + ((trucks[i].path == 1) ? (0) : (6))]));
					}

					if (trucks[i].toNextPoint <= 0) //arrive to some point
					{
						if (trucks[i].done) //to Minsk
						{
							if (!trucks[i].point) //arrived to Minsk
							{
								MissionCompleted(&trucks[i], &Minsk);
								
								if (!timeBoost)
								{
									Sleep(2000);
								}

								continue;
							}

							if (visited[i][trucks[i].point + ((trucks[i].path == 1) ? (0) : (5))]) //have arrived to border
							{
								trucks[i].onBorder = true;
								whenOnBorder[i] = sec; //time when start crossing the border
								trucks[i].point--;
								trucks[i].toNextPoint = capitals[trucks[i].point + ((trucks[i].path == 1) ? (0) : (6))]->toBorder;
							}
							else //have arrived to capital
							{
								Trading(&trucks[i], capitals[trucks[i].point + ((trucks[i].path == 1) ? (0) : (6))]);

								if (!timeBoost)
								{
									Sleep(2000);
								}

								trucks[i].toNextPoint = capitals[trucks[i].point + ((trucks[i].path == 1) ? (0) : (6))]->toBorderBack;
							}
						}
						else //from Minsk
						{
							if (trucks[i].point == ((trucks[i].path == 1) ? (6) : (5))) //end of the path from Minsk, driving back
							{
								Trading(&trucks[i], capitals[trucks[i].point + ((trucks[i].path == 1) ? (0) : (6))]);

								if (!timeBoost)
								{
									Sleep(2000);
								}

								trucks[i].done = true;
								trucks[i].toNextPoint = capitals[trucks[i].point + ((trucks[i].path == 1) ? (0) : (6))]->toBorderBack;
							}
							else //have arrived to border
							{
								trucks[i].point++;
								trucks[i].toNextPoint = capitals[trucks[i].point + ((trucks[i].path == 1) ? (0) : (6))]->toBorder + 
									capitals[trucks[i].point + ((trucks[i].path == 1) ? (0) : (6))]->toBorderBack;

								trucks[i].onBorder = true;
								whenOnBorder[i] = sec; //time when start crossing the border
							}							
						}
					}
				}
			}
		}		

		if (GetAsyncKeyState(0x48)) // 'h' pressed
		{
			while (GetAsyncKeyState(0x48)) {};			

			for (char i = 0; i < 6; i++)
			{
				if ((trucks[i].active) && (!trucks[i].onBorder))
				{
					if ((trucks[i].toNextPoint - trucks[i].speed) > 0) //if it ok and in an hour he will still ride
					{
						trucks[i].toNextPoint -= trucks[i].speed ;

						if ((trucks[i].haveOil -= trucks[i].oilConsumption * 3600) < 100) //instant oil refilling, yep
						{
							oilRefill(&trucks[i], (capitals[trucks[i].point + ((trucks[i].path == 1) ? (0) : (6))]));
						}
					}
					else
					{
						double secToPoint = trucks[i].toNextPoint / (double)trucks[i].speed * 3600;

						if (trucks[i].done) //path to Minsk
						{
							if (visited[i][trucks[i].point + ((trucks[i].path == 1) ? (0) : (5))]) //have arrived to border
							{
								trucks[i].onBorder = true;
								whenOnBorder[i] = sec + secToPoint; //time when start crossing the border
								trucks[i].point--;
								trucks[i].toNextPoint = capitals[trucks[i].point + ((trucks[i].path == 1) ? (0) : (6))]->toBorder;
							}
							else //have arrived to capital
							{
								Trading(&trucks[i], capitals[trucks[i].point + ((trucks[i].path == 1) ? (0) : (6))]);

								if (!timeBoost)
								{
									Sleep(2000);
								}

								trucks[i].toNextPoint = (capitals[trucks[i].point + ((trucks[i].path == 1) ? (0) : (6))]->toBorderBack - (sec + 3600 - secToPoint)*(trucks[i].speed / 3600));
							}
						}
						else //path from Minsk
						{
							if (trucks[i].point == ((trucks[i].path == 1) ? (6) : (5))) //end of the path from Minsk, driving back
							{
								Trading(&trucks[i], capitals[trucks[i].point + ((trucks[i].path == 1) ? (0) : (6))]);

								if (!timeBoost)
								{
									Sleep(2000);
								}

								trucks[i].done = true;
								trucks[i].toNextPoint = (capitals[trucks[i].point + ((trucks[i].path == 1) ? (0) : (6))]->toBorderBack - (sec + 3600 - secToPoint) * (trucks[i].speed / 3600));
							}
							else //have arrived to border
							{
								trucks[i].point++;
								trucks[i].toNextPoint = (capitals[trucks[i].point + ((trucks[i].path == 1) ? (0) : (6))]->toBorder +
									capitals[trucks[i].point + ((trucks[i].path == 1) ? (0) : (6))]->toBorderBack);

								trucks[i].onBorder = true;
								whenOnBorder[i] = sec + secToPoint; //time when start crossing the border
							}
						}

						if ((trucks[i].haveOil -= trucks[i].oilConsumption * secToPoint) < 100) //instant oil refilling, yep
						{
							oilRefill(&trucks[i], (capitals[trucks[i].point + ((trucks[i].path == 1) ? (0) : (6))]));
						}

						
					}
				}
				else if (trucks[i].onBorder)
				{
					double deltatime = ((sec - whenOnBorder[i]) - ((double)3600 * 4));

					if (deltatime >= 0)
					{
						trucks[i].onBorder = false;
						trucks[i].toNextPoint -= (deltatime * ((double)trucks[i].speed / 3600));
					}
				}				
			}

			sec += 3600;
		}

		if (!timeBoost)
		{
			Sleep(850); //~sec, taking into account time spent above
		}
	}

	getchar();
	return 0;
}

void ChangePrices(capital* capitals[])
{
	capitals[0]->fruitPrice = 1.65 * (120. - (double)(rand() % 41)) / (double)100;
	for (char i = 1; i < 12; i++)
	{
		capitals[i]->fruitPrice = 1.5 * (120. - (double)(rand() % 41)) / (double)100;
	}
}

void PrintFruitPrices(capital* capitals[])
{
	printf("Minsk: %.2f\n\n", capitals[0]->fruitPrice);

	puts("path 1:");
	for (char i = 1; i < 7; i++)
	{
		printf("%s: %.2f\n", capitals[i]->name, capitals[i]->fruitPrice);
	}

	puts("\npath 2:");
	for (char i = 7; i < 12; i++)
	{
		printf("%s: %.2f\n", capitals[i]->name, capitals[i]->fruitPrice);
	}
}

void Trading(truck* truck, capital* cap)
{
	int kgs = 3 * (((150.0 - (double)(rand() % 51))/100.0)*1000);

	if (truck->avaiblePlace*1000 - (double)kgs < 0)
	{
		kgs -= (truck->avaiblePlace*1000);
	}

	truck->avaiblePlace -= kgs / 1000.0;
	truck->spent += kgs * cap->fruitPrice;
	truck->spentOnFruits += kgs * cap->fruitPrice;

	printf("\n\nTruck bought %.2f tonnes of fruits in %s", kgs / 1000.0, cap->name);
}

void MissionCompleted(truck* tr, capital* Minsk)
{
	printf("Truck from path %d has arrived.\n Revenue is $%f-$%f", tr->path, Minsk->fruitPrice*((25.0 - tr->avaiblePlace) * 1000)*0.1, Minsk->fruitPrice * ((25.0 - tr->avaiblePlace) * 1000) * 0.15); //%10-15

	tr->avaiblePlace = 25;
	tr->haveOil = 500;
	tr->oilConsumption = 0.003;
	tr->speed = 60; // 1/60 km in s ~ 0,167
	tr->spent = 0;
	tr->active = false;
	tr->toNextPoint = 0;
	tr->onBorder = false;
	tr->point = 0;
	tr->done = false;
	tr->spentOnFruits = 0;
	
}

void PrintTrucks(truck* trucks, capital* capitals[])
{
	puts("# N | path | state | next p. | oil | place | money spent | way | point | where #\n");
	for (char i = 0; i < 6; i++)
	{
		if (trucks[i].active)
		{
			printf("# %d | %d | %s | %.2f | %.2f | %.2f | $%.2f | %s | %d | %s #\n", i + 1, trucks[i].path, ((trucks[i].onBorder) ? ("on border") : ("driving")), trucks[i].toNextPoint, trucks[i].haveOil, trucks[i].avaiblePlace, trucks[i].spent, (trucks[i].done)?("to Minsk"):("from Minsk"), trucks[i].point, capitals[trucks[i].point + ((trucks[i].path == 1)?(0):(+6))]->name);
		}
	}
}

void oilRefill(truck* truck, capital* cap)
{
	(*truck).haveOil += 400;
	(*truck).spent += cap->oilPrice * 400;
}


//only for 666