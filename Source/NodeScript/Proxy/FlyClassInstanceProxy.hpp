#pragma once
#include "../FlyDefines.hpp"
#include "../Execution/FlyExecutionTypes.hpp"

namespace FLY_NAMESPACE
{

	class ClassInstance;
	class CommandTracker;
	class Class;

	class ClassInstanceProxy final
	{
	public:

		ClassInstanceProxy() = default;
		explicit ClassInstanceProxy(ClassInstance& classInstance);

		void InitRuntime();

		void ViewAndEditVariableDefaultValues(CommandTracker* commandTracker);
		void Destroy();

		template<typename EventFunction, typename TargetType>
		void ExecuteEvent(EventFunction eventFunction, TargetType* target, const ExecutionContextBase& context);

		template<typename TargetType>
		void ExecuteAutoTickers(TargetType* target, const ExecutionContextBase& context);

		[[nodiscard]] std::string_view GetName() const;

		[[nodiscard]] ClassInstance& GetClassInstance();


		explicit operator bool() const;

		friend bool operator==(const ClassInstanceProxy& a, const ClassInstanceProxy& b);


	private:

		[[nodiscard]] Class& GetClass() const;

		[[nodiscard]] bool IsSameTarget(DataTypeID dataTypeID) const;
		void ExecuteEventInternal(EventID eventID, void* target, const ExecutionContextBase& context);
		void ExecuteAutoTickersInternal(void* target, const ExecutionContextBase& context);

	private:

		ClassInstance* mClassInstance = nullptr;


	};

	template<typename EventFunction, typename TargetType>
	inline void ClassInstanceProxy::ExecuteEvent(EventFunction eventFunction, TargetType* target, const ExecutionContextBase& context)
	{
		const EventID eventID{ std::hash<EventFunction>()(eventFunction) };
		assert(IsSameTarget(GetDataTypeID<TargetType*>()));
		ExecuteEventInternal(eventID, target, context);
	}

	template<typename TargetType>
	inline void ClassInstanceProxy::ExecuteAutoTickers(TargetType* target, const ExecutionContextBase& context)
	{
		assert(IsSameTarget(GetDataTypeID<TargetType>()));
		ExecuteAutoTickersInternal(target, context);
	}
}