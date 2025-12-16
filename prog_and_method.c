#include <stdio.h>
#include <locale.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


// Функция, с помощью которой вводим каждый элемент массива
void recording_elements(short int choice_2, float* a)
{
	for (int i = 0; i < choice_2; i++)
	{
		scanf_s("%f", &a[i]);
		printf("\n");
	}
}

// Функция, с помощью которой заполняем массив рандомными элементами
void recording_elements_random(short int choice_2, float* a, int s)
{
	srand((unsigned)time(NULL));

	// Переменная, чтобы округлить число до нужного кол-ва знаков после запятой
	float t = (float)pow(10, s);

	for (int i = 0; i < choice_2; i++)
	{
		float dig = (float)rand() / RAND_MAX * 1000.0f;
		a[i] = round(dig * t) / t; // Обрезаем до s знаков после запятой
	}
}

// Сортировка выбором
void selection_sort(short int choice_2, float* a)
{
	for (int i = 0; i < choice_2 - 1; i++)
	{
		int m_index = i;
		for (int j = i + 1; j < choice_2; j++)
		{
			if (a[j] < a[m_index]) m_index = j;
		}
		float times_p = a[i];
		a[i] = a[m_index];
		a[m_index] = times_p;
	}
}

// Сортировка Хоара
void quick_sort(int left, int right, float* a)
{
	int l = left, r = right;
	// Принимаем за ведущий элемент средний индекс
	float pivot = a[(l + r) / 2];

	while (l <= r)
	{
		while (a[l] < pivot) l++;
		while (a[r] > pivot) r--;

		if (l <= r)
		{
			float tmp = a[l];
			a[l] = a[r];
			a[r] = tmp;
			l++;
			r--;
		}
	}

	if (left < r) quick_sort(left, r, a);
	if (right > l) quick_sort(l, right, a);
}

void merge_slice(float* a, int mid, int left, int right); // Прототип функции соеденения массивов для merge сортировки

// Сортировка слиянием
void merge_sort(float* a, int left, int right)
{
	if (left < right)
	{
		int mid = left + (right - left) / 2;  // Ищем средний индекс массива. Способ, чтобы не было переполнения массива
		merge_sort(a, left, mid);  // Сортируем левую часть 
		merge_sort(a, mid + 1, right);
		merge_slice(a, mid, left, right);
	}
}

// Функция слияния двух массивов
void merge_slice(float* a, int mid, int left, int right)
{
	// Зададим переменные, в которых будут длины левого и правого массива
	int l1 = mid - left + 1;
	int l2 = right - mid;

	// Выделяем память под новые массивы, в которых будем сортировать части
	float* m1 = (float*)malloc(l1 * sizeof(float));
	float* m2 = (float*)malloc(l2 * sizeof(float));

	// Заполняем массивы значениями из основного массива
	for (int i = 0; i < l1; i++)
	{
		m1[i] = a[left + i];
	}
	for (int j = 0; j < l2; j++)
	{
		m2[j] = a[mid + j + 1];
	}
	int i = 0, j = 0; // "указатели" на элементы в новых массивах
	int k = left; // Индекс в основном массиве, где нужно поменять значение

	// Сравниваем элементы первого и второго массива, пока где-то не закончится
	while (i < l1 && j < l2)
	{
		if (m1[i] < m2[j])
		{
			a[k] = m1[i];
			k++;
			i++;
		}
		else
		{
			a[k] = m2[j];
			k++;
			j++;
		}
	}

	// Если в каком-то из массивах остались значения, то запишем их в основной массив, так как все "спорные" элементы сравнились до
	while (i < l1)
	{
		a[k] = m1[i];
		k++;
		i++;
	}
	while (j < l2)
	{
		a[k] = m2[j];
		k++;
		j++;
	}

	free(m1);
	free(m2);
}

// Поразрядная сортировка для целых значений
void radix_sort_int(int* a, int choice_2)
{
	// Найдем максимальный элемент массива, чтобы понять, сколько нужно разрядов проверять
	int max = a[0];
	for (int i = 1; i < choice_2; i++)
	{
		if (a[i] > max) max = a[i];
	}

	// Цикл прохода по каждому разряду
	for (int raz = 1; max / raz > 0; raz *= 10)
	{
		// Делаем 2 массива, в одном храним кол-во крайних цифр, во втором новый отсортированный после каждого шага массив
		int m1[10] = { 0 }; // В этом массиве храним кол-во цифр на последних элементах
		int* m2 = (int*)malloc(choice_2 * sizeof(int));  // Отсортированный после каждого шага массив

		// Циклом изменяем общее кол-во последних цифр
		for (int i = 0; i < choice_2; i++) m1[(a[i] / raz) % 10]++;

		// Превратим кол-во цифр в позиции, с помощью префикса суммы
		for (int i = 1; i < 10; i++) m1[i] += m1[i - 1];

		// Сопоставляем последнюю цифру с кол-во цифр и ставим в нужное место
		for (int i = choice_2 - 1; i >= 0; i--)
		{
			int last_dig = (a[i] / raz) % 10; // Последняя цифра 
			int ind = m1[last_dig] - 1; // Максимально возможный индекс, куда можно записать число
			m2[ind] = a[i];
			m1[last_dig]--;
		}

		// Запишем в основной массив последний отсортированный массив (будем записывать пока не пройдем все разряды)
		for (int i = 0; i < choice_2; i++) a[i] = m2[i];

		free(m2);
	}
}


//Поразрядная сортировка для значений с плавающей запятой, которая будет использовтаь radix_sort_int
void radix_sort_float(float* a, int choice_2, int s)
{
	// В зависимости от кол-ва знаков после запятой, берем число равное 10^-кол-во знаков после запятой, чтобы в последствии преобразовать элементы в int
	long long int k = (long long int)pow(10, s);

	// Выделим память под два массива, в одном будут положительные числа, а во втором отрицательные по модулю.
	int* m1 = (int*)malloc(choice_2 * sizeof(int));
	int* m2 = (int*)malloc(choice_2 * sizeof(int));

	// Создадим переменные-указатели на динамические массивы
	int f = 0, se = 0;

	// Разделим все float числа основного массива по 2 новым
	for (int i = 0; i < choice_2; i++)
	{
		// Преобразуем в переменную х
		long long int x = (long long int)round(a[i] * k);

		// В зависимости от знака числа, определяем его массив и поведение
		if (x >= 0)
		{
			m1[f] = x;
			f++;
		}
		else
		{
			m2[se] = -x;
			se++;
		}
	}

	// Сортируем массивы с отриц и полож числами, условия нужно, чтобы не передавать пустой массив
	if (f > 0) radix_sort_int(m1, f);
	if (se > 0) radix_sort_int(m2, se);

	// Собираем массив обратно
	int id = 0;
	// Сначала отрицательные и в обратном порядке
	for (int i = se - 1; i >= 0; i--)
	{
		a[id] = -((float)m2[i] / k);
		id++;
	}

	for (int i = 0; i < f; i++)
	{
		a[id] = (float)m1[i] / k;
		id++;
	}

	free(m1);
	free(m2);
}

int correct_sort(float* a, int choice_2, int choice_2_1)
{
	float* a1 = (float*)malloc(choice_2 * sizeof(float));
	float* a2 = (float*)malloc(choice_2 * sizeof(float));
	float* a3 = (float*)malloc(choice_2 * sizeof(float));
	float* a4 = (float*)malloc(choice_2 * sizeof(float));
	for (int i = 0; i < choice_2; i++)
	{
		a1[i] = a[i];
		a2[i] = a[i];
		a3[i] = a[i];
		a4[i] = a[i];
	}
	selection_sort(choice_2, a1);
	merge_sort(a2, 0, choice_2 - 1);
	quick_sort(0, choice_2 - 1, a3);
	radix_sort_float(a4, choice_2, choice_2_1);
	int fl = 0;
	for (int i = 0; i < choice_2; i++)
	{
		if (a1[i] == a2[i] && a2[i] == a3[i] && a3[i] == a4[i]) continue;
		else fl++;
	}
	free(a1);
	free(a2);
	free(a3);
	free(a4);
	if (!fl) return 1;
	else return 0;
}


int main()
{
	setlocale(LC_ALL, "");

	// Приветствие пользователя
	printf("Привествую, это программа, с помощью которой Вы можете как отсортировать свой или рандомный массив, \nтак и сравнить время различных сортировок.\n");

	short int choice_1 = 0;  // Переменная для выбора 1 режима

	// Используем do-while, чтобы избавиться от ошибки ввода пользователем, т.е пока пользователь не введет 1 или 2, будем просить ввести правильное число
	do {
		printf("Для начала выберете, что будем сортировать:\n1. Свой массив.\n2. Рандомный массив.\n");
		scanf_s("%hd", &choice_1);
	} while (choice_1 < 1 || choice_1 > 2);

	short int choice_2 = 0;  // Переменная для выбора 2 режима
	short int choice_2_1 = 0;  // Переменная для выбора кол-ва знаков после запятой


	// В зависимости от выбора пользователя даем ему разный выбор
	switch (choice_1)
	{
	case 1:
		printf("Супер! Вы выбрали сортировать свой массив, введите кол-во элементов массива: ");
		scanf_s("%hd", &choice_2);
		printf("\n");
		printf("А теперь введите кол-во знаков после запятой в элементах вашего массива: ");
		scanf_s("%hd", &choice_2_1);
		printf("\n");
		break;
	case 2:
		printf("Супер! Вы выбрали сортировать рандомный массив, введите кол-во элементов массива: ");
		scanf_s("%hd", &choice_2);
		printf("\n");
		printf("А теперь введите кол-во знаков после запятой в элементах вашего массива: ");
		scanf_s("%hd", &choice_2_1);
		printf("\n");
		break;
	}

	// Динамически выделяем память под массив 
	float* a = (float*)malloc(choice_2 * sizeof(float));

	switch (choice_1)
	{
	case 1:
		printf("Введите элементы массива: ");
		recording_elements(choice_2, a);
		break;
	case 2:
		printf("Ваш готовый 'рандомный' массив: ");
		recording_elements_random(choice_2, a, choice_2_1);
		break;
	}

	short int choice_3; // Переменная для 3 выбора 

	// Используем do-while, чтобы избавиться от ошибки ввода пользователем, т.е пока пользователь не введет 1 или 2, будем просить ввести правильное число
	do {
		printf("Выберете, что будем делать с массивом:\n1. Просто сортируем массив.\n2. Сортируем массив разными сортировками и сравниваем время сортировки.\n(введите сортировки(у) позже).\n");
		scanf_s("%hd", &choice_3);
		printf("\n");
	} while (choice_3 < 1 || choice_3 > 2);

	short int choice_4; // Переменная выбора сортировки
	int z = 0;
	int r[5] = { 0 };

	// Копии массивов, чтобы корректнее проверить качество сортировок 
	float* a_sort1 = (float*)malloc(choice_2 * sizeof(float));
	for (int i = 0; i < choice_2; i++)
		a_sort1[i] = a[i];

	float* a_sort2 = (float*)malloc(choice_2 * sizeof(float));
	for (int i = 0; i < choice_2; i++)
		a_sort2[i] = a[i];

	float* a_sort3 = (float*)malloc(choice_2 * sizeof(float));
	for (int i = 0; i < choice_2; i++)
		a_sort3[i] = a[i];

	float* a_sort4 = (float*)malloc(choice_2 * sizeof(float));
	for (int i = 0; i < choice_2; i++)
		a_sort4[i] = a[i];
	int o = 0; // Переменная с помощью которой проверяем кол-во отклонившихся от нормы элементов

	// Выбираем между режимами
	switch (choice_3)
	{
	case 1:
		do {
			printf("Выберете какой сортировкой сортировать массив:\n1. Выбором.\n2. Хоара.\n3. Слиянием.\n4. Поразрядная.\n");
			scanf_s("%hd", &choice_4);
		} while (choice_4 < 1 || choice_4 > 4);

		// В зависимости от выбора пользователя сортируем массив сортировками:
		switch (choice_4)
		{
		case 1: // Сортировка выбором
			if (correct_sort(a, choice_2, choice_2_1)) printf("Сравнили все сортировки, они равны -> массив отсортирован кооректно!\n");
			selection_sort(choice_2, a_sort1);
			printf("Отсортированный массив: ");
			for (int i = 0; i < choice_2; i++)
			{
				printf("%f", a_sort1[i]);
				printf(" ");
			}
			break;
		case 2: // Сортировка Хоара
			if (correct_sort(a, choice_2, choice_2_1)) printf("Сравнили все сортировки, они равны -> массив отсортирован кооректно!\n");
			quick_sort(0, choice_2 - 1, a_sort2);
			printf("Отсортированный массив: ");
			for (int i = 0; i < choice_2; i++)
			{
				printf("%f", a_sort2[i]);
				printf(" ");
			}
			break;
		case 3: // Сортировка слиянием
			if (correct_sort(a, choice_2, choice_2_1)) printf("Сравнили все сортировки, они равны -> массив отсортирован кооректно!\n");
			merge_sort(a_sort3, 0, choice_2 - 1);
			printf("Отсортированный массив: ");
			for (int i = 0; i < choice_2; i++)
			{
				printf("%f", a_sort3[i]);
				printf(" ");
			}
			break;
		case 4: // Поразрядная сортировка
			if (correct_sort(a, choice_2, choice_2_1)) printf("Сравнили все сортировки, они равны -> массив отсортирован кооректно!\n");
			radix_sort_float(a_sort4, choice_2, choice_2_1);
			printf("Отсортированный массив: ");
			for (int i = 0; i < choice_2; i++)
			{
				printf("%f", a_sort4[i]);
				printf(" ");
			}
			break;
		}

		break;
	case 2:
		do {
			printf("Выберете кол-во различных сортировок: ");
			scanf_s("%hd", &choice_4);
		} while (choice_4 < 1 || choice_4 > 4);
		printf("Введите через Enter номера сортировок, время которых хотите узнать:\n1. Выбором.\n2. Хоара.\n3. Слиянием.\n4. Поразрядная.\n ");
		for (int i = 0; i < choice_4; i++)
		{
			do {
				scanf_s("%d", &z);
			} while (z < 1 || z > 4);
			r[z]++;
		}
		if (correct_sort(a, choice_2, choice_2_1)) printf("Сравнили все сортировки, они равны -> массив отсортирован кооректно!\n");
		radix_sort_float(a, choice_2, choice_2_1);
		printf("Отсортированный массив: ");
		for (int i = 0; i < choice_2; i++)
		{
			printf("%f", a[i]);
			printf(" ");
		}
		printf("\n");
		for (int i = 1; i < 5; i++)
		{
			// Объявляем старт и стоп
			clock_t start, end;
			double cpu_time_used;

			if (r[i] != 0)
			{
				switch (i)
				{
				case 1:
					start = clock();
					selection_sort(choice_2, a_sort1);
					end = clock();
					cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
					printf("Время выполнения сортировки - Выбором: ");
					printf("%lf", cpu_time_used);
					printf("\n");
					break;
				case 2:
					start = clock();
					quick_sort(0, choice_2 - 1, a_sort2);
					end = clock();
					cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
					printf("Время выполнения сортировки - Хоара: ");
					printf("%lf", cpu_time_used);
					printf("\n");
					break;
				case 3:
					start = clock();
					radix_sort_float(a_sort3, choice_2, choice_2_1);
					end = clock();
					cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
					printf("Время выполнения сортировки - Слиянием: ");
					printf("%lf", cpu_time_used);
					printf("\n");
					break;
				case 4:
					start = clock();
					merge_sort(a_sort4, 0, choice_2 - 1);
					end = clock();
					cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
					printf("Время выполнения сортировки - Поразрядная: ");
					printf("%lf", cpu_time_used);
					printf("\n");
					break;
				}
			}
		}
		break;
	}
	// Освобождаем память
	free(a_sort1);
	free(a_sort2);
	free(a_sort3);
	free(a_sort4);
	free(a);
	return 0;
}
