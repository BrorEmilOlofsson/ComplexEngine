#pragma once
#include <unordered_set>
#include <unordered_map>
#include <string>

#include "../FlyDefines.hpp"
#include "FlyTrait.hpp"
#include "FlyTraitImplementation.hpp"

namespace FLY_NAMESPACE
{

	class TraitManager final
	{
	public:

		TraitManager() = default;

		TraitID CreateTrait(std::string name);
		TraitID CreateTrait(std::string name, DataTypeID dataTypeID);
		[[nodiscard]] bool HasDataTypeTrait(DataTypeID dataTypeID, TraitID traitID) const;
		void CreateTraitImplementation(DataTypeID dataTypeID, TraitID traitID);

		[[nodiscard]] Trait& GetTrait(TraitID traitID);
		[[nodiscard]] const Trait& GetTrait(TraitID traitID) const;
		[[nodiscard]] Trait& GetTrait(DataTypeID dataTypeID);
		[[nodiscard]] const Trait& GetTrait(DataTypeID dataTypeID) const;

		[[nodiscard]] TraitImplementation* GetTraitImplementation(DataTypeID dataTypeID, TraitID traitID);
		[[nodiscard]] const TraitImplementation* GetTraitImplementation(DataTypeID dataTypeID, TraitID traitID) const;

		const std::vector<Trait>& GetTraits() const;
		TraitID GetTraitIDByDataTypeID(DataTypeID dataTypeID) const;

	private:

		std::vector<Trait> mTraits;

		std::unordered_map<DataTypeID, std::unordered_map<TraitID, HeapObject<TraitImplementation>>> mTraitsByDataTypeID;

		std::unordered_map<DataTypeID, TraitID> mTraitIDsByDataTypeID;

	};

	inline TraitImplementation* TraitManager::GetTraitImplementation(const DataTypeID dataTypeID, const TraitID traitID)
	{
		auto it = mTraitsByDataTypeID.find(dataTypeID);
		if (it == mTraitsByDataTypeID.end())
		{
			return nullptr;
		}
		auto it2 = it->second.find(traitID);
		if (it2 == it->second.end())
		{
			return nullptr;
		}
		return it2->second.Get();
	}

	inline const TraitImplementation* TraitManager::GetTraitImplementation(DataTypeID dataTypeID, TraitID traitID) const
	{
		auto it = mTraitsByDataTypeID.find(dataTypeID);
		if (it == mTraitsByDataTypeID.end())
		{
			return nullptr;
		}
		auto it2 = it->second.find(traitID);
		if (it2 == it->second.end())
		{
			return nullptr;
		}
		return it2->second.Get();
	}


	inline Trait& TraitManager::GetTrait(const TraitID traitID)
	{
		return mTraits[traitID];
	}

	inline const Trait& TraitManager::GetTrait(const TraitID traitID) const
	{
		return mTraits[traitID];
	}

	inline Trait& TraitManager::GetTrait(const DataTypeID dataTypeID)
	{
		return GetTrait(GetTraitIDByDataTypeID(dataTypeID));
	}

	inline const Trait& TraitManager::GetTrait(const DataTypeID dataTypeID) const
	{
		return GetTrait(GetTraitIDByDataTypeID(dataTypeID));
	}

	inline const std::vector<Trait>& TraitManager::GetTraits() const
	{
		return mTraits;
	}

	inline TraitID TraitManager::GetTraitIDByDataTypeID(const DataTypeID dataTypeID) const
	{
		return mTraitIDsByDataTypeID.at(dataTypeID);
	}

}