#include <iostream>
#include "myproject.hpp"

// NOLINTNEXTLINE(bugprone-exception-escape)
int main()
{
    std::cout << myrpoject::split("Good luck navigating the vast and winding road ahead", 'n')[0] << '\n';
}
