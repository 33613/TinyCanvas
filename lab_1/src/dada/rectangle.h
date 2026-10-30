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

private:
    Point position_;
    double width_;
    double height_;
};

#endif
