#pragma once

namespace SimpleTracker
{

	class SimpleMemoryTrackerWrapper final
	{
		friend class SimpleMemoryTracker;
	public:
		SimpleMemoryTrackerWrapper();
		~SimpleMemoryTrackerWrapper();

	private:
		inline static bool myShouldActive = false;
	};

	class SimpleMemoryTrackerWrapperWrapper final
	{
		inline static SimpleMemoryTrackerWrapper myStaticMemoryTrackerWrapper;
	};
}

#define BeginMemoryTracking(SHOW_ADVANCED) SimpleTracker::SimpleMemoryTracker::StartMemoryTracking(SHOW_ADVANCED, std::source_location::current().function_name())
#define EndMemoryTracking() SimpleTracker::SimpleMemoryTracker::StopMemoryTracking()