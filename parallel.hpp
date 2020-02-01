#pragma once

#include <atomic>
#include <parallel/algorithm>
#include <thread>
#include <mutex>
#include "prisioner.hpp"

namespace par = __gnu_parallel;

template <size_t S>
class PPrisioners: public Prisioners<S>
{
private:
	static constexpr bool usePar = false;//S>50000ull;
	std::atomic_bool failed = false;
	std::vector<std::thread> thrdPool;

	void oneBatch(pType_t ini, pType_t fin)
	{
		bool found;
		for (pType_t i = ini; i<fin && not failed; ++i) 
		{
		    pType_t nextIndex = i;
		    found = false;
		    for (pType_t j = 0; not found && j < S/2 && not failed; ++j) 
		    {
		        if (v[nextIndex] == i) 
		        {
		            found = true;
		        }
		        else 
		        {
		            nextIndex = v[nextIndex];
		        }
		    }

		    if (not found)
		    {
		        failed = true;
		    }
		}
	}
	
	void shuffle_boxes()
	{
		if constexpr (usePar)
		{
			par::random_shuffle(v.begin(), v.end(), [this](size_t n){
				std::uniform_int_distribution<size_t> d(0, n? n-1: 0);
				return d(gen);
			});
		}
		else
		{
			std::shuffle(v.begin(), v.end(), gen);
		}
	}
	
public:
	static const pType_t numberOfThreads;
	static const pType_t batchSize;
	using Prisioners<S>::v;
	using Prisioners<S>::gen;
	
	PPrisioners(): Prisioners<S>(), thrdPool(PPrisioners::numberOfThreads-1) {}
	
	bool oneRound()
	{
		shuffle_boxes();
		
		failed = false;
		
		pType_t currentStart = 0;
		pType_t next = currentStart+batchSize;
		for (pType_t i=0; i<numberOfThreads-1; ++i)
		{
			thrdPool[i] = std::thread(&PPrisioners<S>::oneBatch, this, currentStart, next);
			currentStart = next;
			next += batchSize;
		}
		
		//thrdPool.back() = std::thread(&PPrisioners<S>::oneBatch, this, currentStart, next + (S%numberOfThreads));
		oneBatch(currentStart, next + (S%numberOfThreads));

		for (auto& t: thrdPool)
		{
			t.join();
		}

		return not failed;
	}
};

template <size_t S>
const pType_t PPrisioners<S>::numberOfThreads = std::thread::hardware_concurrency() * 1;
template <size_t S>
const pType_t PPrisioners<S>::batchSize = S/PPrisioners::numberOfThreads;
