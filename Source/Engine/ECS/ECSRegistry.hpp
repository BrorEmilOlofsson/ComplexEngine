#pragma once
#include <unordered_map>
#include <span>
#include <vector>
#include <memory>

#include "ECSSystem.hpp"
#include "ComponentPool.hpp"
#include "EntityID.hpp"

namespace Simple
{

	struct ECSComponentType final
	{
		ComponentPoolSBO(*createComponentPoolFunction)() = nullptr;
		void(*copyComponentFunction)(void* destination, const void* source) = nullptr;
		void* (*addComponentFunction)(ECS& ecs, const EntityID entityID, const void* defaultValue) = nullptr;
		void(*removeComponentFunction)(ECS& ecs, const EntityID entityID) = nullptr;
		bool isDefault = false;
	};


	template<typename T>
	concept IsSystem = (StaticUpdateFunctionExists<T> || StaticRenderFunctionExists<T>) && sizeof(T) == 1;


	class ECSRegistry final
	{
	public:

		template<typename T>
		void RegisterComponentType(bool isDefault = false);

		template<IsSystem T>
		void RegisterSystem()
		{
			mSystemRegistrationFunctions.push_back([]() -> System
				{
					return System(std::type_identity<T>{});
				});
		}

		[[nodiscard]] std::span<const ECSComponentType> GetComponentTypes() const
		{
			return mComponentTypes;
		}

		[[nodiscard]] std::span<System(* const)()> GetSystemRegistrationFunctions() const
		{
			return mSystemRegistrationFunctions;
		}

		[[nodiscard]] std::size_t GetComponentTypeIndex(std::type_index typeIndex) const
		{
			return mComponentTypeToIDMap.at(typeIndex);
		}

		[[nodiscard]] const ECSComponentType& GetComponentType(std::type_index typeIndex) const
		{
			const std::size_t index = mComponentTypeToIDMap.at(typeIndex);
			return mComponentTypes[index - 1];
		}

		[[nodiscard]] void SetBlackboard(std::shared_ptr<Blackboard> blackboard)
		{
			mBlackboard = blackboard;
		}

		[[nodiscard]] const Blackboard& GetBlackboard() const
		{
			return *mBlackboard;
		}

		void InitializeEntity(ECS& ecs, EntityID entityID) const
		{
			for (const std::type_index& typeIndex : mDefaultComponentTypeIndices)
			{
				const ECSComponentType& componentType = GetComponentType(typeIndex);
				componentType.addComponentFunction(ecs, entityID, nullptr);
			}
		}

		static void Destroy()
		{
			sInstancePtr.reset();
		}

		class ECSRegistryProxy final
		{
		public:

			ECSRegistryProxy(std::unique_ptr<ECSRegistry>& instancePtr)
			{
				instancePtr = std::make_unique<ECSRegistry>();
			}
		};

		static ECSRegistry& Get()
		{
			static ECSRegistryProxy p(sInstancePtr);
			if (sInstancePtr == nullptr)
			{
				throw std::runtime_error("ECSRegistry instance is not initialized.");
            }
			return *sInstancePtr;
		}

	private:

		inline static std::unique_ptr<ECSRegistry> sInstancePtr = nullptr;

		std::unordered_map<std::type_index, std::size_t> mComponentTypeToIDMap;
		std::vector<ECSComponentType> mComponentTypes;
		std::vector<System(*)()> mSystemRegistrationFunctions;
		std::size_t mCurrentID = 1;
		std::shared_ptr<Blackboard> mBlackboard;
		std::vector<std::type_index> mDefaultComponentTypeIndices;
	};
}