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