#pragma once
#include <string>
#include <memory>

namespace CLX
{

	template<typename T>
	concept EditorCommandable = requires(const T & data)
	{
		{ Execute(data) };
		{ Undo(data) };
	};

	template<typename T>
	concept EditorMemberCommandable = requires(const T & data)
	{
		{ data.Execute() };
		{ data.Undo() };
	};

	template<typename T>
	concept EditorInternalMemberCommandable = requires(const T & data)
	{
		{ data.Execute(true) };
		{ data.Undo(true) };
	};

	template<typename T>
	using FunctionType = void(*)(const T&);

	class EditorCommand final
	{
	public:

		EditorCommand() = default;

		template<typename T> requires EditorCommandable<T> || EditorMemberCommandable<T> || EditorInternalMemberCommandable<T>
		EditorCommand(const T & data, const std::string & name)
			: mConcept(std::make_unique<CommandModel<T>>(data))
			, mName(name)
		{
		}


		template<typename T, std::invocable<const T&> DoFunc, std::invocable<const T&> UndoFunc>
		EditorCommand(const T& data, DoFunc doFunction, UndoFunc undoFunction, const std::string& name)
			: mConcept(std::make_unique<CommandModel<T, FunctionType<T>, FunctionType<T>>>(data, doFunction, undoFunction))
			, mName(name)
		{
		}

		EditorCommand(const EditorCommand& other)
			: mConcept(other.mConcept ? other.mConcept->Clone() : nullptr)
			, mName(other.mName)
		{
		}

		EditorCommand(EditorCommand&&) = default;

		EditorCommand& operator=(const EditorCommand& other)
		{
			EditorCommand temp(other);
			std::swap(mConcept, temp.mConcept);
			mName = other.mName;
			return *this;
		}

		EditorCommand& operator=(EditorCommand&&) = default;

		void ExecuteCommand(bool debugPrint) const;
		void UndoCommand(bool debugPrint) const;

	private:

		class CommandConcept
		{
		public:

			virtual ~CommandConcept() = default;

			virtual void ExecuteCommand(bool debugPrint) const = 0;
			virtual void UndoCommand(bool debugPrint) const = 0;

			virtual std::unique_ptr<CommandConcept> Clone() const = 0;

		};

		template<typename... T>
		class CommandModel;

		template<EditorCommandable T>
		class CommandModel<T> final : public CommandConcept
		{
		public:

			CommandModel(const T& data)
				: mData(data)
			{
			}

			void ExecuteCommand(const bool) const override
			{
				Do(mData);
			}

			void UndoCommand(const bool) const override
			{
				Undo(mData);
			}

			std::unique_ptr<CommandConcept> Clone() const override
			{
				return std::make_unique<CommandModel<T>>(*this);
			}

		private:

			T mData;
		};

		template<EditorMemberCommandable T>
		class CommandModel<T> final : public CommandConcept
		{
		public:

			CommandModel(const T& data)
				: mData(data)
			{
			}

			void ExecuteCommand(const bool) const override
			{
				mData.Execute();
			}

			void UndoCommand(const bool) const override
			{
				mData.Undo();
			}

			std::unique_ptr<CommandConcept> Clone() const override
			{
				return std::make_unique<CommandModel<T>>(*this);
			}

		private:

			T mData;
		};

		template<EditorInternalMemberCommandable T>
		class CommandModel<T> final : public CommandConcept
		{
		public:

			explicit CommandModel(const T& data)
				: mData(data)
			{
			}

			void ExecuteCommand(const bool debugPrint) const override
			{
				mData.Execute(debugPrint);
			}

			void UndoCommand(const bool debugPrint) const override
			{
				mData.Undo(debugPrint);
			}

			std::unique_ptr<CommandConcept> Clone() const override
			{
				return std::make_unique<CommandModel<T>>(*this);
			}

		private:

			T mData;
		};

		template<typename T>
		class CommandModel<T, FunctionType<T>, FunctionType<T>> final : public CommandConcept
		{
			using FunctionType = void(*)(const T&);
		public:

			CommandModel(const T& data, FunctionType executeFunction, FunctionType undoFunction)
				: mData(data)
				, mExecuteFunction(executeFunction)
				, mUndoFunction(undoFunction)
			{
			}

			void ExecuteCommand(const bool) const override
			{
				mExecuteFunction(mData);
			}

			void UndoCommand(const bool) const override
			{
				mUndoFunction(mData);
			}

			std::unique_ptr<CommandConcept> Clone() const override
			{
				return std::make_unique<CommandModel>(*this);
			}

		private:

			T mData;
			FunctionType mExecuteFunction;
			FunctionType mUndoFunction;
		};

	private:

		std::unique_ptr<CommandConcept> mConcept;
		std::string mName;
	};

}