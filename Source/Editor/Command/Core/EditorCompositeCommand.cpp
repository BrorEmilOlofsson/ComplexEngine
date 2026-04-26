#include "Editor/Precompiled/EditorPch.hpp"
#include "EditorCompositeCommand.hpp"

namespace CLX
{

    EditorCompositeCommand::EditorCompositeCommand(std::vector<EditorCommand> commands)
        : mCommands(std::move(commands))
    {}

    void EditorCompositeCommand::Execute() const
    {
        for (const EditorCommand& command : mCommands)
        {
            command.ExecuteCommand();
        }
    }

    void EditorCompositeCommand::Undo() const
    {
        for (auto& command : std::views::reverse(mCommands))
        {
            command.UndoCommand();
        }
    }

    std::string EditorCompositeCommand::ToString() const
    {
        std::string result;
        for (const auto& command : mCommands)
        {
            result += "  " + command.ToString() + "\n";
        }
        return result;
    }
}