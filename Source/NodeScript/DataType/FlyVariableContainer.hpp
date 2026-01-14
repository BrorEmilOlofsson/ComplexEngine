#pragma once
#include "../FlyDefines.hpp"
#include "../Memory/FlyMemoryArena.hpp"
#include "FlyVariable.hpp"
#include <vector>

namespace FLY_NAMESPACE
{
	class VariableContainer final
	{
		static constexpr std::size_t BufferSize = 512;
	public:

		VariableContainer() = default;
		~VariableContainer() = default;

		VariableContainer(const VariableContainer& other);
		VariableContainer(VariableContainer&&) = default;
		VariableContainer& operator=(const VariableContainer& other);
		VariableContainer& operator=(VariableContainer&&) = default;

		[[nodiscard]] constexpr const std::vector<Variable>& GetVariables();
		[[nodiscard]] constexpr MemoryArena<BufferSize>& GetMemoryArena();
		[[nodiscard]] constexpr const MemoryArena<BufferSize>& GetMemoryArena() const;
		[[nodiscard]] constexpr Variable& GetVariable(size_t index);
		[[nodiscard]] constexpr const Variable& GetVariable(size_t index) const;
		[[nodiscard]] constexpr std::size_t GetVariableCount() const;
		[[nodiscard]] constexpr std::vector<Variable>::iterator begin();
		[[nodiscard]] constexpr std::vector<Variable>::const_iterator begin() const;
		[[nodiscard]] constexpr std::vector<Variable>::iterator end();
		[[nodiscard]] constexpr std::vector<Variable>::const_iterator end() const;

		void AddVariable(Variable variable);

	private:

		std::vector<Variable> mVariables;
		MemoryArena<BufferSize> mMemoryArena;
	};
	
	constexpr const std::vector<Variable>& VariableContainer::GetVariables()
	{
		return mVariables;
	}

	constexpr MemoryArena<VariableContainer::BufferSize>& VariableContainer::GetMemoryArena()
	{
		return mMemoryArena;
	}

	constexpr const MemoryArena<VariableContainer::BufferSize>& VariableContainer::GetMemoryArena() const
	{
		return mMemoryArena;
	}

	constexpr Variable& VariableContainer::GetVariable(size_t aIndex)
	{
		return mVariables[aIndex];
	}

	constexpr const Variable& VariableContainer::GetVariable(size_t aIndex) const
	{
		return mVariables[aIndex];
	}

	constexpr size_t VariableContainer::GetVariableCount() const
	{
		return mVariables.size();
	}

	constexpr std::vector<Variable>::iterator VariableContainer::begin()
	{
		return mVariables.begin();
	}

	constexpr std::vector<Variable>::const_iterator VariableContainer::begin() const
	{
		return mVariables.begin();
	}

	constexpr std::vector<Variable>::iterator VariableContainer::end()
	{
		return mVariables.end();
	}

	constexpr std::vector<Variable>::const_iterator VariableContainer::end() const
	{
		return mVariables.end();
	}
}