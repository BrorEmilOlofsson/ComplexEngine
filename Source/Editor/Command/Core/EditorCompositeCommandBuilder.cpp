#include "Editor/Precompiled/EditorPch.hpp"
#include "EditorCompositeCommandBuilder.hpp"

namespace CLX
{

	void EditorCompositeCommandBuilder::AddCommand(EditorCommand command, const bool execute)
	{
		mCurrentCompositeCommand->AddCommand(std::move(command), execute);
	}

	void EditorCompositeCommandBuilder::Begin(std::string name)
	{
		if (mCurrentCompositeCommand)
		{
			mCurrentCompositeCommand->Begin(name);
		}
		else
		{
			mCurrentCompositeCommand = std::make_unique<CompositeCommandInternal>(std::string(name));
		}
	}

	std::optional<std::pair<EditorCompositeCommand, std::string>> EditorCompositeCommandBuilder::End()
	{
		if (!mCurrentCompositeCommand)
		{
			return std::nullopt;
		}

		eEndCode endCode = mCurrentCompositeCommand->End();

		if (endCode == eEndCode::Ended)
		{
			std::unique_ptr<CompositeCommandInternal> internalComposite = std::move(mCurrentCompositeCommand);
			mCurrentCompositeCommand.reset();
			return std::make_pair(internalComposite->Build(), internalComposite->GetName());
		}

		return std::nullopt;
	}

	EditorCompositeCommand EditorCompositeCommandBuilder::CompositeCommandInternal::Build() const
	{
		return EditorCompositeCommand(mCommands);
	}

	void EditorCompositeCommandBuilder::CompositeCommandInternal::AddCommand(EditorCommand command, const bool execute)
	{
		if (mCurrentChild)
		{
			mCurrentChild->AddCommand(std::move(command), execute);
		}
		else
		{
			mCommands.emplace_back(std::move(command));
		}
	}

	void EditorCompositeCommandBuilder::CompositeCommandInternal::Begin(std::string_view name)
	{
		if (mCurrentChild)
		{
			mCurrentChild->Begin(name);
		}
		else
		{
			mCurrentChild = std::make_unique<CompositeCommandInternal>(std::string(name));
		}
	}

	EditorCompositeCommandBuilder::eEndCode EditorCompositeCommandBuilder::CompositeCommandInternal::End()
	{
		if (mCurrentChild)
		{
			const eEndCode endCode = mCurrentChild->End();
			if (endCode == eEndCode::Ended)
			{
				const std::string compositeName = mCurrentChild->mName;
				mCommands.push_back(EditorCommand(mCurrentChild->Build(), compositeName));
				mCurrentChild.reset();
			}
			else if (endCode == eEndCode::EndedEmpty) // If the child's commands are empty we don't want to add the child to our commands
			{
				mCurrentChild.reset();
			}
			return eEndCode::InProgress;
		}
		else if (mCommands.empty())
		{
			return eEndCode::EndedEmpty;
		}

		return eEndCode::Ended;
	}
}