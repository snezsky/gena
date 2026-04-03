#include <iostream>
#include "myproject.hpp"

int main()
{
    try
    {
        std::cout << myproject::trim(" The outer spaces will be removed! ") << '\n';
        std::cout << myproject::split("Splitting also works, or is it?", ',')[0] << '\n';
        std::cout << myproject::join({"This", "will", "be", "one", "string"}, " ") << '\n';
    }
    catch ([[maybe_unused]] const std::exception& e)
    {
        std::puts("or is it?");
    }
}
