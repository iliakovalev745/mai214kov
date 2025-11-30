#include <iostream>
#include <stdexcept>

class DynamicArray {
private:
    int* data;
    size_t size;

    bool validValue(int value) const {
        return (value >= -100 && value <= 100);
    }

    bool validIndex(size_t index) const {
        return (index < size);
    }

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
        std::cout << "Array [size: " << size << "]: ";
        if (size == 0) {
            std::cout << "empty";
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
        if (!validIndex(index)) {
            throw std::out_of_range("Index " + std::to_string(index) + 
                                   " out of range (size: " + 
                                   std::to_string(size) + ")");
        }
        if (!validValue(value)) {
            throw std::invalid_argument("Value " + std::to_string(value) + 
                                       " must be between -100 and 100");
        }
        data[index] = value;
    }

    int getValue(size_t index) const {
        if (!validIndex(index)) {
            throw std::out_of_range("Index " + std::to_string(index) + 
                                   " out of range (size: " + 
                                   std::to_string(size) + ")");
        }
        return data[index];
    }

    size_t getSize() const {
        return size;
    }

    void pushBack(int value) {
        if (!validValue(value)) {
            throw std::invalid_argument("Value " + std::to_string(value) + 
                                       " must be between -100 and 100");
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

    int& operator[](size_t index) {
        if (!validIndex(index)) {
            throw std::out_of_range("Index " + std::to_string(index) + 
                                   " out of range (size: " + 
                                   std::to_string(size) + ")");
        }
        return data[index];
    }

    const int& operator[](size_t index) const {
        if (!validIndex(index)) {
            throw std::out_of_range("Index " + std::to_string(index) + 
                                   " out of range (size: " + 
                                   std::to_string(size) + ")");
        }
        return data[index];
    }
};

int main() {
    std::cout << "=== DynamicArray Demo ===" << std::endl;

    try {
        std::cout << "\n1. Create array and set values:" << std::endl;
        DynamicArray arr1(3);
        arr1.setValue(0, 10);
        arr1.setValue(1, -50);
        arr1.setValue(2, 100);
        arr1.print();

        std::cout << "\n2. Access elements:" << std::endl;
        std::cout << "Element at index 1: " << arr1.getValue(1) << std::endl;

        std::cout << "\n3. Test operator []:" << std::endl;
        std::cout << "arr1[0] = " << arr1[0] << std::endl;
        arr1[1] = 75;
        std::cout << "After arr1[1] = 75: ";
        arr1.print();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    try {
        std::cout << "\n4. Test invalid index:" << std::endl;
        DynamicArray arr2(2);
        arr2.setValue(5, 10);
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    try {
        std::cout << "\n5. Test invalid value:" << std::endl;
        DynamicArray arr3(2);
        arr3.setValue(0, 150);
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    try {
        std::cout << "\n6. Test array operations:" << std::endl;
        DynamicArray arrA(3);
        arrA.setValue(0, 10);
        arrA.setValue(1, 20);
        arrA.setValue(2, 30);

        DynamicArray arrB(2);
        arrB.setValue(0, 5);
        arrB.setValue(1, 15);

        std::cout << "Array A: ";
        arrA.print();
        std::cout << "Array B: ";
        arrB.print();

        DynamicArray sumResult = arrA.add(arrB);
        std::cout << "A + B: ";
        sumResult.print();

        DynamicArray diffResult = arrA.subtract(arrB);
        std::cout << "A - B: ";
        diffResult.print();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    try {
        std::cout << "\n7. Test boundary values:" << std::endl;
        DynamicArray arr5(2);
        arr5.setValue(0, -100);
        arr5.setValue(1, 100);
        arr5.print();

        std::cout << "Try to set value -101..." << std::endl;
        arr5.setValue(0, -101);

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    std::cout << "\n=== Program finished ===" << std::endl;
    return 0;
}