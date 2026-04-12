#include <iostream>
#include "myproject.hpp"

// NOLINTNEXTLINE(bugprone-exception-escape)
int main()
{
    std::cout << myproject::split("Splitting also works, or is it?", ',')[0] << '\n';
}
