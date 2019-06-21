
#include <stdio.h>
#include <stdint.h>
#include <list>

class Mopped
{
	public:
	Mopped()
	{
		printf("Barf!\n");
	}
};

Mopped mopped1;
extern uint32_t __CTOR_LIST__;

void staticConstructors()
{
	uint32_t *ptr = &__CTOR_LIST__;
	ptr++; //skip number of pointers

	while (*ptr)
	{
		void (*func)() = (void(*)())*ptr;
		//printf("Execute function %lx\n",*ptr);
		func();

		ptr++;
	}
}

void *__dso_handle;

std::list<int> liste;

int main(int argc, char **argv)
{
	printf("Slamy's Litte C vs C++ Benchmark\n");

	staticConstructors();

	liste.push_back(1);
	liste.push_back(2);
	liste.push_back(3);

	for(auto i : liste)
	{
		printf("%d\n",i);
	}

	//std::cout << "Slamy's Litte C vs C++ Benchmark" << std::endl;

	return 0;
}

//Sorgt dafür, dass exception demangling nicht eingelinkt wird!
//https://developer.mbed.org/forum/platform-32-ST-Nucleo-L152RE-community/topic/4802/?page=2#comment-25593
namespace __gnu_cxx {
    void __verbose_terminate_handler() {
    	//uart_puts((const char*)"NOOO1!!\n");
    	for(;;);
    }
}
extern "C" void __cxa_pure_virtual(void);
extern "C" void __cxa_pure_virtual(void) {
	//uart_puts((const char*)"NOOO2!!\n");
	for(;;);
}


