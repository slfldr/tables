#include "gtest.h"
#include "table.h"

using namespace containers;

TEST(UnorderedTableTest, can_create_unordered_table)
{
    ASSERT_NO_THROW(UnorderedTable<int> table);
}

TEST(UnorderedTableTest, can_insert_element)
{
    UnorderedTable<int> table;

    ASSERT_TRUE(table.insert(1, 100));
}

TEST(UnorderedTableTest, cannot_insert_duplicate_key)
{
    UnorderedTable<int> table;

    table.insert(1, 100);

    ASSERT_FALSE(table.insert(1, 200));
}

TEST(UnorderedTableTest, insert_existing_key_fails)
{
    UnorderedTable<int> table;

    table.insert(1, 100);

    ASSERT_FALSE(table.insert(1, 200));
    ASSERT_EQ(table.find(1)->second, 100);
}

TEST(UnorderedTableTest, can_find_existing_element)
{
    UnorderedTable<int> table;

    table.insert(1, 100);

    auto result = table.find(1);

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result.value().second, 100);
}

TEST(UnorderedTableTest, cannot_find_non_existing_element)
{
    UnorderedTable<int> table;

    ASSERT_FALSE(table.find(3).has_value());
}

TEST(UnorderedTableTest, find_non_existent_returns_nullopt)
{
    UnorderedTable<int> table;

    table.insert(10, 100);

    ASSERT_FALSE(table.find(20));
}

TEST(UnorderedTableTest, can_erase_existing_element)
{
    UnorderedTable<int> table;

    table.insert(1, 100);

    ASSERT_TRUE(table.erase(1));
    ASSERT_FALSE(table.find(1).has_value());
}

TEST(UnorderedTableTest, erase_non_existent_key_fails)
{
    UnorderedTable<int> table;

    ASSERT_FALSE(table.erase(45));
}

TEST(UnorderedTableTest, erase_reduces_size)
{
    UnorderedTable<int> table;

    table.insert(1, 10);
    table.insert(2, 20);
    table.erase(1);

    ASSERT_EQ(table.size(), 1);
    ASSERT_FALSE(table.find(1));
}

TEST(UnorderedTableTest, clear_empties_table)
{
    UnorderedTable<int> table;

    table.insert(1, 10);
    table.insert(2, 20);
    table.clear();

    ASSERT_TRUE(table.empty());
}

TEST(UnorderedTableTest, multiple_insert_erase)
{
    UnorderedTable<std::string> table;

    table.insert(1, "one");
    table.insert(2, "two");
    table.insert(3, "three");

    ASSERT_EQ(table.size(), 3);

    table.erase(2);

    ASSERT_EQ(table.size(), 2);
    ASSERT_FALSE(table.find(2));

    table.erase(1);

    ASSERT_EQ(table.size(), 1);
    ASSERT_FALSE(table.find(1));

    table.erase(3);

    ASSERT_EQ(table.size(), 0);
    ASSERT_TRUE(table.empty());
}



TEST(OrderedTableTest, can_create_ordered_table)
{
    ASSERT_NO_THROW(OrderedTable<int> table);
}

TEST(OrderedTableTest, can_insert_element)
{
    OrderedTable<int> table;

    ASSERT_TRUE(table.insert(1, 100));
}

TEST(OrderedTableTest, cannot_insert_duplicate_key)
{
    OrderedTable<int> table;

    table.insert(1, 100);

    ASSERT_FALSE(table.insert(1, 200));
}

TEST(OrderedTableTest, insert_in_sorted_order)
{
    OrderedTable<int> table;

    table.insert(3, 30);
    table.insert(1, 10);
    table.insert(2, 20);

    ASSERT_EQ(table[0].first, 1);
    ASSERT_EQ(table[1].first, 2);
    ASSERT_EQ(table[2].first, 3);
}

TEST(OrderedTableTest, insert_existing_key_fails)
{
    OrderedTable<int> table;

    table.insert(5, 50);

    ASSERT_FALSE(table.insert(5, 55));
    ASSERT_EQ(table.find(5)->second, 50);
}

TEST(OrderedTableTest, can_find_existing_element)
{
    OrderedTable<int> table;

    table.insert(1, 100);

    auto result = table.find(1);

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result.value().second, 100);
}

TEST(OrderedTableTest, cannot_find_non_existing_element)
{
    OrderedTable<int> table;

    ASSERT_FALSE(table.find(3).has_value());
}

TEST(OrderedTableTest, find_returns_correct_value)
{
    OrderedTable<int> table;

    table.insert(10, 100);

    ASSERT_EQ(table.find(10)->second, 100);
}

TEST(OrderedTableTest, can_erase_existing_element)
{
    OrderedTable<int> table;

    table.insert(1, 100);

    ASSERT_TRUE(table.erase(1));
    ASSERT_FALSE(table.find(1).has_value());
}

TEST(OrderedTableTest, cannot_erase_non_existing_element)
{
    OrderedTable<int> table;

    ASSERT_FALSE(table.erase(1));
}

TEST(OrderedTableTest, erase_and_check_order)
{
    OrderedTable<int> table;

    table.insert(1, 10);
    table.insert(2, 20);
    table.insert(3, 30);
    table.erase(2);

    ASSERT_EQ(table.size(), 2);
    ASSERT_EQ(table[0].first, 1);
    ASSERT_EQ(table[1].first, 3);
}

TEST(OrderedTableTest, erase_non_existent_key_fails)
{
    OrderedTable<int> table;

    table.insert(1, 10);

    ASSERT_FALSE(table.erase(99));
}

TEST(OrderedTableTest, table_remains_sorted_after_multiple_inserts_and_deletes)
{
    OrderedTable<int> table;

    table.insert(10, 100);
    table.insert(30, 300);
    table.insert(20, 200);
    table.insert(40, 400);

    ASSERT_EQ(table[0].first, 10);
    ASSERT_EQ(table[1].first, 20);
    ASSERT_EQ(table[2].first, 30);
    ASSERT_EQ(table[3].first, 40);

    table.erase(20);

    ASSERT_EQ(table.size(), 3);
    ASSERT_EQ(table[0].first, 10);
    ASSERT_EQ(table[1].first, 30);
    ASSERT_EQ(table[2].first, 40);
}

TEST(OrderedTableTest, clear_empties_table)
{
    OrderedTable<int> table;

    table.insert(7, 70);
    table.insert(8, 80);
    table.clear();

    ASSERT_TRUE(table.empty());
}



TEST(HashTableTest, can_create_hash_table)
{
    ASSERT_NO_THROW(HashTable<int> table);
}

TEST(HashTableTest, can_insert_element)
{
    HashTable<int> table;

    ASSERT_TRUE(table.insert(1, 100));
}

TEST(HashTableTest, cannot_insert_duplicate_key)
{
    HashTable<int> table;

    table.insert(1, 100);

    ASSERT_FALSE(table.insert(1, 200));
}

TEST(HashTableTest, insert_existing_key_fails)
{
    HashTable<int> table;

    table.insert(1, 100);

    ASSERT_FALSE(table.insert(1, 200));
    ASSERT_EQ(table.find(1)->second, 100);
}

TEST(HashTableTest, can_find_existing_element)
{
    HashTable<int> table;

    table.insert(1, 100);

    auto result = table.find(1);

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->second, 100);
}

TEST(HashTableTest, cannot_find_non_existing_element)
{
    HashTable<int> table;

    ASSERT_FALSE(table.find(3).has_value());
}

TEST(HashTableTest, find_non_existent_returns_nullopt)
{
    HashTable<int> table;

    table.insert(10, 100);

    ASSERT_FALSE(table.find(20));
}

TEST(HashTableTest, can_erase_existing_element)
{
    HashTable<int> table;

    table.insert(1, 100);

    ASSERT_TRUE(table.erase(1));
    ASSERT_FALSE(table.find(1).has_value());
}

TEST(HashTableTest, erase_non_existent_key_fails)
{
    HashTable<int> table;

    ASSERT_FALSE(table.erase(45));
}

TEST(HashTableTest, erase_reduces_size)
{
    HashTable<int> table;

    table.insert(1, 10);
    table.insert(2, 20);
    table.erase(1);

    ASSERT_EQ(table.size(), 1);
    ASSERT_FALSE(table.find(1));
}

TEST(HashTableTest, clear_empties_table)
{
    HashTable<int> table;

    table.insert(1, 10);
    table.insert(2, 20);
    table.clear();

    ASSERT_TRUE(table.empty());
}

TEST(HashTableTest, multiple_insert_erase)
{
    HashTable<std::string> table;

    table.insert(1, "one");
    table.insert(2, "two");
    table.insert(3, "three");

    ASSERT_EQ(table.size(), 3);

    table.erase(2);
    ASSERT_EQ(table.size(), 2);
    ASSERT_FALSE(table.find(2));

    table.erase(1);
    ASSERT_EQ(table.size(), 1);
    ASSERT_FALSE(table.find(1));

    table.erase(3);
    ASSERT_EQ(table.size(), 0);
    ASSERT_TRUE(table.empty());
}

TEST(HashTableTest, handles_collisions_correctly)
{
    HashTable<std::string> table(5);

    size_t k1 = 1;
    size_t k2 = 6;
    size_t k3 = 11;

    ASSERT_TRUE(table.insert(k1, "one"));
    ASSERT_TRUE(table.insert(k2, "six"));
    ASSERT_TRUE(table.insert(k3, "eleven"));
    ASSERT_EQ(table.size(), 3);
    ASSERT_EQ(table.find(k1)->second, "one");
    ASSERT_EQ(table.find(k2)->second, "six");
    ASSERT_EQ(table.find(k3)->second, "eleven");
}

TEST(HashTableTest, erase_element_among_collisions)
{
    HashTable<std::string> table(5);

    table.insert(1, "one");
    table.insert(6, "six");
    table.insert(11, "eleven");

    ASSERT_TRUE(table.erase(6));
    ASSERT_EQ(table.size(), 2);
    ASSERT_FALSE(table.find(6).has_value());
    ASSERT_TRUE(table.find(1).has_value());
    ASSERT_TRUE(table.find(11).has_value());
}

TEST(HashTableTest, find_returns_correct_value_in_collision_bucket)
{
    HashTable<std::string> table(3);

    table.insert(0, "zero");
    table.insert(3, "three");
    table.insert(6, "six");

    ASSERT_EQ(table.find(0)->second, "zero");
    ASSERT_EQ(table.find(3)->second, "three");
    ASSERT_EQ(table.find(6)->second, "six");
}

TEST(HashTableTest, clear_resets_all_buckets)
{
    HashTable<std::string> table(3);

    table.insert(0, "zero");
    table.insert(3, "three");
    table.insert(6, "six");

    table.clear();

    ASSERT_TRUE(table.empty());
    ASSERT_FALSE(table.find(0));
    ASSERT_FALSE(table.find(3));
    ASSERT_FALSE(table.find(6));
}