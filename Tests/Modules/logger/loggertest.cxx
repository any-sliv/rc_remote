#include <gtest/gtest.h>
#include "logger.hpp"
#include <string>

UART_HandleTypeDef huart1;

class loggerTest : public::testing::Test {
  public:
  char *charText;
  std::string text = "Thisisthetext";

  void SetUp() override {
  }

  void TearDown() override {
  }
};

TEST_F(loggerTest, IsTextPrinted) {
    // Copy string to char
    // charText = new char[text.size() + 1];
    // memcpy(charText, text.c_str(), text.size());
    // // Print text
    // //Logger::Log(text);

    // // Check if what we started if what ended in the buffer
    // EXPECT_EQ(*charText, *GetDataAddress()) << "Char text" << *charText;
    // EXPECT_FALSE(memcmp(charText, GetDataAddress(), text.size()));
}