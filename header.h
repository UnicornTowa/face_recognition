#ifndef FACE_REC_DOUBLE_HEADER_H
#define FACE_REC_DOUBLE_HEADER_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <vector>


using namespace sf;
using namespace std;

/// Каждый метод имеет сходные фунуции.<br>
/// make_base - создание "базы" по эталонам (приписка multi подчеркивает наличие нескольких эталонов)<br>
/// get_result - сравнение тестового изображения с каждым в базе,
/// и получение вектора состоящего из "отличий" от каждого эталона.<br>


/// check_ans - выбор минимального "отличия" в качестве "ответа".<br>
/// calculate_difference - сумма модулей покомпонентных разностей двух векторов.<br>
/// get_acc_w, get_acc - получение точности.<br><br>

/// Common methods

int** picture_to_matrix_int(int s, int n);    ///  int matrix from .pgm
long **int_to_long_matrix(int** &matrix);     /// int to long matrix
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
} /// Удаление матрицы (освобождение памяти)

template <typename T>
int check_ans_int(const T *res, int people){
    T min = res[0];
    int index = 0;
    for(int i = 1; i < people; i++){
        if (min > res[i]) {
            min = res[i];
            index = i;
        }
    }
    return index;
}

float get_acc(int mistakes, int count); /// Точность числом

wstring get_acc_w(int count, int mistakes); /// Точнось строкой (для вывода в приложении)


long * return_hist_v2(long **picture, int prec); /// Гистограмма из матрицы (улучшенный метод) (объявлена тут т.к. нужна ниже)
struct pictures{
    int people;
    long**** pictures_long;
    int prec;
    long*** hists;
    explicit pictures(int _people, int _prec) : people(_people), prec(_prec) {
        long**** all_pictures_long = new long ***[people];
        for(int s = 1; s <= people; s++){
            auto temp_long = new long**[10];
            for(int n = 1; n <= 10; n++){
                auto int_matrix = picture_to_matrix_int(s, n);
                temp_long[n - 1] = int_to_long_matrix(int_matrix);
                delete_matrix(int_matrix);
            }
            all_pictures_long[s - 1] = temp_long;
        }
        pictures_long = all_pictures_long;
        hists = new long**[people];
        for(int s = 1; s <= people; s++){
            auto hists_n = new long*[10];
            for (int n = 1; n <= 10; n++){
                hists_n[n - 1] = return_hist_v2(get_picture(s, n), prec);
            }
            hists[s - 1] = hists_n;
        }
    }
    ~pictures(){
        for(int i = 0; i < people; i++){
            for(int j = 0; j < 10; j++){
                delete_matrix(pictures_long[i][j]);
            }
            delete [] pictures_long[i];
        }
        delete [] pictures_long;
    }
    [[nodiscard]] long** get_picture(int s, int n) const{
        return pictures_long[s - 1][n - 1];
    }
    [[nodiscard]] long* get_hist(int s, int n) const{
        return hists[s - 1][n - 1];
    }
}; /// Структура хранящая изображения




/// Hist            /// prec (precision) - параметр определящиюй количество групп, prec = 256 / {количество групп}


long* get_result_hist(long** base, int n, int s, int people, int prec, pictures& pictures);

long **make_multibase_hist(int people, int prec, pictures &pictures, vector<int> &exemplars); /// n1, n2 номера изображений - эталонов

/// Pixels <br> nop (number of pixels) - количество случайных пикселей
pair<int, int>* choose_pixels(int num); /// Выбор случайных пикселей
long * extract_vector_of_chosen_pixels(long **matrix, pair<int, int>* pixels, int len); /// Вектор по выбранным пикселям
long ** make_multibase_pixels(int people, pair<int, int> *pixels, int nop, pictures &pictures, vector<int> &exemplars);
long * get_result_pixels(long **base, pair<int, int> *pixels, int people, int nop, int s, int n, pictures &pictures);

/// Compress

long ** compress(long **matrix, pair<int, int> scale_factor); /// Сжатие матрицы х16
long *** make_multibase_compress(int people, pictures &pictures, vector<int> &exemplars, pair<int, int> scale_factor);
long calc_difference_compress(long **m1, long **m2, pair<int, int> scale_factor);
long * get_result_compress(long ***matrix_base, int n, int s, int people, pictures &pictures, pair<int, int> scale_factor);



/// Display
struct Screen /// Структура из отображаемых объектов для удостбва визуализации
        {
    /// Sprite - по сути, картинка для отображения
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

    void setTest(const Sprite& _test) {
        Screen::test = _test;
    }
    void setHist(const Sprite& _hist) {
        Screen::hist = _hist;
    }
    void setPixels(const Sprite& _pixels) {
        Screen::pixels = _pixels;
    }
    void setVote(const Sprite& _vote) {
        Screen::vote = _vote;
    }
    void setCompress(const Sprite& _compress) {
        Screen::compress = _compress;
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

double* combine_results(const long* v1, const long* v2, const long*v3, int people);
void set(int _a, int _b, int _c);
Sprite get_sprite(int s, int n); /// Получение спрайта из требуемого изображения

void fill_textures(); /// Заполнить текстурами массив
#endif //FACE_REC_DOUBLE_HEADER_H
