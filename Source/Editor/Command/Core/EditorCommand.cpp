#include "Editor/Precompiled/EditorPch.hpp"
#include "EditorCommand.hpp"

namespace Simple
{

	void EditorCommand::ExecuteCommand(const bool debugPrint) const
	{
		if (debugPrint)
		{
			std::cout << "Do Command: " << mName << std::endl;
		}
		mConcept->ExecuteCommand(debugPrint);
	}

	void EditorCommand::UndoCommand(const bool debugPrint) const
	{
		if (debugPrint)
		{
			std::cout << "Undo Command: " << mName << std::endl;
		}
		mConcept->UndoCommand(debugPrint);
	}
}
