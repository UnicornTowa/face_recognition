#include "header.h"

int main() {

// COMBINED METHODS PIXELS PREF
 /*   for(int nop = 200; nop <= 700; nop+=150) {
        auto pixels = choose_pixels(nop);
        auto base_pixels = make_base_pixels(people, pixels, nop, 1);
        for(int prec = 16; prec >= 4; prec/=2) {
            int mistakes = 0;
            int async = 0;
            auto base_hist = make_base_hist(people, prec, 1);
            for (int s = 1; s <= people; s++) {
                for (int n = 2; n <= 10; n++) {
                    int ans = combine_results(return_3(get_result_ndp(base_pixels, pixels, people, nop, s, n), people),
                                              return_3_hist(get_result_hist(base_hist, n, s, people, prec), people));
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

// COMPRESS
/*
    auto base = make_base_compress(people, 1);
    int mistakes = 0;
    for (int s = 1; s <= people; s++) {
        for (int n = 2; n <= 10; n++) {
            if (!check_ans_bool(get_result_compress(base, n, s, people), people, s))
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
*/

// MIX
/*
int nop = 500;
auto pixels = choose_pixels(nop);
auto base_pixels = add_bases(make_base(people, pixels, nop, 1), make_base_pixels(people, pixels, nop, 5), nop, people);
auto base_matrix = make_base_compress(people, 1);
int mistakes = 0;
for (int s = 1; s <= people; s++) {
    for (int n = 3; n <= 10; n++) {
        if (n == 5)
            n = 2;
        int* res = return_3(get_result_compress(base_matrix, n, s, people), people);
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
for(int nop = 50; nop <= 1000; nop+=50){
    int mistakes = 0;
    auto pixels = choose_pixels(nop);
    auto base = add_bases_3(make_base(people, pixels, nop, 1), make_base(people, pixels, nop, 5), make_base_pixels(people, pixels, nop, 8), nop, people);
    for(int s = 1; s <= people; s++){
        for(int n = 4; n <= 10; n++){
            if (n == 8)
                n = 3;
            if (n == 5)
                n = 2;
            if(!check_ans_bool(get_result_ndp(base, pixels, people, nop, s, n), people, s))
                mistakes++;
            if (n == 2)
                n = 5;
            if (n == 3)
                n = 8;
        }
    }
    //cout << get_acc(mistakes, people, 3) << endl;
    cout << "For " << nop << " pixels accuracy is " << get_acc(mistakes, people, 3) << "%" << endl;
    for (int i = 0; i < people; i++) {
        delete [] base[i];
    }
    delete [] base;
    delete [] pixels;
}
*/

// TEST (intersection of different methods best N candidates
/*
    int nop = 500;
    int prec = 256/32;
    auto pixels = choose_pixels(nop);
    auto base_pixels = add_bases(make_base(people, pixels, nop, 1), make_base_pixels(people, pixels, nop, 2), nop, people);
    auto base_matrix = make_base_compress(people, 1);
    auto base_hist = make_base_hist(people, prec, 1);
    int mistakes = 0;
    for (int s = 1; s <= people; s++) {
        for (int n = 3; n <= 10; n++) {
            //if (n == 5)
            //    n = 2;
            int* res_p = return_n(get_result_ndp(base_pixels, pixels, people, nop, s, n), 5, people);
            int* res_m = return_n(get_result_compress(base_matrix, n, s, people), 5, people);
            int* res_h = return_n(get_result_hist(base_hist, n, s, people, prec), 5, people);

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

// ANGLES
/*
for(int nop = 50; nop <= 1000; nop+=50){
    int mistakes = 0;
    auto pixels = choose_pixels(nop);
    auto base = make_base_angle(people, pixels, nop, 1);
    for(int s = 1; s <= people; s++){
        for(int n = 2; n <= 10; n++){
            if(!check_ans_bool_angle(get_result_ndp_angle(base, pixels, people, nop, s, n), people, s))
                mistakes++;
        }
    }
    cout << get_acc(mistakes, people, 1) << endl;
    //cout << "For " << nop << " pixels accuracy is " << get_acc(mistakes, people, 1) << "%" << endl;
    for (int i = 0; i < people; i++) {
        delete [] base[i];
    }
    delete [] base;
    delete [] pixels;
}
*/
// ANGLES X2
/*
for(int nop = 50; nop <= 1000; nop+=50){
    int mistakes = 0;
    auto pixels = choose_pixels(nop);
    auto base = add_bases(make_base_angle(people, pixels, nop, 1), make_base_angle(people, pixels, nop, 5), nop, people);
    for(int s = 1; s <= people; s++){
        for(int n = 3; n <= 10; n++){
            if (n == 5)
                n = 2;
            if(!check_ans_bool_angle(get_result_ndp_angle(base, pixels, people, nop, s, n), people, s))
                mistakes++;
            if (n == 2)
                n = 5;
        }
    }
    cout << get_acc(mistakes, people, 2) << endl;
    //cout << "For " << nop << " pixels accuracy is " << get_acc(mistakes, people, 1) << "%" << endl;
    for (int i = 0; i < people; i++) {
        delete [] base[i];
    }
    delete [] base;
    delete [] pixels;
}
*/

// Trying different median
/*
for(int nop = 50; nop <= 1000; nop+=50){
    int mistakes = 0;
    auto pixels = choose_pixels(nop);
    auto base = make_base_pixels(people, pixels, nop, 1);
    for(int s = 1; s <= people; s++){
        for(int n = 2; n <= 10; n++){
            if(!check_ans_bool(get_result_ndp_v2(base, pixels, people, nop, s, n), people, s))
                mistakes++;
        }
    }
    cout << get_acc(mistakes, people, 1) << " ";
    //cout << "For " << nop << " pixels accuracy is " << get_acc(mistakes, people, 3) << "%" << endl;
    for (int i = 0; i < people; i++) {
        delete [] base[i];
    }
    delete [] base;
    delete [] pixels;
}
*/


// NEW HIST BASE = PEOPLE*2 + 16HIST FOR PIC

auto people = 40;
for(int precision = 256/32; precision >= 256/32; precision/=2) { // 64 to 2
    int mistakes = 0;
    auto base = make_multibase_hist(people, precision, 3, 8);
    for (int s = 1; s <= people; s++) {
        for (int n = 1; n <= 10; n++) {
            if (n == 3 or n == 8)
                continue;
            if (check_ans_int(get_result_hist(base, n, s, people * 2, precision), 2 * people) != s) {
                //cout << "s = " << s << " n = " << n << " but found " << check_ans_int(
                //        get_result_hist(base, n, s, people, precision), people) << endl;
                mistakes++;
            }
        }
    }
    float acc = get_acc(mistakes, people, 2);
    cout << "For " << 256 / precision << " groups " << acc << "%" << endl;
    for (int i = 0; i < people; i++) {
        delete[] base[i];
    }
    delete[] base;
}





    /*for(int precision = 64; precision >= 2; precision/=2){ // 64 to 2
        int mistakes = 0;
        auto base = make_multibase_hist(people, precision, 1, 1);
        for(int s = 1; s <= people; s++){
            for(int n = 1; n <= 10; n++){
                if (n == 1 or n == 1)
                    continue;
                if(check_ans_int(get_result_hist(base, n, s, people*2, precision), 2 * people) != s) {
                    //cout << "s = " << s << " n = " << n << " but found " << check_ans_int(
                    //        get_result_hist(base, n, s, people, precision), people) << endl;
                    mistakes++;
                }
            }
        }
        float acc = get_acc(mistakes, people, 1);
        cout << "For " << 256 / precision << " groups " << acc << "%" << endl;
        for (int i = 0; i < people; i++) {
            delete [] base[i];
        }
        delete [] base;
    }*/

//RANDOM PIX, BASE = 2*people
/*
for(int nop = 50; nop <= 1000; nop += 50){
    auto pixels = choose_pixels(nop);
    auto base = make_multibase_pixels(people, pixels, nop, 1, 1);
    int mistakes = 0;
    for (int s = 1; s <= people; s++){
        for (int n = 1; n <= 10; n++){
            if (n == 1 or n == 1)
                continue;
            if (check_ans_int(get_result_ndp(base, pixels, 2*people, nop, s, n), 2*people) != s)
                mistakes++;
        }
    }
    cout << "for nop = " << nop << " got acc = " << get_acc(mistakes, people, 2) << endl;
}
*/



}