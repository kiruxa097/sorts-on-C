#include <stdio.h>
#include <locale.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>


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
void recording_elements_random(short int choice_2, float* a)
{
	srand((unsigned)time(NULL));

	for (int i = 0; i < choice_2; i++)
	{
		a[i] = (float)rand() / RAND_MAX * 1000.0f;
	}
}

// Сортировка выбором
void selection_sort(short int choice_2, float* a, int* swap)
{
	for (int i = 0; i < choice_2 - 1; i++)
	{
		int m_index = i;
		for (int j = i + 1; j < choice_2; j++)
		{
			if (a[j] < a[m_index]) m_index = j;
		}
		if (m_index != i)
		{
			float times_p = a[i];
			a[i] = a[m_index];
			a[m_index] = times_p;
			(*swap)++;
		}
	}
}

// Сортировка Хоара
void quick_sort(int left, int right, float* a, int* swap)
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
			if (l != r) (*swap)++;
			float tmp = a[l];
			a[l] = a[r];
			a[r] = tmp;
			l++;
			r--;
		}
	}

	if (left < r) quick_sort(left, r, a, swap);
	if (right > l) quick_sort(l, right, a, swap);
}

void merge_slice(float* a, int mid, int left, int right, int* swap); // Прототип функции соеденения массивов для merge сортировки

// Сортировка слиянием
void merge_sort(float* a, int left, int right, int* swap)
{
	if (left < right)
	{
		int mid = left + (right - left) / 2;  // Ищем средний индекс массива. Способ, чтобы не было переполнения массива
		merge_sort(a, left, mid, swap);  // Сортируем левую часть 
		merge_sort(a, mid + 1, right, swap);
		merge_slice(a, mid, left, right, swap);
	}
}

// Функция слияния двух массивов
void merge_slice(float* a, int mid, int left, int right, int* swap)
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
		(*swap)++;
	}

	// Если в каком-то из массивах остались значения, то запишем их в основной массив, так как все "спорные" элементы сравнились до
	while (i < l1)
	{
		a[k] = m1[i];
		k++;
		i++;
		(*swap)++;
	}
	while (j < l2)
	{
		a[k] = m2[j];
		k++;
		j++;
		(*swap)++;
	}

	free(m1);
	free(m2);
}

// Поразрядная сортировка
// Преобразование из float в ключ для буд. сортировки
uint32_t float_key(float x)
{
	uint32_t u; // Переменная для хранения битов float
	memcpy(&u, &x, sizeof(float));  // Копируем биты из float в uint32_t
	// Проверяем старший бит числа
	if (u & 0x80000000) u = ~u;
	else u ^= 0x80000000;  // Меняет бит знака 
	return u;
}

// Обратное преобразование 
float key_float(uint32_t u)
{
	if (u & 0x80000000) u ^= 0x80000000;
	else u = ~u;
	float x;
	memcpy(&x, &u, sizeof(float));
	return x;
}

// Сортировка по байту
void count_sort(uint32_t* a, uint32_t* tmp, int n, int byte, int* swap) // n - choice_2
{
	int count[256] = { 0 }; // Массив частот 

	for (int i = 0; i < n; i++)
	{
		int b = (a[i] >> (byte * 8)) & 0xFF;  // Берем каждый байт 
		count[b]++;
	}

	// Префикс суммы, чтобы узнать потенциальное место каждого числа
	for (int i = 1; i < 256; i++) count[i] += count[i - 1];

	// Заполняем текущие места в массиве
	for (int i = n - 1; i >= 0; i--)
	{
		int b = (a[i] >> (byte * 8)) & 0xFF;
		tmp[count[b] - 1] = a[i];
		count[b] -= 1;
		(*swap)++;
	}

	memcpy(a, tmp, n * sizeof(uint32_t));
}

// Основная сортировка
void radix_sort_float(float* a, int n, int* swap)
{
	uint32_t* a_cop = (uint32_t*)malloc(n * sizeof(uint32_t));
	uint32_t* tmp = (uint32_t*)malloc(n * sizeof(uint32_t));

	// Преобразуем в битовые записи с помощью ранее написанной функции
	for (int i = 0; i < n; i++) a_cop[i] = float_key(a[i]);
	// Сортируем по байтам с помощью ранее написанной функции
	for (int byte = 0; byte < 4; byte++) count_sort(a_cop, tmp, n, byte, swap);
	// Преобразуем обратно все битовые записи в float
	for (int i = 0; i < n; i++) a[i] = key_float(a_cop[i]);

	// Освобождаем память
	free(a_cop);
	free(tmp);
}

int correct_sort(float* a, int choice_2, int* swap)
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
	selection_sort(choice_2, a1, &swap);
	merge_sort(a2, 0, choice_2 - 1, &swap);
	quick_sort(0, choice_2 - 1, a3, &swap);
	radix_sort_float(a4, choice_2, &swap);
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
		printf("Введите число: ");
		scanf_s("%hd", &choice_1);
	} while (choice_1 < 1 || choice_1 > 2);

	short int choice_2 = 0;  // Переменная для выбора 2 режима

	// В зависимости от выбора пользователя даем ему разный выбор
	switch (choice_1)
	{
	case 1:
		printf("Супер! Вы выбрали сортировать свой массив, введите кол-во элементов массива: ");
		scanf_s("%hd", &choice_2);
		printf("\n");
		break;
	case 2:
		printf("Супер! Вы выбрали сортировать рандомный массив, введите кол-во элементов массива: ");
		scanf_s("%hd", &choice_2);
		printf("\n");
		break;
	}

	// Переменные для подсчета кол-ва перестановок в сортировках
	int swaps_selection = 0;
	int swaps_quick = 0;
	int swaps_merge = 0;
	int swaps_radix = 0;
	int swap1 = 0; // Для функции, которая проверяет 4 массива на правильность сортировки

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
		recording_elements_random(choice_2, a);
		for (int i = 0; i < choice_2; i++)
		{
			printf("%f", a[i]);
			printf(" ");
		}
		break;
	}

	short int choice_3; // Переменная для 3 выбора 

	// Используем do-while, чтобы избавиться от ошибки ввода пользователем, т.е пока пользователь не введет 1 или 2, будем просить ввести правильное число
	do {
		printf("Выберете, что будем делать с массивом:\n1. Просто сортируем массив.\n2. Сортируем массив разными сортировками и сравниваем время сортировки.\n(введите сортировки(у) позже).\n");
		printf("Введите число: ");
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

	float* a_sort5 = (float*)malloc(choice_2 * sizeof(float));
	for (int i = 0; i < choice_2; i++)
		a_sort5[i] = a[i];

	// Выбираем между режимами
	switch (choice_3)
	{
	case 1:
		do {
			printf("Выберете какой сортировкой сортировать массив:\n1. Выбором.\n2. Хоара.\n3. Слиянием.\n4. Поразрядная.\n");
			printf("Введите число: ");
			scanf_s("%hd", &choice_4);
		} while (choice_4 < 1 || choice_4 > 4);

		// В зависимости от выбора пользователя сортируем массив сортировками:
		switch (choice_4)
		{
		case 1: // Сортировка выбором
			if (correct_sort(a, choice_2, &swap1)) printf("Сравнили все сортировки, они равны -> массив отсортирован кооректно!\n");
			selection_sort(choice_2, a_sort1, &swaps_selection);
			printf("Отсортированный массив: ");
			for (int i = 0; i < choice_2; i++)
			{
				printf("%f", a_sort1[i]);
				printf(" ");
			}
			printf("\n");
			printf("Кол-во перестановок: ");
			printf("%d", swap1);
			break;
		case 2: // Сортировка Хоара
			if (correct_sort(a, choice_2, &swap1)) printf("Сравнили все сортировки, они равны -> массив отсортирован кооректно!\n");
			quick_sort(0, choice_2 - 1, a_sort2, &swaps_quick);
			printf("Отсортированный массив: ");
			for (int i = 0; i < choice_2; i++)
			{
				printf("%f", a_sort2[i]);
				printf(" ");
			}
			printf("\n");
			printf("Кол-во перестановок: ");
			printf("%d", swap1);
			break;
		case 3: // Сортировка слиянием
			if (correct_sort(a, choice_2, &swap1)) printf("Сравнили все сортировки, они равны -> массив отсортирован кооректно!\n");
			merge_sort(a_sort3, 0, choice_2 - 1, &swaps_merge);
			printf("Отсортированный массив: ");
			for (int i = 0; i < choice_2; i++)
			{
				printf("%f", a_sort3[i]);
				printf(" ");
			}
			printf("\n");
			printf("Кол-во перестановок: ");
			printf("%d", swap1);
			break;
		case 4: // Поразрядная сортировка
			if (correct_sort(a, choice_2, &swap1)) printf("Сравнили все сортировки, они равны -> массив отсортирован кооректно!\n");
			radix_sort_float(a_sort4, choice_2, &swaps_radix);
			printf("Отсортированный массив: ");
			for (int i = 0; i < choice_2; i++)
			{
				printf("%f", a_sort4[i]);
				printf(" ");
			}
			printf("\n");
			printf("Кол-во перестановок: ");
			printf("%d", swap1);
			break;
		}

		break;
	case 2:
		do {
			printf("Выберете кол-во различных сортировок: ");
			scanf_s("%hd", &choice_4);
		} while (choice_4 < 1 || choice_4 > 4);
		printf("Введите через Enter номера сортировок, время которых хотите узнать:\n1. Выбором.\n2. Хоара.\n3. Слиянием.\n4. Поразрядная.\n");
		for (int i = 0; i < choice_4; i++)
		{
			do {
				scanf_s("%d", &z);
			} while (z < 1 || z > 4);
			r[z]++;
		}
		if (correct_sort(a, choice_2, &swap1)) printf("Сравнили все сортировки, они равны -> массив отсортирован кооректно!\n");
		radix_sort_float(a_sort5, choice_2, &swaps_radix);
		printf("Отсортированный массив: ");
		for (int i = 0; i < choice_2; i++)
		{
			printf("%f", a_sort5[i]);
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
					selection_sort(choice_2, a_sort1, &swaps_selection);
					end = clock();
					cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
					printf("Время выполнения сортировки - Выбором: ");
					printf("%lf", cpu_time_used);
					printf("\n");
					printf("Кол-во перестановок: ");
					printf("%d", swaps_selection);
					printf("\n");
					break;
				case 2:
					start = clock();
					quick_sort(0, choice_2 - 1, a_sort2, &swaps_quick);
					end = clock();
					cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
					printf("Время выполнения сортировки - Хоара: ");
					printf("%lf", cpu_time_used);
					printf("\n");
					printf("Кол-во перестановок: ");
					printf("%d", swaps_quick);
					printf("\n");
					break;
				case 3:
					start = clock();
					merge_sort(a_sort3, 0, choice_2 - 1, &swaps_merge);
					end = clock();
					cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
					printf("Время выполнения сортировки - Слиянием: ");
					printf("%lf", cpu_time_used);
					printf("\n");
					printf("Кол-во перестановок: ");
					printf("%d", swaps_merge);
					printf("\n");
					break;
				case 4:
					start = clock();
					radix_sort_float(a_sort4, choice_2, &swaps_radix);
					end = clock();
					cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
					printf("Время выполнения сортировки - Поразрядная: ");
					printf("%lf", cpu_time_used);
					printf("\n");
					printf("Кол-во перестановок: ");
					printf("%d", swaps_radix);
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
	free(a_sort5);
	free(a);
	return 0;
}
