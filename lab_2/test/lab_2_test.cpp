#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "vendor/doctest/doctest.h"

#include <stdexcept>
#include <string>

#include "graph.h"

/**
 * 验证 R01 的面积和周长计算正确。
 */
TEST_CASE("T1") {
    Graph graph;

    REQUIRE(graph.addRectangle("R01", Rectangle(Point(0, 0), 4, 3)));

    const Rectangle* rectangle = graph.findRectangle("R01");
    REQUIRE(rectangle != nullptr);

    CHECK(rectangle->getArea() == doctest::Approx(12));
    CHECK(rectangle->getPerimeter() == doctest::Approx(14));
}

/**
 * 验证非法尺寸不能创建矩形
 */
TEST_CASE("T2"){
    CHECK_THROWS_AS(Rectangle(Point(0, 0), 0, 3),std::invalid_argument);
    
    CHECK_THROWS_AS(Rectangle(Point(0, 0), -5, 3),std::invalid_argument);

    CHECK_THROWS_AS(Rectangle(Point(0, 0), 4, 0),std::invalid_argument);

    CHECK_THROWS_AS(Rectangle(Point(0, 0), 4, -3),std::invalid_argument);
}

/**
 * 验证修改后真正保存了新数据
 */
TEST_CASE("T3"){
    Graph graph;
    REQUIRE(graph.addRectangle("R01", Rectangle(Point(0,0), 4, 3)));
    
    REQUIRE(graph.updateRectangle("R01", Point(2, 1), 6, 5));

    const Rectangle* rectangle = graph.findRectangle("R01");
    REQUIRE(rectangle != nullptr);

    CHECK(rectangle->getWidth() == doctest::Approx(6));
    CHECK(rectangle->getHeight() == doctest::Approx(5));
    CHECK(rectangle->getPosition().getX() == doctest::Approx(2));
    CHECK(rectangle->getPosition().getY() == doctest::Approx(1));

    CHECK(rectangle->getArea() == doctest::Approx(30));
    CHECK(rectangle->getPerimeter() == doctest::Approx(22));
}

// 功能要求补查
TEST_SUITE("Lab02 通用要求补查") {

/**
 * 按编号修改点的位置，不改变其他点和矩形。
 */
TEST_CASE("修改点的位置与对象隔离") {
    Graph graph;
    REQUIRE(graph.addPoint("P01", Point(0, 0)));
    REQUIRE(graph.addPoint("P02", Point(3, 4)));
    REQUIRE(graph.addRectangle("R01", Rectangle(Point(0, 0), 4, 3)));

    REQUIRE(graph.updatePoint("P01", -2, 1));

    const Point* point = graph.findPoint("P01");
    const Point* otherPoint = graph.findPoint("P02");
    const Rectangle* rectangle = graph.findRectangle("R01");
    REQUIRE(point != nullptr);
    REQUIRE(otherPoint != nullptr);
    REQUIRE(rectangle != nullptr);
    CHECK(point->getX() == doctest::Approx(-2));
    CHECK(point->getY() == doctest::Approx(1));
    CHECK(otherPoint->getX() == doctest::Approx(3));
    CHECK(otherPoint->getY() == doctest::Approx(4));
    CHECK(rectangle->getPosition().getX() == doctest::Approx(0));
    CHECK(rectangle->getPosition().getY() == doctest::Approx(0));
    CHECK(graph.getPoints().size() == 2);
    CHECK(graph.getRectangles().size() == 1);
}

/**
 * 修改列表中的第二个矩形，验证按编号定位，其他对象保持原值。
 */
TEST_CASE("修改指定矩形不影响其他对象") {
    Graph graph;
    REQUIRE(graph.addPoint("P01", Point(1, 2)));
    REQUIRE(graph.addRectangle("R01", Rectangle(Point(3, 4), 4, 3)));
    REQUIRE(graph.addRectangle("R02", Rectangle(Point(5, 6), 2, 2)));

    REQUIRE(graph.updateRectangle("R02", Point(-2, 1), 6, 5));
    const Rectangle* updated = graph.findRectangle("R02");
    const Rectangle* other = graph.findRectangle("R01");
    const Point* point = graph.findPoint("P01");
    REQUIRE(updated != nullptr);
    REQUIRE(other != nullptr);
    REQUIRE(point != nullptr);
    CHECK(updated->getPosition().getX() == doctest::Approx(-2));
    CHECK(updated->getPosition().getY() == doctest::Approx(1));
    CHECK(updated->getWidth() == doctest::Approx(6));
    CHECK(updated->getHeight() == doctest::Approx(5));
    CHECK(updated->getArea() == doctest::Approx(30));
    CHECK(updated->getPerimeter() == doctest::Approx(22));
    CHECK(other->getPosition().getX() == doctest::Approx(3));
    CHECK(other->getPosition().getY() == doctest::Approx(4));
    CHECK(other->getWidth() == doctest::Approx(4));
    CHECK(other->getHeight() == doctest::Approx(3));
    CHECK(point->getX() == doctest::Approx(1));
    CHECK(point->getY() == doctest::Approx(2));
    CHECK(graph.getPoints().size() == 1);
    CHECK(graph.getRectangles().size() == 2);
}

/**
 * 不存在或类型不匹配的编号不能查到对象，也不能修改其他对象。
 */
TEST_CASE("找不到编号时已有对象不变") {
    Graph graph;
    REQUIRE(graph.addPoint("P01", Point(1, 2)));
    REQUIRE(graph.addRectangle("R01", Rectangle(Point(3, 4), 4, 3)));

    CHECK(graph.findPoint("MISSING") == nullptr);
    CHECK(graph.findRectangle("MISSING") == nullptr);
    CHECK(graph.findPoint("R01") == nullptr);
    CHECK(graph.findRectangle("P01") == nullptr);
    CHECK_FALSE(graph.updatePoint("MISSING", 8, 9));
    CHECK_FALSE(graph.updateRectangle("MISSING", Point(8, 9), 6, 5));
    CHECK_FALSE(graph.updatePoint("R01", 8, 9));
    CHECK_FALSE(graph.updateRectangle("P01", Point(8, 9), 6, 5));

    const Point* point = graph.findPoint("P01");
    const Rectangle* rectangle = graph.findRectangle("R01");
    REQUIRE(point != nullptr);
    REQUIRE(rectangle != nullptr);
    CHECK(point->getX() == doctest::Approx(1));
    CHECK(point->getY() == doctest::Approx(2));
    CHECK(rectangle->getPosition().getX() == doctest::Approx(3));
    CHECK(rectangle->getPosition().getY() == doctest::Approx(4));
    CHECK(rectangle->getWidth() == doctest::Approx(4));
    CHECK(rectangle->getHeight() == doctest::Approx(3));
    CHECK(graph.getPoints().size() == 1);
    CHECK(graph.getRectangles().size() == 1);
}

/**
 * 非法宽高修改被拒绝，原位置、尺寸和计算结果均保留。
 */
TEST_CASE("非法修改不破坏矩形") {
    Graph graph;
    REQUIRE(graph.addRectangle("R01", Rectangle(Point(1, 2), 4, 3)));
    REQUIRE(graph.addRectangle("R02", Rectangle(Point(5, 6), 2, 2)));
    const double invalidSizes[][2] = {{0, 8}, {-1, 8}, {6, 0}, {6, -1}};

    for (const auto& size : invalidSizes) {
        INFO("尝试修改为宽度 " << size[0] << "，高度 " << size[1]);
        CHECK_THROWS_AS(
            graph.updateRectangle("R01", Point(9, 9), size[0], size[1]),
            std::invalid_argument
        );
        const Rectangle* rectangle = graph.findRectangle("R01");
        REQUIRE(rectangle != nullptr);
        CHECK(rectangle->getPosition().getX() == doctest::Approx(1));
        CHECK(rectangle->getPosition().getY() == doctest::Approx(2));
        CHECK(rectangle->getWidth() == doctest::Approx(4));
        CHECK(rectangle->getHeight() == doctest::Approx(3));
        CHECK(rectangle->getArea() == doctest::Approx(12));
        CHECK(rectangle->getPerimeter() == doctest::Approx(14));
    }

    const Rectangle* other = graph.findRectangle("R02");
    REQUIRE(other != nullptr);
    CHECK(other->getPosition().getX() == doctest::Approx(5));
    CHECK(other->getPosition().getY() == doctest::Approx(6));
    CHECK(other->getWidth() == doctest::Approx(2));
    CHECK(other->getHeight() == doctest::Approx(2));
    CHECK(graph.getRectangles().size() == 2);

    // 同时验证 Rectangle 自身的公开修改接口，而不仅是 Graph 的包装接口。
    Rectangle rectangle(Point(1, 2), 4, 3);
    for (const auto& size : invalidSizes) {
        CHECK_THROWS_AS(rectangle.setSize(size[0], size[1]), std::invalid_argument);
        CHECK(rectangle.getWidth() == doctest::Approx(4));
        CHECK(rectangle.getHeight() == doctest::Approx(3));
    }
}

/**
 * const 查询可重复执行，查询不改变位置、尺寸、编号或对象数量。
 */
TEST_CASE("重复计算不改变对象状态") {
    Graph graph;
    REQUIRE(graph.addRectangle("R01", Rectangle(Point(2, 1), 4, 3)));
    const Graph& readOnlyGraph = graph;
    const Rectangle* rectangle = readOnlyGraph.findRectangle("R01");
    REQUIRE(rectangle != nullptr);

    for (int i = 0; i < 3; ++i) {
        CHECK(rectangle->getArea() == doctest::Approx(12));
        CHECK(rectangle->getPerimeter() == doctest::Approx(14));
    }
    CHECK(rectangle->getPosition().getX() == doctest::Approx(2));
    CHECK(rectangle->getPosition().getY() == doctest::Approx(1));
    CHECK(rectangle->getWidth() == doctest::Approx(4));
    CHECK(rectangle->getHeight() == doctest::Approx(3));
    REQUIRE(readOnlyGraph.getRectangles().size() == 1);
    CHECK(readOnlyGraph.getRectangles()[0].first == "R01");
}

}

TEST_SUITE("Lab01 数据功能回归") {

/**
 * 用 Lab02 的类连续创建两个点和两个矩形，并读取完整列表数据。
 */
TEST_CASE("连续创建与列表读取") {
    Graph graph;
    REQUIRE(graph.addPoint("P01", Point(0, 0)));
    REQUIRE(graph.addPoint("P02", Point(-2, 1)));
    REQUIRE(graph.addRectangle("R01", Rectangle(Point(0, 0), 4, 3)));
    REQUIRE(graph.addRectangle("R02", Rectangle(Point(2, 1), 2, 2)));

    const auto& points = graph.getPoints();
    const auto& rectangles = graph.getRectangles();
    REQUIRE(points.size() == 2);
    REQUIRE(rectangles.size() == 2);
    CHECK(points[0].first == "P01");
    CHECK(points[0].second.getX() == doctest::Approx(0));
    CHECK(points[0].second.getY() == doctest::Approx(0));
    CHECK(points[1].first == "P02");
    CHECK(points[1].second.getX() == doctest::Approx(-2));
    CHECK(points[1].second.getY() == doctest::Approx(1));
    CHECK(rectangles[0].first == "R01");
    CHECK(rectangles[0].second.getPosition().getX() == doctest::Approx(0));
    CHECK(rectangles[0].second.getPosition().getY() == doctest::Approx(0));
    CHECK(rectangles[0].second.getWidth() == doctest::Approx(4));
    CHECK(rectangles[0].second.getHeight() == doctest::Approx(3));
    CHECK(rectangles[1].first == "R02");
    CHECK(rectangles[1].second.getPosition().getX() == doctest::Approx(2));
    CHECK(rectangles[1].second.getPosition().getY() == doctest::Approx(1));
    CHECK(rectangles[1].second.getWidth() == doctest::Approx(2));
    CHECK(rectangles[1].second.getHeight() == doctest::Approx(2));
}

/**
 * 空编号和重复编号被拒绝，编号在点和矩形之间也不能重复。
 */
TEST_CASE("编号校验与已有数据保护") {
    Graph graph;
    REQUIRE(graph.addPoint("P01", Point(1, 2)));
    REQUIRE(graph.addRectangle("R01", Rectangle(Point(3, 4), 4, 3)));

    for (const std::string& id : {std::string(""), std::string(" \t")}) {
        CHECK_FALSE(graph.addPoint(id, Point(8, 9)));
        CHECK_FALSE(graph.addRectangle(id, Rectangle(Point(8, 9), 6, 5)));
    }
    CHECK_FALSE(graph.addPoint("P01", Point(8, 9)));
    CHECK_FALSE(graph.addRectangle("R01", Rectangle(Point(8, 9), 6, 5)));
    CHECK_FALSE(graph.addPoint("R01", Point(8, 9)));
    CHECK_FALSE(graph.addRectangle("P01", Rectangle(Point(8, 9), 6, 5)));

    const Point* point = graph.findPoint("P01");
    const Rectangle* rectangle = graph.findRectangle("R01");
    REQUIRE(point != nullptr);
    REQUIRE(rectangle != nullptr);
    CHECK(point->getX() == doctest::Approx(1));
    CHECK(point->getY() == doctest::Approx(2));
    CHECK(rectangle->getPosition().getX() == doctest::Approx(3));
    CHECK(rectangle->getPosition().getY() == doctest::Approx(4));
    CHECK(rectangle->getWidth() == doctest::Approx(4));
    CHECK(rectangle->getHeight() == doctest::Approx(3));
    CHECK(graph.getPoints().size() == 1);
    CHECK(graph.getRectangles().size() == 1);
}

}
