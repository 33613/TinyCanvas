#ifndef TINYCANVAS_RECTANGLE_H_
#define TINYCANVAS_RECTANGLE_H_

#include "point.h"
#include <cmath>
#include <stdexcept>
/**
 * 使用左下角坐标、宽度和高度表示一个与坐标轴平行的矩形。
 * 本类只保存几何参数，构造时检查宽高，编号由 Graph 管理。
 */
class Rectangle {
public:
    /**
    * 使用给定的位置和尺寸创建矩形。
    * @param position 左下角坐标。
    * @param width 必须是有限正数的宽度。
    * @param height 必须是有限正数的高度。
    * @throws std::invalid_argument 宽度或高度不合法时抛出。
    */
    Rectangle(Point position, double width, double height)
       : position_(position), width_(width), height_(height) {
       if (!std::isfinite(width) || !std::isfinite(height)
          || width <= 0 || height <= 0) {
            throw std::invalid_argument("矩形宽度和高度必须是有限正数");
        }
    }

    /**
     * 返回矩形左下角坐标的副本。
     * @return 左下角坐标。
     */
    Point getPosition() const {
        return position_;
    }

    /**
     * 返回矩形的宽度。
     * @return 宽度。
     */
    double getWidth() const {
        return width_;
    }

    /**
     * 返回矩形的高度。
     * @return 高度。
     */
    double getHeight() const {
        return height_;
    }

    // 该功能为 lab2 实现。
    /**
     * 修改矩形左下角的位置，校验失败时保留原位置。
     * @param position 新的左下角坐标，两分量必须是有限数值。
     * @throws std::invalid_argument 坐标不是有限数值时抛出。
     */
    void setPosition(Point position) {
        if (!std::isfinite(position.getX()) || !std::isfinite(position.getY())) {
            throw std::invalid_argument("矩形位置必须是有限数值");
        }
        position_ = position;
    }

    // 该功能为 lab2 实现。
    /**
     * 同时修改宽度和高度，全部校验通过后才更新尺寸。
     * @param width 新宽度，必须是有限正数。
     * @param height 新高度，必须是有限正数。
     * @throws std::invalid_argument 任一尺寸不合法时抛出，原尺寸不变。
     */
    void setSize(double width, double height) {
        if (!std::isfinite(width) || !std::isfinite(height)
            || width <= 0 || height <= 0) {
            throw std::invalid_argument("矩形宽度和高度必须是有限正数");
        }
        width_ = width;
        height_ = height;
    }

    // 该功能为 lab2 实现。
    /**
     * 计算矩形面积，不修改任何几何参数。
     * @return 宽度与高度的乘积。
     */
    double getArea() const {
        return width_ * height_;
    }

    // 该功能为 lab2 实现。
    /**
     * 计算矩形周长，不修改任何几何参数。
     * @return 宽度与高度之和的两倍。
     */
    double getPerimeter() const {
        return 2 * (width_ + height_);
    }

private:
    Point position_;
    double width_;
    double height_;
};

#endif
