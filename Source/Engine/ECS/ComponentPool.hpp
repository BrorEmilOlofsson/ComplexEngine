#pragma once
#include <vector>
#include <memory>
#include <array>
#include "EntityID.hpp"

namespace CLX
{


	class ComponentPool final
	{
	public:

		template<typename T>
		constexpr ComponentPool(std::type_identity<T>)
			: mConcept(std::make_unique<ComponentPoolModel<T>>())
			, mTypeInfo(&typeid(T))
		{
		}

		ComponentPool(const ComponentPool& other)
			: mConcept(other.mConcept->Clone())
			, mTypeInfo(other.mTypeInfo)
		{
		}

		ComponentPool& operator=(const ComponentPool& other)
		{
			other.mConcept->Clone().swap(mConcept);
			mTypeInfo = other.mTypeInfo;
			return *this;
		}

		template<typename T>
		T& AddComponent(const EntityID entityID, const T& defaultValue)
		{
			void* componentPtr = mConcept->AddComponent(entityID, &defaultValue);
			return *reinterpret_cast<T*>(componentPtr);
		}

		template<typename T>
		[[nodiscard]] T* GetComponent(const EntityID entityID)
		{
			void* componentPtr = mConcept->GetComponent(entityID);
			return reinterpret_cast<T*>(componentPtr);
		}

		template<typename T>
		[[nodiscard]] const T* GetComponent(const EntityID entityID) const
		{
			const void* component = mConcept->GetComponent(entityID);
			return reinterpret_cast<const T*>(component);
		}

		[[nodiscard]] void* GetComponent(const EntityID entityID)
		{
			return mConcept->GetComponent(entityID);
		}

		[[nodiscard]] const void* GetComponent(const EntityID entityID) const
		{
			return mConcept->GetComponent(entityID);
		}

		[[nodiscard]] bool HasComponent(const EntityID entityID) const
		{
			return GetComponent(entityID) != nullptr;
		}

		void RemoveComponent(const EntityID entityID)
		{
			mConcept->RemoveComponent(entityID);
		}

		[[nodiscard]] std::size_t GetComponentCount() const
		{
			return mConcept->GetComponentCount();
		}

		void ResizeComponentIndices(const std::size_t size)
		{
			mConcept->ResizeComponentIndices(size);
		}

		[[nodiscard]] const std::type_info& GetTypeInfo() const
		{
			return *mTypeInfo;
		}

	private:

		class ComponentPoolConcept
		{
		public:

			ComponentPoolConcept() = default;
			virtual ~ComponentPoolConcept() = default;

			virtual void* AddComponent(EntityID entityID, const void* defaultValuePtr) = 0;
			[[nodiscard]] virtual void* GetComponent(EntityID entityID) = 0;
			[[nodiscard]] virtual const void* GetComponent(EntityID entityID) const = 0;
			virtual void RemoveComponent(const EntityID entityID) = 0;
			[[nodiscard]] virtual std::size_t GetComponentCount() const = 0;
			virtual void ResizeComponentIndices(const std::size_t size) = 0;

			virtual std::unique_ptr<ComponentPoolConcept> Clone() const = 0;
		};


		template<typename T>
		class ComponentPoolModel final : public ComponentPoolConcept
		{
			static constexpr unsigned int InvalidComponentIndex = std::numeric_limits<unsigned int>::max();
		public:

			ComponentPoolModel() = default;

			void* AddComponent(const EntityID entityID, const void* defaultValuePtr) override
			{
				const T& defaultValue = *reinterpret_cast<const T*>(defaultValuePtr);
				const unsigned int previousComponentIndex = mEntityIDToComponentIndex[entityID.id];
				if (previousComponentIndex != InvalidComponentIndex)
				{
					mComponents[previousComponentIndex] = defaultValue;
					return &mComponents[previousComponentIndex];
				}

				const unsigned int componentIndex = static_cast<unsigned int>(mComponents.size());
				mEntityIDToComponentIndex[entityID.id] = componentIndex;
				mComponents.push_back(defaultValue);
				mComponentIndexToEntityID[componentIndex] = entityID;
				return &mComponents.back();
			}

			[[nodiscard]] void* GetComponent(const EntityID entityID) override
			{
				const unsigned int componentIndex = mEntityIDToComponentIndex[entityID.id];
				if (componentIndex == InvalidComponentIndex)
				{
					return nullptr;
				}
				return &mComponents[componentIndex];
			}

			[[nodiscard]] const void* GetComponent(const EntityID entityID) const override
			{
				const unsigned int componentIndex = mEntityIDToComponentIndex[entityID.id];
				if (componentIndex == InvalidComponentIndex)
				{
					return nullptr;
				}
				return &mComponents[componentIndex];
			}

			void RemoveComponent(const EntityID entityID) override
			{
				const unsigned int componentIndex = mEntityIDToComponentIndex[entityID.id];
				const std::size_t lastIndex = mComponents.size() - 1;
				mComponents[componentIndex] = std::move(mComponents[lastIndex]);
				mComponents.pop_back();
				const EntityID lastIndexEntityID = mComponentIndexToEntityID[lastIndex];
				mEntityIDToComponentIndex[lastIndexEntityID.id] = componentIndex;
				mEntityIDToComponentIndex[entityID.id] = InvalidComponentIndex;
				mComponentIndexToEntityID[componentIndex] = lastIndexEntityID;
			}

			std::size_t GetComponentCount() const override
			{
				return mComponents.size();
			}

			void ResizeComponentIndices(const std::size_t size) override
			{
				mEntityIDToComponentIndex.resize(size, InvalidComponentIndex);
				mComponentIndexToEntityID.resize(size);
			}

			std::unique_ptr<ComponentPoolConcept> Clone() const override
			{
				return std::make_unique<ComponentPoolModel<T>>(*this);
			}

		private:

			std::vector<T> mComponents;
			std::vector<unsigned int> mEntityIDToComponentIndex;
			std::vector<EntityID> mComponentIndexToEntityID;
		};

	private:

		std::unique_ptr<ComponentPoolConcept> mConcept;
		const std::type_info* mTypeInfo = nullptr;
	};

	class ComponentPoolSBO final
	{
	public:

		template<typename T>
		constexpr explicit ComponentPoolSBO(std::type_identity<T>)
			: mTypeInfo(&typeid(T))
		{
			using M = Model<T>;
			static_assert(sizeof(M) == BufferSize);
			static_assert(alignof(M) <= BufferAlignment);
			::new (GetConcept()) M();
		}

		~ComponentPoolSBO()
		{
			GetConcept()->~Concept();
		}

		ComponentPoolSBO(const ComponentPoolSBO& other)
			: mTypeInfo(other.mTypeInfo)
		{
			other.GetConcept()->Clone(GetConcept());
		}

		ComponentPoolSBO& operator=(const ComponentPoolSBO& other)
		{
			ComponentPoolSBO temp(other);
			mBuffer.swap(temp.mBuffer);
			mTypeInfo = other.mTypeInfo;
			return *this;
		}

		ComponentPoolSBO(ComponentPoolSBO&& other) noexcept
			: mTypeInfo(std::exchange(other.mTypeInfo, nullptr))
		{
			other.GetConcept()->Move(GetConcept());
		}

		ComponentPoolSBO& operator=(ComponentPoolSBO&& other) noexcept
		{
			ComponentPoolSBO temp(std::move(other));
			mBuffer.swap(temp.mBuffer);
			mTypeInfo = std::exchange(other.mTypeInfo, nullptr);

			return *this;
		}


		template<typename T>
		T& AddComponent(const EntityID entityID, const T* defaultValuePtr)
		{
			void* componentPtr = GetConcept()->AddComponent(entityID, defaultValuePtr);
			return *reinterpret_cast<T*>(componentPtr);
		}

		template<typename T>
		[[nodiscard]] T* GetComponent(const EntityID entityID)
		{
			void* componentPtr = GetConcept()->GetComponent(entityID);
			return reinterpret_cast<T*>(componentPtr);
		}

		template<typename T>
		[[nodiscard]] const T* GetComponent(const EntityID entityID) const
		{
			const void* componentPtr = GetConcept()->GetComponent(entityID);
			return reinterpret_cast<const T*>(componentPtr);
		}

		[[nodiscard]] void* GetComponent(const EntityID entityID)
		{
			return GetConcept()->GetComponent(entityID);
		}

		[[nodiscard]] const void* GetComponent(const EntityID entityID) const
		{
			return GetConcept()->GetComponent(entityID);
		}

		[[nodiscard]] bool HasComponent(const EntityID entityID) const
		{
			return GetComponent(entityID) != nullptr;
		}

		void RemoveComponent(const EntityID entityID)
		{
			GetConcept()->RemoveComponent(entityID);
		}

		[[nodiscard]] std::size_t GetComponentCount() const
		{
			return GetConcept()->GetComponentCount();
		}

		void ResizeComponentIndices(const std::size_t size)
		{
			GetConcept()->ResizeComponentIndices(size);
		}

		[[nodiscard]] const std::type_info& GetTypeInfo() const
		{
			return *mTypeInfo;
		}

	private:

		class Concept
		{
		public:

			Concept() = default;
			virtual ~Concept() = default;

			virtual void* AddComponent(EntityID entityID, const void* defaultValuePtr) = 0;
			[[nodiscard]] virtual void* GetComponent(EntityID entityID) = 0;
			[[nodiscard]] virtual const void* GetComponent(EntityID entityID) const = 0;
			virtual void RemoveComponent(const EntityID entityID) = 0;
			[[nodiscard]] virtual std::size_t GetComponentCount() const = 0;
			virtual void ResizeComponentIndices(const std::size_t size) = 0;

			virtual void Clone(Concept* memory) const = 0;
			virtual void Move(Concept* memory) const = 0;
		};

		template<typename T>
		class Model final : public Concept
		{
			static constexpr uint32_t InvalidComponentIndex = std::numeric_limits<uint32_t>::max();
		public:

			Model() = default;

			void* AddComponent(const EntityID entityID, const void* defaultValuePtr) override
			{
				T defaultValue = defaultValuePtr == nullptr ? T() : *reinterpret_cast<const T*>(defaultValuePtr);
				const uint32_t previousComponentIndex = mEntityIDToComponentIndex[entityID.id];
				if (previousComponentIndex != InvalidComponentIndex)
				{
					mComponents[previousComponentIndex] = std::move(defaultValue);
					return &mComponents[previousComponentIndex];
				}

				const uint32_t componentIndex = static_cast<uint32_t>(mComponents.size());
				mEntityIDToComponentIndex[entityID.id] = componentIndex;
				mComponents.emplace_back(std::move(defaultValue));
				mComponentIndexToEntityID[componentIndex] = entityID;
				return &mComponents.back();
			}

			[[nodiscard]] void* GetComponent(const EntityID entityID) override
			{
				const uint32_t componentIndex = mEntityIDToComponentIndex[entityID.id];
				if (componentIndex == InvalidComponentIndex)
				{
					return nullptr;
				}
				return &mComponents[componentIndex];
			}

			[[nodiscard]] const void* GetComponent(const EntityID entityID) const override
			{
				const uint32_t componentIndex = mEntityIDToComponentIndex[entityID.id];
				if (componentIndex == InvalidComponentIndex)
				{
					return nullptr;
				}
				return &mComponents[componentIndex];
			}

			void RemoveComponent(const EntityID entityID) override
			{
				const uint32_t componentIndex = mEntityIDToComponentIndex[entityID.id];
				const std::size_t lastIndex = mComponents.size() - 1;
				mComponents[componentIndex] = std::move(mComponents[lastIndex]);
				mComponents.pop_back();
				const EntityID lastIndexEntityID = mComponentIndexToEntityID[lastIndex];
				mEntityIDToComponentIndex[lastIndexEntityID.id] = componentIndex;
				mEntityIDToComponentIndex[entityID.id] = InvalidComponentIndex;
				mComponentIndexToEntityID[componentIndex] = lastIndexEntityID;
			}

			std::size_t GetComponentCount() const override
			{
				return mComponents.size();
			}

			void ResizeComponentIndices(const std::size_t size) override
			{
				mEntityIDToComponentIndex.resize(size, InvalidComponentIndex);
				mComponentIndexToEntityID.resize(size);
			}

			void Clone(Concept* memory) const override
			{
				::new(memory) Model(*this);
			}

			void Move(Concept* memory) const override
			{
				::new(memory) Model(std::move(*this));
			}

		private:

			std::vector<T> mComponents;
			std::vector<uint32_t> mEntityIDToComponentIndex;
			std::vector<EntityID> mComponentIndexToEntityID;
		};

	private:

		[[nodiscard]] Concept* GetConcept()
		{
			return reinterpret_cast<Concept*>(mBuffer.data());
		}

		[[nodiscard]] const Concept* GetConcept() const
		{
			return reinterpret_cast<const Concept*>(mBuffer.data());
		}


	private:

		static constexpr std::size_t BufferSize = sizeof(Model<std::byte>);
		static constexpr std::size_t BufferAlignment = 16;

		alignas(BufferAlignment) std::array<std::byte, BufferSize> mBuffer = {};
		const std::type_info* mTypeInfo = nullptr;
	};
}