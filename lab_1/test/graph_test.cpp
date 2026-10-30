/**
 * Lab01 测试：T1、T2、T3 是教师固定验收，Supplementary 是通用要求补查。
 * 固定验收会显示实际输入和输出，运行结果由 doctest 判断，不写死通过状态。
 * 先在仓库根目录执行 cmake --build build，再分别运行：
 * ./build/graph_tests --test-case=T1 --no-colors
 * ./build/graph_tests --test-case=T2 --no-colors
 * ./build/graph_tests --test-case=T3 --no-colors
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "vendor/doctest/doctest.h"

#include <iostream>
#include <sstream>
#include <string>

#include "support/lab1_console.h"

namespace {

/**
 * 使用给定输入执行验收交互，实际图形由 Graph 管理。
 * @param commands 按行排列的菜单选项和图形参数。
 * @return 验收交互产生的全部输出。
 */
std::string runConsole(const std::string& commands) {
    std::istringstream input(commands);
    std::ostringstream output;
    Lab1Console console;
    console.run(input, output);
    return output.str();
}

/**
 * 提取实际输出中的一段列表或提示。
 * @param output 验收交互实际产生的输出。
 * @param marker 对应列表或提示的起始文字。
 * @return 起始文字至下一空行的内容；不存在时返回明确的缺失说明。
 */
std::string extractBlock(const std::string& output, const std::string& marker) {
    std::size_t start = output.find(marker);
    if (start == std::string::npos) {
        return "<未找到对应输出>";
    }
    std::size_t end = output.find("\n\n", start);
    std::string block = output.substr(start,
        end == std::string::npos ? std::string::npos : end - start);
    while (!block.empty() && block.back() == '\n') {
        block.pop_back();
    }
    return block;
}

/**
 * 在测试运行窗口中显示便于截图的验收证据，不自行判断通过或失败。
 * @param id 教师固定测试编号。
 * @param requirement 教师规定的验收内容。
 * @param commands 实际交给验收交互的输入，每一项独占一行。
 * @param actual 从真实运行结果中提取的显示内容。
 */
void showEvidence(const std::string& id, const std::string& requirement,
                  const std::string& commands, const std::string& actual) {
    std::cout << '\n' << '[' << id << "] " << requirement << "\n输入（按行）：";
    std::istringstream input(commands);
    std::string item;
    bool first = true;
    while (std::getline(input, item)) {
        if (!first) {
            std::cout << " -> ";
        }
        std::cout << (item.empty() ? "<空行>" : item);
        first = false;
    }
    std::cout << "\n实际输出：\n" << actual << '\n';
}

}

TEST_SUITE("Lab01 固定验收") {

/**
 * T1 原文：创建 R01 位置 (0,0)、宽 4 高 3，显示正确。
 */
TEST_CASE("T1") {
    const std::string commands =
        "2\nR01\n0\n0\n4\n3\n"
        "4\n0\n";
    const std::string output = runConsole(commands);
    const std::string actual = extractBlock(output, "矩形列表");
    showEvidence("T1", "创建 R01 位置 (0,0)、宽 4 高 3，显示正确。",
                 commands, actual);
    CHECK(output.find("创建成功。") != std::string::npos);
    CHECK(actual == "矩形列表（数量：1）\n矩形 R01 位置=(0,0) 宽度=4 高度=3");
    CHECK(output.find("创建失败") == std::string::npos);
}

/**
 * T2 原文：创建 R02 位置 (2,1)、宽 2 高 2，列表为 2 个。
 * 独立准备 R01，不依赖 T1 的执行顺序。
 */
TEST_CASE("T2") {
    const std::string commands =
        "2\nR01\n0\n0\n4\n3\n"
        "2\nR02\n2\n1\n2\n2\n"
        "4\n0\n";
    const std::string output = runConsole(commands);
    const std::string actual = extractBlock(output, "矩形列表");
    showEvidence("T2", "创建 R02 位置 (2,1)、宽 2 高 2，列表为 2 个。",
                 commands, actual);
    CHECK(actual == "矩形列表（数量：2）\n"
                    "矩形 R01 位置=(0,0) 宽度=4 高度=3\n"
                    "矩形 R02 位置=(2,1) 宽度=2 高度=2");
    CHECK(output.find("创建失败") == std::string::npos);
}

/**
 * T3 原文：非法菜单项有提示。
 * 另检查提示后仍能查看列表并正常退出。
 */
TEST_CASE("T3") {
    const std::string commands = "99\n3\n4\n0\n";
    const std::string output = runConsole(commands);
    const std::string actual = extractBlock(output, "非法菜单项") + '\n'
        + extractBlock(output, "点列表") + '\n'
        + extractBlock(output, "矩形列表") + '\n'
        + extractBlock(output, "已退出。");
    showEvidence("T3", "非法菜单项有提示。", commands, actual);
    std::size_t error = output.find("非法菜单项，请重新选择。");
    std::size_t points = output.find("点列表（数量：0）");
    std::size_t rectangles = output.find("矩形列表（数量：0）");
    std::size_t exit = output.find("已退出。");
    REQUIRE(error != std::string::npos);
    REQUIRE(points != std::string::npos);
    REQUIRE(rectangles != std::string::npos);
    REQUIRE(exit != std::string::npos);
    CHECK(error < points);
    CHECK(points < rectangles);
    CHECK(rectangles < exit);
}

}

TEST_SUITE("Lab01 通用要求补查") {

/**
 * 补查点的显示、基础数据校验和输入健壮性。
 */
TEST_CASE("Supplementary") {
    SUBCASE("连续创建两个独立点并显示列表") {
        std::string output = runConsole("1\nP01\n0\n0\n1\nP02\n-2\n1\n3\n0\n");
        CHECK(extractBlock(output, "点列表") == "点列表（数量：2）\n"
              "点 P01 位置=(0,0)\n点 P02 位置=(-2,1)");
        CHECK(output.find("创建失败") == std::string::npos);
    }

    SUBCASE("空编号、重复编号和非法宽高不改变已有数据") {
        std::string output = runConsole(
            "1\nP01\n0\n0\n"
            "1\nP01\n2\n3\n"
            "2\nP01\n0\n0\n4\n3\n"
            "1\n\n"
            "2\nZERO_WIDTH\n0\n0\n0\n3\n"
            "2\nZERO_HEIGHT\n0\n0\n4\n0\n"
            "2\nNEGATIVE_WIDTH\n0\n0\n-4\n3\n"
            "2\nNEGATIVE_HEIGHT\n0\n0\n4\n-3\n"
            "2\nR01\n0\n0\n4\n3\n"
            "3\n4\n0\n"
        );
        CHECK(output.find("编号不能为空") != std::string::npos);
        CHECK(output.find("编号已存在") != std::string::npos);
        CHECK(output.find("矩形宽度和高度必须是有限正数") != std::string::npos);
        CHECK(extractBlock(output, "点列表") == "点列表（数量：1）\n点 P01 位置=(0,0)");
        CHECK(extractBlock(output, "矩形列表") ==
              "矩形列表（数量：1）\n矩形 R01 位置=(0,0) 宽度=4 高度=3");
    }

    SUBCASE("空输入和格式错误后仍可操作，未完成输入不会创建对象") {
        const std::string invalidInputs[] = {"", "abc", "1junk"};
        for (const std::string& invalid : invalidInputs) {
            std::string menu = runConsole(invalid + "\n3\n4\n0\n");
            CHECK(menu.find("非法菜单项") != std::string::npos);
            CHECK(menu.find("已退出。") != std::string::npos);
            std::string number = runConsole("1\nBAD_COORD\n" + invalid + "\n3\n4\n0\n");
            CHECK(number.find("请输入一个有限数值") != std::string::npos);
            CHECK(number.find("点列表（数量：0）") != std::string::npos);
            CHECK(number.find("已退出。") != std::string::npos);
        }
        CHECK(runConsole("1\nUNFINISHED\n0\n").find("创建成功") == std::string::npos);
    }
}

}
