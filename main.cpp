#include <iostream>
#include "bench.hpp"
#include "prisioner.hpp"
#include "parallel.hpp"

void print_v(std::vector<int>& v)
{
	for (int i: v)
	{
		std::cout << i << ' ';
	}
	std::cout << std::endl;
}



int main()
{
    constexpr pType_t tests = 100;
    constexpr pType_t persons = 1<<16;
    
    Prisioners<persons> p;
    PPrisioners<persons> p2;
    pType_t nSuccess = 0;
    
    std::cout << "Número de hilos en uso: ";
    std::cout << PPrisioners<persons>::numberOfThreads << std::endl;
    std::cout << persons << std::endl;
    {
    Timer t{[](unsigned long, double ms){ std::cout << ms << " ms\n"; }};
    for (pType_t i = 0; i < tests; ++i) 
    {
        if (p2.oneRound())
        {
            ++nSuccess;
        }
        // nSuccess += p2.oneRound();
    }
    }
	
    std::cout << "Éxito: " << (static_cast<double>(nSuccess) / tests) * 100 << '%' << std::endl;
}
