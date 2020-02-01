#pragma once

#include <algorithm>
#include <array>
#include <numeric>
#include <random>

using pType_t = uint32_t;
template <pType_t S>
class Prisioners
{
protected:
	std::array<pType_t, S> v;
	std::default_random_engine gen{std::random_device{}()};

public:
	Prisioners()
	{
		std::iota(v.begin(), v.end(), 0);
	}

	bool oneRound()
	{
		std::shuffle(v.begin(), v.end(), gen);
		
		bool found;
		for (pType_t i = 0; i<S; ++i) 
		{
		    pType_t nextIndex = i;
		    found = false;
		    for (pType_t j = 0; not found && j < S/2; ++j) 
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
		        return false;
		}
		return true;
	}

	const std::array<pType_t, S>& getBoxes() const { return v; }

	pType_t nPrisioners() const { return S; }
};
