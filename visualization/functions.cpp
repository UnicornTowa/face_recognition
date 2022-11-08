#include "header.h"

int a = 0, b = 0, c = 0;
void set(int _a, int _b, int _c){
    a = _a;
    b = _b;
    c = _c;
}
///Common

int** picture_to_matrix_int(int s, int n)
{
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

long **int_to_long_matrix(int** &matrix){
    long** ans = new long*[112];
    for (int i = 0; i < 112; i++){
        auto temp = new long[92];
        for(int j = 0; j < 92; j++){
            temp[j] = long(matrix[i][j]);
        }
        ans[i] = temp;
    }
    return ans;
}

float get_acc(int mistakes, int count) {
    float acc = round(10000*(static_cast<float>(count - mistakes) / static_cast<float>(count))) / 100;
    //float acc = round(100 * ((10 - pb) * people - static_cast<float>(mistakes)) / (10 - pb) / people * 100) / 100;
    return acc;
}
wstring get_acc_w(int count, int mistakes){
    return wstring(to_wstring(round(10000*(static_cast<float>(count - mistakes))/static_cast<float>(count))/100), 0, 5);
}



/// Hist

int get_weight(int h, int w){//14431
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

long * return_hist_v2(long **picture, int prec){

    long* result_v2 = new long[256*16/prec]();
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
    return result_v2;
}

long **make_multibase_hist(int people, int prec, pictures &pictures, vector<int> &exemplars) {
    auto num_of_exemplars = exemplars.size();
    long** base = new long*[people*num_of_exemplars];
    for(int q = 0; q < num_of_exemplars; q++){
        for(int i = 0; i < people; i++){
            base[i + q * people] = return_hist_v2(pictures.get_picture(i + 1, exemplars[q]), prec);
        }
    }
    return base;
}

long* get_result_hist(long** base, int n, int s, int people, int prec, pictures& pictures){
    long* cur = return_hist_v2(pictures.get_picture(s, n), prec);
    long* result = new long[people];
    auto len = 16*256/prec;
    for(int i = 0; i < people; i++){
        result[i] = calculate_difference(cur, base[i], len);
    }
    delete [] cur;
    return result;
}

/// Pixels

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

long * extract_vector_of_chosen_pixels(long **matrix, pair<int, int>* pixels, int len) {
    long* vec = new long[len];
    for (int i = 0; i < len; i++) {
        vec [i] = matrix[pixels[i].first][pixels[i].second];
    }
    return vec;
}

long ** make_multibase_pixels(int people, pair<int, int> *pixels, int nop, pictures &pictures, vector<int> &exemplars) {
    auto num_of_exemplars = exemplars.size();
    long** base = new long*[num_of_exemplars * people];
    for (int q = 0; q < num_of_exemplars; q++){
        for(int i = 0; i < people; i++){
            base[i + q * people] = extract_vector_of_chosen_pixels(pictures.get_picture(i + 1, exemplars[q]), pixels, nop);
        }
    }
    return base;
}

long * get_result_pixels(long **base, pair<int, int> *pixels, int people, int nop, int s, int n, pictures &pictures) {
    auto target = extract_vector_of_chosen_pixels(pictures.get_picture(s, n), pixels, nop);
    long* res = new long[people]();
    for (int i = 0; i < people; i++){
        res[i] = calculate_difference(base[i], target, nop);
    }
    delete [] target;
    return res;
}

/// Compress

long *** make_multibase_compress(int people, pictures &pictures, vector<int> &exemplars) {
    auto num_of_exemplars = exemplars.size();
    long*** base_matrix = new long**[num_of_exemplars*people];
    for(int q = 0; q < num_of_exemplars; q++){
        for(int i = 0; i < people; i++){
            base_matrix[i + q * people] = compress(pictures.get_picture(i + 1, exemplars[q]));
        }
    }
    return base_matrix;
}

long ** compress(long **matrix) {
    long** cropped = new long*[28];
    for(int i = 0; i < 28; i++){
        long* line = new long[23];
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

long calc_difference_compress(long **m1, long **m2){
    long res = 0;
    for(int i = 0; i < 28; i++){
        for (int j = 0; j < 23; j++){
            res += abs(m1[i][j]-m2[i][j]);
        }
    }
    return res;
}

long * get_result_compress(long ***matrix_base, int n, int s, int people, pictures &pictures) {
    long** cur = compress(pictures.get_picture(s, n));
    long* result = new long[people];
    for(int i = 0; i < people; i++){
        result[i] = calc_difference_compress(matrix_base[i], cur);
    }
    for(int i = 0; i < 28; i++)
        delete [] cur[i];
    delete [] cur;
    return result;
}
double* combine_results(const long* v1, const long* v2, const long*v3, int people){
    long v1_size = 0, v2_size = 0, v3_size = 0;
    for (int i = 0; i < people; i++){
        v1_size += v1[i];
        v2_size += v2[i];
        v3_size += v3[i];
    }
    auto* res = new double[people];
    for (int i = 0; i < people; i++){
        res[i] = (double (v1[i]) / v1_size * a + double (v2[i]) / v2_size * b + double (v3[i]) / v3_size * c);
    }
    return res;
}