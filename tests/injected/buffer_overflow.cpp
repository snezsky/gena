int main(int argc, [[maybe_unused]] char **argv)
{
    // NOLINTBEGIN
    const char arr[] = {1, 2, 3, 4, 5};

    // cppcheck-suppress[unusedVariable, unreadVariable, arrayIndexOutOfBounds]
    [[maybe_unused]] const volatile char undefined = arr[argc * 10];
    // NOLINTEND
}
