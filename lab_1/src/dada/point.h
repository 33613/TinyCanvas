#ifndef TINYCANVAS_POINT_H_
#define TINYCANVAS_POINT_H_

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

private:
    double x_;
    double y_;
};

#endif
