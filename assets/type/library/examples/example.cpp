#include <iostream>
#include "<@ project_name @>.hpp"

// NOLINTNEXTLINE(bugprone-exception-escape)
int main()
{
    std::cout << <@project_name @>::split("Good luck navigating the vast and winding road ahead", 'n')[0] << '\n';
}
