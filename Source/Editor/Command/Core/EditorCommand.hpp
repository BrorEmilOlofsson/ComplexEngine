#pragma once
#include <string>
#include <memory>
#include <concepts>

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
	using EditorCommandFunctionType = void(const T&);

	class EditorCommand final
	{
	public:

		EditorCommand() = default;

		template<typename T> requires EditorCommandable<T> || EditorMemberCommandable<T> || EditorInternalMemberCommandable<T>
		EditorCommand(const T & data, std::string name)
			: mConcept(std::make_unique<CommandModel<T>>(data, std::move(name)))
		{
		}

		template<typename T, std::invocable<const T&> DoFunc, std::invocable<const T&> UndoFunc>
		EditorCommand(const T& data, DoFunc doFunction, UndoFunc undoFunction, std::string name)
			: mConcept(std::make_unique<CommandModel<T, EditorCommandFunctionType<T>*, EditorCommandFunctionType<T>*>>(data, doFunction, undoFunction, std::move(name)))
		{
		}

		EditorCommand(const EditorCommand& other)
			: mConcept(other.mConcept ? other.mConcept->Clone() : nullptr)
		{
		}

		EditorCommand(EditorCommand&&) = default;

		EditorCommand& operator=(const EditorCommand& other)
		{
			EditorCommand temp(other);
			std::swap(mConcept, temp.mConcept);
			return *this;
		}

		EditorCommand& operator=(EditorCommand&&) = default;

		void ExecuteCommand() const;
		void UndoCommand() const;
		std::string ToString() const;

	private:

		class CommandConcept
		{
		public:

			virtual ~CommandConcept() = default;

			virtual void ExecuteCommand() const = 0;
			virtual void UndoCommand() const = 0;
			virtual std::string ToString() const = 0;

			virtual std::unique_ptr<CommandConcept> Clone() const = 0;

		};

		template<typename... T>
		class CommandModel;

		template<EditorCommandable T>
		class CommandModel<T> final : public CommandConcept
		{
		public:

			CommandModel(T data, std::string name)
				: mData(std::move(data))
                , mName(std::move(name))
			{
			}

			void ExecuteCommand() override
			{
				Do(mData);
			}

			void UndoCommand() const override
			{
				Undo(mData);
			}

			std::string ToString() const override
			{
				return mName;
            }

			std::unique_ptr<CommandConcept> Clone() const override
			{
				return std::make_unique<CommandModel<T>>(*this);
			}

		private:

			T mData;
			std::string mName;
		};

		template<EditorMemberCommandable T>
		class CommandModel<T> final : public CommandConcept
		{
		public:

			CommandModel(T data, std::string name)
				: mData(std::move(data))
				, mName(std::move(name))
			{
			}

			void ExecuteCommand() const override
			{
				mData.Execute();
			}

			void UndoCommand() const override
			{
				mData.Undo();
			}

			std::string ToString() const override
			{
				return mName;
			}

			std::unique_ptr<CommandConcept> Clone() const override
			{
				return std::make_unique<CommandModel<T>>(*this);
			}

		private:

			T mData;
			std::string mName;
		};

		template<EditorInternalMemberCommandable T>
		class CommandModel<T> final : public CommandConcept
		{
		public:

			explicit CommandModel(T data, std::string name)
				: mData(std::move(data))
				, mName(std::move(name))
			{
			}

			void ExecuteCommand() const override
			{
				mData.Execute();
			}

			void UndoCommand() const override
			{
				mData.Undo();
			}

			std::string ToString() const override
			{
				std::string result = mName;
				if constexpr (requires { mData.ToString(); })
				{
					result += mData.ToString();
				}
				return result;
			}

			std::unique_ptr<CommandConcept> Clone() const override
			{
				return std::make_unique<CommandModel<T>>(*this);
			}

		private:

			T mData;
			std::string mName;
		};

		template<typename T>
		class CommandModel<T, EditorCommandFunctionType<T>*, EditorCommandFunctionType<T>*> final : public CommandConcept
		{
			using FunctionType = EditorCommandFunctionType<T>*;
		public:

			CommandModel(T data, FunctionType executeFunction, FunctionType undoFunction, std::string name)
				: mData(std::move(data))
				, mExecuteFunction(executeFunction)
				, mUndoFunction(undoFunction)
				, mName(std::move(name))
			{
			}

			void ExecuteCommand() const override
			{
				mExecuteFunction(mData);
			}

			void UndoCommand() const override
			{
				mUndoFunction(mData);
			}

			std::string ToString() const override
			{
				return mName;
			}

			std::unique_ptr<CommandConcept> Clone() const override
			{
				return std::make_unique<CommandModel>(*this);
			}

		private:

			T mData;
			FunctionType mExecuteFunction;
			FunctionType mUndoFunction;
			std::string mName;
		};

	private:

		std::unique_ptr<CommandConcept> mConcept;
	};

}

template<>
struct std::formatter<CLX::EditorCommand> : std::formatter<std::string>
{
	auto format(const CLX::EditorCommand& command, auto& context) const
	{
        std::string s = command.ToString();
		return std::format_to(context.out(), "{}", s);
	}
};