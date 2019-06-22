/*
 * strings.cpp
 *
 *  Created on: 13.06.2019
 *      Author: andre
 */

#include "multiplatform.h"

#ifndef DISABLE_STREAMCLASS
#include <iostream>
#include <sstream>
#include <string>
#endif

#include <vector>
#include <string.h>
#include <algorithm>
#include <numeric>
#include <stdlib.h>
#include <stdio.h>
#include "measure.h"
#include <iomanip>
#include "stringtests.h"
#include "iterating.h"

static int nums[4];

char strAr[100];

void carr_build()
{
	sprintf(strAr, "%d %d %d %d", nums[0], nums[1], nums[2], nums[3]);
}

#ifndef DISABLE_STREAMCLASS
std::stringstream cppstrstr;

void stringstream_build()
{
	cppstrstr << nums[0] << " " << nums[1] << " " << nums[2] << " " << nums[3];
}

#endif

void doStringTests()
{
	const struct
	{
		void (*func)();
		const char* name;
	} stringtests[] =
	{
	{ carr_build, "sprintf" },
#ifndef DISABLE_STREAMCLASS
			{	stringstream_build, "c++ stringstream build"},
#endif
		};

	//prepare the tests

	size_t i;
	for (i = 0; i < 4; i++)
	{
		nums[i] = rand();
	}

	printf("Construct strings from 4 integers\n");

	for (auto i : stringtests)
	{
		measure_start();
		i.func();
		measure_end();
		printf("%30s %6d\n", i.name, (int) elapsedTime);
	}

#ifndef DISABLE_STREAMCLASS
	if (strcmp(cppstrstr.str().c_str(), strAr))
	{
		printf("String failure!\n");
	}
	else
	{
		printf("Strings ok\n");
	}
#endif

	printf("\n");
}

