#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <type_traits>
#include <typeinfo>
#include <string>
#include <sstream>

template<typename T>
class TemplateArray {
private:
    std::vector<T> data;

public:
    TemplateArray() = default;
    
    explicit TemplateArray(size_t size) : data(size) {}
    
    TemplateArray(const std::vector<T>& values) : data(values) {}
    
    TemplateArray(std::initializer_list<T> init) : data(init) {}
    
    size_t size() const {
        return data.size();
    }
    
    T& operator[](size_t index) {
        return data[index];
    }
    
    const T& operator[](size_t index) const {
        return data[index];
    }
    
    T& at(size_t index) {
        if (index >= data.size()) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }
    
    const T& at(size_t index) const {
        if (index >= data.size()) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }
    
    void set(size_t index, const T& value) {
        if (index >= data.size()) {
            throw std::out_of_range("Index out of range");
        }
        data[index] = value;
    }
    
    template<typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, void>::type
    setWithValidation(size_t index, const U& value) {
        if (index >= data.size()) {
            throw std::out_of_range("Index out of range");
        }
        
        if constexpr (std::is_integral<U>::value) {
        }
        
        if constexpr (std::is_floating_point<U>::value) {
             if (std::isnan(value)) {
                throw std::invalid_argument("Cannot set NaN value");
            }
            if (std::isinf(value)) {
                throw std::invalid_argument("Cannot set infinite value");
            }
        }
        
        data[index] = value;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const TemplateArray<T>& arr) {
        os << "[";
        for (size_t i = 0; i < arr.data.size(); ++i) {
            os << arr.data[i];
            if (i != arr.data.size() - 1) {
                os << ", ";
            }
        }
        os << "]";
        return os;
    }
    
    template<typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, double>::type
    static euclideanDistance(const TemplateArray<U>& arr1, const TemplateArray<U>& arr2) {
            if (arr1.size() != arr2.size()) {
            throw std::invalid_argument("Arrays must have the same size for distance calculation");
        }
        
        double sum = 0.0;
        for (size_t i = 0; i < arr1.size(); ++i) {
            double diff = static_cast<double>(arr1[i]) - static_cast<double>(arr2[i]);
            sum += diff * diff;
        }
        
        return std::sqrt(sum);
    }
    
    template<typename U = T>
    typename std::enable_if<!std::is_arithmetic<U>::value, double>::type
    static euclideanDistance(const TemplateArray<U>& arr1, const TemplateArray<U>& arr2) {
        throw std::bad_typeid();
    }
    
    const std::vector<T>& getData() const {
        return data;
    }
};

int main() {
    try {
        std::cout << "=== Тестирование с целыми числами ===" << std::endl;
        TemplateArray<int> intArray1 = {1, 2, 3, 4, 5};
        TemplateArray<int> intArray2 = {2, 3, 4, 5, 6};
        
        std::cout << "Массив 1: " << intArray1 << std::endl;
        std::cout << "Массив 2: " << intArray2 << std::endl;
        
        double distanceInt = TemplateArray<int>::euclideanDistance(intArray1, intArray2);
        std::cout << "Евклидово расстояние: " << distanceInt << std::endl;
        
        std::cout << "\nУстановка значения с проверкой:" << std::endl;
        intArray1.setWithValidation(2, 10);
        std::cout << "После установки: " << intArray1 << std::endl;
        
        std::cout << "\n=== Тестирование с числами с плавающей точкой ===" << std::endl;
        TemplateArray<double> doubleArray1 = {1.5, 2.5, 3.5};
        TemplateArray<double> doubleArray2 = {2.5, 3.5, 4.5};
        
        std::cout << "Массив 1: " << doubleArray1 << std::endl;
        std::cout << "Массив 2: " << doubleArray2 << std::endl;
        
        double distanceDouble = TemplateArray<double>::euclideanDistance(doubleArray1, doubleArray2);
        std::cout << "Евклидово расстояние: " << distanceDouble << std::endl;
        
        doubleArray1.setWithValidation(1, 3.14159);
        std::cout << "После установки: " << doubleArray1 << std::endl;
        
        std::cout << "\n=== Тестирование обработки ошибок ===" << std::endl;
        
        try {
            TemplateArray<int> smallArray = {1, 2, 3};
            TemplateArray<int> largeArray = {1, 2, 3, 4, 5};
            TemplateArray<int>::euclideanDistance(smallArray, largeArray);
        } catch (const std::invalid_argument& e) {
            std::cout << "Поймано исключение (разные размеры): " << e.what() << std::endl;
        }
        
        try {
            TemplateArray<std::string> strArray1 = {"hello", "world"};
            TemplateArray<std::string> strArray2 = {"foo", "bar"};
            TemplateArray<std::string>::euclideanDistance(strArray1, strArray2);
        } catch (const std::bad_typeid& e) {
            std::cout << "Поймано исключение (нечисловой тип): " << e.what() << std::endl;
        }
        
        try {
            doubleArray1.setWithValidation(0, std::numeric_limits<double>::quiet_NaN());
        } catch (const std::invalid_argument& e) {
            std::cout << "Поймано исключение (невалидное значение): " << e.what() << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}