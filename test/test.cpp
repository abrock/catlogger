#include <iostream>
#include <random>

#include <gtest/gtest.h>

#include "catlogger/catlogger.h"

static std::random_device dev;
static std::mt19937 rng(dev());

using namespace clog;

TEST(clog, manual) {
    Logger::clear_s();
    Logger::getInstance().log("manual", 0, "some text");
    Logger::getInstance().printAll(std::cout);
}

TEST(clog, stringbuf) {
    std::stringstream out;
    Logger::clear_s();
    clog::L("manual", 0) << "some text and a number " << 5 << " and a double " << 5.4 << std::endl << "and a new line";
    Logger::getInstance().printAll(std::cout);
    Logger::getInstance().printCat(out, "no such category");
    EXPECT_EQ(0, out.str().size());
    Logger::getInstance().printCat(out, "manual");
    EXPECT_GT(out.str().size(), 1);
    out = std::stringstream();
    Logger::getInstance().printCat(out, "manual", 0);
    EXPECT_GT(out.str().size(), 1);

    out = std::stringstream();
    Logger::getInstance().printCat(out, "manual", -1);
    EXPECT_EQ(out.str().size(), 0);
}

int main(int argc, char** argv) {
    Logger::getInstance().addListener(std::cout);

    testing::InitGoogleTest(&argc, argv);
    std::cout << "RUN_ALL_TESTS return value: " << RUN_ALL_TESTS() << std::endl;
}