#include "pch.h"
#include "RNG.h"

thread_local std::mt19937 gen{ std::random_device{}() };

template<typename T>
T random(T min, T max) // range is <min, max> 
{
	return std::uniform_int_distribution<T>{min, max}(gen);
}

template int random<int>(int,int);