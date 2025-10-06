#include <iostream>
#include <stdexcept>

class DynamicArray {
private:
    int* data;
    size_t size;

public:
    // Конструктор, получающий размер массива
    DynamicArray(size_t arraySize = 0) : size(arraySize) {
        if (size > 0) {
            data = new int[size](); // Инициализация нулями
        } else {
            data = nullptr;
        }
    }

    // Конструктор копирования (Задание 2)
    DynamicArray(const DynamicArray& other) : size(other.size) {
        if (size > 0) {
            data = new int[size];
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        } else {
            data = nullptr;
        }
    }

    // Деструктор
    ~DynamicArray() {
        delete[] data;
    }

    // Функция вывода всех значений массива
    void print() const {
        std::cout << "Массив [размер: " << size << "]: ";
        if (size == 0) {
            std::cout << "пустой";
        } else {
            for (size_t i = 0; i < size; ++i) {
                std::cout << data[i];
                if (i < size - 1) {
                    std::cout << ", ";
                }
            }
        }
        std::cout << std::endl;
    }

    // Сеттер (Задание 1)
    void setValue(size_t index, int value) {
        if (index >= size) {
            throw std::out_of_range("Индекс выходит за границы массива");
        }
        if (value < -100 || value > 100) {
            throw std::invalid_argument("Значение должно быть в диапазоне от -100 до 100");
        }
        data[index] = value;
    }

    // Геттер (Задание 1)
    int getValue(size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Индекс выходит за границы массива");
        }
        return data[index];
    }

    // Получение размера массива
    size_t getSize() const {
        return size;
    }

    // Добавление значения в конец массива (Задание 3)
    void pushBack(int value) {
        if (value < -100 || value > 100) {
            throw std::invalid_argument("Значение должно быть в диапазоне от -100 до 100");
        }

        // Создаем новый массив на 1 элемент больше
        int* newData = new int[size + 1];
        
        // Копируем старые данные
        for (size_t i = 0; i < size; ++i) {
            newData[i] = data[i];
        }
        
        // Добавляем новый элемент
        newData[size] = value;
        
        // Удаляем старый массив и обновляем указатель и размер
        delete[] data;
        data = newData;
        ++size;
    }

    // Операция сложения массивов (Задание 4)
    DynamicArray add(const DynamicArray& other) const {
        size_t maxSize = (size > other.size) ? size : other.size;
        DynamicArray result(maxSize);

        for (size_t i = 0; i < maxSize; ++i) {
            int val1 = (i < size) ? data[i] : 0;
            int val2 = (i < other.size) ? other.data[i] : 0;
            int sum = val1 + val2;
            
            // Проверяем, чтобы результат не выходил за допустимые пределы
            if (sum < -100) sum = -100;
            if (sum > 100) sum = 100;
            
            result.data[i] = sum;
        }

        return result;
    }

    // Операция вычитания массивов (Задание 4)
    DynamicArray subtract(const DynamicArray& other) const {
        size_t maxSize = (size > other.size) ? size : other.size;
        DynamicArray result(maxSize);

        for (size_t i = 0; i < maxSize; ++i) {
            int val1 = (i < size) ? data[i] : 0;
            int val2 = (i < other.size) ? other.data[i] : 0;
            int diff = val1 - val2;
            
            // Проверяем, чтобы результат не выходил за допустимые пределы
            if (diff < -100) diff = -100;
            if (diff > 100) diff = 100;
            
            result.data[i] = diff;
        }

        return result;
    }

    // Оператор присваивания (для корректной работы с копированием)
    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            if (size > 0) {
                data = new int[size];
                for (size_t i = 0; i < size; ++i) {
                    data[i] = other.data[i];
                }
            } else {
                data = nullptr;
            }
        }
        return *this;
    }
};

// Демонстрация работы класса
int main() {
    try {
        std::cout << "=== Задание 1: Базовый функционал ===" << std::endl;
        DynamicArray arr1(3);
        arr1.setValue(0, 10);
        arr1.setValue(1, -50);
        arr1.setValue(2, 100);
        arr1.print();

        std::cout << "\nЭлемент с индексом 1: " << arr1.getValue(1) << std::endl;

        std::cout << "\n=== Задание 2: Конструктор копирования ===" << std::endl;
        DynamicArray arr2 = arr1; // Используется конструктор копирования
        arr2.print();

        std::cout << "\n=== Задание 3: Добавление в конец ===" << std::endl;
        arr1.pushBack(25);
        arr1.pushBack(-75);
        arr1.print();

        std::cout << "\n=== Задание 4: Операции с массивами ===" << std::endl;
        DynamicArray arrA(3);
        arrA.setValue(0, 10);
        arrA.setValue(1, 20);
        arrA.setValue(2, 30);

        DynamicArray arrB(2);
        arrB.setValue(0, 5);
        arrB.setValue(1, 15);

        std::cout << "Массив A: ";
        arrA.print();
        std::cout << "Массив B: ";
        arrB.print();

        DynamicArray sumResult = arrA.add(arrB);
        std::cout << "A + B: ";
        sumResult.print();

        DynamicArray diffResult = arrA.subtract(arrB);
        std::cout << "A - B: ";
        diffResult.print();

        // Тест с разными размерами массивов
        std::cout << "\n=== Тест с разными размерами ===" << std::endl;
        DynamicArray arrC(1);
        arrC.setValue(0, 50);

        DynamicArray arrD(4);
        arrD.setValue(0, 10);
        arrD.setValue(1, 20);
        arrD.setValue(2, 30);
        arrD.setValue(3, 40);

        std::cout << "Массив C: ";
        arrC.print();
        std::cout << "Массив D: ";
        arrD.print();

        DynamicArray sumCD = arrC.add(arrD);
        std::cout << "C + D: ";
        sumCD.print();

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}