#include "Editor/Precompiled/EditorPch.hpp"
#include "EditorCompositeCommandBuilder.hpp"

namespace Simple
{

	void EditorCompositeCommandBuilder::AddCommand(EditorCommand&& command)
	{
		mCurrentCompositeCommand->AddCommand(std::move(command));
	}

	void EditorCompositeCommandBuilder::Begin(std::string_view name)
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

	std::optional<EditorCompositeCommand> EditorCompositeCommandBuilder::End()
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
			return internalComposite->Build();
		}

		return std::nullopt;

	}

	EditorCompositeCommand EditorCompositeCommandBuilder::CompositeCommandInternal::Build() const
	{
		return EditorCompositeCommand(mName, mCommands);
	}

	void EditorCompositeCommandBuilder::CompositeCommandInternal::AddCommand(EditorCommand&& command)
	{
		if (mCurrentChild)
		{
			mCurrentChild->AddCommand(std::move(command));
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
			else if (endCode == eEndCode::Ended_Empty) // If the child's commands are empty we don't want to add the child to our commands
			{
				mCurrentChild.reset();
			}
			return eEndCode::InProgress;
		}
		else if (mCommands.empty())
		{
			return eEndCode::Ended_Empty;
		}

		return eEndCode::Ended;
	}
}