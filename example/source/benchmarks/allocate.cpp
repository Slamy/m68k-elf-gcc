/*
 * allocate.cpp
 *
 *  Created on: 13.06.2019
 *      Author: andre
 */

#include "multiplatform.h"

#include <array>
#include <functional>
#include <iomanip>
#include <list>
#include <memory>
#include <numeric>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include <algorithm> //algorithm must be included latest for bebbo toolchain

#include "measure.h"
#include <allocate.h>

constexpr int numberOfElements = 30;

void benchmark_allocate::c_pointer()
{
	int i;
	int* pointers[numberOfElements];

	for (i = 0; i < numberOfElements; i++)
	{
		pointers[i] = (int*)malloc(sizeof(int));
		if (pointers[i] == NULL)
		{
			printf("Failed to allocate memory\n");
			exit(1);
		}
	}

	for (i = 0; i < numberOfElements; i++)
		free(pointers[i]);
}

void benchmark_allocate::cpp_pointer()
{
	int i;
	int* pointers[numberOfElements];

	for (i = 0; i < numberOfElements; i++)
	{
		pointers[i] = new int;
		if (pointers[i] == NULL)
		{
			printf("Failed to allocate memory\n");
			exit(1);
		}
	}

	for (i = 0; i < numberOfElements; i++)
		delete pointers[i];
}

void benchmark_allocate::cpp_unique()
{
	int i;
	std::unique_ptr<int> pointers[numberOfElements];

	for (i = 0; i < numberOfElements; i++)
	{
		pointers[i] = std::make_unique<int>();
	}
}

void benchmark_allocate::cpp_shared()
{
	int i;
	std::shared_ptr<int> pointers[numberOfElements];

	for (i = 0; i < numberOfElements; i++)
	{
		pointers[i] = std::make_shared<int>();
	}
}

void benchmark_allocate::cpp_vectorUnique()
{
	int i;
	std::vector<std::unique_ptr<int>> pointers;

	for (i = 0; i < numberOfElements; i++)
	{
		pointers.push_back(std::make_unique<int>());
	}
}

void benchmark_allocate::cpp_vectorUnique_reserved()
{
	int i;
	std::vector<std::unique_ptr<int>> pointers;
	pointers.reserve(numberOfElements);

	for (i = 0; i < numberOfElements; i++)
	{
		pointers.push_back(std::make_unique<int>());
	}
}

void benchmark_allocate::cpp_vectorUnique_presized()
{
	int i;
	std::vector<std::unique_ptr<int>> pointers(numberOfElements);

	for (i = 0; i < numberOfElements; i++)
	{
		pointers.at(i) = std::make_unique<int>();
	}
}

void benchmark_allocate::execute()
{
	static const struct
	{
		std::function<void(benchmark_allocate&)> func;
		const char* name;
	} stringtests[] = {
		{&benchmark_allocate::c_pointer, "c array of malloc'd pointers"},
		{&benchmark_allocate::cpp_pointer, "c array of new'd pointers"},
		{&benchmark_allocate::cpp_unique, "c array of unique pointers"},
		{&benchmark_allocate::cpp_vectorUnique_reserved, "reserve c++ vector of unique pointers"},
		{&benchmark_allocate::cpp_vectorUnique_presized, "presized c++ vector of unique pointers"},
		{&benchmark_allocate::cpp_shared, "c array of shared pointers"},
		{&benchmark_allocate::cpp_vectorUnique, "c++ vector of unique pointers"},
	};

	printf("Allocate %d int pointers and manage them\n", numberOfElements);

	for (auto i : stringtests)
	{
		measure_start();
		i.func(*this);
		measure_end();
		printf("%40s %6d\n", i.name, (int)elapsedTime);
	}

	printf("\n");
}
