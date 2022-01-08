//
// Created by keren on 03/01/2022.
//

#ifndef UTILS_H
#define UTILS_H
class Utils {
public:
    static int max(const int a, const int b) {
        return a > b ? a : b;
    }

    static int min(const int a, const int b) {
        return a < b ? a : b;
    }

    static double percent(const double x, const double out_of) {
        return out_of == 0 ? 0 : x / out_of * 100.0;
    }
    static double percent(const int x, const int out_of) {
        return Utils::percent((double)x, (double)out_of);
    }
    static double percent(const double x, const int out_of) {
        return Utils::percent(x, (double)out_of);
    }
    static double percent(const int x, double out_of) {
        return Utils::percent((double)x, out_of);
    }
};
#endif //UTILS_H
