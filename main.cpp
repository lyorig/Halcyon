#include <iostream>

#include "factorizer.hpp"

#include "lyo/argparse.hpp"

#define PROGRAM_NAME "x-factorizer"

int main(int argc, char* argv[])
{
	// No argument given.
	if (argc == 1)
	{
		std::cout << "\nUsage: " PROGRAM_NAME " -num=[input number]\n";
		return EXIT_FAILURE;
	}

	const lyo::parser p{ argc, argv };

	const auto input{ p.parse<x::arith_type>("-num=") };

	// Parsing the number failed.
	if (!input)
	{
		std::cout << "\nInvalid number entered. Exiting.\n";
		return EXIT_FAILURE;
	}

	// Good to go! Let's initialize the factorizer and get this
	// show on the road.

	x::factorizer fact{ *input };

	// An unordered map of divisors.
	const auto& divs{ fact.divisors() };

	std::size_t idx{ 0 };

	std::cout << *input << " = ";

	for (const auto& pair : divs)
	{
		std::cout << pair.first << '^' << int(pair.second);

		// Prevent a trailing "x".
		if (idx != divs.size() - 1)
		{
			std::cout << " x ";
			++idx;
		}

		else
		{
			std::cout << '\n';
		}
	}

	return EXIT_SUCCESS;
}