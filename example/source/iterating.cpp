/*
 * iterating.cpp
 *
 *  Created on: 13.06.2019
 *      Author: andre
 */

#include "multiplatform.h"
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <numeric>
#include <stdlib.h>
#include <stdio.h>
#include "measure.h"
#include <iomanip>

#include "iterating.h"

constexpr int numberOfElements = 1000;

int nums[numberOfElements];
std::array<int,numberOfElements> cpparraynums;
std::vector<int> vecnums;
std::list<int> listnums;

int arraySum_ppi()
{
	size_t i;
	int sum = 0;
	for (i = 0; i < sizeof(nums) / sizeof(int); ++i)
	{
		sum += nums[i];
	}

	return sum;
}

int arraySum_ipp()
{
	size_t i;
	int sum = 0;
	for (i = 0; i < sizeof(nums) / sizeof(int); i++)
	{
		sum += nums[i];
	}

	return sum;
}

int arraySum_rangeloop()
{
	int sum = 0;
	for (auto i : nums)
	{
		sum += i;
	}

	return sum;
}

int vectorSum_rangeloop()
{
	int sum = 0;
	for (auto i : vecnums)
	{
		sum += i;
	}

	return sum;
}

int vectorSum_foreach()
{
	int sum = 0;
	std::for_each(vecnums.begin(), vecnums.end(), [&] (int n)
	{
		sum += n;
	});

	return sum;
}

int vectorSum_iterator()
{
	int sum = 0;

	for (std::vector<int>::iterator it = vecnums.begin(); it != vecnums.end();
				++it)
	{
		sum += *it;
	}

	return sum;
}

int vectorSum_accumulate()
{
	int sum = std::accumulate(vecnums.begin(), vecnums.end(), 0);
	return sum;
}


int listSum_rangeloop()
{
	int sum = 0;
	for (auto i : listnums)
	{
		sum += i;
	}
	return sum;
}


int listSum_iterator()
{
	int sum = 0;

	for (std::list<int>::iterator it = listnums.begin(); it != listnums.end();
				++it)
	{
		sum += *it;
	}

	return sum;
}


void doSumTests()
{
	const struct
	{
		int (*func)();
		const char* name;
	} sumtests[] =
	{
	{ arraySum_ipp, "arraysum - for i++" },
	{ arraySum_ppi, "arraysum - for ++i" },
	{ arraySum_rangeloop, "arraysum - range loop" },
	{ vectorSum_rangeloop, "vectorsum - range loop" },
	{ vectorSum_foreach, "vectorsum - std::for_each" },
	{ vectorSum_iterator, "vectorsum - iterator" },
	{ vectorSum_accumulate, "vectorsum - std::accumulate" },
	{ listSum_rangeloop, "listsum - range loop" },
	{ listSum_iterator, "listsum - iterator" },
	};

	//printf("listnums size : %lu\n",listnums.size());
	//printf("listnums size : %lu\n",listnums.size());

	size_t i;
	int sum = 0;
	for (i = 0; i < sizeof(nums) / sizeof(int); i++)
	{
		//nums[i] = rand();
		nums[i] = i;
		vecnums.push_back(nums[i]);
		listnums.push_back(nums[i]);
		sum += nums[i];
	}
	//printf("vecnums size : %lu\n",vecnums.size());
	//printf("listnums size : %lu  %d\n",listnums.size(), listnums.front());

	printf("Iterate over %d ints and sum them up\n", numberOfElements);

	for (auto i : sumtests)
	{
		measure_start();
		int result = i.func();
		measure_end();

		printf("%30s %6d %s\n",i.name, (int) elapsedTime,(result == sum ? "ok" : "invalid") );
	}

	printf("\n");
}

