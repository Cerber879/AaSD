// 4. Имеется массив целых чисел. Отсортировать его  на  месте
// методом шейкер - сортировки. Удалить повторное вхождение  чисел,
// не используя дополнительной памяти (7).

// VS 2022  Афанасьев Никита Андреевич  ПС 23

#include <iostream>

const int MAX_SIZE = 100;

void ShuffleSort(int arr[], int size) {
	bool isSorted = false;
	int left = 0;
	int right = size - 1;

	while (!isSorted) {
		isSorted = true;
		for (int i = left; i < right; i++) {
			if (arr[i] > arr[i + 1]) {
				int temp = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = temp;
				isSorted = false;
			}
		}
		right--;

		for (int i = right; i > left; i--) {
			if (arr[i-1] > arr[i]) {
				int temp = arr[i-1];
				arr[i-1] = arr[i];
				arr[i] = temp;
				isSorted = false;
			}
		}
		left++;
	}
}

void addElement(int arr[], int& size, int element) {
	if (size >= MAX_SIZE) {
		std::cout << "Массив заполнен. Не удалось добавить элемент." << std::endl;
		return;
	}

	arr[size] = element;
	size++;

	std::cout << "Элемент добавлен" << std::endl;
}

void removeElement(int arr[], int& size, int element) {
	int foundIndex = -1;
	for (int i = 0; i < size; i++) {
		if (arr[i] == element) {
			foundIndex = i;
			break;
		}
	}

	if (foundIndex != -1) {
		for (int i = foundIndex; i < size - 1; i++) {
			arr[i] = arr[i + 1];
		}
		size--;
		std::cout << "Элемент удален" << std::endl;
	}
	else {
		std::cout << "Элемент не найден" << std::endl;
	}
}

void DeleteDuplicates(int arr[], int& size) {
	int curr = 0;
	for (int i = 1; i < size; i++) {
		if (arr[i] != arr[curr]) {
			curr++;
			arr[curr] = arr[i];
		}
	}
	size = curr + 1;
}

void printArray(int arr[], int size) {
	for (int i = 0; i < size; i++) {
		std::cout << arr[i];
		std::cout << " ";
	}
	std::cout << std::endl;
}

void clearArray(int arr[], int& size) {
    size = 0;
}

void info() {
	std::cout << "Программа отсортировывает массив методом шейкер-сортировки" << std::endl;
	std::cout << "1 - Добавление элемента в массив" << std::endl;
	std::cout << "2 - Удаление элемента из массива" << std::endl;
	std::cout << "3 - Вывод массива" << std::endl;
	std::cout << "4 - Шейкер-отсортировка массива" << std::endl;
	std::cout << "5 - Выход из программы" << std::endl;
}

bool checkCorrectInput() {
	if (std::cin.fail()) {
		std::cout << "Ошибка ввода! Пожалуйста, введите корректное значение." << std::endl;
		std::cin.clear(); 
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return false;
	}
	else
		return true;
}

int main() {
	setlocale(LC_ALL, "RU");
	info();

	int arr[MAX_SIZE];
	int size = 0;

    bool done = false;
    while (!done) {

        int choice;
        std::cout << "Введите (1/2/3/4/5): ";
        std::cin >> choice;

		if (!checkCorrectInput())
			continue;

        if (choice != 5) std::cout << "-------------------------------" << std::endl;
        switch (choice) {
        case 1: {
			int element;
			std::cout << "Введите элемент добавления: " << std::endl;
			std::cin >> element;
			addElement(arr, size, element);
            break;
        }
		case 2: {
			int element;
			std::cout << "Введите элемент удаления: " << std::endl;
			std::cin >> element;
			removeElement(arr, size, element);
			break;
		case 3: {
			if (size > 0) {
				printArray(arr, size);
			}
			else 
				std::cout << "Массив пустой." << std::endl;
			break;
		}
		}
        case 4: {
			if (size > 0) {
				ShuffleSort(arr, size);
				DeleteDuplicates(arr, size);

				std::cout << "Отсортированный массив: " << std::endl;
				printArray(arr, size);
			}
			else 
				std::cout << "Массив пустой." << std::endl;
            break;
        }
        case 5: {
            done = true;
            break;
        }
        default:
            std::cout << "Некорректный выбор. Попробуйте ещё раз." << std::endl;
            break;
        }
        if (choice != 5) std::cout << "-------------------------------" << std::endl;
    }

	return 0;
}