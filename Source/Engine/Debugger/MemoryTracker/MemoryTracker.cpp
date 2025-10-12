#include "Engine/Precompiled/EnginePch.hpp"
#include "Engine/Debugger/MemoryTracker/MemoryTracker.h"
#include "Utility/Algorithm.hpp"
#include "External/nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include "External/TheGameAssembly/StackTrace/StackTrace.h"
#include <mutex>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <atomic>
#include <string>
#include <source_location>
#include <memory>


typedef struct _SimpleCrtMemBlockHeader
{
	struct _SimpleCrtMemBlockHeader* pBlockHeaderNext;
	struct _SimpleCrtMemBlockHeader* pBlockHeaderPrev;
	char* szFileName;
	int                         nLine;
#ifdef _WIN64
	/* These items are reversed on Win64 to eliminate gaps in the struct
	 * and ensure that sizeof(struct)%16 == 0, so 16-byte alignment is
	 * maintained in the debug heap.
	 */
	int                         nBlockUse;
	size_t                      nDataSize;
#else 
	size_t                      nDataSize;
	int                         nBlockUse;
#endif
	long                        lRequest;
	unsigned char               gap[4];
} _SimpleCrtMemBlockHeader;


namespace SimpleTracker
{

	class SimpleMemoryTracker;
	class SimpleMemoryTrackerWrapper;

	struct MemoryTrackingSettings final
	{
		bool shouldStoreStackTraces;
		bool shouldTrackAllAllocations;
	};

	struct Allocation final
	{
		size_t size = 0;
		StackTrace stackTrace;
	};

	class TemporaryScopedMemoryTrackerData final
	{
		friend class SimpleTracker::SimpleMemoryTracker;
	public:
		std::unordered_map<StackTrace, int> stackTraceToAllocationCount;
		std::unordered_map<long, Allocation> allocationMap;
		MemoryTrackingSettings memoryTrackingSettings = { false, false };
		std::atomic<bool> hasStarted = false;
		int totalAllocationCount = 0;

	public:

		~TemporaryScopedMemoryTrackerData() = default;

	private:
		static TemporaryScopedMemoryTrackerData& GetInstance()
		{
			std::call_once(myInitInstanceFlag, &TemporaryScopedMemoryTrackerData::Init); //TO-DO(v11.2.1): learn more about this, as this was guided by ChatGPT and I have no clue how this work but it works
			return *myInstance;
		}

		TemporaryScopedMemoryTrackerData() {};

		static void Init()
		{
			myInstance.reset(new TemporaryScopedMemoryTrackerData());
		}

	private:
		inline static std::unique_ptr<TemporaryScopedMemoryTrackerData> myInstance;
		inline static std::once_flag myInitInstanceFlag;
	};

	class SimpleMemoryTracker final
	{
		friend class SimpleTracker::SimpleMemoryTrackerWrapper;

	public:
		static void StartMemoryTracking(const bool aShowAdvanced = false, const std::string& aCallerName = "");
		static void StopMemoryTracking();
	private:
		static void Init();
		static void Destroy();

		static int AllocHook(int aAllocType, void* aUserData, size_t aSize, int aBlockType, long aRequestNumber, const unsigned char*, int);
		static void PrintTopLeaks(std::unordered_map<StackTrace, int>& aMap, const bool aScopedTracking);
		static void PrintShortLivedToOutput();
		static void WriteToTxtFile(const std::vector<std::pair<StackTrace, int>>& aStackTraces, const bool aScopedTracking);
	private:
		inline static std::mutex myStaticAllocationMapMutex{};
		inline static std::unordered_map<StackTrace, int> myStaticStackTraceToAllocationCount{};
		inline static std::unordered_map<long, Allocation> myStaticAllocationMap{};
		inline static int myStaticTotalAllocationCount = 0;
		inline static thread_local bool myStaticIsAllocationInProgress = false;
		inline static MemoryTrackingSettings myStaticMemoryTrackingSettings = { false, false };
	};


	char localBuffer[255];

#ifdef _DEBUG
	int SimpleMemoryTracker::AllocHook(int aAllocType, void* aUserData, size_t aSize, int aBlockType, long aRequestNumber, const unsigned char*, int)
	{
		if (aBlockType == _CRT_BLOCK || myStaticIsAllocationInProgress)
			return 1;

		// Do not track allocations by the memory tracker itself
		myStaticIsAllocationInProgress = true;

		std::lock_guard<std::mutex> guard(myStaticAllocationMapMutex);

		bool alloc = false;
		bool free = false;

		switch (aAllocType)
		{
		case _HOOK_ALLOC:
			alloc = true;
			break;
		case _HOOK_REALLOC:
			alloc = true;
			free = true;
			break;
		case _HOOK_FREE:
			free = true;
			break;
		}

		if (alloc)
		{
			Allocation& entry = myStaticAllocationMap[aRequestNumber];

			if (myStaticMemoryTrackingSettings.shouldStoreStackTraces)
			{
				entry.stackTrace = StackTrace::CaptureStackTrace(1);
				myStaticStackTraceToAllocationCount[entry.stackTrace]++;
			}

			entry.size = aSize;
			SimpleMemoryTracker::myStaticTotalAllocationCount++;

			if (TemporaryScopedMemoryTrackerData::GetInstance().hasStarted)
			{
				Allocation& entry2 = TemporaryScopedMemoryTrackerData::GetInstance().allocationMap[aRequestNumber];

				if (TemporaryScopedMemoryTrackerData::GetInstance().memoryTrackingSettings.shouldStoreStackTraces)
				{
					entry2.stackTrace = StackTrace::CaptureStackTrace(1);
					TemporaryScopedMemoryTrackerData::GetInstance().stackTraceToAllocationCount[entry2.stackTrace]++;
				}

				entry2.size = aSize;
				TemporaryScopedMemoryTrackerData::GetInstance().totalAllocationCount++;
			}
		}

		if (free)
		{
			// read request number from header
			const size_t headerSize = sizeof(_SimpleCrtMemBlockHeader);
			const size_t ptr = (size_t)aUserData - headerSize;

			_SimpleCrtMemBlockHeader* pHead;
			pHead = (_SimpleCrtMemBlockHeader*)(ptr);

			long freeRequestNumber = pHead->lRequest;

			myStaticStackTraceToAllocationCount.erase(myStaticAllocationMap[freeRequestNumber].stackTrace);
			myStaticAllocationMap.erase(freeRequestNumber);

			if (TemporaryScopedMemoryTrackerData::GetInstance().hasStarted)
			{
				if (myStaticMemoryTrackingSettings.shouldStoreStackTraces)
				{
					TemporaryScopedMemoryTrackerData::GetInstance().stackTraceToAllocationCount.erase(TemporaryScopedMemoryTrackerData::GetInstance().allocationMap[freeRequestNumber].stackTrace);
				}

				TemporaryScopedMemoryTrackerData::GetInstance().allocationMap.erase(freeRequestNumber);
			}
		}

		myStaticIsAllocationInProgress = false;
		return 1;
	}

	void SimpleMemoryTracker::Init()
	{
		const std::filesystem::path filePath = std::filesystem::absolute(SIMPLE_SETTINGS_DEBUG);

		std::ifstream file(filePath);

		if (file.is_open() == false)
		{
			const std::string fileName = SIMPLE_FILENAME_DEBUGJSON;
			const std::filesystem::path source = std::filesystem::absolute(SIMPLE_DIR_DEPENDENCIES_SETTINGS) / fileName;
			const std::filesystem::path destination = std::filesystem::absolute(SIMPLE_DIR_SETTINGS) / fileName;
			std::filesystem::copy_file(source, destination, std::filesystem::copy_options::overwrite_existing);
		}

		file.open(filePath);
		assert(file.is_open() && "Failed to open file");

		const nlohmann::json json = nlohmann::json::parse(file);
		file.close();

		const nlohmann::json memoryTrackerSettings = json["Debug_Settings"]["MemoryTracker"];

		SimpleMemoryTrackerWrapper::myShouldActive = memoryTrackerSettings["Active"];

		if (SimpleMemoryTrackerWrapper::myShouldActive == false)
		{
			return;
		}

		const MemoryTrackingSettings& trackingSettings = { memoryTrackerSettings["Advanced"], true };

		myStaticMemoryTrackingSettings = trackingSettings;
		_CrtSetAllocHook(&SimpleMemoryTracker::AllocHook);
	}

	void SimpleMemoryTracker::PrintTopLeaks(std::unordered_map<StackTrace, int>& aMap, const bool aScopedTracking)
	{
		std::vector<std::pair<StackTrace, int>> pairs;

		for (const auto& p : aMap)
		{
			pairs.push_back(p);
		}

		std::sort(pairs.begin(), pairs.end(), [](const auto& a, const auto& b) {return a.second > b.second; });

		OutputDebugStringA("================================================================================\n");
		int i = 0;
		for (const auto& p : pairs)
		{
			if (i >= 5)
				break;

			char buffer[100];
			sprintf_s(buffer, "Count: %d\n, Stack Trace:", p.second);
			OutputDebugStringA(buffer);
			p.first.Print();

			OutputDebugStringA("================================================================================\n");

			i++;
		}

		WriteToTxtFile(pairs, aScopedTracking);
	}

	void SimpleMemoryTracker::Destroy()
	{
		if (SimpleMemoryTrackerWrapper::myShouldActive == false)
		{
			return;
		}

		TemporaryScopedMemoryTrackerData::GetInstance().hasStarted = false;
		TemporaryScopedMemoryTrackerData::GetInstance().allocationMap.clear();
		TemporaryScopedMemoryTrackerData::GetInstance().stackTraceToAllocationCount.clear();
		TemporaryScopedMemoryTrackerData::GetInstance().totalAllocationCount = 0;

		std::lock_guard<std::mutex> guard(myStaticAllocationMapMutex);

		_CrtSetAllocHook(nullptr);

		if (myStaticAllocationMap.size() == 0)
		{
			if (myStaticMemoryTrackingSettings.shouldTrackAllAllocations)
			{
				OutputDebugStringA("================================================================================\n");
				char buffer[100];
				sprintf_s(buffer, "== Total Allocation Count: %d\n", myStaticTotalAllocationCount);
				OutputDebugStringA(buffer);
			}

			OutputDebugStringA("================================================================================\n");
			OutputDebugStringA("== No memory leaks found! \n");
			OutputDebugStringA("================================================================================\n");
		}
		else
		{
			if (myStaticMemoryTrackingSettings.shouldStoreStackTraces)
			{
				OutputDebugStringA("================================================================================\n");
				std::unordered_map<StackTrace, int> stackTraceToLeakCountMap;

				for (const auto& p : myStaticAllocationMap)
				{
					stackTraceToLeakCountMap[p.second.stackTrace]++;
				}

				OutputDebugStringA("== Top 5 Leaks: \n");
				PrintTopLeaks(stackTraceToLeakCountMap, false);
			}

			if (myStaticMemoryTrackingSettings.shouldTrackAllAllocations)
			{
				OutputDebugStringA("================================================================================\n");
				char buffer[100];
				sprintf_s(buffer, "== Total Allocation Count: %d\n", myStaticTotalAllocationCount);
				OutputDebugStringA(buffer);
			}

			OutputDebugStringA("================================================================================\n");
			char buffer[100];
			sprintf_s(buffer, "== Total Number of Memory Leaks: %d\n", (int)myStaticAllocationMap.size());
			OutputDebugStringA(buffer);
			OutputDebugStringA("================================================================================\n");
			OutputDebugStringA("== For more details modify SimpleTracker::MemoryTrackingSettings\n");
			OutputDebugStringA("== SimpleTracker::MemoryTrackingSettings({ StoreStackTraces(Very slow, gives precise info), TrackAllAllocations(decently fast, lots of spam) });\n");
			OutputDebugStringA("================================================================================\n");
		}

		myStaticAllocationMap.clear();
		myStaticStackTraceToAllocationCount.clear();
	}

	void SimpleMemoryTracker::StartMemoryTracking(const bool aShowAdvanced, const std::string& aCallerName)
	{
		if (SimpleMemoryTrackerWrapper::myShouldActive == false)
		{
			return;
		}

		TemporaryScopedMemoryTrackerData::GetInstance();

		//if (myScopedTrackerData == nullptr)
		//{
		//	myScopedTrackerData = new TemporaryScopedMemoryTrackerData();
		//}
		//else
		//{
		//	return; //TO-DO(v10.0.0): Better error and warning messages
		//}

		sprintf_s(localBuffer, std::string("== " + aCallerName).c_str());

		TemporaryScopedMemoryTrackerData::GetInstance().hasStarted = true;
		TemporaryScopedMemoryTrackerData::GetInstance().memoryTrackingSettings = { aShowAdvanced, true };
	}

	void SimpleMemoryTracker::StopMemoryTracking()
	{
		if (SimpleMemoryTrackerWrapper::myShouldActive == false)
		{
			return;
		}

		if (TemporaryScopedMemoryTrackerData::GetInstance().hasStarted == false)
		{
			return;
		}

		PrintShortLivedToOutput();

		TemporaryScopedMemoryTrackerData::GetInstance().hasStarted = false;
	}


	void SimpleMemoryTracker::PrintShortLivedToOutput()
	{
		myStaticIsAllocationInProgress = true;

		std::lock_guard<std::mutex> guard(myStaticAllocationMapMutex);

		if (TemporaryScopedMemoryTrackerData::GetInstance().allocationMap.size() == 0)
		{
			OutputDebugStringA("\n================================================================================\n");
			OutputDebugStringA(localBuffer);
			char buffer[100];
			sprintf_s(buffer, "\n== Total Allocation Count: %d\n", TemporaryScopedMemoryTrackerData::GetInstance().totalAllocationCount);
			OutputDebugStringA(buffer);
			OutputDebugStringA("== No memory leaks found! \n");
			OutputDebugStringA("================================================================================\n");
		}
		else
		{
			if (TemporaryScopedMemoryTrackerData::GetInstance().memoryTrackingSettings.shouldStoreStackTraces)
			{
				OutputDebugStringA("\n================================================================================\n");
				std::unordered_map<StackTrace, int> stackTraceToLeakCountMap;

				for (const auto& p : TemporaryScopedMemoryTrackerData::GetInstance().allocationMap)
				{
					stackTraceToLeakCountMap[p.second.stackTrace]++;
				}

				OutputDebugStringA("== Top 5 Leaks: \n");
				PrintTopLeaks(stackTraceToLeakCountMap, true);
			}

			OutputDebugStringA(localBuffer);

			if (TemporaryScopedMemoryTrackerData::GetInstance().memoryTrackingSettings.shouldTrackAllAllocations)
			{
				OutputDebugStringA("\n");
				char buffer[100];
				sprintf_s(buffer, "== Total Allocation Count: %d\n", TemporaryScopedMemoryTrackerData::GetInstance().totalAllocationCount);
				OutputDebugStringA(buffer);
			}

			char buffer[100];
			sprintf_s(buffer, "== Number of Memory Leaks: %d\n", (int)TemporaryScopedMemoryTrackerData::GetInstance().allocationMap.size());
			OutputDebugStringA(buffer);
			OutputDebugStringA("================================================================================\n\n");
		}

		TemporaryScopedMemoryTrackerData::GetInstance().allocationMap.clear();
		TemporaryScopedMemoryTrackerData::GetInstance().stackTraceToAllocationCount.clear();
		TemporaryScopedMemoryTrackerData::GetInstance().totalAllocationCount = 0;

		myStaticIsAllocationInProgress = false;
	}

	void SimpleMemoryTracker::WriteToTxtFile(const std::vector<std::pair<StackTrace, int>>& aStackTraces, const bool aScopedTracking)
	{
		std::string filePath = std::filesystem::absolute(SIMPLE_FILENAME_STACKTRACES).string();
		const size_t dotPosition = filePath.find_last_of('.');
		const std::string extension = filePath.substr(dotPosition);

		if (aScopedTracking)
		{
			//std::string test(localBuffer); test; //TO-DO(v11.2.1): find a way to get pretty name for function name
			filePath = filePath.substr(0, dotPosition) + "_scoped";
		}
		else
		{
			filePath = filePath.substr(0, dotPosition) + "_full";
		}

		filePath += extension;

		const std::filesystem::path fileNameWithCounter = Simple::AppendCounterIfAlreadyExist(filePath);

		std::ofstream writeFile(fileNameWithCounter);
		assert(writeFile.is_open() && "Failed to open the file");

		writeFile << "================================================================================" << "\n";

		for (const auto& traces : aStackTraces)
		{
			char buffer[100];
			sprintf_s(buffer, "Count: %d\n, Stack Trace:", traces.second);
			writeFile << buffer << "\n";

			const std::vector<const char*> lines = traces.first.GetLines(); //NOTE(v11.2.0): Guaranteed to not be nullptr

			for (const auto& line : lines)
			{
				writeFile << line << "\n";
			}

			writeFile << "================================================================================" << "\n";
		}

		writeFile.close();
	}

#else 
	void SimpleMemoryTracker::Init() {};
	void SimpleMemoryTracker::Destroy() {};
	void SimpleMemoryTracker::StartMemoryTracking(const bool /*aShowAdvanced*/, const std::string& /*aCallerName*/) {}
	void SimpleMemoryTracker::StopMemoryTracking() {}
#endif 

	SimpleMemoryTrackerWrapper::SimpleMemoryTrackerWrapper()
	{
		SimpleMemoryTracker::Init();
	}

	SimpleMemoryTrackerWrapper::~SimpleMemoryTrackerWrapper()
	{
		SimpleMemoryTracker::Destroy();
	}


}