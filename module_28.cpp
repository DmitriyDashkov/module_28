#include <iostream>
#include <future>
#include <thread>

using namespace std;

int make_thread = 2;

void merge(int* arr, int l, int m, int r)

{
	int nl = m - l + 1;
	int nr = r - m;
	// создаем временные массивы
	int* left{ new int[nl] };
	int* right{ new int[nr] };
	// копируем данные во временные массивы
	for (int i = 0; i < nl; i++)
		left[i] = arr[l + i];
	for (int j = 0; j < nr; j++)
		right[j] = arr[m + 1 + j];
	int i = 0, j = 0;
	int k = l; // начало левой части
	while (i < nl && j < nr) {
		// записываем минимальные элементы обратно во входной массив
		if (left[i] <= right[j]) {
			arr[k] = left[i];
			i++;
		}
		else {
			arr[k] = right[j];
			j++;
		}
		k++;
	}

	// записываем оставшиеся элементы левой части
	while (i < nl) {
		arr[k] = left[i];
		i++;
		k++;
	}
	// записываем оставшиеся элементы правой части
	while (j < nr) {
		arr[k] = right[j];
		j++;
		k++;
	}
}

void mergeSort(int* arr, int l, int r)

{
	if (l >= r) return; //выход из рекурсии
	int m = (l + r - 1) / 2;
	//многопоточный запуск
	if (make_thread == 2) {
		future<void> t1 = async(std::launch::async, mergeSort, std::ref(arr), l, m);
		future<void> t2 = async(std::launch::async, mergeSort, std::ref(arr), m + 1, r);
		t1.wait();
		t2.wait();
		merge(arr, l, m, r);
	}
	//однопоточный запуск
	else if (make_thread == 1)
	{
		future<void> t3 = async(std::launch::async, mergeSort, std::ref(arr), l, r);
		t3.wait();
		merge(arr, l, m, r);
	}	
}

int main()

{
	srand(0);
	long arr_size = 50;
	int* arr = new int[arr_size];
	for (long i = 0; i < arr_size; i++) 
		arr[i] = rand() % 500000;
	time_t start, end;

	time(&start);

	//int arr[] = { 2, 1, 10, -5, 40, -20, 0, 4 };
	//int n = sizeof(arr) / sizeof(arr[0]);
	//for (int i = 0; i < n; i++) cout << arr[i] << " ";
	//cout << endl;
	mergeSort(arr, 0, arr_size - 1);
	
	time(&end);
	double seconds = difftime(end, start);
	cout << "Time = " << seconds << endl;

	for (int i = 0; i < arr_size; i++) cout << arr[i] << " ";

	return 0;
}