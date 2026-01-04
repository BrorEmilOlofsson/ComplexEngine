#pragma once
#include <string>
#include "../FlyDefines.hpp"
#include "../CustomEvent/FlyCustomEvent.hpp"
#include "FlyNodeTypeProxy.hpp"

namespace FLY_NAMESPACE
{
	
	class DataTypeProxy;
	class GenericDataTypeProxy;
	class CommandTracker;

	class CustomEventProxy final
	{
	public:

		explicit CustomEventProxy(CustomEventID id);


		[[nodiscard]] NodeTypeProxy GetCallerNodeType() const;
		[[nodiscard]] NodeTypeProxy GetExecutorNodeType() const;
		[[nodiscard]] CustomEventID GetID() const;

		void SetName(std::string name, CommandTracker* commandTracker);
		void AddPin(GenericDataTypeProxy dataTypeProxy, std::string name, CommandTracker* commandTracker);
		void SetPinNameAtIndex(std::string name, std::size_t index, CommandTracker* commandTracker);
		void SetPinDataTypeAtIndex(GenericDataTypeProxy dataTypeProxy, std::size_t index, CommandTracker* commandTracker);
		void DeletePinAtIndex(std::size_t index, CommandTracker* commandTracker);

		[[nodiscard]] explicit operator bool() const;

		friend bool operator==(const CustomEventProxy& lhs, const CustomEventProxy& rhs);

	private:

		const CustomEvent& GetCustomEvent() const;


	private:

		CustomEventID mCustomEventID;
	};
}