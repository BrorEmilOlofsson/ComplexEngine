#pragma once
#include "FlyDefines.hpp"
#include "SystemTypes/FlyVec2.hpp"
#include "Utilities/FlyMeta.hpp"
#include "Utilities/FlyUtilities.hpp"
#include "Proxy/FlyNodeProxy.hpp"
#include "Proxy/FlyVariableProxy.hpp"
#include "Proxy/FlyLinkProxy.hpp"
#include "Proxy/FlyFunctionProxy.hpp"
#include "Proxy/FlyCustomEventProxy.hpp"
#include "Proxy/FlyDataTypeProxy.hpp"
#include "Proxy/FlyStructProxy.hpp"
#include "Proxy/FlyClassProxy.hpp"
#include "Proxy/FlyGenericDataTypeProxy.hpp"
#include "Proxy/FlyNodeGraphProxy.hpp"
#include "Proxy/FlyClassInstanceProxy.hpp"
#include "Proxy/FlyProxy.hpp"
#include "SystemTypes/FlyNone.hpp"
#include "SystemTypes/FlyWildcard.hpp"
#include "Proxy/ProxyIterators/FlyDataTypeProxyIterator.hpp"
#include <unordered_map>
#include <functional>

namespace FLY_NAMESPACE
{

	class CommandTracker;

	void InitializeFoundation();
	void DestroyFoundation();

	void LoadAllFlyFiles(const std::filesystem::path& filePath);
	void SaveCustomEvents(const std::filesystem::path& filePath);

	GenericDataTypeProxy CreateStruct(std::string name, const std::filesystem::path& savePath);
	ClassProxy CreateClass(GenericDataTypeProxy targetProxy, std::string name, const std::filesystem::path& savePath);
	ClassProxy CreateClassWithoutTarget(std::string name, const std::filesystem::path& savePath);

	[[nodiscard]] GenericDataTypeProxy FindDataTypeByName(std::string_view name);
	[[nodiscard]] ClassProxy FindClassByName(std::string_view name);

	void SetDefaultDataTypeColor(const Fly::Color& color);
	void SetEditorTextFunction(const std::function<void(const std::string&)>& editorTextFunction);

	void CreateCopyBuffer(std::span<NodeID> nodeIDs, NodeGraphProxy copiedFromNodeGraph);
	void PasteCopyBuffer(NodeGraphProxy targetNodeGraphProxy, Vec2 position, CommandTracker* commandTracker);

	CustomEventProxy CreateCustomEvent(std::string name);
	FunctionProxy CreateGlobalFunction(std::string name);

	void BeginFrame(CommandTracker* commandTracker);
	[[nodiscard]] bool& IsDebugging();

	[[nodiscard]] std::vector<DataTypeProxy> GetDataTypes();
	[[nodiscard]] std::vector<GenericDataTypeProxy> GetGenericDataTypes();
	[[nodiscard]] DataTypeProxy GetDataTypeProxyByName(std::string_view name);

	[[nodiscard]] std::vector<NodeTypeProxy> GetNodeTypes();

	[[nodiscard]] std::vector<FunctionProxy> GetFunctions();
	[[nodiscard]] std::vector<CustomEventProxy> GetCustomEvents();

	[[nodiscard]] std::vector<LinkProxy> GetTraversedLinks();

	using NodeTypeProxyIterator = ProxyGlobalIterator<NodeTypeID, NodeTypeProxy>;
	using NodeTypeProxyIteratorService = ProxyGlobalIteratorService<NodeTypeID, NodeTypeProxyIterator>;

	[[nodiscard]] NodeTypeProxyIteratorService IterateNodeTypes();
	[[nodiscard]] NodeTypeProxyIteratorService IterateNodeTypes(Predicate<NodeTypeProxy> filterPredicate);

	[[nodiscard]] std::unordered_map<DataTypeProxy, std::vector<ClassProxy>> GetClasses();
	[[nodiscard]] std::vector<ClassProxy> GetClassesByTargetDataType(DataTypeProxy dataTypeProxy);

	[[nodiscard]] inline DataTypeProxyIteratorService IterateDataTypes()
	{
		return DataTypeProxyIteratorService();
	}

	[[nodiscard]] inline DataTypeProxyIteratorService IterateDataTypes(Predicate<DataTypeProxy> filterPredicate)
	{
		return DataTypeProxyIteratorService(filterPredicate);
	}

	template<IsPredicate<const GenericDataTypeProxy&> FilterPredicate>
	[[nodiscard]] std::vector<GenericDataTypeProxy> GetGenericDataTypesFiltered(FilterPredicate&& filterPredicate)
	{
		const std::vector<GenericDataTypeProxy> dataTypes = GetGenericDataTypes();

		std::vector<GenericDataTypeProxy> filtered;
		filtered.reserve(dataTypes.size());

		for (const GenericDataTypeProxy& dataType : dataTypes)
		{
			if (filterPredicate(dataType))
			{
				filtered.push_back(dataType);
			}
		}

		return filtered;
	}
}
