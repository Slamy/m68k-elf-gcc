#include "config.h"

#ifndef DISABLE_STREAMCLASS
#include <iostream>
#endif

#include "measure.h"
#include <algorithm>
#include <iomanip>
#include <list>
#include <numeric>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "allocate.h"
#include "iterating.h"
#include "stringtests.h"

extern "C"
{
#include "gdb-m68k-stub.h"
#include "uart.h"
}

#if defined(AMIGACROSS) && !defined(BUILD_FOR_AMIGADOS)
/*
 * FIXME I'm quite confused why this is needed. As it seems this is normally done
 * by libgcc and crtbegin.o inside of that lib. But I have the feeling that libgloss
 * which also provides crt0.o is not compatible with crtbegin.o.
 * While the .ctors segment seems to be correct the .init segment is not and the system will crash.
 * I'm not sure why this happens. So until then global C++ constructors must be called by manually
 * processing .ctors after main is called.
 * Usually this would happen before main is called.
 */
void staticConstructors()
{
	extern uint32_t __CTOR_LIST__;

	uint32_t* ptr = &__CTOR_LIST__;
	ptr++; // skip number of pointers

	while (*ptr)
	{
		void (*func)() = (void (*)()) * ptr;
		// printf("Execute static constructor %lx\n", *ptr);
		func();

		ptr++;
	}
}

// FIXME Why is this needed?
void* __dso_handle;

#endif

benchmark_iterating iteratingGlobal;

int main(int argc, char** argv)
{
	// set_debug_traps();
#if defined(AMIGACROSS) && !defined(BUILD_FOR_AMIGADOS)
	staticConstructors();
#endif

	printf("Slamy's Litte C vs C++ Benchmark\n");

	std::vector<std::unique_ptr<benchmark>> bmarks;

	bmarks.push_back(std::make_unique<benchmark_allocate>());
	bmarks.push_back(std::make_unique<benchmark_strings>());
	bmarks.push_back(std::make_unique<benchmark_iterating>());

	std::for_each(bmarks.begin(), bmarks.end(), [](auto& i) { i->execute(); });

	iteratingGlobal.execute();

	printMemoryUsage();

	return 0;
}

// Only useful for GDB
#if 0
#ifndef BUILD_FOR_AMIGADOS
void stub_putDebugChar(char c) /* write a single character      */
{
	uart_printChar(c);
}

int stub_getDebugChar() /* read and return a single char */
{
	return uart_blockedGetChar();
}

typedef void (*ExceptionHook)(int); /* pointer to function with int parm */

void exceptionHandler(int id, void* ptr) /* assign an exception handler */
{
	// PRINTF("exceptionHandler %d\n",id);
	*(volatile uint32_t*)(id * 4) = (uint32_t)ptr;// set level 5 interrupt handler
}

ExceptionHook exceptionHook = 0; /* hook variable for errors/exceptions */
#endif
#endif

// Sorgt dafür, dass exception demangling nicht eingelinkt wird!
// https://developer.mbed.org/forum/platform-32-ST-Nucleo-L152RE-community/topic/4802/?page=2#comment-25593
namespace __gnu_cxx
{
void __verbose_terminate_handler()
{
	// uart_puts((const char*)"NOOO1!!\n");
	for (;;)
		;
}
} // namespace __gnu_cxx
extern "C" void __cxa_pure_virtual(void);
extern "C" void __cxa_pure_virtual(void)
{
	// uart_puts((const char*)"NOOO2!!\n");
	for (;;)
		;
}
