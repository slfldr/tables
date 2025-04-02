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

        optional<size_t> find_idx(size_t key) const
        {
            for (size_t i = 0; i < data_.size(); ++i)
            {
                if (data_[i].first == key)
                {
                	return i;
                }
            }

            return std::nullopt;
        }

		bool insert(size_t key, const T& value) override
		{
    		if (!find(key))
    		{
        		data_.emplace_back(key, value);

        		return true;
    		}

            return false;
        }

		bool erase(size_t key) override
		{
    		auto idx = find_idx(key);

    		if (idx)
   			{
        		std::swap(data_[idx.value()], data_.back());

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

        optional<pair<size_t, T>> find(size_t key) const override
        {
            auto it = std::lower_bound(data_.begin(), data_.end(), key,
                [](const std::pair<size_t, T>& item, size_t k) 
                {
                    return item.first < k;
                });

            if (it != data_.end() && it->first == key)
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
}