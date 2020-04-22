#include <iostream>
#include <cmath>

const double E = 0.000000000001;    // precision

class vector3 {
public:
    vector3(double _x, double _y, double _z);

    friend vector3 operator*(double c, const vector3 &v);   // is visible like public and can change private
    friend vector3 operator+(const vector3 &v_1, const vector3 &v_2);

    friend vector3 operator-(const vector3 &v_1, const vector3 &v_2);

    double norm();

    friend double sum_sqr_coord(const vector3 &v_1, const vector3 &v_2);    // x^2 + y^2 + z^2

private:
    double x;
    double y;
    double z;
};


class segment3 {
public:
    segment3(const vector3 &_from, const vector3 &_to);

    std::pair<vector3, vector3> get_segment();

private:
    vector3 from;
    vector3 to;
};

///---------------------------------vector3---------------------------------------
vector3::vector3(double _x, double _y, double _z)
        : x(_x), y(_y), z(_z) {}

vector3 operator*(double c, const vector3 &v) {
    return vector3(c * v.x, c * v.y, c * v.z);
}

vector3 operator+(const vector3 &v_1, const vector3 &v_2) {
    return vector3(v_1.x + v_2.x, v_1.y + v_2.y, v_1.z + v_2.z);
}

vector3 operator-(const vector3 &v_1, const vector3 &v_2) {
    return vector3(v_1.x - v_2.x, v_1.y - v_2.y, v_1.z - v_2.z);
}

double sum_sqr_coord(const vector3 &v_1, const vector3 &v_2) {  // x^2 + y^2 + z^2
    return v_1.x * v_2.x + v_1.y * v_2.y + v_1.z * v_2.z;
}

double vector3::norm() {
    return std::sqrt(sum_sqr_coord(*this, *this));
}

///---------------------------------segment3---------------------------
segment3::segment3(const vector3 &_from, const vector3 &_to)
        : from(_from), to(_to) {}

std::pair<vector3, vector3> segment3::get_segment() {
    return std::make_pair(from, to);
}

double segment_to_segment_dist(segment3 s_1, segment3 s_2) {
    vector3 u = s_1.get_segment().first - s_1.get_segment().second;
    vector3 v = s_2.get_segment().first - s_2.get_segment().second;
    vector3 w = s_1.get_segment().second - s_2.get_segment().second;

    double u_u = sum_sqr_coord(u, u);
    double u_v = sum_sqr_coord(u, v);
    double v_v = sum_sqr_coord(v, v);
    double u_w = sum_sqr_coord(u, w);
    double v_w = sum_sqr_coord(v, w);

    double d = u_u * v_v - u_v * u_v;
    double s, s_n, s_d = d;
    double t, t_n, t_d = d;

    if (d <= E) {
        t_n = v_w;
        t_d = v_v;
        s_d = 1.0;
        s_n = 0.0;
    } else {
        t_n = (u_u * v_w - u_v * u_w);
        s_n = (u_v * v_w - v_v * u_w);
        if (s_n <= 0.0) {
            t_n = v_w;
            t_d = v_v;
            s_n = 0.0;
        } else if (s_n >= s_d) {
            s_n = s_d;
            t_n = v_w + u_v;
            t_d = v_v;
        }
    }

    if (t_n <= 0.0) {
        t_n = 0.0;
        if (-u_w <= 0.0) {
            s_n = 0.0;
        } else if (-u_w >= u_u) {
            s_n = s_d;
        } else {
            s_n = -u_w;
            s_d = u_u;
        }
    } else if (t_n >= t_d) {
        t_n = t_d;
        if ((-u_w + u_v) <= 0.0) {
            s_n = 0;
        } else if ((-u_w + u_v) >= u_u) {
            s_n = s_d;
        } else {
            s_n = (-u_w + u_v);
            s_d = u_u;
        }
    }

    if (std::abs(s_n) <= E)
        s = 0.0;
    else
        s = s_n / s_d;
    if (std::abs(t_n) <= E)
        t = 0.0;
    else
        t = t_n / t_d;

    vector3 d_p = w + (s * u) - (t * v);
    return d_p.norm();
}


int main() {
    double x1, y1, z1, x2, y2, z2;
    std::cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
    segment3 s_1 = segment3(vector3(x1, y1, z1), vector3(x2, y2, z2));

    double x3, y3, z3, x4, y4, z4;
    std::cin >> x3 >> y3 >> z3 >> x4 >> y4 >> z4;
    segment3 s_2 = segment3(vector3(x3, y3, z3), vector3(x4, y4, z4));

    printf("%.12f", segment_to_segment_dist(s_1, s_2));

    return 0;
}
