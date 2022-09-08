#include "facerec.h"

int** make_arr(int s, int n){
    string name = "database/s" + to_string(s) + "/" + to_string(n) + ".pgm";
    int row = 0, col = 0, num_of_rows = 0, num_of_cols = 0;
    stringstream ss;
    ifstream infile(name, ios::binary);
    string inputLine;

    getline(infile,inputLine); // read the first line : P5
    if(inputLine.compare("P5") != 0) cerr << "Version error" << endl;
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

int** make_base(int people, pair<int, int>*pixels, int nop) {
    int** base = new int*[people];
    for (int i = 0; i < people; i++){
        base[i] = extract_vector_and_delete_matrix(make_arr(i + 1, 1), pixels, nop);
    }
    return base;
}
