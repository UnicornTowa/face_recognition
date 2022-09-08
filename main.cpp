#include "facerec.h"

int main(){
    int nop = 500;
    int people = 15;
    int s = 8;
    int n = 2;

    auto pixels = choose_pixels(nop);

    auto base = make_base(people, pixels, nop);

    int* res = new int[people];
    auto target = extract_vector_and_delete_matrix(make_arr(s, n), pixels, nop);
    for (int i = 0; i < people; i++){
        res[i] = calculate_difference(base[i], target, nop);
    }
    cout << "RESULT" << endl;

    for (int i = 0; i < people; i++) {
        cout << res[i] << endl;
        delete [] base[i];
    }
    delete [] base;
    delete [] res;
    delete [] target;
    delete [] pixels;
    return 0;
}
