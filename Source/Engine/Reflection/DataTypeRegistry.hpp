#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <concepts>
#include <typeindex>
#include <External/nlohmann/json.hpp>
#include "Engine/ECS/ECS.hpp"
#include "Engine/Reflection/DataTypeID.hpp"
#include "Engine/Reflection/ViewAndEditResult.hpp"
#include "Engine/Utility/Algorithm.hpp"

namespace CLX
{
	using InPlaceAllocateFunction = void(*)(void* data, const void* defaultValuePtr);
	using DestroyFunction = void(*)(void* data);
	using CopyFunction = void (*)(void* destinationPtr, const void* sourcePtr);
	using SwapFunction = void (*)(void* dataPtr1, void* dataPtr2);

	template<typename T>
	concept Editable = requires(T & data, const Blackboard & blackboard)
	{
		{ ViewAndEditValue(data, blackboard) } -> std::same_as<ViewAndEditResult>;
	};

	template<typename T>
	concept Savable = requires(const T & data)
	{
		{ ToJSON(data) } -> std::same_as<nlohmann::json>;
	};

	template<typename T>
	concept Loadable = requires(T & data, const nlohmann::json & json, const Blackboard & blackboard)
	{
		{ FromJSON(data, json, blackboard) };
	};

	struct DataTypeMemberVariable final
	{
		std::string name = "Null";
		std::string customVariableName = "Null";
		DataTypeID dataTypeID = InvalidDataTypeID;
		std::size_t byteOffset = 0;
		bool shouldExpose = true;
		bool canEdit = true;
	};

	class DataType final
	{
	public:
		std::string name;
		std::string prettyName;
		std::vector<DataTypeMemberVariable> memberVariables;

		void* (*addComponentFunctionPointer)(ECS& ecs, const EntityID entityID) = nullptr;
		ViewAndEditResult(*viewAndEdit)(void* data, const Blackboard& blackboard) = nullptr;
		nlohmann::json(*toJSON)(const void* data) = nullptr;
		void (*fromJSON)(void* data, const nlohmann::json& json, const Blackboard& blackboard) = nullptr;

		InPlaceAllocateFunction inplaceAllocate = nullptr;
		DestroyFunction destroy = nullptr;
		CopyFunction copy = nullptr;
		SwapFunction swap = nullptr;

		std::size_t size = 0;
		std::size_t alignment = 1;
		const std::type_info* typeInfo = nullptr;
		bool isComponent = false;
		bool hasBeenAdded = false;
	};

	template<typename MemberType, typename OwnerType>
	[[nodiscard]] constexpr std::size_t GetByteOffset(MemberType OwnerType::* aMember)
	{
		return (std::size_t) & reinterpret_cast<const volatile char&>((((OwnerType*)0)->*aMember));
	}

	class DataTypeRegistry final
	{
		friend class __RegisterMemberVariable;
		friend class __RegisterComponent;
		template<typename T> friend class __RegisterDataType;

	public:

		ViewAndEditResult ViewAndEditData(DataTypeID dataTypeID, void* data, const Blackboard& blackboard) const;
		void LoadDataJSON(const DataType& dataType, void* dataPtr, const nlohmann::json& json, const Blackboard& blackboard) const;
		void LoadDataJSON(DataTypeID dataTypeID, void* dataPtr, const nlohmann::json& json, const Blackboard& blackboard) const;
		nlohmann::json SaveDataJSON(const DataType& dataType, const void* dataPtr) const;
		nlohmann::json SaveDataJSON(DataTypeID dataTypeID, const void* dataPtr) const;

		void* AddComponent(DataTypeID dataTypeID, ECS& ecs, EntityID entityID) const;

		void InplaceAllocateData(DataTypeID dataTypeID, void* destinationPtr, const void* defaultValuePtr = nullptr) const;
		void CopyData(DataTypeID dataTypeID, void* destinationPtr, const void* sourcePtr) const;
		void SwapData(DataTypeID dataTypeID, void* dataPtr1, void* dataPtr2) const;
		void DestroyData(DataTypeID dataTypeID, void* dataPtr) const;

		[[nodiscard]] DataType* Find(DataTypeID dataTypeID);
		[[nodiscard]] const DataType* Find(DataTypeID dataTypeID) const;

		[[nodiscard]] DataTypeID Find(std::string_view name) const;

		template<typename T>
		[[nodiscard]] DataType* Find();

		template<typename T>
		[[nodiscard]] const DataType* Find() const;

		template<typename Filter>
		[[nodiscard]] auto GetDataTypesFiltered(Filter&& filter) const;

	public:

		[[nodiscard]] size_t GetDataTypeSize(DataTypeID dataTypeID) const;
		[[nodiscard]] InPlaceAllocateFunction GetInplaceAllocateFunction(DataTypeID dataTypeID) const;
		[[nodiscard]] DestroyFunction GetDestroyFunction(DataTypeID dataTypeID) const;
		[[nodiscard]] CopyFunction GetCopyFunction(DataTypeID dataTypeID) const;

	public:

		static DataTypeRegistry& GetInstance();

		static void Destroy();

		void Assert() const;

		template<typename T>
		void RegisterComponentType(const bool isDefault);

		template<typename T>
		void RegisterDataType();

		template<typename MemberType, typename ParentType>
		void RegisterMemberVariable(MemberType ParentType::* variable, const std::string& variableName, const char* customName, const bool shouldExpose, const bool canEdit);

	public:
		DataTypeRegistry();

	private:
		inline static DataTypeRegistry* sInstance = nullptr;
	private:
		std::unordered_map<DataTypeID, DataType> mDataTypes;
		std::unordered_map<std::string, DataTypeID> mNameToID;
	};

	template<typename T>
	DataType* DataTypeRegistry::Find()
	{
		return Find(GetDataTypeID<T>());
	}

	template<typename T>
	const DataType* DataTypeRegistry::Find() const
	{
		return Find(GetDataTypeID<T>());
	}

	template<typename Filter>
	auto DataTypeRegistry::GetDataTypesFiltered(Filter&& filter) const
	{
		return mDataTypes | std::views::filter([filter](const auto& p) { return filter(p.second); });
	}

	template<typename T>
	void DataTypeRegistry::RegisterComponentType(const bool isDefault)
	{
		const DataTypeID dataTypeID = GetDataTypeID<T>();
		const bool alreadyExistOrHashCollision = mDataTypes.contains(dataTypeID);

		if (alreadyExistOrHashCollision)
		{
			if (mDataTypes[dataTypeID].hasBeenAdded)
			{
				return;
			}
			else
			{
				throw std::runtime_error("Hash collision between component types");
			}
		}

		DataType dataType;
		dataType.typeInfo = &typeid(T);
		dataType.hasBeenAdded = true;
		dataType.size = sizeof(T);
		dataType.alignment = alignof(T);

		dataType.name = ConvertTypeIndexNameToPrettyName(typeid(T).name());
		dataType.prettyName = RemoveSubStringIfExist(dataType.name, "Component");

		dataType.addComponentFunctionPointer = [](ECS& ecs, const EntityID entityID) -> void*
			{
				return &ecs.AddComponent<T>(entityID);
			};

		dataType.inplaceAllocate = [](void* dataPtr, const void* defaultValuePtr) -> void
			{
				if (defaultValuePtr != nullptr)
				{
					const T& defaultValue = *reinterpret_cast<const T*>(defaultValuePtr);
					new(dataPtr)T(defaultValue);
				}
				else
				{
					new(dataPtr)T();
				}
			};

		dataType.destroy = [](void* dataPtr) -> void
			{
				T& value = *reinterpret_cast<T*>(dataPtr);
				value.~T();
			};

		dataType.copy = [](void* destinationPtr, const void* sourcePtr) -> void
			{
				if constexpr (std::is_trivially_copyable_v<T>)
				{
					std::memcpy(destinationPtr, sourcePtr, sizeof(T));
				}
				else
				{
					T& destination = *reinterpret_cast<T*>(destinationPtr);
					const T& source = *reinterpret_cast<const T*>(sourcePtr);
					destination = source;
				}
			};

		dataType.swap = [](void* dataPtr1, void* dataPtr2) -> void
			{
				using std::swap;

				T& value1 = *reinterpret_cast<T*>(dataPtr1);
				T& value2 = *reinterpret_cast<T*>(dataPtr2);
				swap(value1, value2);
			};


		if constexpr (Editable<T>)
		{
			dataType.viewAndEdit = [](void* dataPointer, const std::string& aVariableName) -> bool
				{
					T* pointer = reinterpret_cast<T*>(dataPointer);
					return ViewAndEditValue(*pointer, aVariableName + "##" + std::to_string(reinterpret_cast<size_t>(dataPointer)));
				};
		}

		dataType.isComponent = true;

		mDataTypes[dataTypeID] = dataType;

		mNameToID[dataType.name] = dataTypeID;
		ECSRegistry::Get().RegisterComponentType<T>(isDefault);
	}

	// Concept that matches std::vector of any type and allocator
	template<typename T>
	concept IsVector = requires
	{
		typename T::value_type;
		requires std::same_as<T, std::vector<typename T::value_type, typename T::allocator_type>>;
	};

	template<typename T>
	void DataTypeRegistry::RegisterDataType()
	{
		const DataTypeID dataTypeID = GetDataTypeID<T>();
		const bool alreadyExistOrHashCollision = mDataTypes.contains(dataTypeID);

		if (alreadyExistOrHashCollision)
		{
			assert(false && "Component already exist or has hash collision!");
			return;
		}

		DataType dataType;

		dataType.name = ConvertTypeIndexNameToPrettyName(typeid(T).name());

		if constexpr (Editable<T>)
		{
			dataType.viewAndEdit = [](void* dataPtr, const Blackboard& blackboard) -> ViewAndEditResult
				{
					T* pointer = reinterpret_cast<T*>(dataPtr);
					return ViewAndEditValue(*pointer, blackboard);
				};
		}

		if constexpr (std::is_fundamental_v<T>)
		{
			dataType.toJSON = [](const void* dataPtr) -> nlohmann::json
				{
					const T* pointer = reinterpret_cast<const T*>(dataPtr);
					return ::ToJSON(*pointer);
				};
		}
		else if constexpr (Savable<T>)
		{

			dataType.toJSON = [](const void* dataPtr) -> nlohmann::json
				{
					const T* pointer = reinterpret_cast<const T*>(dataPtr);
					return ToJSON(*pointer);
				};
		}

		if constexpr (std::is_fundamental_v<T>)
		{
			dataType.fromJSON = [](void* dataPtr, const nlohmann::json& json, const Blackboard&)
				{
					T* pointer = reinterpret_cast<T*>(dataPtr);
					::FromJSON(*pointer, json);
				};
		}
		else if constexpr (Loadable<T>)
		{
			dataType.fromJSON = [](void* dataPtr, const nlohmann::json& json, const Blackboard& blackboard)
				{
					T* pointer = reinterpret_cast<T*>(dataPtr);
					FromJSON(*pointer, json, blackboard);
				};
		}

		mDataTypes[dataTypeID] = dataType;


		if constexpr (!IsVector<T>)
		{
			RegisterDataType<std::vector<T>>();
		}
	}

	template<typename MemberType, typename OwnerType>
	void DataTypeRegistry::RegisterMemberVariable(MemberType OwnerType::* variable, const std::string& variableName, const char* customName, const bool shouldExpose, const bool canEdit)
	{
		DataTypeMemberVariable componentProperty;
		componentProperty.name = variableName;
		componentProperty.dataTypeID = GetDataTypeID<MemberType>();
		componentProperty.byteOffset = GetByteOffset(variable);
		componentProperty.shouldExpose = shouldExpose;
		componentProperty.canEdit = canEdit;

		if (customName != nullptr)
		{
			componentProperty.customVariableName = customName;
		}
		else
		{
			componentProperty.customVariableName = ConvertAndAddSpaceToSubStringWithUpperCase(variableName);
		}

		{
			const DataType* ownerDataType = Find<OwnerType>();

			if (ownerDataType == nullptr)
			{
				RegisterDataType<OwnerType>();
			}
		}

		auto& ownerMembers = Find<OwnerType>()->memberVariables;
		auto it = std::ranges::find_if(ownerMembers, [&variableName](const DataTypeMemberVariable& obj) { return obj.name == variableName; });

		if (it == end(ownerMembers))
		{
			ownerMembers.push_back(componentProperty);
		}
	}
}