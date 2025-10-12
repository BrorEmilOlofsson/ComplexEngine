#include "Editor/Precompiled/EditorPch.hpp"
#include "EditorCompositeCommandBuilder.hpp"

namespace Simple
{

	void EditorCompositeCommandBuilder::AddCommand(EditorCommand&& aCommand)
	{
		myCurrentCompositeCommand->AddCommand(std::move(aCommand));
	}

	void EditorCompositeCommandBuilder::Begin(std::string_view aName)
	{
		if (myCurrentCompositeCommand)
		{
			myCurrentCompositeCommand->Begin(aName);
		}
		else
		{
			myCurrentCompositeCommand = std::make_unique<CompositeCommandInternal>(std::string(aName));
		}
	}

	std::optional<EditorCompositeCommand> EditorCompositeCommandBuilder::End()
	{
		if (!myCurrentCompositeCommand)
		{
			return std::nullopt;
		}

		/*if (myCurrentCompositeCommand->myCommands.empty())
		{
			return std::nullopt;
		}*/

		eEndCode endCode = myCurrentCompositeCommand->End();

		if (endCode == eEndCode::Ended)
		{
			std::unique_ptr<CompositeCommandInternal> internalComposite = std::move(myCurrentCompositeCommand);
			myCurrentCompositeCommand.reset();
			return internalComposite->Build();
		}

		return std::nullopt;

	}

	EditorCompositeCommand EditorCompositeCommandBuilder::CompositeCommandInternal::Build() const
	{
		return EditorCompositeCommand(myName, myCommands);
	}

	void EditorCompositeCommandBuilder::CompositeCommandInternal::AddCommand(EditorCommand&& aCommand)
	{
		if (myCurrentChild)
		{
			myCurrentChild->AddCommand(std::move(aCommand));
		}
		else
		{
			myCommands.emplace_back(std::move(aCommand));
		}
	}


	void EditorCompositeCommandBuilder::CompositeCommandInternal::Begin(std::string_view aName)
	{
		if (myCurrentChild)
		{
			myCurrentChild->Begin(aName);
		}
		else
		{
			myCurrentChild = std::make_unique<CompositeCommandInternal>(std::string(aName));
		}
	}

	EditorCompositeCommandBuilder::eEndCode EditorCompositeCommandBuilder::CompositeCommandInternal::End()
	{
		if (myCurrentChild)
		{
			const eEndCode endCode = myCurrentChild->End();
			if (endCode == eEndCode::Ended)
			{
				const std::string compositeName = myCurrentChild->myName;
				myCommands.push_back(EditorCommand(myCurrentChild->Build(), compositeName));
				myCurrentChild.reset();
			}
			else if (endCode == eEndCode::Ended_Empty) // If the child's commands are empty we don't want to add the child to our commands
			{
				myCurrentChild.reset();
			}
			return eEndCode::InProgress;
		}
		else if (myCommands.empty())
		{
			return eEndCode::Ended_Empty;
		}

		return eEndCode::Ended;
	}
}