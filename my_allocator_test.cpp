#include <gtest/gtest.h>
#include "my_allocator.h"

TEST(MyAllocator, Create_1) {
	MyAllocator<int, 10>	allocator;

	EXPECT_NO_THROW(allocator.allocate(1));
	EXPECT_NO_THROW(allocator.deallocate(nullptr, 1));
	EXPECT_FALSE(allocator.allocate(1) == nullptr);
}
TEST(MyAllocator, Create_2) {
	MyAllocator<int, 10>	allocator;

	EXPECT_THROW(allocator.allocate(2), std::invalid_argument);
	EXPECT_THROW(allocator.deallocate(nullptr, 2), std::invalid_argument);
}
TEST(MyAllocator, Construct) {
	MyAllocator<int, 10>	allocator;
	int		var;
	EXPECT_NO_THROW(allocator.construct<int>(&var, 10));
	EXPECT_EQ(var, 10);
}
TEST(MyAllocator, Destroy) {
	MyAllocator<int, 10>	allocator;
	int		var;
	EXPECT_NO_THROW(allocator.destroy(&var));
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
