#pragma once
#include <memory>
#include <typeindex>
#include "Utility/Blackboard.hpp"

namespace Simple
{
	class ECS;

	template<typename T>
	concept StaticBeginPlayFunctionExists = requires(ECS & ecs, const Blackboard & blackboard)
	{
		{ T::BeginPlay(ecs, blackboard) };
	};

	template<typename T>
	concept StaticEndPlayFunctionExists = requires(ECS & ecs, const Blackboard & blackboard)
	{
		{ T::EndPlay(ecs, blackboard) };
	};

	template<typename T>
	concept StaticEarlyUpdateFunctionExists = requires(ECS & ecs, const Blackboard & blackboard)
	{
		{ T::EndPlay(ecs, blackboard) };
	};

	template<typename T>
	concept StaticUpdateFunctionExists = requires(ECS & ecs, const float deltaTime, const Blackboard & blackboard)
	{
		{ T::Update(ecs, deltaTime, blackboard) };
	};

	template<typename T>
	concept StaticRenderFunctionExists = requires(const ECS & ecs, const Blackboard & blackboard)
	{
		{ T::Render(ecs, blackboard) };
	};

	template<typename T>
	concept StaticEditorUpdateFunctionExists = requires(ECS & ecs, const Blackboard & blackboard)
	{
		{ T::EditorUpdate(ecs, blackboard) };
	};


	using BeginPlayFunction = void(*)(ECS&, const Blackboard&);
	using EndPlayFunction = void(*)(ECS&, const Blackboard&);
	using EarlyUpdateFunction = void(*)(ECS&, const float, const Blackboard&);
	using UpdateFunction = void(*)(ECS&, const float, const Blackboard&);
	using RenderFunction = void(*)(const ECS&, const Blackboard&);
	using EditorUpdateFunction = void(*)(ECS&, const Blackboard&);

	template<typename T>
	[[nodiscard]] constexpr BeginPlayFunction CreateBeginPlayFunction()
	{
		if constexpr (StaticBeginPlayFunctionExists<T>)
		{
			return [](ECS& ecs, const Blackboard& blackboard)
				{
					T::BeginPlay(ecs, blackboard);
				};
		}
		else
		{
			return nullptr;
		}
	}

	template<typename T>
	[[nodiscard]] constexpr EndPlayFunction CreateEndPlayFunction()
	{
		if constexpr (StaticEndPlayFunctionExists<T>)
		{
			return [](ECS& ecs, const Blackboard& blackboard)
				{
					T::EndPlay(ecs, blackboard);
				};
		}
		else
		{
			return nullptr;
		}
	}

	template<typename T>
	[[nodiscard]] constexpr EarlyUpdateFunction CreateEarlyUpdateFunction()
	{
		if constexpr (StaticEarlyUpdateFunctionExists<T>)
		{
			return [](ECS& ecs, const float deltaTime, const Blackboard& blackboard)
				{
					T::EarlyUpdate(ecs, deltaTime, blackboard);
				};
		}
		else
		{
			return nullptr;
		}
	}

	template<typename T>
	[[nodiscard]] constexpr UpdateFunction CreateUpdateFunction()
	{
		if constexpr (StaticUpdateFunctionExists<T>)
		{
			return [](ECS& ecs, const float deltaTime, const Blackboard& blackboard)
				{
					T::Update(ecs, deltaTime, blackboard);
				};
		}
		else
		{
			return nullptr;
		}
	}

	template<typename T>
	[[nodiscard]] constexpr RenderFunction CreateRenderFunction()
	{
		if constexpr (StaticRenderFunctionExists<T>)
		{
			return [](const ECS& ecs, const Blackboard& blackboard)
				{
					T::Render(ecs, blackboard);
				};
		}
		else
		{
			return nullptr;
		}
	}

	template<typename T>
	[[nodiscard]] constexpr EditorUpdateFunction CreateEditorUpdateFunction()
	{
		if constexpr (StaticEditorUpdateFunctionExists<T>)
		{
			return [](ECS& ecs, const Blackboard& blackboard)
				{
					T::EditorUpdate(ecs, blackboard);
				};
		}
		else
		{
			return nullptr;
		}
	}

	class System final
	{
	public:

		template<typename T>
		explicit System(std::type_identity<T>)
			: mTypeIndex(typeid(T))
			, mBeginPlayFunction(CreateBeginPlayFunction<T>())
			, mEndPlayFunction(CreateEndPlayFunction<T>())
			, mEarlyUpdateFunction(CreateEarlyUpdateFunction<T>())
			, mUpdateFunction(CreateUpdateFunction<T>())
			, mRenderFunction(CreateRenderFunction<T>())
			, mEditorUpdateFunction(CreateEditorUpdateFunction<T>())
		{
		}

		void BeginPlay(ECS& ecs, const Blackboard& blackboard) const
		{
			if (mBeginPlayFunction)
			{
				mBeginPlayFunction(ecs, blackboard);
			}
		}

		void EndPlay(ECS& ecs, const Blackboard& blackboard) const
		{
			if (mEndPlayFunction)
			{
				mEndPlayFunction(ecs, blackboard);
			}
		}

		void EarlyUpdate(ECS& ecs, const float deltaTime, const Blackboard& blackboard) const
		{
			if (mEarlyUpdateFunction)
			{
				mEarlyUpdateFunction(ecs, deltaTime, blackboard);
			}
		}

		void Update(ECS& ecs, const float deltaTime, const Blackboard& blackboard) const
		{
			if (mUpdateFunction)
			{
				mUpdateFunction(ecs, deltaTime, blackboard);
			}
		}

		void Render(const ECS& ecs, const Blackboard& blackboard) const
		{
			if (mRenderFunction)
			{
				mRenderFunction(ecs, blackboard);
			}
		}

		void EditorUpdate(ECS& ecs, const Blackboard& blackboard) const
		{
			if (mEditorUpdateFunction)
			{
				mEditorUpdateFunction(ecs, blackboard);
			}
		}

		[[nodiscard]] constexpr std::type_index GetTypeIndex() const
		{
			return mTypeIndex;
		}

	private:

		std::type_index mTypeIndex;
		BeginPlayFunction mBeginPlayFunction;
		EndPlayFunction mEndPlayFunction;
		EarlyUpdateFunction mEarlyUpdateFunction;
		UpdateFunction mUpdateFunction;
		RenderFunction mRenderFunction;
		EditorUpdateFunction mEditorUpdateFunction;
	};
}