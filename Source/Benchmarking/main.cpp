#define CATCH_CONFIG_RUNNER
#include <External/Catch2/catch_amalgamated.hpp>

int main(int argc, char* argv[])
{
    // Optional: Engine::Init(); if needed
    int result = Catch::Session().run(argc, argv);
    // Optional: Engine::Shutdown();
    return result;
}