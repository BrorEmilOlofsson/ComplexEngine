#include "Editor/Precompiled/EditorPch.hpp"
#include "EditorCompositeCommand.hpp"

namespace Simple
{

	EditorCompositeCommand::EditorCompositeCommand(std::string name, std::vector<EditorCommand> commands)
		: mName(std::move(name))
		, mCommands(std::move(commands))
	{
	}

	void EditorCompositeCommand::Execute(const bool debugPrint) const
	{
		for (const EditorCommand& command : mCommands)
		{
			command.ExecuteCommand(debugPrint);
		}
	}

	void EditorCompositeCommand::Undo(const bool debugPrint) const
	{
		for (auto& command : std::views::reverse(mCommands))
		{
			command.UndoCommand(debugPrint);
		}
	}
}