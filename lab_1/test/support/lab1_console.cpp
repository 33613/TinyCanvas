#include "lab1_console.h"

#include <cmath>
#include <istream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace {

/**
 * 读取非空白编号，失败时说明原因。
 * @param input 提供编号的输入流。
 * @param output 接收提示的输出流。
 * @param id 接收读取到的编号。
 * @return 读取到非空白编号时返回 true，否则返回 false。
 */
bool readId(std::istream& input, std::ostream& output, std::string& id) {
    output << "编号：";
    if (!std::getline(input, id)) {
        return false;
    }
    if (id.find_first_not_of(" \t\n\r\f\v") == std::string::npos) {
        output << "创建失败：编号不能为空。\n";
        return false;
    }
    return true;
}

/**
 * 从一整行输入中读取一个有限数值，拒绝空行及额外文本。
 * @param input 提供数值的输入流。
 * @param output 接收提示的输出流。
 * @param prompt 说明当前需要输入的参数。
 * @param value 接收读取到的数值，只有返回 true 时才可使用。
 * @return 读取成功返回 true；输入结束或格式错误返回 false。
 */
bool readNumber(std::istream& input, std::ostream& output,
                const std::string& prompt, double& value) {
    output << prompt;
    std::string line;
    if (!std::getline(input, line)) {
        return false;
    }
    std::istringstream parser(line);
    std::string extra;
    if (!(parser >> value) || (parser >> extra) || !std::isfinite(value)) {
        output << "创建失败：请输入一个有限数值。\n";
        return false;
    }
    return true;
}

/**
 * 显示一个独立点的编号和坐标。
 * @param output 接收图形信息的输出流。
 * @param id 点的编号。
 * @param point 待显示的点。
 */
void printPoint(std::ostream& output, const std::string& id, const Point& point) {
    output << "点 " << id << " 位置=(" << point.getX() << ',' << point.getY()
           << ")\n";
}

/**
 * 显示一个矩形的编号、左下角坐标、宽度和高度。
 * @param output 接收图形信息的输出流。
 * @param id 矩形的编号。
 * @param rectangle 待显示的矩形。
 */
void printRectangle(std::ostream& output, const std::string& id,
                    const Rectangle& rectangle) {
    Point position = rectangle.getPosition();
    output << "矩形 " << id << " 位置=(" << position.getX() << ',' << position.getY()
           << ") 宽度=" << rectangle.getWidth() << " 高度=" << rectangle.getHeight()
           << '\n';
}

}

void Lab1Console::run(std::istream& input, std::ostream& output) {
    std::string line;
    while (true) {
        output << "\n1 创建点\n2 创建矩形\n3 查看全部点\n4 查看全部矩形\n0 退出\n请选择：";
        if (!std::getline(input, line)) {
            return;
        }
        std::istringstream parser(line);
        int choice = -1;
        std::string extra;
        if (!(parser >> choice) || (parser >> extra)) {
            output << "非法菜单项，请重新选择。\n";
            continue;
        }
        switch (choice) {
            case 0:
                output << "已退出。\n";
                return;
            case 1:
                createPoint(input, output);
                break;
            case 2:
                createRectangle(input, output);
                break;
            case 3:
                showPoints(output);
                break;
            case 4:
                showRectangles(output);
                break;
            default:
                output << "非法菜单项，请重新选择。\n";
                break;
        }
    }
}

void Lab1Console::createPoint(std::istream& input, std::ostream& output) {
    std::string id;
    double x = 0;
    double y = 0;
    if (!readId(input, output, id) || !readNumber(input, output, "横坐标：", x)
        || !readNumber(input, output, "纵坐标：", y)) {
        return;
    }
    Point point(x, y);
    if (!graph_.addPoint(id, point)) {
        output << "创建失败：编号已存在。\n";
        return;
    }
    output << "创建成功。\n";
    printPoint(output, id, point);
}

void Lab1Console::createRectangle(std::istream& input, std::ostream& output) {
    std::string id;
    double x = 0;
    double y = 0;
    double width = 0;
    double height = 0;
    if (!readId(input, output, id) || !readNumber(input, output, "横坐标：", x)
        || !readNumber(input, output, "纵坐标：", y)
        || !readNumber(input, output, "宽度：", width)
        || !readNumber(input, output, "高度：", height)) {
        return;
    }
    try {
        Rectangle rectangle(Point(x, y), width, height);
        if (!graph_.addRectangle(id, rectangle)) {
            output << "创建失败：编号已存在。\n";
            return;
        }
        output << "创建成功。\n";
        printRectangle(output, id, rectangle);
    } catch (const std::invalid_argument& error) {
        output << "创建失败：" << error.what() << '\n';
    }
}

void Lab1Console::showPoints(std::ostream& output) const {
    output << "点列表（数量：" << graph_.getPoints().size() << "）\n";
    for (const auto& point : graph_.getPoints()) {
        printPoint(output, point.first, point.second);
    }
}

void Lab1Console::showRectangles(std::ostream& output) const {
    output << "矩形列表（数量：" << graph_.getRectangles().size() << "）\n";
    for (const auto& rectangle : graph_.getRectangles()) {
        printRectangle(output, rectangle.first, rectangle.second);
    }
}
