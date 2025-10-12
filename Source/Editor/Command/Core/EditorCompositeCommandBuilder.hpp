#pragma once
#include <string>
#include <memory>
#include "Editor/Command/Core/EditorCommand.hpp"
#include "Editor/Command/Core/EditorCompositeCommand.hpp"

namespace Simple
{
	class EditorCompositeCommandBuilder final
	{
	public:
		enum class eEndCode
		{
			Ended,
			InProgress,
			Ended_Empty,
		};
	public:

		EditorCompositeCommandBuilder() = default;

		void AddCommand(EditorCommand&& aCommand);

		void Begin(std::string_view aName);
		[[nodiscard]] std::optional<EditorCompositeCommand> End();

		[[nodiscard]] bool IsActive() const
		{
			return myCurrentCompositeCommand.operator bool();
		}

	private:

		class CompositeCommandInternal final
		{
			friend class CompositeCommandBuilder;
		public:

			CompositeCommandInternal(std::string aName = std::string())
				: myName(aName)
			{

			}

			void AddCommand(EditorCommand&& aCommand);

			void Begin(std::string_view aName);
			[[nodiscard]] eEndCode End();

			[[nodiscard]] EditorCompositeCommand Build() const;

		private:

			std::unique_ptr<CompositeCommandInternal> myCurrentChild;
			std::vector<EditorCommand> myCommands;
			std::string myName;
		};

		std::unique_ptr<CompositeCommandInternal> myCurrentCompositeCommand;
	};
}