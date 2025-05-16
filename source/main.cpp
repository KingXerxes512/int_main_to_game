#include <iostream>
#include <print>
#include <stacktrace>

#include "exception.h"
#include "window.h"
#include "opengl.h"

int main()
{
    try
    {
        game::Window window(800u, 600u);

        while (window.Running())
        {
            ::glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
            ::glClear(GL_COLOR_BUFFER_BIT);

            window.Swap();
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
