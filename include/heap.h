#pragma once
#include "tree.h"
#include "list.h"
#include <iostream>
#include <stdexcept>
#include <vector>

class MinHeap {
private:
	Vector<node*> heap;
	
  void heapify_up(int index) {
    int parent = (index - 1) / 2;
    while (index > 0 && heap[index] < heap[parent]) {
	      std::swap(heap[index], heap[parent]);
	      index = parent;
	      parent = (index - 1) / 2;
	    }
  }

  void heapify_down(int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int smallest = index;
    int size = heap.size();

    if (left < size && heap[left]->frequency < heap[smallest]->frequency)
      smallest = left;

	  if (right < size && heap[right]->frequency < heap[smallest]->frequency)
	    smallest = right;

	  if (smallest != index) {
	    std::swap(heap[index], heap[smallest]);
	    heapify_down(smallest);
	  }
  }

  public:
  bool empty() {
      return heap.empty();
  }

  void push(node *value) {
    heap.push_back(value);
    heapify_up(heap.size() - 1);
  }

  void pop() {
      if (empty()) {
        throw std::runtime_error("Top em heap vazia");
      }

      heap[0] = heap.back();
      heap.pop_back();
      heapify_down(0);
  }

  node *top() {
    if (empty()) {
      throw std::runtime_error("Top em heap vazia");
    }

    return heap[0];
  }
};
