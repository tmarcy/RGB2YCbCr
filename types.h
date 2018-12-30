#if !defined TYPES_H
#define TYPES_H

#include <vector>

typedef unsigned char byte;

template <typename T, int N>
struct vec {
	T _data[N];

	vec() {}

	vec(const T& el0) {
		_data[0] = el0;
	}

	vec(const T& el0, const T& el1) {
		_data[0] = el0;
		_data[1] = el1;
	}

	vec(const T& el0, const T& el1, const T& el2) {
		_data[0] = el0;
		_data[1] = el1;
		_data[2] = el2;
	}

	T& operator[](unsigned i) { return _data[i]; }
	const T& operator[](unsigned i) const { return _data[i]; }

	bool operator==(const vec& other) {
		for (int i = 0; i < N; ++i)
			if (_data[i] != other._data[i])
				return false;
		return true;
	}


};

typedef vec<byte, 3> vec3b;

#endif // TYPES_H