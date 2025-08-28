#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

TEST(HelloTest, Basic) {
    EXPECT_EQ(1 + 1, 2);
}

TEST(SpdlogTest, LogInfo) {
    spdlog::set_level(spdlog::level::info);
    EXPECT_NO_THROW(spdlog::info("Test info log"));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
