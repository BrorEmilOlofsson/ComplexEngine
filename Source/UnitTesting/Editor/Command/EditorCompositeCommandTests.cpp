#include <External/Catch2/catch_amalgamated.hpp>

#include "Editor/Command/Core/EditorCompositeCommand.hpp"

using namespace CLX;

TEST_CASE("EditorCompositeCommand::Commands execute in correct order")
{

    EditorCompositeCommand command("Test Command", {});
    command.Execute(false);
}