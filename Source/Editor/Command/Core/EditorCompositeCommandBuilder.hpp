#pragma once
#include <string>
#include <memory>
#include <optional>
#include <string_view>
#include <vector>
#include "Editor/Command/Core/EditorCommand.hpp"
#include "Editor/Command/Core/EditorCompositeCommand.hpp"

namespace CLX
{
	class EditorCompositeCommandBuilder final
	{
	public:
		enum class eEndCode
		{
			Ended,
			InProgress,
			EndedEmpty,
		};
	public:

		EditorCompositeCommandBuilder() = default;

		void AddCommand(EditorCommand command, bool execute);

		void Begin(std::string_view name);
		[[nodiscard]] std::optional<EditorCompositeCommand> End();

		[[nodiscard]] bool IsActive() const
		{
			return mCurrentCompositeCommand != nullptr;
		}

	private:

		class CompositeCommandInternal final
		{
			friend class CompositeCommandBuilder;
		public:

			CompositeCommandInternal(std::string name = std::string())
				: mName(std::move(name))
			{
			}

			void AddCommand(EditorCommand command, bool execute);

			void Begin(std::string_view name);
			[[nodiscard]] eEndCode End();

			[[nodiscard]] EditorCompositeCommand Build() const;

		private:

			std::unique_ptr<CompositeCommandInternal> mCurrentChild;
			std::vector<EditorCommand> mCommands;
			std::string mName;
		};

		std::unique_ptr<CompositeCommandInternal> mCurrentCompositeCommand;
	};
}