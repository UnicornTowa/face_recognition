#include "header.h"

// PART 1

int** picture_to_matrix(int s, int n){
    string name = "database/s" + to_string(s) + "/" + to_string(n) + ".pgm";
    int row = 0, col = 0, num_of_rows = 0, num_of_cols = 0;
    stringstream ss;
    ifstream infile(name, ios::binary);
    string inputLine;

    getline(infile,inputLine); // read the first line : P5
    if(inputLine != "P5") cerr << "Version error" << endl;
    //cout << "Version : " << inputLine << endl;

    ss << infile.rdbuf(); //read the third line : width and height
    ss >> num_of_cols >> num_of_rows;
    //cout << num_of_cols << " columns and " << num_of_rows << " rows" << endl;

    int max_val; //maximum intensity value : 255
    ss >> max_val;
    //cout<<max_val;

    unsigned char pixel;

    int **pixel_value = new int*[num_of_rows];
    for(int i = 0; i < num_of_rows; ++i) {
        pixel_value[i] = new int[num_of_cols];
    }


    int **integral = new int*[num_of_rows];
    for(int i = 0; i < num_of_rows; ++i) {
        integral[i] = new int[num_of_cols];
    }


    for (row = 0; row < num_of_rows; row++){ //record the pixel values
        for (col = 0; col < num_of_cols; col++){
            ss >> pixel;
            pixel_value[row][col]= pixel;
        }
    }


    integral[0][0]=pixel_value[0][0];
    for(int i=1; i<num_of_cols;i++){ //compute integral image
        integral[0][i]=integral[0][i-1]+pixel_value[0][i];
    }
    for (int i=1;i<num_of_rows; i++){
        integral[i][0]=integral[i-1][0]+pixel_value[i][0];
    }
    for (int i = 1; i < num_of_rows; i++){
        for (int j = 1; j < num_of_cols; j++){
            integral[i][j] = integral[i - 1 ][j] + integral [i][j - 1] - integral[i - 1] [j - 1] + pixel_value[i][j];
        }
    }
    //cout << "\nval " << pixel_value[0][1] << endl;
    infile.close();


    /*for(int i = 0; i < num_of_rows; ++i) {
        delete [] pixel_value[i];
    }
    delete [] pixel_value;*/

    for(int i = 0; i < num_of_rows; ++i) {
        delete [] integral[i];
    }
    delete [] integral;

    return pixel_value;
}

pair<int, int>* choose_pixels(int num) {
    auto* arr = new pair<int, int>[num];
    for (int i = 0; i < num; i++) {
        pair<int, int> p;
        p.first = rand()%112;
        p.second = rand()%92;
        arr[i] = p;
    }
    return arr;
}

int* extract_vector_and_delete_matrix(int** matrix, pair<int, int>* pixels, int len) {
    int* vec = new int[len];
    for (int i = 0; i < len; i++) {
        vec [i] = matrix[pixels[i].first][pixels[i].second];
    }
    delete_matrix(matrix);
    return vec;
}

int calculate_difference(int* first, int* second, int len) {
    int diff = 0;
    for(int i = 0; i < len; i++){
        diff += abs(first[i] - second[i]);
    }
    return diff;
}

void delete_matrix(int** matrix) {
    for(int i = 0; i < 112; ++i) {
        delete [] matrix[i];
    }
    delete [] matrix;
}

int** make_base_pixels(int people, pair<int, int>*pixels, int nop, int n) {
    int** base = new int*[people];
    for (int i = 0; i < people; i++){
        base[i] = extract_vector_and_delete_matrix(picture_to_matrix(i + 1, n), pixels, nop);
    }
    return base;
}

int** make_multibase_pixels(int people, pair<int, int>*pixels, int nop, int n1, int n2) {
    int** base = new int*[2*people];
    for (int i = 0; i < people; i++){
        base[i] = extract_vector_and_delete_matrix(picture_to_matrix(i + 1, n1), pixels, nop);
    }
    for (int i = 0; i < people; i++){
        base[i + people] = extract_vector_and_delete_matrix(picture_to_matrix(i + 1, n2), pixels, nop);
    }
    return base;
}

int* get_result_pixels (int** base, pair<int,int>*pixels, int people, int nop, int s, int n) {
    auto target = extract_vector_and_delete_matrix(picture_to_matrix(s, n), pixels, nop);
    int* res = new int[people];
    for (int i = 0; i < people; i++){
        res[i] = calculate_difference(base[i], target, nop);
    }
    delete [] target;
    delete [] pixels;
    return res;
}

int* get_result_ndp (int** base, pair<int,int>*pixels, int people, int nop, int s, int n) {
    auto target = extract_vector_and_delete_matrix(picture_to_matrix(s, n), pixels, nop);
    int* res = new int[people];
    for (int i = 0; i < people; i++){
        res[i] = calculate_difference(base[i], target, nop);
    }
    delete [] target;
    return res;
}

int * get_result_ndp_v2 (int** base, pair<int,int>*pixels, int people, int nop, int s, int n) {
    auto target = extract_vector_and_delete_matrix(picture_to_matrix(s, n), pixels, nop);
    auto * res = new int[people];
    for (int i = 0; i < people; i++){
        res[i] = calculate_difference_v2(base[i], target, nop);
    }
    delete [] target;
    return res;
}

int calculate_difference_v2(const int* first, const int* second, int len) {
    double diff = 0;
    for(int i = 0; i < len; i++){
        diff += atan(abs((first[i] - second[i])));
    }
    //cout << diff << endl;
    return static_cast<int>(diff);
}


int* main_sequence (int nop, int people, int s, int n) {
    auto pixels = choose_pixels(nop);
    auto base = make_base_pixels(people, pixels, nop, n);
    auto res = get_result_pixels(base, pixels, people, nop, s, n);
    for (int i = 0; i < people; i++) {
        delete [] base[i];
    }
    delete [] base;
    return res;
}

void print_res(const int* res, int people){
    for (int i = 0; i < people; i++) {
        cout << res[i] << endl;
    }
}

void check_ans(const int* res, int people, int s){
    int min = res[0];
    int index = 1;
    for(int i = 1; i < people; i++){
        if (min > res[i]) {
            min = res[i];
            index = i + 1;
        }
    }
    if (min == res[s-1])
        cout << "CORRECT answer, " << index << endl;
    else
        cout << "INCORRECT answer, " << index << " (not " << s << ")" << endl;
}

bool check_ans_bool(const int* res, int people, int s){
    int min = res[0];
    int index = 1;
    for(int i = 1; i < people; i++){
        if (min > res[i]) {
            min = res[i];
            index = i + 1;
        }
    }
    if (min == res[s-1])
        return true;
    else
        return false;
}

int check_ans_int(const int* res, int people){
    int min = res[0];
    int index = 1;
    for(int i = 1; i < people; i++){
        if (min > res[i]) {
            min = res[i];
            index = i + 1;
        }
    }
    //cout << index << endl;
    while(index > 40)
        index -= 40;
    return index;
}

int* return_3(int* res, int people){
    int* three = new int[3];
    int min = res[0];
    int index = 1;
    for(int j = 0; j < 3; j++) {
        for (int i = 0; i < people; i++) {
            if (min > res[i]) {
                min = res[i];
                index = i + 1;
            }
        }
        three[j] = index;
        min = 3000000;
        res[index-1] = 3000000;
    }
    return three;
}

int* return_n(int* res, int how_many, int people){
    int* n = new int[how_many];
    int min = res[0];
    int index = 1;
    for(int j = 0; j < how_many; j++) {
        for (int i = 0; i < people; i++) {
            if (min > res[i]) {
                min = res[i];
                index = i + 1;
            }
        }
        n[j] = index;
        min = 3000000;
        res[index-1] = 3000000;
    }
    return n;
}

int** add_bases(int** base1, int** base2, int nop, int people) {
    int** newbase = new int*[people];
    for (int i = 0; i < people; i++) {
        int* newline = new int[nop];
        for (int j = 0; j < nop; j++) {
            newline[j] = (base1[i][j]+base2[i][j]) / 2;
        }
        newbase[i] = newline;
    }
    for (int i = 0; i < people; i++){
        delete [] base1[i];
        delete [] base2[i];
    }
    delete [] base1;
    delete [] base2;
    return newbase;
}

int** add_bases_3(int** base1, int** base2, int** base3, int nop, int people) {
    int** newbase = new int*[people];
    for (int i = 0; i < people; i++) {
        int* newline = new int[nop];
        for (int j = 0; j < nop; j++) {
            newline[j] = (base1[i][j]+base2[i][j]+ base3[i][j]) / 3;
        }
        newbase[i] = newline;
    }
    for (int i = 0; i < people; i++){
        delete [] base1[i];
        delete [] base2[i];
        delete [] base3[i];
    }
    delete [] base1;
    delete [] base2;
    delete [] base3;
    return newbase;
}

// PART 2

int* return_hist(int** arr, int prec){
    int* result = new int[256/prec];
    for(int i = 0; i < 112; i++){
        for(int j = 0; j < 92; j++){
            result[arr[i][j]/(prec)] += 1;
        }
    }
    delete_matrix(arr);
    return result;
}

int get_weight(int h, int w){
    if ((h == 1 or h == 2) and (w == 1 or w == 2)) // mid 4 sectors
        return 1;
    else if ((h == 0 or h == 3) and (w == 0 or w == 3)) // 4 corner sectors
        return 4;
    else if (h == 0) // upper-mid sectors
        return 4;
    else if (h == 3) // lower-mid sectors
        return 3;
    else return 1; // side-mid sectors
}

int* return_hist_v2(int** picture, int prec){

    int* result_v2 = new int[256*16/prec];
    for(int w = 0; w < 4; w++){
        for (int h = 0; h < 4; h++){
            int correction = (4 * h + w) * 256 / prec;
            int current_weight = get_weight(h, w);
            for(int i = h * 28; i < (h + 1) * 28; i++){
                for(int j = w * 23; j < (w + 1) * 23; j++){
                    result_v2[picture[i][j] / (prec) + correction] += current_weight;
                }
            }
        }
    }
    delete_matrix(picture);
    return result_v2;
}

/*int difference(int* vec1, int* vec2, int prec){
    int nogr = 16*256/prec;
    int diff = 0;
    for(int i = 0; i < nogr; i++){
        diff += abs(vec1[i] - vec2[i]);
    }
    return diff;
}*/

int** make_base_hist(int people, int prec, int n){
    int** base = new int*[people];
    for(int i = 0; i < people; i++){
        base[i] = return_hist(picture_to_matrix(i + 1, n), prec);
    }
    return base;
}

int** make_multibase_hist(int people, int prec, int n1, int n2){
    int** base = new int*[people*2];
    for(int i = 0; i < people; i++){
        base[i] = return_hist_v2(picture_to_matrix(i + 1, n1), prec);
    }
    for(int i = 0; i < people; i++){
        base[i + people] = return_hist_v2(picture_to_matrix(i + 1, n2), prec);
    }
    return base;
}

int* get_result_hist(int** base, int n, int s, int people, int prec){
    int* cur = return_hist_v2(picture_to_matrix(s, n), prec);
    int* result = new int[people];
    auto len = 16*256/prec;
    for(int i = 0; i < people; i++){
        result[i] = calculate_difference(cur, base[i], len);
    }
    delete [] cur;
    return result;
}

bool check_ans_bool_hist(const int* res, int people, int s){
    int min = res[0];
    int index = 1;
    for(int i = 1; i < people; i++){
        if (min > res[i]) {
            min = res[i];
            index = i + 1;
        }
    }
    if (min == res[s-1])
        return true;
    else
        return false;
}

void check_ans_hist(const int* res, int people, int s){
    int min = res[0];
    int index = 1;
    for(int i = 1; i < people; i++){
        if (min > res[i]) {
            min = res[i];
            index = i + 1;
        }
    }
    if (min == res[s-1])
        cout << "CORRECT answer, " << index << endl;
    else
        cout << "INCORRECT answer, " << index << " (not " << s << ")" << endl;
}

int** add_bases_hist(int** base1, int** base2, int prec, int people) {
    int** newbase = new int*[people];
    for (int i = 0; i < people; i++) {
        int* newline = new int[256/prec];
        for (int j = 0; j < 256/prec; j++) {
            newline[j] = (base1[i][j]+base2[i][j]) / 2;
        }
        newbase[i] = newline;
    }
    for (int i = 0; i < people; i++){
        delete [] base1[i];
        delete [] base2[i];
    }
    delete [] base1;
    delete [] base2;
    return newbase;
}

int* return_3_hist(int* res, int people){
    int* three = new int[3];
    int min = res[0];
    int index = 1;
    for(int j = 0; j < 3; j++) {
        for (int i = 1; i < people; i++) {
            if (min > res[i]) {
                min = res[i];
                index = i + 1;
            }
        }
        three[j] = index;
        min = 3000000;
        res[index-1] = 3000000;
    }
    return three;
}
// COMBINATION

int combine_results(const int* res_1, const int* ref_2){
    if (res_1[0] == ref_2[0])
        return ref_2[0];
    else if (res_1[0] == ref_2[1])
        return res_1[0];
    else if (res_1[1] == ref_2[0])
        return ref_2[0];
    else if (res_1[0] == ref_2[2])
        return res_1[0];
    else if (ref_2[0] == res_1[2])
        return ref_2[0];
    else if (res_1[1] == ref_2[1])
        return res_1[1];
    else if (res_1[1] == ref_2[2])
        return res_1[1];
    else if (ref_2[1] == res_1[2])
        return ref_2[1];
    else if (ref_2[2] == res_1[2])
        return res_1[2];
    else
        return 0;
}

// COMPRESS

int** compress(int** matrix) {
    int** cropped = new int*[28];
    for(int i = 0; i < 28; i++){
        int* line = new int[23];
        for(int j = 0; j < 23; j++){
            //line[j] = (matrix[2*i][2*j] + matrix[2*i][2*j+1] + matrix[2*i+1][j] + matrix[2*i+1][2*j+1])/4;
            line[j] = 0;
            for (int k = 0; k < 4; k++){
                for (int l = 0; l < 4; l++)
                    line[j] += matrix[4*i+k][4*j+l];
            }
        }
        cropped[i] = line;
    }
    return cropped;
}

int difference_compress(int** m1, int** m2){
    int res = 0;
    for(int i = 0; i < 28; i++){
        for (int j = 0; j < 23; j++){
            res += abs(m1[i][j]-m2[i][j]);
        }
    }
    return res;
}

int*** make_base_compress(int people, int n) {
    int*** base_matrix = new int**[people];
    for(int i = 0; i < people; i++){
        base_matrix[i] = compress(picture_to_matrix(i + 1, n));
    }
    return base_matrix;
}

int* get_result_compress(int*** matrix_base, int n, int s, int people){
    int** cur = compress(picture_to_matrix(s, n));
    int* result = new int[people];
    for(int i = 0; i < people; i++){
        result[i] = difference_compress(matrix_base[i], cur);
    }
    for(int i = 0; i < 28; i++)
        delete [] cur[i];
    delete [] cur;
    return result;
}

int s_from_3(int a, int b, int c){
    if (a == b == c)
        return a;
    else if (a == b)
        return a;
    else if (a == c)
        return c;
    else if (b == c)
        return b;
    else return a;
}

pair<int*, int> intersect(const int* res1, const int* res2, const int* res3){
    int* temp = new int[5];
    int count = 0;
    for(int i = 0; i < 1; i++){
        for(int j = 0; j < 5; j++){
            for(int k = 0; k < 5; k++){
                if(res2[j] == res3[k]){
                    temp[count] = res1[i];
                    count += 1;
                }
            }
        }
    }
    int* res = new int[count];
    for (int i = 0; i < count; i++){
        res[i] = temp[i];
    }
    delete [] temp;
    pair<int*, int> ans;
    ans.first = res;
    ans.second = count;
    return ans;
}

float get_acc(int mistakes, int people, int pb){
    float acc = round(10000*(static_cast<float>((10 - pb)*people - mistakes)/static_cast<float>((10 - pb)*people)))/100;
    //float acc = round(100 * ((10 - pb) * people - static_cast<float>(mistakes)) / (10 - pb) / people * 100) / 100;
    return acc;
}

float* normalize(int* vec, int len){
    int sum = 0;
    for(int i = 0; i < len; i++){
        sum += vec[i]*vec[i];
    }
    float vec_size = sqrt(static_cast<float>(sum));
    auto res = new float[len];
    for(int i = 0; i < len; i++){
        res[i] = static_cast<float>(vec[i])/vec_size;
    }
    delete [] vec;
    return res;
}

float get_angle(float* v1, float* v2, int len){
    float s_p = 0;
    for(int i = 0; i < len; i++){
        s_p += v1[i]*v2[i];
    }
    //cout << s_p << endl;
    return acos(s_p);
}

float** make_base_angle(int people, pair<int, int>*pixels, int nop, int n) {
    auto base = new float*[people];
    for (int i = 0; i < people; i++){
        auto line = extract_vector_and_delete_matrix(picture_to_matrix(i + 1, n), pixels, nop);
        auto temp = normalize(line, nop);
        base[i] = temp;
    }
    return base;
}

float* get_result_ndp_angle (float** base, pair<int,int>*pixels, int people, int nop, int s, int n) {
    auto vec = extract_vector_and_delete_matrix(picture_to_matrix(s, n), pixels, nop);
    auto target = normalize(vec, nop);
    auto* res = new float[people];
    for (int i = 0; i < people; i++){
        res[i] = get_angle(base[i], target, nop);
    }
    delete [] target;
    return res;
}

bool check_ans_bool_angle(const float* res, int people, int s){
    float min = res[0];
    int index = 1;
    for(int i = 1; i < people; i++){
        if (min > res[i]) {
            min = res[i];
            index = i + 1;
        }
    }
    if (min == res[s-1])
        return true;
    else
        return false;
}

float** add_bases(float** base1, float** base2, int nop, int people) {
    auto** newbase = new float*[people];
    for (int i = 0; i < people; i++) {
        auto* newline = new float[nop];
        for (int j = 0; j < nop; j++) {
            newline[j] = (base1[i][j]+base2[i][j]) / 2;
        }
        newbase[i] = newline;
    }
    for (int i = 0; i < people; i++){
        delete [] base1[i];
        delete [] base2[i];
    }
    delete [] base1;
    delete [] base2;
    return newbase;
}