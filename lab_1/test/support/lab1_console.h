#ifndef TINYCANVAS_TEST_SUPPORT_LAB1_CONSOLE_H_
#define TINYCANVAS_TEST_SUPPORT_LAB1_CONSOLE_H_

#include <iosfwd>

#include "graph.h"

/**
 * 提供 Lab01 验收使用的文本交互，仅作为测试辅助。
 * 图形由真实的 Graph 管理；输入输出均由测试提供，不属于 src 数据层。
 */
class Lab1Console {
public:
    /**
     * 连续处理创建和列表操作，直到选择退出或输入结束。
     * @param input 提供菜单选项和图形参数的输入流。
     * @param output 接收菜单、图形信息及错误提示的输出流。
     */
    void run(std::istream& input, std::ostream& output);

private:
    /**
     * 根据输入创建一个独立点，失败时提示原因，不修改已有数据。
     * @param input 提供编号和坐标的输入流。
     * @param output 接收创建结果的输出流。
     */
    void createPoint(std::istream& input, std::ostream& output);

    /**
     * 根据输入创建一个矩形，失败时提示原因，不修改已有数据。
     * @param input 提供编号、位置和尺寸的输入流。
     * @param output 接收创建结果的输出流。
     */
    void createRectangle(std::istream& input, std::ostream& output);

    /**
     * 显示全部独立点的数量、编号和坐标。
     * @param output 接收点列表的输出流。
     */
    void showPoints(std::ostream& output) const;

    /**
     * 显示全部矩形的数量、编号、位置和尺寸。
     * @param output 接收矩形列表的输出流。
     */
    void showRectangles(std::ostream& output) const;

    Graph graph_;
};

#endif
