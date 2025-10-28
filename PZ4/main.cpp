#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <vector>

class DynamicArray {
private:
    int* data;
    size_t size;

public:
    DynamicArray(size_t arraySize = 0) : size(arraySize) {
        if (size > 0) {
            data = new int[size]();
        } else {
            data = nullptr;
        }
    }

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

    ~DynamicArray() {
        delete[] data;
    }

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

    void setValue(size_t index, int value) {
        if (index >= size) {
            throw std::out_of_range("Индекс выходит за границы массива");
        }
        if (value < -100 || value > 100) {
            throw std::invalid_argument("Значение должно быть в диапазоне от -100 до 100");
        }
        data[index] = value;
    }

    int getValue(size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Индекс выходит за границы массива");
        }
        return data[index];
    }

    size_t getSize() const {
        return size;
    }

    void pushBack(int value) {
        if (value < -100 || value > 100) {
            throw std::invalid_argument("Значение должно быть в диапазоне от -100 до 100");
        }

        int* newData = new int[size + 1];
        
        for (size_t i = 0; i < size; ++i) {
            newData[i] = data[i];
        }
        
        newData[size] = value;
        
        delete[] data;
        data = newData;
        ++size;
    }

    DynamicArray add(const DynamicArray& other) const {
        size_t maxSize = (size > other.size) ? size : other.size;
        DynamicArray result(maxSize);

        for (size_t i = 0; i < maxSize; ++i) {
            int val1 = (i < size) ? data[i] : 0;
            int val2 = (i < other.size) ? other.data[i] : 0;
            int sum = val1 + val2;
            
            if (sum < -100) sum = -100;
            if (sum > 100) sum = 100;
            
            result.data[i] = sum;
        }

        return result;
    }

    DynamicArray subtract(const DynamicArray& other) const {
        size_t maxSize = (size > other.size) ? size : other.size;
        DynamicArray result(maxSize);

        for (size_t i = 0; i < maxSize; ++i) {
            int val1 = (i < size) ? data[i] : 0;
            int val2 = (i < other.size) ? other.data[i] : 0;
            int diff = val1 - val2;
            
            if (diff < -100) diff = -100;
            if (diff > 100) diff = 100;
            
            result.data[i] = diff;
        }

        return result;
    }

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

protected:
    int* getData() const {
        return data;
    }
};

class ExtendedArray : public DynamicArray {
public:
    ExtendedArray(size_t arraySize = 0) : DynamicArray(arraySize) {}
    
    ExtendedArray(const DynamicArray& other) : DynamicArray(other) {}
    
    double calculateAverage() const {
        size_t currentSize = getSize();
        if (currentSize == 0) {
            throw std::runtime_error("Массив пустой");
        }
        
        int sum = 0;
        for (size_t i = 0; i < currentSize; ++i) {
            sum += getValue(i);
        }
        
        return static_cast<double>(sum) / currentSize;
    }
    
    double calculateMedian() const {
        size_t currentSize = getSize();
        if (currentSize == 0) {
            throw std::runtime_error("Массив пустой");
        }
        
        std::vector<int> sortedData;
        for (size_t i = 0; i < currentSize; ++i) {
            sortedData.push_back(getValue(i));
        }
        
        std::sort(sortedData.begin(), sortedData.end());
        
        if (currentSize % 2 == 0) {
            int mid1 = sortedData[currentSize / 2 - 1];
            int mid2 = sortedData[currentSize / 2];
            return (mid1 + mid2) / 2.0;
        } else {
            return sortedData[currentSize / 2];
        }
    }
    
    int findMin() const {
        size_t currentSize = getSize();
        if (currentSize == 0) {
            throw std::runtime_error("Массив пустой");
        }
        
        int minVal = getValue(0);
        for (size_t i = 1; i < currentSize; ++i) {
            if (getValue(i) < minVal) {
                minVal = getValue(i);
            }
        }
        return minVal;
    }
    
    int findMax() const {
        size_t currentSize = getSize();
        if (currentSize == 0) {
            throw std::runtime_error("Массив пустой");
        }
        
        int maxVal = getValue(0);
        for (size_t i = 1; i < currentSize; ++i) {
            if (getValue(i) > maxVal) {
                maxVal = getValue(i);
            }
        }
        return maxVal;
    }
};

int main() {
    try {
        ExtendedArray arr;
        arr.pushBack(10);
        arr.pushBack(-50);
        arr.pushBack(100);
        arr.pushBack(25);
        arr.pushBack(-75);
        
        arr.print();
        
        std::cout << "Среднее: " << arr.calculateAverage() << std::endl;
        std::cout << "Медиана: " << arr.calculateMedian() << std::endl;
        std::cout << "Минимум: " << arr.findMin() << std::endl;
        std::cout << "Максимум: " << arr.findMax() << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}