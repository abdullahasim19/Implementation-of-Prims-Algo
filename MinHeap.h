#pragma once
#include <iostream>
#include <vector>
#include <map>
using namespace std;
template <class T>
class MinHeap
{
	vector <T>heap;
	int left(int i)
	{
		return 2 * i;
	}
	int right(int i)
	{
		return 2 * i + 1;
	}
	int parent(int i)
	{
		return i / 2;
	}
	void min_heapify(int i, int n)
	{
		int min = i;
		int l = left(i);
		int r = right(i);
		if (l <= n && heap[l] < heap[min])
		{
			min = l;
		}
		if (r <= n && heap[r] < heap[min])
		{
			min = r;
		}
		if (min != i)
		{
			swap(heap[i], heap[min]);
			min_heapify(min, n);
		}
	}
	void perculateup(int i)
	{
		while (i > 0 && heap[parent(i)] > heap[i]&& parent(i)>0)
		{
			swap(heap[i], heap[parent(i)]);
			i = parent(i);
		}
	}
public:
	MinHeap()
	{
		heap.resize(1);
	}
	void build_min_heap(vector<T> data)
	{
		if (!empty())
		{
			heap.clear();
			heap.resize(1);
		}
		heap = data;
		int n = heap.size() - 1;
		for (int i = n / 2; i > 0; i--)
			min_heapify(i, n);
	}
	void build_min_heap(T* data,int n)
	{
		if (!empty())
		{
			heap.clear();
			heap.resize(1);
		}
		for (int j = 1; j <= n; j++)
			heap.push_back(data[j]);
	
		for (int i = n / 2; i > 0; i--)
			min_heapify(i, n);
	}
	void build_min_heap(map <int, T*>m)
	{
		if (!empty())
		{
			heap.clear();
			heap.resize(1);
		}
		typename map<int, T*>::iterator it;
		T *d;
		int j;
		for (j = 1, it = m.begin(); it != m.end(); j++, ++it)
		{
			d = it->second;
			heap.push_back(*d);
		}
		int n = m.size();
		for (int i = n / 2; i > 0; i--)
			min_heapify(i, n);
	}
	void insert(T d)
	{
		heap.push_back(d);
		perculateup(heap.size() - 1);
	}
	T getMin()
	{
		return heap[1];
	}
	void removeMin()
	{
		heap[1] = heap[heap.size() - 1];
		heap.pop_back();
		min_heapify(1, heap.size() - 1);
	}
	void print() //prints heap in tree form
	{
		cout << heap[1] << endl;
		int p = 2;
		for (int i = 2; i < heap.size(); i++)
		{
			for (int j = 0; j < p && i < heap.size(); j++)
			{
				cout << heap[i] << " ";
				i++;

			}
			i--;
			cout << endl;
			p = p * 2;
		}
	}
	int size()
	{
		return heap.size() - 1;
	}
	void decreasekey(T d, int i)
	{
		heap[i] = d;
		perculateup(i);
	}
	bool empty()
	{
		return heap.size() == 1;
	}
	int search(T d)
	{
		for (int i = 1; i < heap.size(); i++)
		{
			if (heap[i] == d)
				return i;
		}
		return -1;
	}
};

