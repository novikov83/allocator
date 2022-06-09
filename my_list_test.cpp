#include <gtest/gtest.h>
#include "my_list.h"

TEST(MyList, Create_1) {
	MyList<int>	list;

	EXPECT_THROW(list[0], std::range_error);
	EXPECT_NO_THROW(list.push_back(12));
	EXPECT_EQ(list[0], 12);
}

TEST(MyList, OperatorEQ) {
	MyList<int>	list;

	list.push_back(12);
	EXPECT_NO_THROW(list[0] = 144);
	EXPECT_EQ(list[0], 144);
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
