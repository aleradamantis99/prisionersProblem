#pragma once
#include <iostream>
#include <chrono>
#include <cmath>

namespace chr = std::chrono;

void print(unsigned long us, double ms) {
    std::cout << us << "us, ";
    std::cout << ms << "ms\n";
}

using default_print = decltype( & print);
template < typename F >
    class Timer {
    public:
		Timer():
		Timer(print) {}
    	Timer(F f):
		    startPoint(chr::high_resolution_clock::now()),
		    funct(f) {}

        ~Timer() {
            stop();
        }
		void stop() {
		    auto end = chr::high_resolution_clock::now();
		    auto startT = chr::time_point_cast < chr::microseconds > (startPoint).time_since_epoch().count();
		    auto stopT = chr::time_point_cast < chr::microseconds > (end).time_since_epoch().count();
		    auto duration = (stopT - startT);
		    auto ms = duration * 0.001;
		    funct(duration, ms);
    	}

    private:
        chr::time_point < chr::high_resolution_clock > startPoint;
        F funct;
    };

#ifdef MAIN_RUN
int main() {
    int micro, mili;
    auto getVals = [ & ](unsigned long us, double ms) {
        micro = us;
        mili = ms;
    }; {
        Timer t {
            getVals
        };
    }
    std::cout << mili;
    return 0;
}
#endif
