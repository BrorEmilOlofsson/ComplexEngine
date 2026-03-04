#pragma once
#include "../FlyDefines.hpp"
#include "FlyDataType.hpp"
#include "FlyStruct.hpp"
#include "FlyClass.hpp"
#include "../Instance/FlyStructInstance.hpp"
#include "../Instance/FlyClassInstance.hpp"
#include "../Utilities/FlyMeta.hpp"
#include "../Utilities/FlyUtilities.hpp"
#include "../Memory/FlyMemoryArena.hpp"
#include "FlyGenericDataType.hpp"
#include "../Pin/FlyPinType.hpp"
#include <External/nlohmann/json.hpp>

namespace FLY_NAMESPACE
{

	constexpr const char* NullptrString = "Nullptr";

	template<typename T>
	constexpr InplaceAllocateInterface CreateInplaceAllocateInterface()
	{
		if constexpr (DefaultConstructible<T>)
		{
			return [](void* dataPtr, const void* defaultValuePtr) -> void
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
		}
		else
		{
			return nullptr;
		}
	}

	template<typename T>
	constexpr ReleaseInterface CreateReleaseInterface()
	{
		return [](void* const dataPtr) -> void
			{
				T& value = *reinterpret_cast<T*>(dataPtr);
				value.~T();
			};
	}

	template<Copyable T>
	constexpr CopyInterface CreateCopyInterface()
	{
		return [](void* destinationPtr, const void* sourcePtr)
			{
				T& destination = *reinterpret_cast<T*>(destinationPtr);
				const T& source = *reinterpret_cast<const T*>(sourcePtr);
				destination = source;
			};
	}

	template<std::swappable T>
	constexpr SwapInterface CreateSwapInterface()
	{
		return [](void* dataPtr1, void* dataPtr2)
			{
				T& value1 = *reinterpret_cast<T*>(dataPtr1);
				T& value2 = *reinterpret_cast<T*>(dataPtr2);

				std::swap(value1, value2);
			};
	}

	//template<typename T>
	//void DoSomething()
	//{
	//	if constexpr (/*T Is std::vector*/)
	//	{
	//		// Do something
	//	}
	//}

	template<typename T>
	constexpr EqualsInterface CreateEqualsInterface()
	{
		if constexpr (is_specialization_of_v<std::vector, T>)
		{
			return nullptr;
		}
		else
		{
			if constexpr (HasOperator_EqualTo<T>)
			{
				return [](const void* dataPtr1, const void* dataPtr2) -> bool
					{
						const T& value1 = *reinterpret_cast<const T*>(dataPtr1);
						const T& value2 = *reinterpret_cast<const T*>(dataPtr2);

						return value1 == value2;
					};
			}
			else
			{
				return nullptr;
			}
		}
	}

	template<typename T>
	constexpr ViewAndEditF CreateViewAndEditInterface()
	{
		if constexpr (ViewAndEditable<T>)
		{
			return ViewAndEditF([](void* dataPtr, EditorTextFunction) -> ViewAndEditResult
				{
					T& value = *reinterpret_cast<T*>(dataPtr);
					return ViewAndEdit(value);
				});
		}
		else if constexpr (PointerType<T>)
		{
			if constexpr (ViewAndEditable<std::remove_pointer_t<T>>)
			{
				return nullptr;
			}
			else
			{
				return [](void*, EditorTextFunction aTextFunction) -> ViewAndEditResult
					{
						//T& value = *reinterpret_cast<T*>(dataPtr);
						//if (value)
						//{
						//	//return ViewAndEdit(*value);
						//}
						//else if (editorNullptrFunction)
						//{
						if (aTextFunction)
						{
							aTextFunction(NullptrString);
						}
						//}

						return ViewAndEditResult{};
					};
			}
		}
		else if constexpr (Fundamental<T>)
		{
			return [](void* dataPtr, EditorTextFunction) -> ViewAndEditResult
				{
					T& value = *reinterpret_cast<T*>(dataPtr);
					return ::ViewAndEdit(value);
				};
		}
		else
		{
			return nullptr;
		}
	}

	template<typename T>
	constexpr ViewF CreateViewInterface()
	{
		if constexpr (Viewable<T>)
		{
			return [](const void* dataPtr, EditorTextFunction) -> void
				{
					const T& value = *reinterpret_cast<const T*>(dataPtr);
					View(value);
				};
		}
		else if constexpr (PointerType<T>)
		{
			if constexpr (Viewable<std::remove_pointer_t<T>>)
			{
				return [](const void* dataPtr, EditorTextFunction aTextFunction) -> void
					{
						const T& value = *reinterpret_cast<const T*>(dataPtr);

						if (value)
						{
							View(*value);
						}
						else if (aTextFunction)
						{
							aTextFunction(NullptrString);
						}
					};
			}
			else if constexpr (Fundamental<std::remove_pointer_t<T>>)
			{
				if constexpr (GlobalViewable<std::remove_pointer_t<T>>)
				{
					return [](const void* dataPtr, EditorTextFunction aTextFunction) -> void
						{
							const T& value = *reinterpret_cast<const T*>(dataPtr);

							if (value)
							{
								::View(*value);
							}
							else if (aTextFunction)
							{
								aTextFunction(NullptrString);
							}
						};
				}
				else
				{
					return nullptr;
				}
			}
			else
			{
				return nullptr;
			}

		}
		else if constexpr (Fundamental<T> && GlobalViewable<T>)
		{
			return [](const void* dataPtr, EditorTextFunction) -> void
				{
					const T& value = *reinterpret_cast<const T*>(dataPtr);

					::View(value);
				};
		}
		else
		{
			return nullptr;
		}
	}

	template<typename T>
	constexpr SaveF CreateSaveInterface()
	{
		if constexpr (Savable<T, nlohmann::json>)
		{
			return [](const void* dataPtr, nlohmann::json& json) -> void
				{
					const T& value = *reinterpret_cast<const T*>(dataPtr);
					Save(value, json);
				};
		}
		else if constexpr (Fundamental<T>)
		{
			return [](const void* dataPtr, nlohmann::json& json) -> void
				{
					const T& value = *reinterpret_cast<const T*>(dataPtr);
					::Save(value, json);
				};
		}
		else
		{
			return nullptr;
		}

	}

	template<typename T>
	constexpr LoadF CreateLoadInterface()
	{
		if constexpr (Loadable<T, nlohmann::json>)
		{
			return [](void* dataPtr, const nlohmann::json& json) -> void
				{
					T& value = *reinterpret_cast<T*>(dataPtr);
					Load(value, json);
				};
		}
		else if constexpr (Fundamental<T>)
		{
			return [](void* dataPtr, const nlohmann::json& json) -> void
				{
					T& value = *reinterpret_cast<T*>(dataPtr);
					::Load(value, json);
				};
		}
		else
		{
			return nullptr;
		}
	}

	template<typename T>
	constexpr ViewAndEditF CreateEditTemplateInterface()
	{
		return [](void* dataPtr) -> bool
			{
				T& value = *reinterpret_cast<T*>(dataPtr);
				return EditTemplate(value);
			};
	}

	template<typename T>
	constexpr SaveF CreateSaveTemplateInterface()
	{
		return [](nlohmann::json& json, const void* dataPtr) -> void
			{
				const T& value = *reinterpret_cast<const T*>(dataPtr);
				SaveTemplate(json, value);
			};
	}

	template<typename T>
	constexpr LoadF CreateLoadTemplateInterface()
	{
		return [](const nlohmann::json& json, void* dataPtr) -> void
			{
				T& value = *reinterpret_cast<T*>(dataPtr);
				LoadTemplate(json, value);
			};
	}

	template<typename T>
	constexpr FundamentalInterface CreateFundamentalInterface()
	{
		return FundamentalInterface
		{
			.mInplaceAllocateF = CreateInplaceAllocateInterface<T>(),
			.mReleaseF = CreateReleaseInterface<T>(),
			.mCopyF = CreateCopyInterface<T>(),
			.mSwapF = CreateSwapInterface<T>(),
			.mEqualsF = CreateEqualsInterface<T>()
		};
	}

	template<typename T>
	constexpr FunctionInterface CreateFunctionInterface()
	{
		return FunctionInterface
		{
			.mViewAndEditF = CreateViewAndEditInterface<T>(),
			.mViewF = CreateViewInterface<T>(),
			.mSaveF = CreateSaveInterface<T>(),
			.mLoadF = CreateLoadInterface<T>()
		};
	}

	template<typename T>
	constexpr ExecutionInterface CreateExecutionInterface()
	{
		return ExecutionInterface
		{
			.mSetInputPinValueF = CreateSetPinValueFunction<T, eIODirection::Input>(),
			.mSetOutputPinValueF = CreateSetPinValueFunction<T, eIODirection::Output>(),
			.mSetInputPinValueFromPinF = CreateSetPinValueFromPinFunction<T, eIODirection::Input>(),
			.mSetOutputPinValueFromPinF = CreateSetPinValueFromPinFunction<T, eIODirection::Output>(),
		};
	}

	template<typename T>
	constexpr DataTypeInterface CreateDataTypeInterface()
	{
		return DataTypeInterface(CreateFundamentalInterface<T>(), CreateFunctionInterface<T>(), CreateExecutionInterface<T>());
	}


	struct TypeParameters final
	{
		std::string mName;
		Color mColor;
		bool mIsTargetable = false;
		bool mRegisterPointer = true;
	};

	struct FlyCustomVectorType;

	class DataTypeManager final
	{
		friend class DataTypeRegistry;
	public:

		DataTypeManager();

	private:

		[[nodiscard]] ViewAndEditResult ViewAndEditData(const DataType& dataType, void* dataPtr, const bool viewAndEditMembers) const;
		void ViewData(const DataType& dataType, const void* dataPtr) const;
		bool SaveData(const DataType& dataType, const void* dataPtr, nlohmann::json& json) const;
		bool LoadData(const DataType& dataType, void* dataPtr, const nlohmann::json& json) const;

	public:

		[[nodiscard]] ViewAndEditResult ViewAndEditData(DataTypeID dataTypeID, void* dataPtr, const bool viewAndEditMembers = false) const;
		void ViewData(DataTypeID dataTypeID, const void* dataPtr) const;
		bool SaveData(DataTypeID dataTypeID, const void* dataPtr, nlohmann::json& json) const;
		bool LoadData(DataTypeID dataTypeID, void* dataPtr, const nlohmann::json& json) const;

		template<size_t BufferCapacity>
		[[nodiscard]] void* AllocateData(DataTypeID dataTypeID, MemoryArena<BufferCapacity>& arena, const void* defaultValue = nullptr) const;

		template<size_t BufferCapacity>
		[[nodiscard]] StructInstance* AllocateStructInstance(StructID structID, MemoryArena<BufferCapacity>& arena) const;

		template<size_t BufferCapacity>
		[[nodiscard]] ClassInstance* AllocateClassInstance(ClassID classID, MemoryArena<BufferCapacity>& arena) const;


		void ReleaseData(DataTypeID dataTypeID, void* dataPtr) const;

		void CopyData(DataTypeID dataTypeID, void* destination, const void* source) const;
		void SwapData(DataTypeID dataTypeID, void* dataPtr1, void* dataPtr2) const;
		[[nodiscard]] bool DataEqualsTo(DataTypeID dataTypeID, const void* dataPtr1, const void* dataPtr2) const;


		[[nodiscard]] ViewAndEditResult ViewAndEditData(GenericDataTypeID dataTypeID, void* dataPtr) const;
		void ViewData(GenericDataTypeID dataTypeID, const void* dataPtr) const;
		bool SaveData(GenericDataTypeID dataTypeID, const void* dataPtr, nlohmann::json& json) const;
		bool LoadData(GenericDataTypeID dataTypeID, void* dataPtr, const nlohmann::json& json) const;

		template<size_t BufferCapacity>
		[[nodiscard]] void* AllocateData(GenericDataTypeID dataTypeID, MemoryArena<BufferCapacity>& arena, const void* defaultValue = nullptr) const;
		void ReleaseData(GenericDataTypeID dataTypeID, void* dataPtr) const;

		void CopyData(GenericDataTypeID dataTypeID, void* destination, const void* source) const;
		void SwapData(GenericDataTypeID dataTypeID, void* dataPtr1, void* dataPtr2) const;
		[[nodiscard]] bool DataEqualsTo(GenericDataTypeID dataTypeID, const void* dataPtr1, const void* dataPtr2) const;

		[[nodiscard]] bool AreDataTypesRelated(DataTypeID dataTypeID1, DataTypeID dataTypeID2) const;
		[[nodiscard]] bool AreDataTypesRelated(GenericDataTypeID dataTypeID1, GenericDataTypeID dataTypeID2) const;
		[[nodiscard]] eDataTypeRelation GetDataTypeRelation(DataTypeID dataTypeID1, DataTypeID dataTypeID2) const;
		[[nodiscard]] eDataTypeRelation GetDataTypeRelation(GenericDataTypeID dataTypeID1, GenericDataTypeID dataTypeID2) const;

		[[nodiscard]] const std::string& GetName(GenericDataTypeID dataTypeID) const;

		[[nodiscard]] Color GetDataTypeColor(GenericDataTypeID dataTypeID) const;
		[[nodiscard]] eDataTypeTrait GetDataTypeTraits(GenericDataTypeID dataTypeID) const;
		[[nodiscard]] size_t GetDataTypeSize(GenericDataTypeID dataTypeID) const;
		[[nodiscard]] size_t GetDataTypeAlignment(GenericDataTypeID dataTypeID) const;

		[[nodiscard]] SetPinValueF GetSetPinValueFunction(DataTypeID dataTypeID, eIODirection ioDirection) const;
		[[nodiscard]] SetPinValueF GetSetPinValueFunction(GenericDataTypeID dataTypeID, eIODirection ioDirection) const;
		[[nodiscard]] SetPinValueFromPinF GetSetPinValueFromPinFunction(DataTypeID dataTypeID, eIODirection ioDirection) const;
		[[nodiscard]] SetPinValueFromPinF GetSetPinValueFromPinFunction(GenericDataTypeID dataTypeID, eIODirection ioDirection) const;

		[[nodiscard]] DataTypeID GetDataTypeIDByName(std::string_view name) const;
		[[nodiscard]] GenericDataTypeID GetGenericDataTypeIDByName(std::string_view name) const;

		[[nodiscard]] EditorTextFunction GetEditorTextFunction() const;
		void SetEditorTextFunction(EditorTextFunction textFunction);

		[[nodiscard]] const std::unordered_map<DataTypeID, DataType>& GetDataTypes() const;

		[[nodiscard]] DataType* Find(DataTypeID dataTypeID);
		[[nodiscard]] const DataType* Find(DataTypeID dataTypeID) const;
		[[nodiscard]] DataType* Find(ClassID classID);
		[[nodiscard]] const DataType* Find(ClassID classID) const;

		template<typename T>
		[[nodiscard]] DataType* Find();

		template<typename T>
		[[nodiscard]] const DataType* Find() const;

		[[nodiscard]] DataType* Find(GenericDataTypeID dataTypeID);
		[[nodiscard]] const DataType* Find(GenericDataTypeID dataTypeID) const;

		template<Decayed T>
		[[nodiscard]] bool IsRegistered() const;

		[[nodiscard]] bool IsRegistered(DataTypeID dataTypeID) const;

		void SetDataTypeColor(GenericDataTypeID dataTypeID, const Color& color);
		void SetDefaultColor(const Color& color);

		[[nodiscard]] Color GetDefaultColor() const;

		template<typename T>
		void Register(const TypeParameters& typeParameters);

		template<typename T>
		DataTypeID CreateDataTypeDuplication();

		DataTypeID CreateStruct(std::string name);

		ClassID CreateClass(DataTypeID targetID, std::string name);
		[[nodiscard]] Class& GetClass(ClassID classID);
		[[nodiscard]] const Class& GetClass(ClassID classID) const;
		[[nodiscard]] ClassID GetClassIDByName(std::string_view name) const;
		[[nodiscard]] const std::vector<HeapObject<Class>>& GetClasses() const;

	private:

		template<template<typename> typename TemplateType>
		void RegisterTemplateType(const std::string& aName);

		template<typename T>
		void RegisterTemplateSpecification(const std::string& aName, const Color& aColor);

		template<typename T>
		void RegisterTemplateSpecification(const std::string& aName);

		template<Decayed T>
		void RegisterInternal(const std::string& aName, const Color& aColor, const DataTypeInterface& anInterface, bool aIsTargetable);

		template<Decayed ClassType, Decayed MemberType>
		void RegisterMemberVariable(MemberType ClassType::* aMemberVariable, const std::string& aName);

	private:

		std::unordered_map<DataTypeID, DataType> mDataTypes;
		std::unordered_map<DataTypeID, TemplateDataType> mTemplateDataTypes;
		std::vector<HeapObject<DataType>> mClassesNew;
		std::vector<HeapObject<Class>> mClasses;

		std::string mNullNameStr;
		Color mDefaultColor = Color(0.3f, 0.78f, 0.72f, 1.f);
		Color mDefaultStructColor = Color(0.f, 0.6f, 1.f, 1.f);
		Color mDefaultClassColor = Color(1.f, 0.6f, 0.f, 1.f);

		DataTypeID mDataTypeIDCounter{ 0 };

		EditorTextFunction mEditorTextFunction = nullptr;
	};

	template<typename T>
	DataTypeID DataTypeManager::CreateDataTypeDuplication()
	{
		const DataType* dataType = Find<T>();

		if (!dataType)
		{
			return InvalidID<DataTypeID>();
		}

		const DataTypeID dataTypeID = mDataTypeIDCounter;
		mDataTypes.emplace(mDataTypeIDCounter, *dataType);

		mDataTypeIDCounter++;

		return dataTypeID;
	}

	template<typename T>
	inline void DataTypeManager::Register(const TypeParameters& aTypeParameters)
	{
		RegisterInternal<T>(aTypeParameters.mName, aTypeParameters.mColor, CreateDataTypeInterface<T>(), aTypeParameters.mIsTargetable);

		if constexpr (!PointerType<T>)
		{
			if (aTypeParameters.mRegisterPointer)
			{
				RegisterInternal<T*>(aTypeParameters.mName + " (Ptr)", aTypeParameters.mColor, CreateDataTypeInterface<T*>(), false);
			}
		}
	}

	template<template<typename> typename TemplateType>
	inline void DataTypeManager::RegisterTemplateType(const std::string& aName)
	{
		TemplateDataType templateObjectType
		{
			aName
		};
		mTemplateDataTypes.emplace(typeid(TemplateType).hash_code(), templateObjectType);
	}

	template<typename T>
	inline void DataTypeManager::RegisterTemplateSpecification(const std::string& aName, const Color& aColor)
	{
		DataTypeInterface dataTypeInterface
		{
			.function = FunctionInterface
			{
				.edit = CreateEditTemplateInterface<T>(),
				.save = CreateSaveTemplateInterface<T>(),
				.load = CreateLoadTemplateInterface<T>()
			},
			.fundamental = FundamentalInterface
			{
				.allocate = CreateAllocateInterface<T>(),
				.copy = CreateCopyInterface<T>(),
			}
		};
		RegisterInternal<T>(aName, aColor, dataTypeInterface);
	}

	template<typename T>
	inline void DataTypeManager::RegisterTemplateSpecification(const std::string& aName)
	{
		RegisterTemplateSpecification<T>(aName, mDefaultColor);
	}

	template<Decayed T>
	inline void DataTypeManager::RegisterInternal(const std::string& aName, const Color& aColor, const DataTypeInterface& aInterface, const bool aIsTargetable)
	{
		eDataTypeTrait typeTraits = eDataTypeTrait::None;
		if constexpr (Fundamental<T>)
		{
			typeTraits |= eDataTypeTrait::Fundamental;
		}
		if (aInterface.GetViewAndEditFunction())
		{
			typeTraits |= eDataTypeTrait::ViewAndEditable;
		}
		if (aInterface.GetSaveFunction() && aInterface.GetLoadFunction())
		{
			typeTraits |= eDataTypeTrait::SaveLoadable;
		}
		if (aIsTargetable)
		{
			typeTraits |= eDataTypeTrait::Targetable;
		}
		if constexpr (PointerType<T>)
		{
			typeTraits |= eDataTypeTrait::Pointer;
		}
		if constexpr (std::is_trivially_copyable_v<T>)
		{
			typeTraits |= eDataTypeTrait::TriviallyCopyable;
		}

		const std::type_info& typeInfo = typeid(T);
		DataType dataType
		{
			aName,
			sizeof(T),
			alignof(T),
			aColor,
			&typeInfo,
			aInterface,
			GetDataTypeID<T*>(),
			GetDataTypeID<std::remove_pointer_t<T>>(),
			typeTraits,
		};

		auto [it, success] = mDataTypes.emplace(GetDataTypeID<T>(), std::move(dataType));
		if (!success && aName != it->second.Name())
		{
			throw std::runtime_error("Two data types have the same hash value");
		}
	}

	template<Decayed ClassType, Decayed MemberType>
	inline void DataTypeManager::RegisterMemberVariable(MemberType ClassType::* aMemberVariable, const std::string& aName)
	{
		const size_t byteOffset = GetByteOffset(aMemberVariable);

		const DataTypeID dataTypeID = GetDataTypeID<MemberType>();

		DataType* const parentDataType = Find<ClassType>();

		if (parentDataType)
		{
			parentDataType->GetVariableContainer().AddVariable(Variable(GenericDataTypeID{dataTypeID}, aName, byteOffset));
		}
	}

	template<Decayed T>
	inline bool DataTypeManager::IsRegistered() const
	{
		return IsRegistered(GetDataTypeID<T>());
	}

	template<size_t BufferCapacity>
	inline void* DataTypeManager::AllocateData(const DataTypeID dataTypeID, MemoryArena<BufferCapacity>& arena, const void* const defaultValue) const
	{
		if (const DataType* dataType = Find(dataTypeID))
		{
			auto inplaceAllocateFunction = dataType->GetInterface().GetInplaceAllocateFunction();
			if (inplaceAllocateFunction)
			{
				void* dataPtr = arena.AllocateSizeAligned(dataType->GetSize(), dataType->GetAlignment());
				inplaceAllocateFunction(dataPtr, defaultValue);

				if (HasNotFlag(dataType->GetTypeTraits(), eDataTypeTrait::TriviallyCopyable))
				{
					arena.RegisterMemoryObject(dataPtr, dataTypeID);
				}

				return dataPtr;
			}
		}
		return nullptr;
	}

	template<size_t BufferCapacity>
	inline ClassInstance* DataTypeManager::AllocateClassInstance(const ClassID classID, MemoryArena<BufferCapacity>& arena) const
	{
		return &arena.template Allocate<ClassInstance>(classID);
	}

	template<size_t BufferCapacity>
	inline void* DataTypeManager::AllocateData(const GenericDataTypeID dataTypeID, MemoryArena<BufferCapacity>& arena, const void* defaultValue) const
	{
		return std::visit(Visitor{
			[this, &arena, defaultValue](const DataTypeID dataTypeID) -> void*
			{
				return AllocateData(dataTypeID, arena, defaultValue);
			},
			[this, &arena](const StructID structID) -> void*
			{
				return AllocateStructInstance(structID, arena);
			},
			[this, &arena](const ClassID classID) -> void*
			{
				return AllocateClassInstance(classID, arena);
			}
			}, dataTypeID.mID
		);
	}

	template<typename T>
	inline DataType* DataTypeManager::Find()
	{
		return Find(GetDataTypeID<T>());
	}

	template<typename T>
	inline const DataType* DataTypeManager::Find() const
	{
		return Find(GetDataTypeID<T>());
	}
}