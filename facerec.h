#ifndef FACE_REC1_FACEREC_H
#define FACE_REC1_FACEREC_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <array>

using namespace std;

int** make_arr(int s, int n);
pair<int, int>* choose_pixels(int num);
int* extract_vector_and_delete_matrix(int** matrix, pair<int, int>* pixels, int len);
int calculate_difference(int* first, int* second, int n);
void delete_matrix(int** matrix);
int** make_base(int people, pair<int, int>*pixels, int nop);


#endif //FACE_REC1_FACEREC_H
