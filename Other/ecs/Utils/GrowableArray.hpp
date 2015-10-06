#pragma once

#include <cassert>
#include <memory>
#include <cstring>

namespace ecs
{
    // Low-level array unique ptr wrapper.
    // Wraps an array unique_ptr instance and provides functions to retrieve
    // items and grow the buffer size.
    template <typename T>
    class GrowableArray
    {
    private:
        std::unique_ptr<T[]> data;

        // Grow implementation for non-trivially-copyable types.
        void growImpl(T* mNewData, std::size_t mCapacityOld, std::false_type)
        {
            for(auto i(0u); i < mCapacityOld; ++i)
                mNewData[i] = std::move(data[i]);
        }

        // Grow implementation for trivially-copyable types.
        void growImpl(T* mNewData, std::size_t mCapacityOld, std::true_type)
        {
            static_assert(std::is_trivially_copyable<T>{}, "");
            std::memcpy(mNewData, data.get(), sizeof(T) * mCapacityOld);
        }

    public:
        GrowableArray() noexcept = default;
        ~GrowableArray() noexcept = default;

        GrowableArray(GrowableArray&&) noexcept = default;
        GrowableArray& operator=(GrowableArray&&) noexcept = default;

        GrowableArray(const GrowableArray&) = delete;
        GrowableArray& operator=(const GrowableArray&) = delete;

        // Grows the internal storage from `mCapacityOld` to `mCapacityNew`.
        // The new capacity must be greater or equal than the old one.
        void grow(std::size_t mCapacityOld, std::size_t mCapacityNew)
        {
            assert(mCapacityOld <= mCapacityNew);

            auto newData(std::make_unique<T[]>(mCapacityNew));
            growImpl(
                newData.get(), mCapacityOld, std::is_trivially_copyable<T>{});
            data = std::move(newData);
        }

        auto& operator[](std::size_t mI) noexcept { return data[mI]; }

        const auto& operator[](std::size_t mI) const noexcept
        {
            return data[mI];
        }
    };
}