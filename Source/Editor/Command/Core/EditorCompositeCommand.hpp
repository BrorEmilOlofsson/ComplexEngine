#pragma once
#include "EditorCommand.hpp"
#include <vector>

namespace Simple
{
	class EditorCompositeCommand final
	{
	public:

		EditorCompositeCommand(std::string aName, std::vector<EditorCommand> aCommands);


		void Execute(bool aDebugPrint) const;
		void Undo(bool aDebugPrint) const;

		const std::string& GetName() const
		{
			return myName;
		}

		void AddCommand(EditorCommand&& aCommand)
		{
			myCommands.push_back(std::move(aCommand));
		}

		void AddCommand(const EditorCommand& aCommand)
		{
			myCommands.push_back(aCommand);
		}

	private:

		std::vector<EditorCommand> myCommands;
		std::string myName;
	};
}