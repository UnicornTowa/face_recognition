#ifndef FACE_REC_DOUBLE_HEADER_H
#define FACE_REC_DOUBLE_HEADER_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

int** make_arr(int s, int n);
pair<int, int>* choose_pixels(int num);
int* extract_vector_and_delete_matrix(int** matrix, pair<int, int>* pixels, int len);
int calculate_difference(int* first, int* second, int n);
void delete_matrix(int** matrix);
int** make_base(int people, pair<int, int>*pixels, int nop, int n);
int* get_result (int** base, pair<int,int>*pixels, int people, int nop, int s, int n);
int* main_sequence (int nop = 500, int people = 15, int s = 8, int n = 2);
void print_res(const int* res, int people);
void check_ans(const int* res, int people, int s);
bool check_ans_bool(const int* res, int people, int s);
int* get_result_ndp (int** base, pair<int,int>*pixels, int people, int nop, int s, int n);
int** add_bases(int** base1, int** base2, int nop, int people);
int* return_3(int* res, int people);

//PART 2

int* calculate(int** arr, int prec);
int** make_base_hist(int people, int prec, int n);
int difference(int* vec1, int* vec2, int prec);
int* result(int** base, int n, int s, int people, int prec);
bool check_ans_bool_hist(const int* res, int people, int s);
void check_ans_hist(const int* res, int people, int s);
int** add_bases_hist(int** base1, int** base2, int prec, int people);
int* return_3_hist(int* res, int people);

int combine_results(const int* res_pixels, const int* res_hist);

//PART 3

int** compress(int** matrix);
int diff_matrix(int** m1, int** m2);
int*** make_base_matrix(int people, int n);
int* result_m(int*** matrix_base, int n, int s, int people);

int check_ans_int(const int* res, int people);
int* return_n(int* res, int how_many, int people);
int** add_bases_3(int** base1, int** base2, int** base3, int nop, int people);
int combine_results_3(const int* res_a, const int* res_b, const int* res_c);
int s_from_3(int a, int b, int c);
pair<int*, int> intersect(const int* res1, const int* res2, const int* res3);

#endif //FACE_REC_DOUBLE_HEADER_H
