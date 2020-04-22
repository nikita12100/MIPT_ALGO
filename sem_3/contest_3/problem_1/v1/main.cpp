#include <iostream>
#include <cmath>

class segment3 {
public:
    segment3(double _x_1, double _y_1, double _z_1, double _x_2, double _y_2, double _z_2);

private:
    double x_1;
    double y_1;
    double z_1;
    double x_2;
    double y_2;
    double z_2;
};

segment3::segment3(double _x_1, double _y_1, double _z_1, double _x_2, double _y_2, double _z_2) :
        x_1(_x_1),
        x_2(_x_2),
        y_1(_y_1),
        y_2(_y_2),
        z_1(_z_1),
        z_2(_z_2) {};

double ras(double x1, double y1, double x2, double y2, double x3, double y3) {
    double k, d;
    if (x1 == x2) { //Если отрезок вертикальный - меняем местами координаты каждой точки.
        std::swap(x1, y1);
        std::swap(x2, y2);
        std::swap(x3, y3);
    }
    k = (y1 - y2) / (x1 - x2);//Ищем коэффициенты уравнения прямой, которому принадлежит данный отрезок.
    d = y1 - k * x1;
    double xz = (x3 * x2 - x3 * x1 + y2 * y3 - y1 * y3 + y1 * d - y2 * d) / (k * y2 - k * y1 + x2 - x1);
    double dl = -1;
    if ((xz <= x2 && xz >= x1) || (xz <= x1 && xz >= x2))
        dl = sqrt((x3 - xz) * (x3 - xz) + (y3 - xz * k - d) *
                                          (y3 - xz * k -
                                           d));//Проверим лежит ли основание высоты на отрезке.
    return dl;
}

int main() {
    double x_1, x_2;
    double y_1, y_2;
    double z_1, z_2;
    double x_3, x_4;
    double y_3, y_4;
    double z_3, z_4;

    double dl, dl1, dl2, dl3, dl4, min = -1, o, o1, o2, t = -2, s = -2;

    std::cin >> x_1 >> y_1 >> z_1 >> x_2 >> y_2 >> z_2;
    segment3 s_1 = segment3(x_1, y_1, z_1, x_2, y_2, z_2);

    std::cin >> x_3 >> y_3 >> z_3 >> x_4 >> y_4 >> z_4;
    segment3 s_2 = segment3(x_3, y_3, z_3, x_4, y_4, z_4);

    o = (x_2 - x_1) * (-y_4 + y_3) - (y_2 - y_1) * (-x_4 + x_3);
    o1 = (x_2 - x_1) * (y_3 - y_1) - (y_2 - y_1) * (x_3 - x_1);
    o2 = (-y_4 + y_3) * (x_3 - x_1) - (-x_4 + x_3) * (y_3 - y_1);
    if (o != 0) {
        t = o1 / o;
        s = o2 / o;
    }
    if ((t >= 0 && s >= 0) && (t <= 1 && s <= 1))min = 0;//Проверим пересекаются ли отрезки.
    else {
        //Найдём наименьшую высоту опущенную из конца одного отрезка на другой.
        dl1 = ras(x_1, y_1, x_2, y_2, x_3, y_3);
        min = dl1;
        dl2 = ras(x_1, y_1, x_2, y_2, x_4, y_4);
        if ((dl2 < min && dl2 != -1) || min == -1)min = dl2;
        dl3 = ras(x_3, y_3, x_4, y_4, x_1, y_1);
        if ((dl3 < min && dl3 != -1) || min == -1)min = dl3;
        dl4 = ras(x_3, y_3, x_4, y_4, x_2, y_2);
        if ((dl4 < min && dl4 != -1) || min == -1)min = dl4;
        if (min == -1) {
            //В случае, если невозможно опустить высоту найдём минимальное расстояние между точками.
            dl1 = sqrt((x_1 - x_3) * (x_1 - x_3) + (y_1 - y_3) * (y_1 - y_3));
            min = dl1;
            dl2 = sqrt((x_2 - x_4) * (x_2 - x_4) + (y_2 - y_4) * (y_2 - y_4));
            if (dl2 < min)min = dl2;
            dl3 = sqrt((x_2 - x_3) * (x_2 - x_3) + (y_2 - y_3) * (y_2 - y_3));
            if (dl3 < min)min = dl3;
            dl4 = sqrt((x_1 - x_4) * (x_1 - x_4) + (y_1 - y_4) * (y_1 - y_4));
            if (dl4 < min)min = dl4;
        }
    }
    std::cout << min;
    return 0;
}