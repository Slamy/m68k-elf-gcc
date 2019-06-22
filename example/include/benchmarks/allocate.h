/*
 * allocate.h
 *
 *  Created on: 13.06.2019
 *      Author: andre
 */

#ifndef ALLOCATE_H_
#define ALLOCATE_H_

#include "benchmark.h"

class benchmark_allocate : public benchmark
{
private:
	void c_pointer();
	void cpp_pointer();
	void cpp_unique();
	void cpp_shared();
	void cpp_vectorUnique();
	void cpp_vectorUnique_reserved();
	void cpp_vectorUnique_presized();

public:
	void execute();
};


#endif /* ALLOCATE_H_ */
