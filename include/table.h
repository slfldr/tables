#pragma once

#include <vector>
#include <cstddef>
#include <utility>
#include <iostream>
#include <optional>
#include <algorithm>

using std::size_t;
using std::optional;
using std::pair;

namespace containers
{
    template<typename T>
    class TableInterface
    {
    public:
        virtual bool insert(size_t key, const T& value) = 0;
        virtual bool erase(size_t key) = 0;
        virtual optional<pair<size_t, T>> find(size_t key) const = 0;
        virtual size_t size() const noexcept = 0;
        virtual bool empty() const noexcept = 0;
        virtual void clear() = 0;
        virtual ~TableInterface() = default;
    };



    template<typename T, typename CellType = std::pair<size_t, T>>
    class TableArray : public TableInterface<T>
    {
    protected:
        std::vector<CellType> data_;

    public:
        size_t size() const noexcept override
        {
            return data_.size();
        }

        bool empty() const noexcept override
        {
            return data_.empty();
        }

        const std::pair<size_t, T>& operator[](size_t index) const
        {
            return data_.at(index);
        }

        void clear() override
        {
            data_.clear();
        }
    };



    template<typename T>
    class UnorderedTable : public TableArray<T>
    {
   	protected:
        using TableArray<T>::data_;

    public:
        using TableArray<T>::clear;
        using TableArray<T>::empty;
        using TableArray<T>::operator[];

        UnorderedTable() = default;
        ~UnorderedTable() = default;

        optional<pair<size_t, T>> find(size_t key) const override
        {
            for (const auto& item : data_)
            {
                if (item.first == key)
                {
                    return item;
                }
            }

            return std::nullopt;
        }

        typename std::vector<std::pair<size_t, T>>::iterator find_iter(size_t key)
        {
            return std::find_if(data_.begin(), data_.end(),
                [key](const auto& item) { return item.first == key; });
        }

        typename std::vector<std::pair<size_t, T>>::const_iterator find_iter(size_t key) const
        {
            return std::find_if(data_.cbegin(), data_.cend(),
                [key](const auto& item) { return item.first == key; });
        }

        bool insert(size_t key, const T& value) override
        {
            if (find_iter(key) == data_.end())
            {
                data_.emplace_back(key, value);

                return true;
            }

            return false;
        }

        bool erase(size_t key) override
        {
            auto it = find_iter(key);

            if (it != data_.end())
            {
                std::swap(*it, data_.back());

                data_.pop_back();

                return true;
            }

            return false;
        }

        friend std::ostream& operator<<(std::ostream& os, const UnorderedTable<T>& table)
        {
            for (const auto& item : table.data_)
            {
                os << item.first << " " << item.second << std::endl;
            }

            return os;
        }
    };



    template<typename T>
    class OrderedTable : public TableArray<T>
    {
    protected:
        using TableArray<T>::data_;

    public:
        using TableArray<T>::clear;
        using TableArray<T>::empty;
        using TableArray<T>::operator[];

        OrderedTable() = default;
        ~OrderedTable() = default;

        typename std::vector<std::pair<size_t, T>>::const_iterator find_iter(size_t key) const
        {
            return std::lower_bound(data_.cbegin(), data_.cend(), key,
                [](const std::pair<size_t, T>& item, size_t k)
                {
                    return item.first < k;
                });
        }

        optional<pair<size_t, T>> find(size_t key) const override
        {
            auto it = find_iter(key);

            if (it != data_.end())
            {
                return *it;
            }

            return std::nullopt;
        }

        bool insert(size_t key, const T& value) override
        {
            auto it = std::lower_bound(data_.begin(), data_.end(), key,
                [](const std::pair<size_t, T>& item, size_t k)
                {
                    return item.first < k;
                });

            if (it != data_.end() && it->first == key)
            {
                return false;
            }

            data_.insert(it, {key, value});

            return true;
        }

        bool erase(size_t key) override
        {
            auto it = std::lower_bound(data_.begin(), data_.end(), key,
                [](const auto& item, size_t k)
                {
                	return item.first < k;
                });

            if (it != data_.end() && it->first == key)
            {
                data_.erase(it);

                return true;
            }

            return false;
        }

        friend std::ostream& operator<<(std::ostream& os, const OrderedTable<T>& table)
        {
            for (const auto& item : table.data_)
            {
                os << item.first << " " << item.second << std::endl;
            }

            return os;
        }
    };



    template<typename T>
    class HashTable : public TableInterface<T>
    {
    protected:
        static const size_t DEFAULT_CAPACITY = 101;

        std::vector<std::vector<std::pair<size_t, T>>> buckets_;

        size_t count_ = 0;

        size_t hash(size_t key) const
        {
            return key % buckets_.size();
        }

    public:
        HashTable(size_t capacity = DEFAULT_CAPACITY) : buckets_(capacity) {}

        ~HashTable() = default;

        bool insert(size_t key, const T& value) override
        {
            size_t idx = hash(key);

            auto& bucket = buckets_[idx];

            for (auto& item : bucket)
            {
                if (item.first == key)
                {
                    return false;
                }
            }

            bucket.emplace_back(key, value);

            ++count_;

            return true;
        }

        bool erase(size_t key) override
        {
            size_t idx = hash(key);

            auto& bucket = buckets_[idx];

            for (auto it = bucket.begin(); it != bucket.end(); ++it)
            {
                if (it->first == key)
                {
                    bucket.erase(it);

                    --count_;

                    return true;
                }
            }

            return false;
        }

        optional<std::pair<size_t, T>> find(size_t key) const override
        {
            size_t idx = hash(key);

            const auto& bucket = buckets_[idx];

            for (const auto& item : bucket)
            {
                if (item.first == key)
                {
                    return item;
                }
            }

            return std::nullopt;
        }

        size_t size() const noexcept override
        {
            return count_;
        }

        bool empty() const noexcept override
        {
            return count_ == 0;
        }

        void clear() override
        {
            for (auto& bucket : buckets_)
            {
                bucket.clear();
            }

            count_ = 0;
        }

        friend std::ostream& operator<<(std::ostream& os, const HashTable<T>& table)
        {
            for (const auto& bucket : table.buckets_)
            {
                for (const auto& item : bucket)
                {
                    os << item.first << " " << item.second << std::endl;
                }
            }

            return os;
        }
    };
}