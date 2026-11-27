#pragma once

template <typename T>
class Set {
	T* arr = nullptr;
	int capacity = 10;
	int count = 0;

public:
	Set(int cap = 10) : capacity(cap) {
		arr = new T[capacity];
		for (int i = 0; i < capacity; ++i) {
			arr[i] = T(-1);
		}
	}

	~Set() {
		delete[] arr;
	}



	// Insert unique value in descending order (keeps only top N) O(n)
	bool insert(const T& val) {
		// 1. Ignore duplicates (set behavior)
		if (contains(val)) return false;

		// 2. If full and new value is smaller than the lowest top value, skip
		if (full() && val <= arr[count - 1]) return false;

		// 3. Find sorted insertion position (largest at index 0)
		int pos = 0;
		while (pos < count && arr[pos] > val) {
			++pos;
		}

		// 4. Shift smaller elements down
		int limit = full() ? capacity - 1 : count;
		for (int i = limit; i > pos; --i) {
			arr[i] = arr[i - 1];
		}

		arr[pos] = val;
		if (!full()) ++count;
		return true;
	}

	// Check if value exists O(n)
	bool contains(const T& val) const {
		for (int i = 0; i < count; ++i) {
			if (arr[i] == val) return true;
		}
		return false;
	}

	// Remove a specific value O(n)
	bool remove(const T& val) {
		int pos = -1;
		for (int i = 0; i < count; ++i) {
			if (arr[i] == val) {
				pos = i;
				break;
			}
		}
		if (pos == -1) return false;

		for (int i = pos; i < count - 1; ++i) {
			arr[i] = arr[i + 1];
		}
		arr[count - 1] = T(-1);
		--count;
		return true;
	}


	// Iterators for range-based for loops (visits max to min)
	const T* begin() const { return arr; }
	const T* end() const { return arr + count; }
	T* begin() { return arr; }
	T* end() { return arr + count; }

	int size() const { return count; }
	int max_size() const { return capacity; }
	bool empty() const { return count == 0; }
	bool full() const { return count == capacity; }

	void clear() {
		for (int i = 0; i < capacity; ++i) {
			arr[i] = T(-1);
		}
		count = 0;
	}
};
