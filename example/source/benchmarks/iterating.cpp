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
#include <functional>
#include <list>
#include <algorithm>
#include <numeric>
#include <stdlib.h>
#include <stdio.h>
#include "measure.h"
#include <iomanip>

#include "iterating.h"



int benchmark_iterating::arraySum_ppi()
{
	size_t i;
	int sum = 0;
	for (i = 0; i < sizeof(nums) / sizeof(int); ++i)
	{
		sum += nums[i];
	}

	return sum;
}

int benchmark_iterating::arraySum_ipp()
{
	size_t i;
	int sum = 0;
	for (i = 0; i < sizeof(nums) / sizeof(int); i++)
	{
		sum += nums[i];
	}

	return sum;
}

int benchmark_iterating::arraySum_rangeloop()
{
	int sum = 0;
	for (auto i : nums)
	{
		sum += i;
	}

	return sum;
}

int benchmark_iterating::vectorSum_rangeloop()
{
	int sum = 0;
	for (auto i : vecnums)
	{
		sum += i;
	}

	return sum;
}

int benchmark_iterating::vectorSum_foreach()
{
	int sum = 0;
	std::for_each(vecnums.begin(), vecnums.end(), [&] (int n)
	{
		sum += n;
	});

	return sum;
}


int benchmark_iterating::vectorSum_iterator()
{
	int sum = 0;

	for (std::vector<int>::iterator it = vecnums.begin(); it != vecnums.end();
			++it)
	{
		sum += *it;
	}

	return sum;
}

int benchmark_iterating::vectorSum_accumulate()
{
	int sum = std::accumulate(vecnums.begin(), vecnums.end(), 0);
	return sum;
}

int benchmark_iterating::listSum_rangeloop()
{
	int sum = 0;
	for (auto i : listnums)
	{
		sum += i;
	}
	return sum;
}

int benchmark_iterating::listSum_iterator()
{
	int sum = 0;

	for (std::list<int>::iterator it = listnums.begin(); it != listnums.end();
			++it)
	{
		sum += *it;
	}

	return sum;
}

void benchmark_iterating::execute()
{
	const struct
	{
		std::function<int(benchmark_iterating&)> func;
		const char* name;
	} sumtests[] =
	{
	{ &benchmark_iterating::arraySum_ipp, "arraysum - for i++" },
	{ &benchmark_iterating::arraySum_ppi, "arraysum - for ++i" },
	{ &benchmark_iterating::arraySum_rangeloop, "arraysum - range loop" },
	{ &benchmark_iterating::vectorSum_rangeloop, "vectorsum - range loop" },
	{ &benchmark_iterating::vectorSum_foreach, "vectorsum - std::for_each" },
	{ &benchmark_iterating::vectorSum_iterator, "vectorsum - iterator" },
	{ &benchmark_iterating::vectorSum_accumulate, "vectorsum - std::accumulate" },
	{ &benchmark_iterating::listSum_rangeloop, "listsum - range loop" },
	{ &benchmark_iterating::listSum_iterator, "listsum - iterator" }, };

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
		int result = i.func(*this);
		measure_end();

		printf("%30s %6d %s\n", i.name, (int) elapsedTime,
				(result == sum ? "ok" : "invalid"));
	}

	printf("\n");
}
