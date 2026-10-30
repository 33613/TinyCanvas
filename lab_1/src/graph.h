#ifndef TINYCANVAS_GRAPH_H_
#define TINYCANVAS_GRAPH_H_

#include <cmath>
#include <string>
#include <utility>
#include <vector>

#include "dada/point.h"
#include "dada/rectangle.h"

/**
 * 管理多个点和矩形的编号、添加、删除与遍历。
 * 每条记录使用 pair 保存编号和图形，所有记录按添加顺序存入 vector。
 * 编号在当前 Graph 的点和矩形之间唯一。图形按值保存，随容器自动释放。
 */
class Graph {
public:
    /**
     * 添加一个带编号的点，保存 point 的副本。
     * @param id 点的编号，不得为空白或与已有图形重复。
     * @param point 待保存的点，坐标必须是有限数值。
     * @return 添加成功返回 true；编号或坐标不合法时返回 false，不修改已有记录。
     */
    bool addPoint(const std::string& id, const Point& point) {
        if (!isIdAvailable(id) || !std::isfinite(point.getX())
            || !std::isfinite(point.getY())) {
            return false;
        }
        points_.push_back(std::make_pair(id, point));
        return true;
    }

    /**
     * 添加一个带编号的矩形，保存 rectangle 的副本。
     * @param id 矩形的编号，不得为空白或与已有图形重复。
     * @param rectangle 待保存的矩形，坐标须有限，宽高须为有限正数。
     * @return 添加成功返回 true；编号或参数不合法时返回 false，不修改已有记录。
     */
    bool addRectangle(const std::string& id, const Rectangle& rectangle) {
        Point position = rectangle.getPosition();
        if (!isIdAvailable(id) || !std::isfinite(position.getX())
            || !std::isfinite(position.getY()) || !std::isfinite(rectangle.getWidth())
            || !std::isfinite(rectangle.getHeight()) || rectangle.getWidth() <= 0
            || rectangle.getHeight() <= 0) {
            return false;
        }
        rectangles_.push_back(std::make_pair(id, rectangle));
        return true;
    }

    /**
     * 按编号删除一个点，保留其余点的顺序。删除后该编号可重新使用。
     * @param id 待删除点的编号。
     * @return 找到并删除时返回 true；不存在时返回 false，不修改已有记录。
     */
    bool removePoint(const std::string& id) {
        for (auto it = points_.begin(); it != points_.end(); ++it) {
            if (it->first == id) {
                points_.erase(it);
                return true;
            }
        }
        return false;
    }

    /**
     * 按编号删除一个矩形，保留其余矩形的顺序。删除后该编号可重新使用。
     * @param id 待删除矩形的编号。
     * @return 找到并删除时返回 true；不存在时返回 false，不修改已有记录。
     */
    bool removeRectangle(const std::string& id) {
        for (auto it = rectangles_.begin(); it != rectangles_.end(); ++it) {
            if (it->first == id) {
                rectangles_.erase(it);
                return true;
            }
        }
        return false;
    }

    /**
     * 返回全部点的只读列表，供范围 for 循环遍历。
     * 记录的 first 是编号，second 是坐标。列表只在当前 Graph 存活时有效。
     * 添加或删除点可能使已有的元素引用和迭代器失效。
     * @return 按添加顺序保存的点列表的常量引用。
     */
    const std::vector<std::pair<std::string, Point>>& getPoints() const {
        return points_;
    }

    /**
     * 返回全部矩形的只读列表，供范围 for 循环遍历。
     * 记录的 first 是编号，second 是矩形。列表只在当前 Graph 存活时有效。
     * 添加或删除矩形可能使已有的元素引用和迭代器失效。
     * @return 按添加顺序保存的矩形列表的常量引用。
     */
    const std::vector<std::pair<std::string, Rectangle>>& getRectangles() const {
        return rectangles_;
    }

private:
    /**
     * 检查编号是否可供新图形使用，不修改已有记录。
     * @param id 待检查的编号。
     * @return 非空白且未被任何点或矩形使用时返回 true，否则返回 false。
     */
    bool isIdAvailable(const std::string& id) const {
        if (id.find_first_not_of(" \t\n\r\f\v") == std::string::npos) {
            return false;
        }
        for (const auto& point : points_) {
            if (point.first == id) {
                return false;
            }
        }
        for (const auto& rectangle : rectangles_) {
            if (rectangle.first == id) {
                return false;
            }
        }
        return true;
    }

    std::vector<std::pair<std::string, Point>> points_;
    std::vector<std::pair<std::string, Rectangle>> rectangles_;
};

#endif
