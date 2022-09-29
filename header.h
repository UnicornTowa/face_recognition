#ifndef FACE_REC_DOUBLE_HEADER_H
#define FACE_REC_DOUBLE_HEADER_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

inline int a,b,c,d,e;

int** picture_to_matrix(int s, int n);
pair<int, int>* choose_pixels(int num);
int* extract_vector_and_delete_matrix(int** matrix, pair<int, int>* pixels, int len);
int calculate_difference(int* first, int* second, int n);
void delete_matrix(int** matrix);
int** make_base_pixels(int people, pair<int, int>*pixels, int nop, int n);
int* get_result_pixels (int** base, pair<int,int>*pixels, int people, int nop, int s, int n);
int* get_result_ndp (int** base, pair<int,int>*pixels, int people, int nop, int s, int n); // For different nop in cycle
int** add_bases(int** base1, int** base2, int nop, int people); // base of 1 picture which is a medium of 2 pictures
int* return_3(int* res, int people);
int** make_multibase_pixels(int people, pair<int, int>*pixels, int nop, int n1, int n2); // base of 2 pictures for every person

//PART 2

int* return_hist(int** arr, int prec);
int** make_base_hist(int people, int prec, int n);
int* get_result_hist(int** base, int n, int s, int people, int prec);
bool check_ans_bool_hist(const int* res, int people, int s);
void check_ans_hist(const int* res, int people, int s);
int** add_bases_hist(int** base1, int** base2, int prec, int people); // base of 1 picture which is a medium of 2 pictures
int** make_multibase_hist(int people, int prec, int n1, int n2); // base of 2 pictures for every person

//PART 3

int** compress(int** matrix);
int difference_compress(int** m1, int** m2);
int*** make_base_compress(int people, int n);

int check_ans_int(const int* res, int people);
float get_acc(int mistakes, int people, int pb);

//ANGLES

float* normalize(int* vec, int len);
float get_angle(float* v1, float* v2, int len);
float** make_base_angle(int people, pair<int, int>*pixels, int nop, int n);
float* get_result_ndp_angle (float** base, pair<int,int>*pixels, int people, int nop, int s, int n);
bool check_ans_bool_angle(const float* res, int people, int s);
float** add_bases(float** base1, float** base2, int nop, int people);

//Funcs below are only my experiments or obsolete

int calculate_difference_v2(const int* first, const int* second, int len); // Not important
int * get_result_ndp_v2 (int** base, pair<int,int>*pixels, int people, int nop, int s, int n); // Not important

int* return_n(int* res, int how_many, int people); // Not important
int** add_bases_3(int** base1, int** base2, int** base3, int nop, int people); // Not important
int combine_results_3(const int* res_a, const int* res_b, const int* res_c); // Not important
int s_from_3(int a, int b, int c); // Not important
pair<int*, int> intersect(const int* res1, const int* res2, const int* res3); // Not important
int* get_result_compress(int*** matrix_base, int n, int s, int people); // Not important

int combine_results(const int* res_1, const int* ref_2); // Not important
int* return_3_hist(int* res, int people); // Not important
void print_res(const int* res, int people); // Not important
void check_ans(const int* res, int people, int s); // Not important
bool check_ans_bool(const int* res, int people, int s); // Not important
int* main_sequence (int nop = 500, int people = 15, int s = 8, int n = 2); // Not important



#endif //FACE_REC_DOUBLE_HEADER_H
