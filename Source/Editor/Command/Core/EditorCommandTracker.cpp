#include "Editor/Precompiled/EditorPch.hpp"
#include "EditorCommandTracker.hpp"
#include "EditorCompositeCommand.hpp"

namespace CLX
{

	void EditorCommandTracker::Clear()
	{
		mUndoStack = {};
		mRedoStack = {};
	}

	void EditorCommandTracker::ExecuteCommand(EditorCommand command)
	{
		ExecuteCommandInternal(true, std::move(command));
	}

	void EditorCommandTracker::RegisterCommand(EditorCommand command)
	{
		ExecuteCommandInternal(false, std::move(command));
	}

	void EditorCommandTracker::BeginComposite(std::string name)
	{
		mCompositeCommandBuilder.Begin(std::move(name));
	}

	void EditorCommandTracker::EndComposite()
	{
		std::optional<std::pair<EditorCompositeCommand, std::string>> compositeCommand = mCompositeCommandBuilder.End();

		if (compositeCommand)
		{
			ExecuteCommandInternal(false, EditorCommand(compositeCommand->first, compositeCommand->second));
		}
	}

	void EditorCommandTracker::UndoCommand(const bool debugPrint)
	{
		if (mUndoStack.empty())
		{
			return;
		}
		EditorCommand& topCommand = mUndoStack.top(); 
		if (debugPrint)
		{
			std::println("Undoing command:\n{}", topCommand);
		}
		topCommand.UndoCommand();
		mRedoStack.push(std::move(topCommand));
		mUndoStack.pop();
	}

	void EditorCommandTracker::RedoCommand(const bool debugPrint)
	{
		if (mRedoStack.empty())
		{
			return;
		}
		EditorCommand& topCommand = mRedoStack.top();
		if (debugPrint)
		{
			std::println("Redoing command:\n{}", topCommand);
        }
		topCommand.ExecuteCommand();
		mUndoStack.push(std::move(topCommand));
		mRedoStack.pop();
	}

	std::size_t EditorCommandTracker::GetUndoSize() const
	{
		return mUndoStack.size();
	}

	std::size_t EditorCommandTracker::GetRedoSize() const
	{
		return mRedoStack.size();
	}

	void EditorCommandTracker::ExecuteCommandInternal(const bool execute, EditorCommand&& command)
	{
		if (execute)
		{
			command.ExecuteCommand();
		}

		if (mCompositeCommandBuilder.IsActive())
		{
			mCompositeCommandBuilder.AddCommand(std::move(command), execute);
			return;
		}

		mUndoStack.push(std::move(command));

		mRedoStack = {};
	}
}