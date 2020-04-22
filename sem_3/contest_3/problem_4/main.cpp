#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <set>

double const INFTY = 1e9;


struct point {
    double x;
    double y;
    double z;
    int id;
    point *next, *prev;

    point(double x = 0, double y = 0, double z = 0, int id = -1);
    
    bool make_event();

    friend bool operator<(const point& p_1, const point& p_2);
    friend point operator-(const point& p_1, const point& p_2);
    friend double vec_composition(const point &p_1, const point &p_2);
};

point::point(double _x, double _y, double _z, int _id):
        x(_x),
        y(_y),
        z(_z),
        id(_id),
        next(nullptr),
        prev(nullptr)
        {}

bool point::make_event() {
    if (prev->next != this) {
        next->prev = this;
        prev->next = this;
        return true;
    } else {
        next->prev = prev;
        prev->next = next;
        return false;
    }
}

bool operator<(const point& p_1, const point& p_2) {
    return p_1.x < p_2.x;
}
point operator-(const point& p_1, const point& p_2) {
    return point(p_1.x - p_2.x, p_1.y - p_2.y, p_1.z - p_2.z);
}
double vec_composition(const point &p_1, const point &p_2) {
    return p_1.x * p_2.y - p_1.y * p_2.x;
}



struct face {
    int first;
    int second;
    int third;
    
    face(int _first, int _second, int _third);

    friend bool operator<(const face& f_1, const face& f_2);
};

face::face(int _first, int _second, int _third):
    first(_first),
    second(_second),
    third(_third)
    {}

bool operator<(const face& f_1, const face& f_2) {
    if (f_1.first < f_2.first) {
        return true;
    } else if (f_1.first > f_2.first) {
        return false;
    } else {
        if (f_1.second < f_2.second) {
            return true;
        } else if (f_1.second > f_2.second) {
            return false;
        } else {
            return f_1.third < f_2.third;
        }
    }
}



double sign(const point *a, const point *b, const point *c) {
    if (a == nullptr || b == nullptr || c == nullptr) {
        return INFTY;
    }
    return (b->x - a->x) * (c->y - b->y) - (b->y - a->y) * (c->x - b->x);
}

double time(const point *a, const point *b, const point *c) {
    if (a == nullptr || b == nullptr || c == nullptr) {
        return INFTY;
    }
    return ((b->x - a->x) * (c->z - b->z) - (b->z - a->z) * (c->x - b->x)) / sign(a, b, c);
}




std::vector<point*> build_down_hull(std::vector<point> &points, int left, int right) {
    if (right - left == 1) {
        return std::vector<point*>();
    }
    int medium = (left + right) / 2;
    std::vector<point*> evs[2] = {
            build_down_hull(points, left, medium),
            build_down_hull(points, medium, right)
    };
    std::vector<point*> result;

    point* u = &points[medium - 1];
    point* v = &points[medium];
    for (;;) {
        if (sign(u, v, v->next) < 0) {
            v = v->next;
        } else if (sign(u->prev, u, v) < 0) {
            u = u->prev;
        } else {
            break;
        }
    }

    int p_1 = 0, p_2 = 0;
    for (double current_t = -INFTY;;) {
        point* left = nullptr;
        point* right = nullptr;
        std::vector<double> next_t(6, INFTY);

        if (p_1 < evs[0].size()) {
            left = evs[0][p_1];
            next_t[0] = time(left->prev, left, left->next);
        }
        if (p_2 < evs[1].size()) {
            right = evs[1][p_2];
            next_t[1] = time(right->prev, right, right->next);
        }
        next_t[2] = time(u, v, v->next);
        next_t[3] = time(u, v->prev, v);
        next_t[4] = time(u->prev, u, v);
        next_t[5] = time(u, u->next, v);

        int min_i = -1;
        double min_t = INFTY;
        for (int i = 0; i < 6; i++) {
            if (next_t[i] > current_t && next_t[i] < min_t) {
                min_t = next_t[i];
                min_i = i;
            }
        }
        if (min_i == -1 || min_t >= INFTY) {
            break;
        }

        switch (min_i) {
            case 0:
                if (left->x < u->x) {
                    result.push_back(left);
                }
                left->make_event();
                p_1++;
                break;
            case 1:
                if (right->x > v->x) {
                    result.push_back(right);
                }
                right->make_event();
                p_2++;
                break;
            case 2:
                result.push_back(v);
                v = v->next;
                break;
            case 3:
                v = v->prev;
                result.push_back(v);
                break;
            case 4:
                result.push_back(u);
                u = u->prev;
                break;
            case 5:
                u = u->next;
                result.push_back(u);
                break;
            default:
                break;
        }
        current_t = min_t;
    }
    u->next = v;
    v->prev = u;
    for (int i = static_cast<int>(result.size() - 1); i >= 0; i--) {
        point* current = result[i];
        if (current->x > u->x && current->x < v->x) {
            u->next = v->prev = current;
            current->prev = u;
            current->next = v;
            if (current->x <= points[medium - 1].x) {
                u = current;
            } else {
                v = current;
            }
        } else {
            current->make_event();
            if (current == u) {
                u = u->prev;
            }
            if (current == v) {
                v = v->next;
            }
        }
    }
    return result;
}



std::vector<face> build_convex_hull(std::vector<point> points) {
    int n = points.size();
    std::vector<face> result;
    std::sort(points.begin(), points.end());
    std::vector<point*> events = build_down_hull(points, 0, n);
    for (point* event : events) {
        face current(event->prev->id, event->id, event->next->id);
        if (!event->make_event())
            std::swap(current.first, current.second);

        result.push_back(current);
    }
    for (point &p : points) {
        p.next = nullptr;
        p.prev = nullptr;
        p.z = -p.z;
    }
    events = build_down_hull(points, 0, n);
    for (point* event : events) {
        face current(event->prev->id, event->id, event->next->id);
        if (event->make_event()) {
            std::swap(current.first, current.second);
        }
        result.push_back(current);
    }
    return result;
}



bool is_acceptable(const point &a, const point &b, const point &c) {
    return vec_composition(b - a, c - b) > 0;
}



typedef std::pair<int, int> Edge;


double count_avg_voronoi_edge(std::vector<point> & sites) {
    std::vector<face> dimentional_hull = build_convex_hull(sites);
    std::set<Edge> edges;
    std::vector<int> edges_number(sites.size());
    std::vector<bool> is_in_planar_hull(sites.size(), false);

    for (face& face : dimentional_hull) {
        Edge edge_i[3];
        edge_i[0] = std::make_pair(face.first, face.second);
        edge_i[1] = std::make_pair(face.second, face.third);
        edge_i[2] = std::make_pair(face.third, face.first);

        for(auto i:edge_i){
            Edge edge(i.first, i.second);

            if (edge.first > edge.second) {
             std::swap(edge.first, edge.second);
            }
            edges.insert(edge);
        }

    }
    for (const Edge &edge : edges) {
        edges_number[edge.first]++;
        edges_number[edge.second]++;
    }

    std::sort(sites.begin(), sites.end());
    std::vector<point> planar_hull;
    for (const point &site : sites) {
        while (planar_hull.size() >= 2) {
            if (is_acceptable(planar_hull[planar_hull.size() - 2], planar_hull.back(), site)) {
                break;
            }
            planar_hull.pop_back();
        }
        planar_hull.push_back(site);
    }
    for (int i = sites.size() - 2, bottom = planar_hull.size(); i >= 0; --i) {
        while (static_cast<int>(planar_hull.size()) > bottom) {
            if (is_acceptable(planar_hull[planar_hull.size() - 2], planar_hull.back(), sites[i])) {
                break;
            }
            planar_hull.pop_back();
        }
        planar_hull.push_back(sites[i]);
    }
    for (auto i : planar_hull)
        is_in_planar_hull[i.id] = true;


    int delaunay_inner_pts = 0;
    int summ_degree = 0;
    for (int i = 0; i < sites.size(); i++) {
        if (!is_in_planar_hull[i]) {
            summ_degree += edges_number[i];
            delaunay_inner_pts++;
        }
    }

    return (delaunay_inner_pts == 0) ? 0.0 : static_cast<double>(summ_degree) / delaunay_inner_pts;
}



int main() {
    std::vector<point> points;
    double x = 0;
    double y = 0;
    int i = 0;
    while (std::cin >> x >> y){
        point p(x, y, x * x + y * y, i++);
        points.push_back(p);
    }

    std::cout << count_avg_voronoi_edge(points) << "\n";

    return 0;
}