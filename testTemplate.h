#include <cstdio>
#include <cstdlib>
#include <vector>

#pragma once

class WallClockTimer {
public:
	struct timeval t1, t2;
	WallClockTimer() :
		t1(), t2() {
		gettimeofday(&t1, 0);
		t2 = t1;
	}
	void reset() {
		gettimeofday(&t1, 0);
		t2 = t1;
	}
	int elapsed() {
		return (t2.tv_sec * 1000 + t2.tv_usec / 1000) - (t1.tv_sec * 1000 + t1.tv_usec / 1000);
	}
	int split() {
		gettimeofday(&t2, 0);
		return elapsed();
	}
	int gettimeofday(struct timeval * tp, struct timezone * tzp)
	{
		// Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
		static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);

		SYSTEMTIME  system_time;
		FILETIME    file_time;
		uint64_t    time;

		GetSystemTime(&system_time);
		SystemTimeToFileTime(&system_time, &file_time);
		time = ((uint64_t)file_time.dwLowDateTime);
		time += ((uint64_t)file_time.dwHighDateTime) << 32;

		tp->tv_sec = (long)((time - EPOCH) / 10000000L);
		tp->tv_usec = (long)(system_time.wMilliseconds * 1000);
		return 0;
	}
};

template <class T>
void runtest() {
	size_t N = 10 * 1000 * 1000;
	int repeat = 20;
	WallClockTimer timer;
	const bool paranoid = false;
	cout << " processing word of size " << sizeof(T) << endl;
	for (unsigned int offset = 0; offset<sizeof(T); ++offset) {
		vector<T> bigarray(N + 2);
		cout << "offset = " << offset << endl;
		T * const begin = reinterpret_cast<T *> (reinterpret_cast<uintptr_t>(&bigarray[0]) + offset);
		//assert(offset + reinterpret_cast<uintptr_t>(&bigarray[0]) == reinterpret_cast<uintptr_t>(begin));
		T * const end = begin + N;
		if (paranoid) {} //assert(reinterpret_cast<uintptr_t>(end) < reinterpret_cast<uintptr_t>(&bigarray.back()));
		int sumt = 0;
		cout << " ignore this: ";
		for (int k = 0; k < repeat; ++k) {
			timer.reset();
			for (size_t i = 0; i <N; ++i) {
				begin[i] = static_cast<T>(i);
			}
			T val = 1;
			for (size_t i = 0; i <N; ++i) {
				val += begin[i] * val + 33;
			}
			int time = timer.split();
			sumt += time;
			cout << val;
		}
		cout << endl;
		cout << " average time for offset " << (offset % sizeof(T)) << " is " << sumt * 1.0 / repeat << endl;
	}

}
