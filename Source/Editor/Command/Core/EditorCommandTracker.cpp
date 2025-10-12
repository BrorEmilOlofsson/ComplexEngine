#include "Editor/Precompiled/EditorPch.hpp"
#include "EditorCommandTracker.hpp"
#include "EditorCompositeCommand.hpp"

namespace Simple
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

	void EditorCommandTracker::BeginComposite(std::string_view name)
	{
		mCompositeCommandBuilder.Begin(name);
	}

	void EditorCommandTracker::EndComposite()
	{
		std::optional<EditorCompositeCommand> compositeCommand = mCompositeCommandBuilder.End();

		if (compositeCommand)
		{
			ExecuteCommand(EditorCommand(compositeCommand.value(), compositeCommand->GetName()));
		}
	}

	void EditorCommandTracker::UndoCommand(const bool debugPrint)
	{
		if (mUndoStack.empty())
		{
			return;
		}
		EditorCommand& topCommand = mUndoStack.top();
		topCommand.UndoCommand(debugPrint);
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
		topCommand.ExecuteCommand(debugPrint);
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
		if (mCompositeCommandBuilder.IsActive())
		{
			mCompositeCommandBuilder.AddCommand(std::move(command));
			return;
		}

		if (execute)
		{
			command.ExecuteCommand(false);
		}

		mUndoStack.push(std::move(command));

		mRedoStack = {};
	}
}