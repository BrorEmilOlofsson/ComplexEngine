#include "NodeScript/Precompiled/NodeScriptPch.hpp"
#include "FlyTraitManager.hpp"

namespace FLY_NAMESPACE
{
	TraitID TraitManager::CreateTrait(std::string name)
	{
		const TraitID id{ mTraits.size() };
		mTraits.emplace_back(std::move(name));
		return id;
	}

	TraitID TraitManager::CreateTrait(std::string name, const DataTypeID aDataTypeID)
	{
		const TraitID id = CreateTrait(std::move(name));
		mTraitIDsByDataTypeID[aDataTypeID] = id;
		return id;
	}

	bool TraitManager::HasDataTypeTrait(const DataTypeID aDataTypeID, const TraitID aTraitID) const
	{
		auto it = mTraitsByDataTypeID.find(aDataTypeID);

		if (it == mTraitsByDataTypeID.end())
		{
			return false;
		}
		return it->second.contains(aTraitID);
	}

	void TraitManager::CreateTraitImplementation(const DataTypeID aDataTypeID, const TraitID aTraitID)
	{
		mTraitsByDataTypeID[aDataTypeID].emplace(aTraitID, TraitImplementation{});
	}
}
