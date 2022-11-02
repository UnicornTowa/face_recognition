#ifndef FACE_REC_DOUBLE_HEADER_H
#define FACE_REC_DOUBLE_HEADER_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <unistd.h>


using namespace sf;
using namespace std;

//// Common funcs

int** picture_to_matrix_int(int s, int n);
long ** picture_to_matrix_long(int s, int n);
template <typename T>
T calculate_difference(T* first, T* second, int len) {
    T diff = 0;
    for(int i = 0; i < len; i++){
        diff += abs(first[i] - second[i]);
    }
    return diff;
}

template <typename T>
void delete_matrix(T** matrix) {
    for(int i = 0; i < 112; ++i) {
        delete [] matrix[i];
    }
    delete [] matrix;
}

template <typename T>
int check_ans_int(const T *res, int people){
    T min = res[0];
    int index = 1;
    for(int i = 1; i < people; i++){
        if (min > res[i]) {
            min = res[i];
            index = i + 1;
        }
    }
    return index;
}

float get_acc(int mistakes, int people, int pb);
wstring get_acc(int count, int mistakes);

/// Hist
long * return_hist_v2(long **picture, int prec);

long* get_result_hist(long** base, int n, int s, int people, int prec);

long ** make_multibase_hist(int people, int prec, int n1, int n2);

/// Pixels

/// Pixels
pair<int, int>* choose_pixels(int num);
int* extract_vector_of_chosen_pixels(int** matrix, pair<int, int>* pixels, int len);
int** make_multibase_pixels(int people, pair<int, int>*pixels, int nop, int n1, int n2);
int* get_result_pixels (int** base, pair<int,int>*pixels, int people, int nop, int s, int n);

/// Compress

int** compress(int** matrix);
int*** make_multibase_compress(int people, int n1, int n2);
int calc_difference_compress(int** m1, int** m2);
int* get_result_compress(int*** matrix_base, int n, int s, int people);



/// Display
struct Screen{
    Sprite test;
    Text test_text;
    Sprite hist;
    Text hist_text;
    Sprite pixels;
    Text pixels_text;
    Sprite compress;
    Text compress_text;
    Sprite vote;
    Text vote_text;
    Font font;
    void update_window(RenderWindow &window){
        test.setPosition(Vector2f(0.0f +17.0f, 0.0f));
        hist.setPosition(Vector2f(350.0f +17.0f, 0.0f));
        pixels.setPosition(Vector2f(700.0f +17.0f, 0.0f));
        compress.setPosition(Vector2f(152.66f, 450.0f));
        vote.setPosition(Vector2f(2*152.66f + 3 * 92.0f, 450.0f));

        test_text.setPosition(Vector2f(0.0f, 350.0f));
        hist_text.setPosition(Vector2f(350.0f, 350.0f));
        pixels_text.setPosition(Vector2f(700.0f, 350.0f));
        compress_text.setPosition(Vector2f(152.66f, 800.0f));
        vote_text.setPosition(Vector2f(2*152.66f + 3 * 92.0f, 800.0f));

        window.draw(test);
        window.draw(test_text);
        window.draw(hist);
        window.draw(hist_text);
        window.draw(pixels);
        window.draw(pixels_text);
        window.draw(compress);
        window.draw(compress_text);
        window.draw(vote);
        window.draw(vote_text);
    }

    void setTest(const Sprite test) {
        Screen::test = test;
    }

    void setHist(const Sprite hist) {
        Screen::hist = hist;
    }

    void setPixels(const Sprite pixels) {
        Screen::pixels = pixels;
    }

    void setVote(const Sprite vote) {
        Screen::vote = vote;
    }

    void setCompress(const Sprite compress) {
        Screen::compress = compress;
    }

    void setTestText(const wstring& text) {
        test_text.setFont(font);
        test_text.setString(text);
        test_text.setCharacterSize(27);
        test_text.setFillColor(Color::Black);
    }

    void setHistText(const wstring& text) {
        hist_text.setFont(font);
        hist_text.setString(text);
        hist_text.setCharacterSize(27);
        hist_text.setFillColor(Color::Black);
    }

    void setPixelsText(const wstring& text) {
        pixels_text.setFont(font);
        pixels_text.setString(text);
        pixels_text.setCharacterSize(27);
        pixels_text.setFillColor(Color::Black);
    }

    void setCompressText(const wstring& text) {
        compress_text.setFont(font);
        compress_text.setString(text);
        compress_text.setCharacterSize(27);
        compress_text.setFillColor(Color::Black);
    }

    void setVoteText(const wstring& text) {
        vote_text.setFont(font);
        vote_text.setString(text);
        vote_text.setCharacterSize(27);
        vote_text.setFillColor(Color::Black);
    }

    Screen(){
        font.loadFromFile("myfont.ttf");
    }
};

Sprite get_sprite(int s, int n);

void fill_textures();
#endif //FACE_REC_DOUBLE_HEADER_H
