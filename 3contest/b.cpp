#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <istream>
#include <ostream>
#include <vector>

/*! \class Point2d
    \brief Two-dimensional point(may also be treated as two-dimensional vector).

    Provides addition, subtraction, negation, comparison operations with two-dimensional corteges.
*/
template <typename Type>
class Point2d {
    template <typename AnotherType>
    friend class Point2d;

public:
    //---------------------------------------------------------------------------------
    Point2d() : Point2d(0, 0) {
    }

    Point2d(Type x, Type y) : x_(x), y_(y) {
    }
    //---------------------------------------------------------------------------------
    Type X() const {
        return x_;
    }

    Type Y() const {
        return y_;
    }

    void SetX(Type x) {
        x_ = x;
    }

    void SetY(Type y) {
        y_ = y;
    }

    template <typename AnotherType>
    void operator+=(const Point2d<AnotherType> &another_point) {
        x_ += static_cast<Type>(another_point.x_);
        y_ += static_cast<Type>(another_point.y_);
    }

    template <typename AnotherType>
    void operator-=(const Point2d<AnotherType> &another_point) {
        x_ -= static_cast<Type>(another_point.x_);
        y_ -= static_cast<Type>(another_point.y_);
    }

    template <typename AnotherType>
    Point2d<Type> operator+(const Point2d<AnotherType> &another_point) const {
        return {x_ + static_cast<Type>(another_point.x_), y_ + static_cast<Type>(another_point.y_)};
    }

    template <typename AnotherType>
    Point2d<Type> operator-(const Point2d<AnotherType> &another_point) const {
        return {x_ - static_cast<Type>(another_point.x_), y_ - static_cast<Type>(another_point.y_)};
    }

    Point2d<Type> operator-() const {
        return {-x_, -y_};
    }

    template <typename AnotherType>
    bool operator==(const Point2d<AnotherType> &another_point) const {
        return (x_ == static_cast<Type>(another_point.x_)) && (y_ == static_cast<Type>(another_point.y_));
    }

    template <typename AnotherType>
    bool operator!=(const Point2d<AnotherType> &another_point) const {
        return !(*this == another_point);
    }

    template <typename AnotherType>
    bool operator<=(const Point2d<AnotherType> &another_point) const {
        return x_ == static_cast<Type>(another_point.x_) ? y_ <= static_cast<Type>(another_point.y_)
                                                         : x_ <= static_cast<Type>(another_point.x_);
    }

    template <typename AnotherType>
    bool operator<(const Point2d<AnotherType> &another_point) const {
        return x_ == static_cast<Type>(another_point.x_) ? y_ < static_cast<Type>(another_point.y_)
                                                         : x_ < static_cast<Type>(another_point.x_);
    }

    template <typename AnotherType>
    bool operator>=(const Point2d<AnotherType> &another_point) const {
        return x_ == static_cast<Type>(another_point.x_) ? y_ >= static_cast<Type>(another_point.y_)
                                                         : x_ >= static_cast<Type>(another_point.x_);
    }

    template <typename AnotherType>
    bool operator>(const Point2d<AnotherType> &another_point) const {
        return x_ == static_cast<Type>(another_point.x_) ? y_ > static_cast<Type>(another_point.y_)
                                                         : x_ > static_cast<Type>(another_point.x_);
    }

private:
    //-----------------------------------Variables-------------------------------------
    Type x_;
    Type y_;
};

/// \brief Calculates distance between two points
/// \param p1 - first  point
/// \param p2 - second point
/// \return Distance between two points
template <typename Type, typename AnotherType>
double Distance(const Point2d<Type> &p1, const Point2d<AnotherType> &p2) {
    auto dx = static_cast<double>(static_cast<Type>(p2.X()) - p1.X());
    auto dy = static_cast<double>(static_cast<Type>(p2.Y()) - p1.Y());

    return sqrt(dx * dx + dy * dy);
}

/// \brief Calculates skew product of two vectors
/// \param v1 - first  vector
/// \param v2 - second vector
/// \return Skew product of two vectors
///
/// Skew product is asymmetrical: v1 is being rotated COUNTERCLOCKWISE!
///
template <typename Type, typename AnotherType>
int64_t SkewProduct(const Point2d<Type> &v1, const Point2d<AnotherType> &v2) {
    return (v1.X() * static_cast<Type>(v2.Y()) - v1.Y() * static_cast<Type>(v2.X()));
}

template <typename Type>
std::istream &operator>>(std::istream &in, Point2d<Type> &point) {
    Type temp_x = 0;
    Type temp_y = 0;
    std::cin >> temp_x >> temp_y;
    point.SetX(temp_x);
    point.SetY(temp_y);

    return in;
}

template <typename Type>
std::ostream &operator<<(std::ostream &out, const Point2d<Type> &point) {
    out << point.X() << " " << point.Y();

    return out;
}

/// \brief Compares two points by their polar angle
/// \param p1 - first  point
/// \param p2 - second point
/// \param rel_point - point polar angle of all other points is dependent on
/// \return True if polar angle of p1 is smaller than polar angle p2
///         or equal and distance from rel_point to p1 is smaller that distance from rel_point to p2
bool PolarAngleComparator(const Point2d<int64_t> &p1, const Point2d<int64_t> &p2, const Point2d<int64_t> &rel_point) {
    int64_t skew_prod = SkewProduct(p1 - rel_point, p2 - rel_point);

    return ((skew_prod < 0) || ((skew_prod == 0) && (Distance(rel_point, p1) < Distance(rel_point, p2))));
}

/// \brief Builds convex hull using Graham's algorithm
/// \param points - vector of points for which convex hull should be built
/// \return Vector with points that form convex hull of points given NOTE: result is enumerated in CLOCKWISE order!
///
/// Graham's algorithm:
///     First of all, we search for left bottom point. Then we sort all other points by polar angle relative to that
///     point. After that we go through the sorted array of points. On each point we check whether adding it (let it be
///     cur_point) to convex hull makes it not convex anymore. If it does, we delete points from the convex hull until
///     it becomes convex again with cur_point as last point or contains only one point.
///
std::vector<Point2d<int64_t>> GetConvexHull(const std::vector<Point2d<int64_t>> &points) {
    auto points_begin = points.cbegin();
    auto points_iterator = points_begin + 1;
    auto points_end = points.cend();
    Point2d<int64_t> left_bottom_point = *points_begin;
    while (points_iterator != points_end) {
        Point2d<int64_t> cur_point = *points_iterator;
        if ((cur_point.X() < left_bottom_point.X()) ||
            ((cur_point.X() == left_bottom_point.X()) && (cur_point.Y() < left_bottom_point.Y()))) {
            left_bottom_point = cur_point;
        }

        ++points_iterator;
    }

    std::vector<Point2d<int64_t>> all_points = points;
    std::sort(all_points.begin(), all_points.end(), [&](const Point2d<int64_t> &p1, const Point2d<int64_t> &p2) {
        return PolarAngleComparator(p1, p2, left_bottom_point);
    });
    std::vector<Point2d<int64_t>> result;
    uint64_t cur_result_size = 0;
    uint64_t deletion_threshold =
        2;  // to decide whether we should omit last point added to result
            // we need to have at least one point in our convex hull, which means result size should be >= 2
    auto all_points_begin = all_points.cbegin();
    auto all_points_iterator = all_points_begin;
    auto all_points_end = all_points.cend();
    while (all_points_iterator != all_points_end) {
        Point2d<int64_t> cur_point = *all_points_iterator;
        while (cur_result_size >= deletion_threshold) {
            auto hull_part_last_points_iterator = result.cend() - 1;
            Point2d<int64_t> last_included_point = *hull_part_last_points_iterator;
            Point2d<int64_t> penultimate_included_point = *(hull_part_last_points_iterator - 1);

            Point2d<int64_t> new_border = cur_point - last_included_point;
            Point2d<int64_t> last_border = last_included_point - penultimate_included_point;

            if (SkewProduct(new_border, last_border) <= 0) {
                result.pop_back();

                --cur_result_size;
            } else {
                break;
            }
        }

        result.push_back(cur_point);

        ++cur_result_size;

        ++all_points_iterator;
    }

    return result;
}

double GetPolygonArea(const std::vector<Point2d<int64_t>> &polygon_contour) {
    int64_t result = 0;

    auto polygon_contour_begin = polygon_contour.cbegin();
    auto polygon_contour_iterator = polygon_contour_begin + 1;
    auto polygon_contour_end = polygon_contour.cend();
    while (polygon_contour_iterator != polygon_contour_end) {
        Point2d<int64_t> prev_point = *(polygon_contour_iterator - 1);
        Point2d<int64_t> cur_point = *polygon_contour_iterator;

        result += prev_point.X() * cur_point.Y() - cur_point.X() * prev_point.Y();

        ++polygon_contour_iterator;
    }

    Point2d<int64_t> first_point = *polygon_contour_begin;
    Point2d<int64_t> last_point = *(polygon_contour_end - 1);

    result += last_point.X() * first_point.Y() - first_point.X() * last_point.Y();

    return std::abs((static_cast<double>(result)) / 2);
}

int main() {
    int n = 0;
    std::cin >> n;
    std::vector<Point2d<int64_t>> points(n);
    auto points_iterator = points.begin();
    auto points_end = points.cend();
    while (points_iterator != points_end) {
        Point2d<int64_t> input;
        std::cin >> input;
        *points_iterator = input;

        ++points_iterator;
    }

    std::vector<Point2d<int64_t>> hull = GetConvexHull(points);
    uint64_t hull_size = hull.size();
    std::cout << hull_size << std::endl;
    auto hull_iterator = hull.cbegin();
    auto hull_end = hull.cend();
    while (hull_iterator != hull_end) {
        std::cout << *hull_iterator << std::endl;

        ++hull_iterator;
    }

    std::cout << std::fixed << std::setprecision(1) << GetPolygonArea(hull) << std::endl;

    return 0;
}

