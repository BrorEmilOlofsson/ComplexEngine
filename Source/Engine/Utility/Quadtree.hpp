#pragma once
#include <limits>
#include <array>
#include <vector>
#include <ranges>

#include "Engine/Math/Point2.hpp"
#include "Engine/Math/Shapes/AABB2.hpp"
#include "Engine/Utility/Grid/Grid2.hpp"
#include "Engine/Math/ShapeMath.hpp"

namespace CLX
{

    template<typename T, typename U>
    class QuadtreeNodeRecursive final
    {
    public:

        struct DataEntry
        {
            T value;
            AABB2<U> bounds;
        };

        constexpr QuadtreeNodeRecursive() = default;

        std::span<std::unique_ptr<QuadtreeNodeRecursive<T, U>> const> GetChildNodes() const
        {
            return std::span<std::unique_ptr<QuadtreeNodeRecursive<T, U>> const>(mChildNodes.data(), mChildNodes.size());
        }

        std::span<std::unique_ptr<QuadtreeNodeRecursive<T, U>>> GetChildNodes()
        {
            return std::span<std::unique_ptr<QuadtreeNodeRecursive<T, U>>>(mChildNodes.data(), mChildNodes.size());
        }

        [[nodiscard]] std::vector<DataEntry>& GetData()
        {
            return mData;
        }
        [[nodiscard]] const std::vector<DataEntry>& GetData() const
        {
            return mData;
        }


        void Add(const T& value, const AABB2<U>& bounds)
        {
            mData.push_back({ value, bounds });
        }

        void Add(T&& value, const AABB2<U>& bounds)
        {
            mData.push_back({ std::move(value), bounds });
        }

        [[nodiscard]] constexpr bool IsLeaf() const
        {
            return mChildNodes.front() == nullptr;
        }

        void CreateChildren()
        {
            for (auto& childNode : mChildNodes)
            {
                childNode = std::make_unique<QuadtreeNodeRecursive<T, U>>();
            }
        }

    private:

        std::array<std::unique_ptr<QuadtreeNodeRecursive<T, U>>, 4> mChildNodes;
        std::vector<DataEntry> mData;
    };

    template<typename T>
    [[nodiscard]] constexpr AABB2<T> CalculateBoundsFromIndex(const uint32_t index, const AABB2<T>& parentBounds)
    {
        const Vector2<T> extent = parentBounds.GetExtent() / 2.0;
        return AABB2<T>::FromMinAndExtent(
            parentBounds.GetMin() + Vector2<T>(
                (index % 2) * extent.x,
                (index / 2) * extent.y
            ),
            extent
        );
    }


    template<typename T, typename U = float>
    class QuadtreeRecursive final
    {
    public:

        constexpr QuadtreeRecursive() = default;

        static QuadtreeRecursive CreateFromBounds(const AABB2<U>& bounds)
        {
            QuadtreeRecursive quadtree;
            quadtree.mBounds = bounds;
            return quadtree;
        }

        void Insert(const T& value, const AABB2<U>& aabb)
        {
            if (!IsOverlapping(aabb, mBounds))
            {
                return;
            }

            QuadtreeNodeRecursive<T, U>* currentNode = &mRootNode;
            AABB2<U> currentBounds = mBounds;

            constexpr std::size_t nodeCapacity = 4;
            constexpr std::size_t maxDepth = 10;
            std::size_t currentDepth = 0;
            while (true)
            {
                if (currentDepth == maxDepth || (currentNode->IsLeaf() && currentNode->GetData().size() < nodeCapacity))
                {
                    currentNode->Add(value, aabb);
                    break;
                }

                if (currentNode->IsLeaf())
                {
                    currentNode->CreateChildren();

                    std::vector<typename QuadtreeNodeRecursive<T, U>::DataEntry>& data = currentNode->GetData();

                    for (std::size_t i = data.size(); i > 0; i--)
                    {
                        auto& existingEntry = data[i - 1];
                        const uint32_t index1 = GetGridIndexByPosition(existingEntry.bounds.GetMin(), currentBounds.GetMin(), currentBounds.GetExtent() / 2.0, Vector2f::Zero(), Vector2u(2, 2));
                        const uint32_t index2 = GetGridIndexByPosition(existingEntry.bounds.GetMax(), currentBounds.GetMin(), currentBounds.GetExtent() / 2.0, Vector2f::Zero(), Vector2u(2, 2));

                        if (index1 == index2)
                        {
                            currentNode->GetChildNodes()[index1]->Add(existingEntry.value, existingEntry.bounds);
                            data.erase(begin(data) + (i - 1));
                        }
                    }
                }

                const uint32_t index1 = GetGridIndexByPosition(aabb.GetMin(), currentBounds.GetMin(), currentBounds.GetExtent() / 2.0, Vector2f::Zero(), Vector2u(2, 2));
                const uint32_t index2 = GetGridIndexByPosition(aabb.GetMax(), currentBounds.GetMin(), currentBounds.GetExtent() / 2.0, Vector2f::Zero(), Vector2u(2, 2));

                if (index1 != index2)
                {
                    currentNode->Add(value, aabb);
                    break;
                }

                currentNode = currentNode->GetChildNodes()[index1].get();
                currentBounds = CalculateBoundsFromIndex(index1, currentBounds);
                currentDepth++;
            }

        }

        void Insert(const T& value, const Point2<U>& position)
        {
            Insert(value, AABB2<U>::FromMinAndMax(position, position));
        }

        [[nodiscard]] std::vector<T*> QueryValues(const Point2<U>& position)
        {
            std::vector<T*> results;
            if (!IsInside(position, mBounds))
            {
                return results;
            }

            

            QuadtreeNodeRecursive<T, U>* currentNode = &mRootNode;
            AABB2<U> currentBounds = mBounds;
            constexpr Vector2u gridSize(2, 2);

            while (true)
            {
                std::ranges::transform(currentNode->GetData(), std::back_inserter(results), [] (typename QuadtreeNodeRecursive<T, U>::DataEntry& entry) { return &entry.value; });
                auto childNodes = currentNode->GetChildNodes();
                if (currentNode->IsLeaf())
                {
                    break;
                }
                const uint32_t index = GetGridIndexByPosition(position, currentBounds.GetMin(), currentBounds.GetExtent() / 2.0, Vector2f::Zero(), gridSize);
                currentNode = childNodes[index].get();
                currentBounds = CalculateBoundsFromIndex(index, currentBounds);
            }
            return results;
        }

    private:

        QuadtreeNodeRecursive<T, U> mRootNode;
        AABB2<U> mBounds;
    };




    template<typename T, typename U>
    class QuadtreeNode final
    {
    public:

        struct DataEntry
        {
            T value;
            AABB2<U> bounds;
        };

        QuadtreeNode() = default;

        [[nodiscard]] std::span<uint32_t> GetChildNodeIndices()
        {
            return std::span<uint32_t>(mChildNodeIndices.data(), mChildNodeIndices.size());
        }

        [[nodiscard]] std::span<const uint32_t> GetChildNodeIndices() const
        {
            return std::span<const uint32_t>(mChildNodeIndices.data(), mChildNodeIndices.size());
        }

        [[nodiscard]] bool HasChildren() const
        {
            return mChildNodeIndices[0] != std::numeric_limits<uint32_t>::max();
        }

    private:

        std::array<uint32_t, 4> mChildNodeIndices = { std::numeric_limits<uint32_t>::max() };
        std::vector<DataEntry> mData;
    };

    template<typename T, typename U = float>
    class Quadtree final
    {
    public:

        constexpr Quadtree()
        {
            mNodes.emplace_back();
        }

        constexpr Quadtree(AABB2<T> bounds)
            : mBounds(bounds)
        {
            mNodes.emplace_back();
        }


        [[nodiscard]] QuadtreeNode<T, U>& GetNode(const uint32_t index)
        {
            return mNodes[index];
        }

        [[nodiscard]] QuadtreeNode<T, U>& GetRootNode()
        {
            return mNodes[0];
        }

        void Insert(const T& value, const AABB2<U>& bounds)
        {
            if (!IsOverlapping(bounds, mBounds))
            {
                return;
            }


        }

        [[nodiscard]] auto QueryValues(const Point2<U>& position)
        {
            if (!IsInside(position, mBounds))
            {
                return;
            }


        }

    private:

        std::vector<QuadtreeNode<T, U>> mNodes;
        AABB2<U> mBounds;
    };
}