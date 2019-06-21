/*
 * pointer.cpp
 *
 *  Created on: 13.06.2019
 *      Author: andre
 */

#include "multiplatform.h"

#include <allocate.h>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <stdlib.h>
#include <stdio.h>
#include "measure.h"
#include <iomanip>
#include <memory>

constexpr int numberOfElements = 20;

void c_pointer()
{
	int i;
	int *pointers[numberOfElements];

	for (i = 0; i < numberOfElements; i++)
	{
		pointers[i] = (int*) malloc(sizeof(int));
		if (pointers[i] == NULL)
		{
			printf("Failed to allocate memory\n");
			exit(1);
		}
	}

	for (i = 0; i < numberOfElements; i++)
		free(pointers[i]);

}

#ifndef DEACTIVATE_LIBSTDCPP
void cpp_pointer()
{
	int i;
	int *pointers[numberOfElements];

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

void cpp_unique()
{
	int i;
	std::unique_ptr<int> pointers[numberOfElements];

	for (i = 0; i < numberOfElements; i++)
	{
		pointers[i] = std::make_unique<int>();
	}
}

void cpp_shared()
{
	int i;
	std::shared_ptr<int> pointers[numberOfElements];

	for (i = 0; i < numberOfElements; i++)
	{
		pointers[i] = std::make_shared<int>();
	}
}

void cpp_vectorUnique()
{
	int i;
	std::vector<std::unique_ptr<int>> pointers;

	for (i = 0; i < numberOfElements; i++)
	{
		pointers.push_back(std::make_unique<int>());
	}
}

void cpp_vectorUnique_reserved()
{
	int i;
	std::vector<std::unique_ptr<int>> pointers;
	pointers.reserve(numberOfElements);

	for (i = 0; i < numberOfElements; i++)
	{
		pointers.push_back(std::make_unique<int>());
	}
}
#endif

void doAllocationTests()
{
	static const struct
	{
		void (*func)();
		const char *name;
	} stringtests[] =
	{
	{ c_pointer, "c array of malloc'd pointers" },
#ifndef DEACTIVATE_LIBSTDCPP
	{ cpp_pointer, "c array of new'd pointers" },
	{ cpp_unique, "c array of unique pointers" },

	{ cpp_vectorUnique_reserved, "reserved c++ vector of unique pointers" },
	{ cpp_shared, "c array of shared pointers" },
	{ cpp_vectorUnique, "c++ vector of unique pointers" },
#endif
	};

	printf ("Allocate %d int pointers and manage them\n", numberOfElements);

	for (auto i : stringtests)
	{
		measure_start();
		i.func();
		measure_end();
		printf("%40s %6d\n",i.name, (int) elapsedTime);
	}

	printf("\n");
}
