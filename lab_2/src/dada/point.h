#ifndef TINYCANVAS_POINT_H_
#define TINYCANVAS_POINT_H_

#include <cmath>
#include <stdexcept>

/**
 * 表示一个二维坐标，只保存位置，不保存编号。
 */
class Point {
public:
    /**
     * 使用给定的横、纵坐标创建点。
     * @param x 横坐标。
     * @param y 纵坐标。
     */
    Point(double x, double y) : x_(x), y_(y) {}

    /**
     * 返回点的横坐标。
     * @return 横坐标。
     */
    double getX() const {
        return x_;
    }

    /**
     * 返回点的纵坐标。
     * @return 纵坐标。
     */
    double getY() const {
        return y_;
    }

    // 该功能为 lab2 实现。
    /**
     * 修改点的位置，校验失败时保留原坐标。
     * @param x 新的横坐标，必须是有限数值。
     * @param y 新的纵坐标，必须是有限数值。
     * @throws std::invalid_argument 任一坐标不是有限数值时抛出。
     */
    void setPosition(double x, double y) {
        if (!std::isfinite(x) || !std::isfinite(y)) {
            throw std::invalid_argument("点的坐标必须是有限数值");
        }
        x_ = x;
        y_ = y;
    }

private:
    double x_;
    double y_;
};

#endif
