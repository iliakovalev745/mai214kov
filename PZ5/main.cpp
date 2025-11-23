#include <iostream>
#include <stdexcept>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>

class DynamicArray {
protected:
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

    virtual ~DynamicArray() {
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
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        if (value < -100 || value > 100) {
            throw std::invalid_argument("Value must be between -100 and 100");
        }
        data[index] = value;
    }

    int getValue(size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    size_t getSize() const {
        return size;
    }

    void pushBack(int value) {
        if (value < -100 || value > 100) {
            throw std::invalid_argument("Value must be between -100 and 100");
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

    virtual void saveToFile() const = 0;

protected:
    std::string getCurrentDateTime() const {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d_%H-%M-%S");
        return ss.str();
    }
};

class ArrTxt : public DynamicArray {
public:
    ArrTxt(size_t arraySize = 0) : DynamicArray(arraySize) {}
    ArrTxt(const DynamicArray& other) : DynamicArray(other) {}

    ArrTxt add(const ArrTxt& other) const {
        size_t maxSize = (size > other.size) ? size : other.size;
        ArrTxt result(maxSize);

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

    ArrTxt subtract(const ArrTxt& other) const {
        size_t maxSize = (size > other.size) ? size : other.size;
        ArrTxt result(maxSize);

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

    void saveToFile() const override {
        std::string filename = getCurrentDateTime() + ".txt";
        std::ofstream file(filename);
        
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }
        
        file << "Dynamic Array (TXT)\n";
        file << "Size: " << size << "\n";
        file << "Elements: ";
        
        if (size == 0) {
            file << "empty";
        } else {
            for (size_t i = 0; i < size; ++i) {
                file << data[i];
                if (i < size - 1) {
                    file << ", ";
                }
            }
        }
        file << "\n";
        
        file.close();
        std::cout << "Saved to: " << filename << std::endl;
    }

    ArrTxt& operator=(const ArrTxt& other) {
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

class ArrCSV : public DynamicArray {
public:
    ArrCSV(size_t arraySize = 0) : DynamicArray(arraySize) {}
    ArrCSV(const DynamicArray& other) : DynamicArray(other) {}

    ArrCSV add(const ArrCSV& other) const {
        size_t maxSize = (size > other.size) ? size : other.size;
        ArrCSV result(maxSize);

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

    ArrCSV subtract(const ArrCSV& other) const {
        size_t maxSize = (size > other.size) ? size : other.size;
        ArrCSV result(maxSize);

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

    void saveToFile() const override {
        std::string filename = getCurrentDateTime() + ".csv";
        std::ofstream file(filename);
        
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }
        
        file << "Index,Value\n";
        
        for (size_t i = 0; i < size; ++i) {
            file << i << "," << data[i] << "\n";
        }
        
        file.close();
        std::cout << "Saved to: " << filename << std::endl;
    }

    ArrCSV& operator=(const ArrCSV& other) {
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

int main() {
    try {
        std::cout << "=== Creating arrays ===" << std::endl;
        
        ArrTxt txtArray(4);
        txtArray.setValue(0, 10);
        txtArray.setValue(1, -50);
        txtArray.setValue(2, 75);
        txtArray.setValue(3, 100);
        txtArray.print();
        txtArray.saveToFile();

        std::cout << "\n";

        ArrCSV csvArray(3);
        csvArray.setValue(0, 5);
        csvArray.setValue(1, -25);
        csvArray.setValue(2, 50);
        csvArray.print();
        csvArray.saveToFile();

        std::cout << "\n=== Working with base class pointers ===" << std::endl;
        
        DynamicArray* arrays[2];
        arrays[0] = new ArrTxt(2);
        arrays[0]->setValue(0, 30);
        arrays[0]->setValue(1, -10);
        
        arrays[1] = new ArrCSV(3);
        arrays[1]->setValue(0, 15);
        arrays[1]->setValue(1, 45);
        arrays[1]->setValue(2, -5);

        for (int i = 0; i < 2; i++) {
            arrays[i]->print();
            arrays[i]->saveToFile();
            delete arrays[i];
        }

        std::cout << "\n=== Testing pushBack ===" << std::endl;
        ArrTxt dynamicArray;
        dynamicArray.pushBack(10);
        dynamicArray.pushBack(-20);
        dynamicArray.pushBack(30);
        dynamicArray.print();
        dynamicArray.saveToFile();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}