#pragma once
#include "../FlyDefines.hpp"
#include "../Memory/FlyMemoryArena.hpp"
#include "../DataType/FlyVariableRef.hpp"
#include "../Node/FlyNodeRef.hpp"
#include "../Graph/FlyEventGraph.hpp"
#include "../DataType/FlyDataTypeManager.hpp"
#include "../Node/FlyNodeTypeManager.hpp"
#include "../Pin/FlyPinTypeManager.hpp"
#include "../Trait/FlyTraitManager.hpp"
#include "../Execution/FlyNodeExecutor.hpp"
#include "../Memory/FlyHeapObject.hpp"
#include <memory>
#include <string_view>

namespace FLY_NAMESPACE
{
	class NodeExecutor;

	class Foundation final
	{
		struct FoundationProxy
		{
			FoundationProxy(Foundation*& foundationPtr)
				: mInstancePtr(foundationPtr)
			{
				foundationPtr = new Foundation();
			}

			~FoundationProxy()
			{
				delete mInstancePtr;
				mInstancePtr = nullptr;
			}

			Foundation*& mInstancePtr;
		};

	public:

		inline static Foundation& GetInstance()
		{
			static FoundationProxy p(sInstance);
			return *sInstance;
		}

		static void Destroy();

	private:

		Foundation();
		~Foundation();

	public:

		void Initialize();

		[[nodiscard]] MemoryPool& GetMemoryPool();
		[[nodiscard]] const MemoryPool& GetMemoryPool() const;
		[[nodiscard]] NodeTypeManager& GetNodeTypeManager();
		[[nodiscard]] const NodeTypeManager& GetNodeTypeManager() const;
		[[nodiscard]] PinTypeManager& GetPinTypeManager();
		[[nodiscard]] const PinTypeManager& GetPinTypeManager() const;
		[[nodiscard]] DataTypeManager& GetDataTypeManager();
		[[nodiscard]] const DataTypeManager& GetDataTypeManager() const;
		[[nodiscard]] TraitManager& GetTraitManager();
		[[nodiscard]] const TraitManager& GetTraitManager() const;
		[[nodiscard]] NodeExecutor& GetNodeExecutor();
		[[nodiscard]] const NodeExecutor& GetNodeExecutor() const;
		[[nodiscard]] EventGraph& GetNodeGraphCopy();
		[[nodiscard]] const EventGraph& GetNodeGraphCopy() const;
		const VariableRef& GetVariableRefByNodeRef(const GlobalNodeRef& aNodeRef) const;
		std::vector<GlobalNodeRef> GetNodeRefsByVariableRef(const VariableRef& aVarRef) const;

		// Editor only
		[[nodiscard]] bool& IsDebugging();
		[[nodiscard]] MemoryArena<1024>& GetFrameMemoryArena();
		[[nodiscard]] MemoryArena<10000>& GetEditMemoryArena();

	private:

		inline static Foundation* sInstance = nullptr;

	private:

		MemoryPool mMemoryPool;
		NodeTypeManager mNodeTypeManager;
		PinTypeManager mPinTypeManager;
		DataTypeManager mDataTypeManager;
		TraitManager mTraitManager;

		NodeExecutor mNodeExecutor;
		EventGraph mNodeGraphCopy;

		// Editor only
		bool mIsDebugging = false;
		MemoryArena<1024> mFrameArena;
		MemoryArena<10000> mEditArena;

	public:

		std::unordered_map<GlobalNodeRef, VariableRef, GlobalNodeRefHasher> mNodeRefToVarRef;
	};
}