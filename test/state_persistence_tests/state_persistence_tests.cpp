/**
 * @author Alejandro Solozabal
 *
 * @file liveview_tests.cpp
 *
 */

/*******************************************************************
 * Includes
 *******************************************************************/
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>

#include "../../inc/state_persistence.hpp"

/*******************************************************************
 * Test class definition
 *******************************************************************/
using ::testing::_;
using ::testing::Return; 
using ::testing::NiceMock;
using ::testing::StrictMock;
using ::testing::SetArgReferee;
using ::testing::Ref;

class StatePersistenceTest : public ::testing::Test
{
public:
    std::shared_ptr<Database> m_database;

    const Entry m_table1_item_def{
        {"Var0", DataType::Integer,},
        {"Var1", DataType::String,},
        {"Var2", DataType::Float,},
        {"Var3", DataType::Boolean,}
    };

    const Entry m_table1_item_1{
        {"Var0", DataType::Integer, 10},
        {"Var1", DataType::String,  std::string("test")},
        {"Var2", DataType::Float,   123.23f},
        {"Var3", DataType::Boolean, true}
    };

    const Entry m_table1_item_2{
        {"Var0", DataType::Integer, 20},
        {"Var1", DataType::String,  std::string("test2")},
        {"Var2", DataType::Float,   288.28f},
        {"Var3", DataType::Boolean, false}
    };

    StatePersistenceTest()
    {
        m_database = std::make_shared<Database>("test.db");
    }

    ~StatePersistenceTest()
    {
        m_database->RemoveDatabase();
    }

protected:
};

/*******************************************************************
 * Test cases
 *******************************************************************/
TEST_F(StatePersistenceTest, Contructor)
{
    ASSERT_NO_THROW(
        Database database1("test.db");
    );
}

TEST_F(StatePersistenceTest, CreateTable)
{
    ASSERT_NO_THROW(
        DataTable data_table(m_database, "testtable", m_table1_item_def);
    );
}


TEST_F(StatePersistenceTest, DeleteTable)
{
    DataTable data_table(m_database, "testtable", m_table1_item_def);
    EXPECT_EQ(0, data_table.DeleteTable());
}

TEST_F(StatePersistenceTest, InsertItems)
{
    DataTable data_table(m_database, "testtable", m_table1_item_def);
    EXPECT_EQ(0, data_table.InsertItem(m_table1_item_1));
}

TEST_F(StatePersistenceTest, NumberItems)
{
    int number_items = 0;

    DataTable data_table(m_database, "testtable", m_table1_item_def);

    EXPECT_EQ(0, data_table.NumberItems(number_items));
    EXPECT_EQ(0, number_items);

    EXPECT_EQ(0, data_table.InsertItem(m_table1_item_1));

    EXPECT_EQ(0, data_table.NumberItems(number_items));
    EXPECT_EQ(1, number_items);

    EXPECT_EQ(0, data_table.InsertItem(m_table1_item_2));

    EXPECT_EQ(0, data_table.NumberItems(number_items));
    EXPECT_EQ(2, number_items);
}

TEST_F(StatePersistenceTest, GetItems)
{
    DataTable data_table(m_database, "testtable", m_table1_item_def);
    EXPECT_EQ(0, data_table.InsertItem(m_table1_item_1));

    Entry item1{
        {"Var0", DataType::Integer,10},
        {"Var1", DataType::String,},
        {"Var2", DataType::Float,},
        {"Var3", DataType::Boolean,}
    };
    EXPECT_EQ(0, data_table.GetItem(item1));

    float val2 = std::get<float>(m_table1_item_1[2].value);
    float val2_read = std::get<float>(item1[2].value);

    bool val3 = std::get<bool>(m_table1_item_1[3].value);
    bool val3_read = std::get<bool>(item1[3].value);

    EXPECT_EQ(val2, val2_read);
    EXPECT_EQ(val3, val3_read);
}

TEST_F(StatePersistenceTest, SetItems)
{
    DataTable data_table(m_database, "testtable", m_table1_item_def);
    EXPECT_EQ(0, data_table.InsertItem(m_table1_item_1));

    Entry m_table1_item_1_mod = m_table1_item_1;
    m_table1_item_1_mod[2].value = 111.11f;

    EXPECT_EQ(0, data_table.SetItem(m_table1_item_1_mod));

    Entry item1{
        {"Var0", DataType::Integer,10},
        {"Var1", DataType::String,},
        {"Var2", DataType::Float,},
        {"Var3", DataType::Boolean,}
    };

    EXPECT_EQ(0, data_table.GetItem(item1));

    float val_read = std::get<float>(item1[2].value);

    EXPECT_EQ(111.11f, val_read);
}

TEST_F(StatePersistenceTest, DeleteItem)
{
    int number_items = 0;
    DataTable data_table(m_database, "testtable", m_table1_item_def);
    EXPECT_EQ(0, data_table.InsertItem(m_table1_item_1));

    EXPECT_EQ(0, data_table.DeleteItem(m_table1_item_1));
    EXPECT_EQ(0, data_table.NumberItems(number_items));
    EXPECT_EQ(0, number_items);
}

TEST_F(StatePersistenceTest, DeleteAllItems)
{
    int number_items = 0;
    DataTable data_table(m_database, "testtable", m_table1_item_def);
    EXPECT_EQ(0, data_table.InsertItem(m_table1_item_1));
    EXPECT_EQ(0, data_table.InsertItem(m_table1_item_2));
    EXPECT_EQ(0, data_table.NumberItems(number_items));
    EXPECT_EQ(2, number_items);

    EXPECT_EQ(0, data_table.DeleteAllItems());
    EXPECT_EQ(0, data_table.NumberItems(number_items));
    EXPECT_EQ(0, number_items);
}