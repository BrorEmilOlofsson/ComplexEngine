#pragma once
#include "EditorCommand.hpp"
#include "EditorCompositeCommand.hpp"
#include "EditorCompositeCommandBuilder.hpp"
#include <stack>
#include <memory>

namespace CLX
{

	class EditorCommandTracker final
	{
	public:

		EditorCommandTracker() = default;

		void Clear();

		void ExecuteCommand(EditorCommand command);

		// Register Command without executing it
		void RegisterCommand(EditorCommand command);

		void BeginComposite(std::string_view name);
		void EndComposite();

		void UndoCommand(const bool debugPrint = false);
		void RedoCommand(const bool debugPrint = false);

		[[nodiscard]] std::size_t GetUndoSize() const;
		[[nodiscard]] std::size_t GetRedoSize() const;

	private:

		void ExecuteCommandInternal(bool execute, EditorCommand&& command);

	private:

		std::stack<EditorCommand> mUndoStack;
		std::stack<EditorCommand> mRedoStack;
		EditorCompositeCommandBuilder mCompositeCommandBuilder;
	};
}