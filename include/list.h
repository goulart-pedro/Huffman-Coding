#include <iostream>
#include <cassert>

template <typename T>
class Vector {
private:
    T* elements;    // Armazena os elementos do vetor
    size_t capacity;    // Capacidade atual do vetor
    size_t length;    // Número de elementos atualmente no vetor

public:
    // Construtor padrão
    Vector() : capacity(1), length(0) {
        elements = new T[capacity];
    }

    // Destrutor
    ~Vector() {
        delete[] elements;
    }

    // Adiciona um elemento ao final do vetor
    void push_back(T& element) {
        if (length == capacity) {
            // Dobrar a capacidade do vetor se estiver cheio
            capacity *= 2;
            T* newElements = new T[capacity];
            for (size_t i = 0; i < length; ++i) {
                newElements[i] = elements[i];
            }
            delete[] elements;
            elements = newElements;
        }

        elements[length++] = element;
    }

    // Remove o último elemento do vetor
    void pop_back() {
        assert(length > 0 && "O vetor está vazio!");
        --length;
    }

    // Retorna uma referência para o último elemento do vetor
    T& back() {
        assert(length > 0 && "O vetor está vazio!");
        return elements[length - 1];
    }

    T& operator[](size_t index) {
        assert(index < length && "Índice fora dos limites!");
        return elements[index];
    }


    // Verifica se o vetor está vazio
    bool empty() {
        return length == 0;
    }

    // Retorna o número de elementos no vetor
    size_t size() {
        return length;
    }
};