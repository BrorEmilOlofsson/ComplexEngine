#include "Editor/Precompiled/EditorPch.hpp"
#include "EditorCompositeCommand.hpp"

namespace Simple
{

	EditorCompositeCommand::EditorCompositeCommand(std::string aName, std::vector<EditorCommand> aCommands)
		: myName(std::move(aName))
		, myCommands(std::move(aCommands))
	{
	}

	void EditorCompositeCommand::Execute(const bool aDebugPrint) const
	{
		for (const EditorCommand& command : myCommands)
		{
			command.ExecuteCommand(aDebugPrint);
		}
	}

	void EditorCompositeCommand::Undo(const bool aDebugPrint) const
	{
		for (int i = static_cast<int>(myCommands.size()) - 1; i >= 0; --i)
		{
			myCommands[i].UndoCommand(aDebugPrint);
		}
	}
}