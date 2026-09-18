#include <gtest/gtest.h>
#include <QCoreApplication>

int main(int argc, char **argv)
{
  // Initialize Qt event loop (required for Qt event processing)
  QCoreApplication app(argc, argv);
  
  // Initialize Google Test
  ::testing::InitGoogleTest(&argc, argv);
  
  // Run all tests
  return RUN_ALL_TESTS();
}
