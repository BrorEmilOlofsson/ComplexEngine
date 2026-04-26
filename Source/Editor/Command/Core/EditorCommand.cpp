#include "Editor/Precompiled/EditorPch.hpp"
#include "EditorCommand.hpp"

namespace CLX
{

	void EditorCommand::ExecuteCommand() const
	{
		mConcept->ExecuteCommand();
	}

	void EditorCommand::UndoCommand() const
	{
		mConcept->UndoCommand();
	}

	std::string EditorCommand::ToString() const
	{
		return mConcept->ToString();
    }
}
