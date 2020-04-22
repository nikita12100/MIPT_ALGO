#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <algorithm>

const double E = 0.00001;


class vector2 {
public:
    vector2(double x, double y);


    friend vector2 operator+(const vector2 &v1, const vector2 &v2);
    friend vector2 operator-(const vector2 &v1, const vector2 &v2);

    double norm(); // sqrt(dot)

    friend double dot(const vector2 &v1, const vector2 &v2);  // x_1^2 + x_2^2

    double x;
    double y;
};


bool less_polar_angle(const vector2 &v1, const vector2 &v2) {
    if (v1.x > 0 && v2.x < 0) {
        return true;
    }
    if (v1.x < 0 && v2.x > 0) {
        return false;
    }

    double tang_1 = v1.y / v1.x;
    double tang_2 = v2.y / v2.x;

    if (fabs(v1.x) < E) {
        if (v1.y > 0) {
            return v2.x < 0;
        } else {
            return false;
        }
    }
    if (fabs(v2.x) < E) {
        if (v2.y > 0) {
            return v1.x > 0;
        } else {
            return fabs(tang_1 - tang_2) > E;
        }
    }

    if (fabs(tang_1 - tang_2) < E) {
        return false;
    }
    return tang_1 < tang_2;
}

std::vector<vector2> minkowski_sum(const std::vector<vector2> &polygon_1, const std::vector<vector2> &polygon_2) {
    std::vector<vector2> close_polygon_1 = polygon_1;   // to close polygon
    close_polygon_1.push_back(close_polygon_1[0]);
    std::vector<vector2> close_polygon_2 = polygon_2;
    close_polygon_2.push_back(close_polygon_2[0]);

    std::vector<vector2> sum_polygon;
    int i = 0, j = 0;
    while (i < close_polygon_1.size() - 1 && j < close_polygon_2.size() - 1) {
        sum_polygon.push_back(close_polygon_1[i] + close_polygon_2[j]);
        if (less_polar_angle(close_polygon_1[i + 1] - close_polygon_1[i], close_polygon_2[j + 1] - close_polygon_2[j])) {
            i++;
        } else if (less_polar_angle(close_polygon_2[j + 1] - close_polygon_2[j], close_polygon_1[i + 1] - close_polygon_1[i])) {
            j++;
        } else {
            i++;
            j++;
        }
    }

    return sum_polygon;
}

bool is_different_signs(double a, double b) {
    return (a < 0 && b > 0) || (a > 0 && b < 0);
}

bool contains_zero(const std::vector<vector2> &polygon) {
    std::vector<double> x_0;    // applicants for (0,0)

    if (is_different_signs(polygon[0].y, polygon[polygon.size() - 1].y))    // close
        x_0.push_back((polygon[0].x + polygon[polygon.size() - 1].x));

    for (int i = 0; i < polygon.size() - 1; i++)
        if (is_different_signs(polygon[i].y, polygon[i + 1].y))
            x_0.push_back((polygon[i].x + polygon[i + 1].x));

    for (auto i : polygon)
        if (fabs(i.y) < E)
            x_0.push_back(i.x);


    if (x_0.empty())
        return false;


    double min = x_0[0];
    double max = x_0[0];
    for (auto i: x_0) {
        if (i < min)
            min = i;
        if (i > max)
            max = i;
    }

    return fabs(min) < E || is_different_signs(max, min);
}

std::vector<vector2> normalize(const std::vector<vector2> &polygon) {
    int min = 0;    // min - left down
    for (int i = 1; i < polygon.size(); i++)
        if (polygon[i].x < polygon[min].x || (fabs(polygon[i].x - polygon[min].x) < E  && polygon[i].y < polygon[min].y))
            min = i;

    std::vector<vector2> norm_polygon = polygon;    // min will be last
    for (int i = 0; i < polygon.size(); i++)
        norm_polygon[polygon.size() - i - 1] = (polygon[(i + min + 1) % polygon.size()]);

    return norm_polygon;
}

vector2::vector2(double x, double y)
        : x(x), y(y) {}


vector2 operator+(const vector2 &v1, const vector2 &v2) {
    return vector2(v1.x + v2.x, v1.y + v2.y);
}
vector2 operator-(const vector2 &v1, const vector2 &v2) {
    return vector2(v1.x - v2.x, v1.y - v2.y);
}
double vector2::norm() {
    return sqrt(dot(*this, *this));
}
double dot(const vector2 &v1, const vector2 &v2) {
    return v1.x * v2.x + v1.y * v2.y;
}


int main() {
    unsigned int n = 0;
    unsigned int m = 0;

    std::vector<vector2> polygon_1;
    std::cin >> n;
    for (int i = 0; i < n; i++) {
        double x, y;
        std::cin >> x >> y;
        polygon_1.emplace_back(x, y);
    }

    // if {exist p| p in polygon_1 and p in polygon_2}, then {exist p| p in polygon_1 and -p in -polygon_2}
    // then {0 in mink_summ(polygon_1, -polygon_2)}
    std::vector<vector2> polygon_2; // -polygon_2
    std::cin >> m;
    for (int i = 0; i < m; i++) {
        double x, y;
        std::cin >> x >> y;
        polygon_2.emplace_back(-x, -y);
    }

    polygon_1 = normalize(polygon_1);
    polygon_2 = normalize(polygon_2);

    for (auto i : polygon_1) {
        std::cout << i.x << ' ' << i.y << '\n';
    }

    for (auto i : polygon_2) {
        std::cout << i.x << ' ' << i.y << '\n';
    }

    std::vector<vector2> mink_sum = minkowski_sum(polygon_1, polygon_2);

    contains_zero(mink_sum) ? std::cout << "YES\n" : std::cout << "NO\n";

    return 0;
}