#pragma once
#include <cstdint>
#include <unordered_map>
#include <span>
#include <vector>
#include <memory>
#include <type_traits>

#include "ECSSystem.hpp"
#include "ComponentPool.hpp"
#include "EntityID.hpp"
#include "Engine/Reflection/DataTypeID.hpp"
#include "Engine/Utility/EnumUtility.hpp"
#include "Engine/Utility/DynamicBitset.hpp"

namespace CLX
{

	struct ECSRegisterComponentHelperNew
	{
		template<typename T>
		inline static std::size_t ComponentPoolID{ 0 };
	};

	enum class eECSComponentTraitNew : uint8_t
	{
		None = 0,
		Default = 1 << 0,
		NoManualAdd = 1 << 1
	};

	template<>
	struct IsEnumBitfield<eECSComponentTraitNew> : std::true_type {};


	struct ECSComponentTypeNew final
	{
		ComponentPoolSBO(*createComponentPoolFunction)() = nullptr;
		void(*copyComponentFunction)(void* destination, const void* source) = nullptr;
		void* (*addComponentFunction)(class ECSNew& ecs, const EntityID entityID, const void* defaultValue) = nullptr;
		void(*removeComponentFunction)(class ECSNew& ecs, const EntityID entityID) = nullptr;
		eECSComponentTraitNew traits = eECSComponentTraitNew::None;
	};

	template<typename T>
	concept IsSystem = (StaticUpdateFunctionExists<T> || StaticRenderFunctionExists<T>) && sizeof(T) == 1;

	class ECSRegistryNew final
	{
	public:

		template<typename T>
		void RegisterComponentType(eECSComponentTraitNew traits = eECSComponentTraitNew::None);

		template<IsSystem T>
		void RegisterSystem()
		{
			mSystemRegistrationFunctions.push_back([]() -> System
				{
					return System(std::type_identity<T>{});
				});
		}

		[[nodiscard]] std::span<const ECSComponentTypeNew> GetComponentTypes() const
		{
			return mComponentTypes;
		}

        size_t GetComponentTypeIndex(const std::type_index typeIndex) const
        {
            return mComponentTypeToIDMap.at(typeIndex);
        }

		[[nodiscard]] const ECSComponentTypeNew& GetComponentType(const std::type_index typeIndex) const
		{
            const std::size_t componentTypeIndex = GetComponentTypeIndex(typeIndex);
            return mComponentTypes[componentTypeIndex];
		}

		[[nodiscard]] std::span<System(* const)()> GetSystemRegistrationFunctions() const
		{
			return mSystemRegistrationFunctions;
		}

		void SetBlackboard(std::shared_ptr<Blackboard> blackboard)
		{
			mBlackboard = blackboard;
		}

		[[nodiscard]] const Blackboard& GetBlackboard() const
		{
			return *mBlackboard;
		}

		void InitializeEntity(class ECSNew& ecs, EntityID entityID) const
		{
			for (const std::type_index& typeIndex : mDefaultComponentTypeIndices)
			{
				const ECSComponentTypeNew& componentType = GetComponentType(typeIndex);
				componentType.addComponentFunction(ecs, entityID, nullptr);
			}
		}

		template<typename... Types> requires (sizeof...(Types) > 0)
			[[nodiscard]] DynamicBitset CreateComponentMaskFromTypeList() const
		{
			DynamicBitset mask = DynamicBitset::FromSize(GetComponentTypes().size(), false);
			mask[GetComponentTypeIndex(typeid(Types))...] = true;
			return mask;
		}

	private:

        std::unordered_map<std::type_index, std::size_t> mComponentTypeToIDMap;
		std::vector<ECSComponentTypeNew> mComponentTypes;
		std::vector<System(*)()> mSystemRegistrationFunctions;
		std::size_t mCurrentID = 0;
		std::shared_ptr<Blackboard> mBlackboard;
		std::vector<std::type_index> mDefaultComponentTypeIndices;
	};
}