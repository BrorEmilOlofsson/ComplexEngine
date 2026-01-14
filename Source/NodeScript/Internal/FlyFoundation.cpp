#include "FlyFoundation.hpp"
#include "NodeTypes/SystemNodes.hpp"

namespace FLY_NAMESPACE
{
	void Foundation::Destroy()
	{
		delete sInstance;
		sInstance = nullptr;
	}

	Foundation::Foundation()
		: mMemoryPool(10000)
		, mNodeExecutor(mDataTypeManager, mNodeTypeManager, mPinTypeManager, mTraitManager, mMemoryPool)
	{
		
	}

	Foundation::~Foundation()
	{
	}

	void Foundation::Initialize()
	{
		Fly::RegisterType::ValueType<Wildcard>("Wildcard", Colors::Gray);
		Fly::RegisterType::ValueType<Flow>("Flow", eNodeOperatorType::None, Color(0.9f, 0.9f, 0.9f));
		Fly::RegisterType::PointerType<None>("None", Colors::Black);
		mNodeTypeManager.Assert();

		Internal::InitializeSubPins();

		TestUtilityFunction();
	}

	MemoryPool& Foundation::GetMemoryPool()
	{
		return mMemoryPool;
	}

	const MemoryPool& Foundation::GetMemoryPool() const
	{
		return mMemoryPool;
	}

	NodeTypeManager& Foundation::GetNodeTypeManager()
	{
		return mNodeTypeManager;
	}

	const NodeTypeManager& Foundation::GetNodeTypeManager() const
	{
		return mNodeTypeManager;
	}

	PinTypeManager& Foundation::GetPinTypeManager()
	{
		return mPinTypeManager;
	}

	const PinTypeManager& Foundation::GetPinTypeManager() const
	{
		return mPinTypeManager;
	}

	DataTypeManager& Foundation::GetDataTypeManager()
	{
		return mDataTypeManager;
	}

	const DataTypeManager& Foundation::GetDataTypeManager() const
	{
		return mDataTypeManager;
	}

	TraitManager& Foundation::GetTraitManager()
	{
		return mTraitManager;
	}

	const TraitManager& Foundation::GetTraitManager() const
	{
		return mTraitManager;
	}

	NodeExecutor& Foundation::GetNodeExecutor()
	{
		return mNodeExecutor;
	}

	const NodeExecutor& Foundation::GetNodeExecutor() const
	{
		return mNodeExecutor;
	}

	EventGraph& Foundation::GetNodeGraphCopy()
	{
		return mNodeGraphCopy;
	}

	const EventGraph& Foundation::GetNodeGraphCopy() const
	{
		return mNodeGraphCopy;
	}

	const VariableRef& Foundation::GetVariableRefByNodeRef(const GlobalNodeRef& nodeRef) const
	{
		return mNodeRefToVarRef.at(nodeRef);
	}

	std::vector<GlobalNodeRef> Foundation::GetNodeRefsByVariableRef(const VariableRef& varRef) const
	{
		std::vector<GlobalNodeRef> nodeIDs;
		for (auto& [nodeRef, varID] : mNodeRefToVarRef)
		{
			if (varID == varRef)
			{
				nodeIDs.push_back(nodeRef);
			}
		}
		return nodeIDs;
	}

	bool& Foundation::IsDebugging()
	{
		return mIsDebugging;
    }

	MemoryArena<1024>& Foundation::GetFrameMemoryArena()
	{
		return mFrameArena;
    }

	MemoryArena<10000>& Foundation::GetEditMemoryArena()
	{
        return mEditArena;
	}
}