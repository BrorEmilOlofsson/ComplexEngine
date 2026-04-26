#pragma once
#include "EditorCommand.hpp"
#include <vector>

namespace CLX
{
	class EditorCompositeCommand final
	{
	public:

		EditorCompositeCommand(std::vector<EditorCommand> commands);


		void Execute() const;
		void Undo() const;

        std::string ToString() const;

	private:

		std::vector<EditorCommand> mCommands;
	};
}