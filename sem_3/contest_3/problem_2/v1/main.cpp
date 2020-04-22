#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cfloat>
#include <limits.h>

const double EPSILON =  0.0000000001;


struct point {
public:
    point(): x(-1), y(-1), z(-1), counter(-1) {}
    point(int _x, int _y, int _z, int _counter) : x(_x), y(_y), z(_z), counter(_counter) {}

    [[nodiscard]] int get_x() const {
        return x;
    }

    [[nodiscard]] int get_y() const {
        return y;
    }

    [[nodiscard]] int get_z() const {
        return z;
    }

    [[nodiscard]] int get_counter() const {
        return counter;
    }

    [[nodiscard]] double length() const {
        return sqrt(x * x + y * y + z * z);
    }


private:
    int x;
    int y;
    int z;
    int counter;
};

point operator-(const point &a, const point &b) {
    return point(a.get_x() - b.get_x(), a.get_y() - b.get_y(), a.get_z() - b.get_z(), -1);
}

bool operator==(const point &a, const point &b) {
    return (a.get_x() == b.get_x() && a.get_y() == b.get_y() && a.get_z() == b.get_z());
}

struct edge {
    edge(): begin(-1), end(-1), face_point(-1) {}
    edge(int _begin, int _end, int _face_point) : begin(_begin), end(_end), face_point(_face_point) {}

    int begin;
    int end;
    int face_point;
};

struct plane {
    double A;   // my_plane = Ax+By+Cz, (A, B, C) - normal
    double B;
    double C;

    point a;
    point b;
    point c;

    plane(point a, point b, point c) : a(a), b(b), c(c) {
        point v = b - a;
        point u = c - a;
        A = v.get_y() * u.get_z() - v.get_z() * u.get_y();
        B = v.get_z() * u.get_x() - v.get_x() * u.get_z();
        C = v.get_x() * u.get_y() - v.get_y() * u.get_x();
    }

    [[nodiscard]] double length() {
        return sqrt(A * A + B * B + C * C);
    }
};

double sin_from_line_and_plane(const point a, const point b, double A, double B, double C) {
    point v = b - a;
    double len = sqrt(A * A + B * B + C * C);
    return (A * v.get_x() + B * v.get_y() + C * v.get_z()) / (len * v.length());
}

double cos_from_plane_and_plane(point a, point b, point c, point d) {
    plane p_1 = plane(a, b, c);
    plane p_2 = plane(a, b, d);

    if (fabs(p_1.length()) <= EPSILON || fabs(p_2.length()) <= EPSILON) {
        return 1;
    }

    return  (p_1.A * p_2.A + p_1.B * p_2.B + p_1.C * p_2.C) / (p_1.length() * p_2.length());
}

bool operator<(const std::vector<int> &a, const std::vector<int> &b) {
    unsigned int min_len = std::min(a.size(), b.size());
    for(unsigned int i = 0; i < min_len; i++){
        if (a[i] != b[i])
            return (a[i] < b[i]);
    }
    return a.size() < b.size();
}

class convex_hull {
public:
    explicit convex_hull(int ver_count);

    void add_point(double x_, double y_, double z_, int vertex_count_);

    void wrapping();

    void sort_plate();

    void print();

private:
    std::vector <point> point_array;
    std::vector <edge> edge_stak; //edge_stak[i][j] = k, if exists edge i,j,k
    std::vector <std::vector<int>> my_plane;
};

convex_hull::convex_hull(int ver_count) : point_array(0), my_plane(0), edge_stak(0) {}

void convex_hull::add_point(double x_, double y_, double z_, int vertex_count_) {
    point_array.emplace_back(x_, y_, z_, vertex_count_);
}


void convex_hull::wrapping() {
    // bottom point, min(Oz)
    point first = point_array[0];
    for (auto point : point_array) {
        if (first.get_z() > point.get_z()) {
            first = point;
        }
    }
    // find min edge
    point second;
    auto min_sin = DBL_MAX;
    for (auto point : point_array) {
        if (point == first) {
            continue;
        }
        if (sin_from_line_and_plane(first, point, 0, 0, 1) < min_sin + EPSILON) {
            min_sin = sin_from_line_and_plane(first, point, 0, 0, 1);
            second = point;
        }
    }
    // find third point for start plane
    point third;
    point additional;
    double max_cos = 0;
    additional = point(second.get_y() - first.get_y() + first.get_x(), first.get_x() - second.get_x() + first.get_y(),
                       first.get_z(), -1);
    for (const auto point : point_array) {
        if (point == first || point == second) {
            continue;
        }
        if (fabs(cos_from_plane_and_plane(first, second, additional, point)) > max_cos - EPSILON) {
            third = point;
            max_cos = fabs(cos_from_plane_and_plane(first, second, additional, point));
        }
    }
    // it's out start plane
    plane lower_plane(first, second, third);
    // put all possible pairs of edge in stack
    if (lower_plane.C < 0) {
        my_plane.push_back({first.get_counter(), second.get_counter(), third.get_counter()});
        edge_stak.emplace_back(first.get_counter(), second.get_counter(), third.get_counter());
        edge_stak.emplace_back(second.get_counter(), third.get_counter(), first.get_counter());
        edge_stak.emplace_back(third.get_counter(), first.get_counter(), second.get_counter());
    } else {
        my_plane.push_back({third.get_counter(), second.get_counter(), first.get_counter()});
        edge_stak.emplace_back(third.get_counter(), second.get_counter(), first.get_counter());
        edge_stak.emplace_back(second.get_counter(), first.get_counter(), third.get_counter());
        edge_stak.emplace_back(first.get_counter(), third.get_counter(), second.get_counter());
    }

    while (!edge_stak.empty()) {
        edge current_edge = edge_stak[0];
        const point current_start = point_array[current_edge.begin];
        const point current_end = point_array[current_edge.end];
        const point current_face = point_array[current_edge.face_point];
        point next;
        auto min_cos = DBL_MAX;
        for (auto point : point_array) {
            if (point == current_start || point == current_end || point == current_face) {
                continue;
            }
            if (min_cos + EPSILON > cos_from_plane_and_plane(current_start, current_end, current_face, point)) {
                min_cos = cos_from_plane_and_plane(current_start, current_end, current_face, point);
                next = point;
            }
        }
        edge_stak.erase(edge_stak.begin()); // pop
        bool fst_edge_is_used = false;
        bool snd_edge_is_used = false;
        int pos_fst_edge = 0;
        int pos_snd_edge = 0;
        for (int i = 0; i < edge_stak.size(); i++) {
            const edge edge = edge_stak[i];
            const point edge_start = point_array[edge.begin];
            const point edge_end = point_array[edge.end];
            if ((edge_start == current_start && edge_end == next) ||
                (edge_end == current_start && edge_start == next)) {
                fst_edge_is_used = true;
                pos_fst_edge = i;
            }
            if ((edge_start == current_end && edge_end == next) ||
                (edge_end == current_end && edge_start == next)) {
                snd_edge_is_used = true;
                pos_snd_edge = i;
            }
        }
        if (fst_edge_is_used) {
            edge_stak.erase(edge_stak.begin() + pos_fst_edge);
        }
        if (snd_edge_is_used) {
            edge_stak.erase(edge_stak.begin() + pos_snd_edge);
        }

        my_plane.push_back({current_end.get_counter(), current_start.get_counter(), next.get_counter()});
        if (!fst_edge_is_used) {
            edge_stak.emplace_back(current_edge.begin, next.get_counter(), current_edge.end);
        }
        if (!snd_edge_is_used) {
            edge_stak.emplace_back(next.get_counter(), current_edge.end, current_edge.begin);
        }
    }
}

void convex_hull::sort_plate() {
    for(unsigned int i = 0; i < my_plane.size(); i++) {
        int my_min = INT_MAX;
        int min_index = 0;
        for (int j = 0; j < 3; j++) {
            if (my_min > my_plane[i][j]) {
                my_min = my_plane[i][j];
                min_index = j;
            }
        }
        my_plane[i] = {my_plane[i][min_index], my_plane[i][(min_index + 1) % 3], my_plane[i][(min_index + 2) % 3]};
    }
    std::sort(my_plane.begin(), my_plane.end());
}

void convex_hull::print(){
    std::cout << my_plane.size() << '\n';
    for (auto element : my_plane) {
        std::cout << "3 " << element[0] << " " << element[1] << " " << element[2] << "\n";
    }
}



int main() {
    int test_count = 0;
    std::cin >> test_count;
    for (int i = 0; i < test_count; i++) {
        int vertex_count = 0;
        std::cin >> vertex_count;
        convex_hull conv_hull(vertex_count);
        for (int j = 0; j < vertex_count; j++) {
            double x = 0;
            double y = 0;
            double z = 0;
            std::cin >> x >> y >> z;
            conv_hull.add_point(x, y, z, j);
        }
        conv_hull.wrapping();
        conv_hull.sort_plate();
        conv_hull.print();
    }
}
