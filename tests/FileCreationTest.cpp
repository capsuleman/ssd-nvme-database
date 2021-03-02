#include "gtest/gtest.h"

#include "Table.h"
TEST(FileCreationTest, FileCreation)
{
    Table table{1, 1};
    for (int i = 0; i < 16; i++)
    {
        table.writeInt(i, 0, i);
        table.writeDouble(i, 1, i+0.1);
    }
    EXPECT_EQ(table.readInt(5, 0), 5);
    EXPECT_EQ(table.readDouble(5, 1), 5.1);

}
