#include <stdexcept>

template <typename T>
class Vector {
public:
    // Создал псевдонимы для удобства
    typedef T* iterator;
    typedef const T* const_iterator;

    iterator begin() { return data; }
    iterator end() { return data + length; }
    const_iterator begin() const { return data; }
    const_iterator end() const { return data + length; }

    Vector() {
        capacity = 10;
        length = 0;
        data = new T[capacity];
    }

    Vector(const Vector& other) {
        capacity = other.capacity;
        length = other.length;
        data = new T[capacity];
        for (int i = 0; i < length; i++) {
            data[i] = other.data[i];
        }
    }

    ~Vector() {
        delete[] data;
    }

    void push_back(const T& value) {
        if (length >= capacity) {
            capacity *= 2;
            T* newData = new T[capacity];
            for (int i = 0; i < length; i++) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
        }
        data[length++] = value;
    }

    void remove(int index) {
        if (index < 0 || index >= length) return;
        for (int i = index; i < length - 1; ++i) {
            data[i] = data[i + 1];
        }
        length--;
    }

    void clear() {
        length = 0;
    }

    int size() const {
        return length;
    }

    T& operator[] (int index) {
        if (index < 0 || index >= length) throw std::out_of_range("Индекс вне диапазона");
        return data[index];
    }

    const T& operator[] (int index) const {
        if (index < 0 || index >= length) throw std::out_of_range("Индекс вне диапазона");
        return data[index];
    }

private:
    T* data;
    int capacity; // ёмкость
    int length;
};
