#pragma once
#include "EditorCommand.hpp"
#include <vector>

namespace Simple
{
	class EditorCompositeCommand final
	{
	public:

		EditorCompositeCommand(std::string name, std::vector<EditorCommand> commands);


		void Execute(bool debugPrint) const;
		void Undo(bool debugPrint) const;

		const std::string& GetName() const
		{
			return mName;
		}

		void AddCommand(EditorCommand&& command)
		{
			mCommands.push_back(std::move(command));
		}

		void AddCommand(const EditorCommand& command)
		{
			mCommands.push_back(command);
		}

	private:

		std::vector<EditorCommand> mCommands;
		std::string mName;
	};
}