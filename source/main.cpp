#include <iostream>
#include <print>
#include <stacktrace>

#include "exception.h"
#include "window.h"

int main()
{
    try
    {
        game::Window window(800u, 600u);

        while (window.Running())
        {
        }
    }
    catch (const game::Exception& err)
    {
        std::println(std::cerr, "{}", err);
    }
    catch (...)
    {
        std::println(std::cerr, "unknown exception");
    }

    return 0;
}
