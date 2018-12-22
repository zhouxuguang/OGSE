#ifndef BASELIB_HEAPSORT_INCLUDE_H_HJDF546GH
#define BASELIB_HEAPSORT_INCLUDE_H_HJDF546GH

#include "PreCompile.h"

NS_BASELIB_BEGIN

/************************************************************************/
/* ∂—≈≈–Úƒ£∞Â µœ÷                                                       */
/************************************************************************/

//! Sinks an element into the heap.
template<class T>
inline void HeapSink(T*array, eUInt32 element, eUInt32 max)
{
	while ((element<<1) < max) // there is a left child
	{
		eUInt32 j = (element<<1);

		if (j+1 < max && array[j] < array[j+1])
			j = j+1; // take right child

		if (array[element] < array[j])
		{
			T t = array[j]; // swap elements
			array[j] = array[element];
			array[element] = t;
			element = j;
		}
		else
			return;
	}
}


//! Sorts an array with size 'size' using heapsort.
template<class T>
inline void HeapSort(T* array_, eUInt32 size)
{
	// for heapsink we pretent this is not c++, where
	// arrays start with index 0. So we decrease the array pointer,
	// the maximum always +2 and the element always +1

	T* virtualArray = array_ - 1;
	eUInt32 virtualSize = size + 2;
	eUInt32 i;

	// build heap

	for (i=((size-1)/2); i>=0; --i)
	{
		HeapSink(virtualArray, i+1, virtualSize-1);
	}

	// sort array, leave out the last element (0)
	for (i=size-1; i>0; --i)
	{
		T t = array_[0];
		array_[0] = array_[i];
		array_[i] = t;
		HeapSink(virtualArray, 1, i + 1);
	}
}

NS_BASELIB_END

#endif
