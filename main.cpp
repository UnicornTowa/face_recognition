#include "header.h"

int main() {
    int people = 10;


 /*   for(int nop = 200; nop <= 700; nop+=150) { // COMBINED METHODS PIXELS PREF
        auto pixels = choose_pixels(nop);
        auto base_pixels = make_base(people, pixels, nop, 1);
        for(int prec = 16; prec >= 4; prec/=2) {
            int mistakes = 0;
            int async = 0;
            auto base_hist = make_base_hist(people, prec, 1);
            for (int s = 1; s <= people; s++) {
                for (int n = 2; n <= 10; n++) {
                    int ans = combine_results(return_3(get_result_ndp(base_pixels, pixels, people, nop, s, n), people),
                                              return_3_hist(result(base_hist, n, s, people, prec), people));
                    if (ans != s){
                        mistakes++;
                        if(ans == 0)
                            async++;
                    }

                }
            }
            float acc = round(100 * (9*people - static_cast<float>(mistakes)) / 9 / people * 100) / 100;
            cout << "For " << nop << " pixels and " << 256/prec << " groups accuracy is " << acc << "%, async " << async << endl;
            for (int i = 0; i < people; i++) {
                delete [] base_hist[i];
            }
            delete [] base_hist;
        }
            for (int i = 0; i < people; i++) {
                delete[] base_pixels[i];
            }
            delete[] base_pixels;
            delete[] pixels;
    }*/
    /*
    for(int nop = 200; nop <= 700; nop+=150) { // COMBINED METHODS HIST PREF
        auto pixels = choose_pixels(nop);
        auto base_pixels = make_base(people, pixels, nop, 1);
        for(int prec = 16; prec >= 4; prec/=2) {
            int mistakes = 0;
            int async = 0;
            auto base_hist = make_base_hist(people, prec, 1);
            for (int s = 1; s <= people; s++) {
                for (int n = 2; n <= 10; n++) {
                    int ans = combine_results(return_3_hist(result(base_hist, n, s, people, prec), people),
                                              return_3(get_result_ndp(base_pixels, pixels, people, nop, s, n), people));
                    if (ans != s){
                        mistakes++;
                        if(ans == 0)
                            async++;
                    }

                }
            }
            float acc = round(100 * (9*people - static_cast<float>(mistakes)) / 9 / people * 100) / 100;
            cout << "For " << nop << " pixels and " << 256/prec << " groups accuracy is " << acc << "%, async " << async << endl;
            for (int i = 0; i < people; i++) {
                delete [] base_hist[i];
            }
            delete [] base_hist;
        }
        for (int i = 0; i < people; i++) {
            delete[] base_pixels[i];
        }
        delete[] base_pixels;
        delete[] pixels;
    }
    */
    // COMPRESS


    auto base = make_base_matrix(people, 1);
    int mistakes = 0;
    for (int s = 1; s <= people; s++) {
        for (int n = 2; n <= 10; n++) {
            if (!check_ans_bool(result_m(base, n, s, people), people, s))
                mistakes++;
        }
    }
    float acc = round(100 * (9*people - static_cast<float>(mistakes)) / 9 / people * 100) / 100;
    cout << "Accuracy is " << acc << "% " << endl;
    for (int i = 0; i < people; i++) {
        for ( int j = 0; j < 28; j++)
            delete [] base[i][j];
        delete [] base[i];
    }
    delete [] base;
    return 0;

    /*
    int nop = 500;
    auto pixels = choose_pixels(nop);
    auto base_pixels = add_bases(make_base(people, pixels, nop, 1), make_base(people, pixels, nop, 5), nop, people);
    auto base_matrix = make_base_matrix(people, 1);
    int mistakes = 0;
    for (int s = 1; s <= people; s++) {
        for (int n = 3; n <= 10; n++) {
            if (n == 5)
                n = 2;
            int* res = return_3(result_m(base_matrix, n, s, people), people);
            int** afterbase = new int*[3];
            for(int i = 0; i < 3; i++) {
                afterbase[i] = base_pixels[res[i] - 1];
            }
            if(res[check_ans_int(get_result_ndp(afterbase, pixels, 3, nop, s, n), 3) - 1] != s)
                mistakes++;
            //for(int i = 0; i < 3; i++) {
            //    cout << afterbase[i][0];
            //    delete[] afterbase[i];
            //}
            delete [] afterbase;
            delete [] res;
            if ( n == 2)
                n = 5;
        }
    }
    float acc = round(100 * (8*people - static_cast<float>(mistakes)) / 8 / people * 100) / 100;
    cout << "Accuracy is " << acc << "% " << endl;
    for (int i = 0; i < people; i++) {
        delete [] base_pixels[i];
        for ( int j = 0; j < 28; j++)
            delete [] base_matrix[i][j];
        delete [] base_matrix[i];
    }
    delete [] base_pixels;
    delete [] base_matrix;
    delete [] pixels;
    */

    // X3 PIX



/*
    int nop = 500;
    int prec = 256/32;
    auto pixels = choose_pixels(nop);
    auto base_pixels = add_bases(make_base(people, pixels, nop, 1), make_base(people, pixels, nop, 2), nop, people);
    auto base_matrix = make_base_matrix(people, 1);
    auto base_hist = make_base_hist(people, prec, 1);
    int mistakes = 0;
    for (int s = 1; s <= people; s++) {
        for (int n = 3; n <= 10; n++) {
            //if (n == 5)
            //    n = 2;
            int* res_p = return_n(get_result_ndp(base_pixels, pixels, people, nop, s, n), 5, people);
            int* res_m = return_n(result_m(base_matrix, n, s, people), 5, people);
            int* res_h = return_n(result(base_hist, n, s, people, prec), 5, people);

            auto res = intersect(res_p, res_m, res_h);
            cout << res.second;
            if (res.second == 0)
                mistakes++;
            for(int i = 0; i < res.second; i++){
                if (res.first[i] == s)
                    break;
                if (i == res.second - 1)
                    mistakes++;
            }
            delete [] res.first;
            delete [] res_m;
            delete [] res_p;
            delete [] res_h;
            //if (n == 2)
            //    n = 5;
        }
    }
    float acc = round(100 * (8 * people - static_cast<float>(mistakes)) / 8 / people * 100) / 100;
    cout << "Accuracy is " << acc << "% " << endl;
    for (int i = 0; i < people; i++) {
        delete[] base_pixels[i];
        for (int j = 0; j < 28; j++)
            delete[] base_matrix[i][j];
        delete[] base_matrix[i];
    }
    delete[] base_pixels;
    delete[] base_matrix;
    delete[] pixels;

    */
}