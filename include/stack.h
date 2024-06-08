#pragma once

#include <stdexcept>
#include <string>

template<class T>
class stack {
  T* elems;
  int tail;
  int _max_size;

  public:
  stack(int max_size) {
    this->elems = new T[max_size];
    this->tail = -1;
    this->_max_size = max_size;
  }

  void push(T new_elem) {
    if (this->size() == this->_max_size)
      throw std::runtime_error("pilha cheia");

    this->tail++;
    this->elems[this->tail] = new_elem;
  }
  
  T pop() {
    if (is_empty())
      throw std::runtime_error("pop em pilha vazia");

    return this->elems[this->tail--];
  }

  T peek() const {
    if (is_empty())
      throw std::runtime_error("peek em pilha vazia");

    return this->elems[this->tail];
  }

  T peek_below() const {
    if (is_empty() || size() == 1)
      throw std::runtime_error("peek below em pilha vazia");

    return this->elems[this->tail - 1];
  }

  int size() const {
    return this->tail + 1;
  }

  operator std::string () {
    std::string repr;
    for (int i=0; i<this->size(); i++) {
      repr += (std::string)*elems[i];
      repr += " ";
    }

    return repr;
  }

  bool is_empty() const {
    return this->size() == 0;
  }

  ~stack() {
    delete [] this->elems;
  }
};
