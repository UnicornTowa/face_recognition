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
#include <random>

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
extern array<array<Texture, 10>, 40> textures;
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
                delete [] hists[i][j];
            }
            delete [] pictures_long[i];
            delete [] hists[i];
        }
        delete [] hists;
        delete [] pictures_long;
    }
    [[nodiscard]] long** get_picture(int s, int n) const{
        return pictures_long[s - 1][n - 1];
    }
    [[nodiscard]] long* get_hist(int s, int n) const{
        return hists[s - 1][n - 1];
    }
}; /// Структура хранящая изображения

struct Histogram{
    vector<RectangleShape> rectangles;
    Histogram(const long* hist, int prec){
        for(int i = 0; i < 256*16/prec/4; i++){
            long sum = hist[4*i]+hist[4*i + 1] + hist[4*i + 2]+hist[4*i + 3];
            if (sum > 336)
                sum = 336;
            RectangleShape rect(Vector2f(1.0f, (float) sum));
            rect.setFillColor(Color(100, 100, 100));
            rect.move((float) (i), (float) -sum);
            rectangles.push_back(rect);
        }

    }
    Histogram(){rectangles = {};}
    void set_pos(float x, float y){
        for (auto& rect : rectangles){
            rect.move(x, y);
        }
    }
    void display(RenderWindow &window){
        for (auto & rect : rectangles){
            window.draw(rect);
        }
    }
};
struct Pixel_Mask{
    Sprite sprite;
    vector<RectangleShape> pix;
    Pixel_Mask()= default;;
    Pixel_Mask(int s, int n, pair<int, int>* pixels, int num_of_pixels){
        for(int i = 0; i < num_of_pixels; i++){
            RectangleShape rect(Vector2f(3.0f, 3.0f));
            rect.setFillColor(Color::Yellow);
            rect.move(float(3*pixels[i].second), float(3*pixels[i].first));
            pix.push_back(rect);
        }
        sprite.setTexture(textures[s - 1][n - 1]);/////// !!!!!new
        sprite.scale(Vector2f(3.0f, 3.0f));
    }
    void display(RenderWindow &window) const{
        window.draw(sprite);
        for (auto& rect : pix){
            window.draw(rect);
        }
    }
    void set_pos(float x, float y){
        for (auto& rect : pix){
            rect.move(x, y);
            sprite.setPosition(x, y);
        }
    }
};
struct DownSampled{
    vector<RectangleShape> rects = {};
    DownSampled(int s, int n, pair<int, int> scale_factor){
        auto image = textures[s - 1][n - 1].copyToImage();
        for(int i = 0; i < 112 / scale_factor.first; i++){
            for(int j = 0; j < 92 / scale_factor.second; j++){
                //auto brightness = Uint8(matrix[i][j] / (scale_factor.first * scale_factor.second));
                auto brightness = image.getPixel(j * scale_factor.second, i * scale_factor.first).r;
                Color color(brightness, brightness, brightness);
                RectangleShape rect(Vector2f(float(3 * scale_factor.second), float(3 * scale_factor.first)));
                rect.setFillColor(color);
                rect.setPosition(float(3 * j * scale_factor.second), float(3 * i * scale_factor.first));
                rects.push_back(rect);
            }
        }
    }
    DownSampled()= default;;
    void set_pos(float x, float y){
        for (auto& rect : rects){
            rect.move(x, y);
        }
    }
    void display(RenderWindow &window){
        for (auto & rect : rects){
            window.draw(rect);
        }
    }
};
/// Hist    prec (precision) - параметр определящиюй количество групп, prec = 256 / {количество групп}

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
    Pixel_Mask test_mask;
    Pixel_Mask pixel_mask;
    Histogram test_hist;
    Text test_hist_text;
    Text hist_hist_text;
    Histogram hist_hist;
    Sprite hist;
    Text hist_text;
    Sprite pixels;
    Text pixels_text;
    Sprite compress;
    Text compress_text;
    Sprite vote;
    Text vote_text;
    Font font;
    DownSampled test_cropped;
    DownSampled compress_cropped;
    Text run_info_text;
    void update_window(RenderWindow &window){
        test.setPosition(Vector2f(0.0f +17.0f, 0.0f));
        hist.setPosition(Vector2f(0.0f +17.0f, 420.0f));
        pixels.setPosition(Vector2f(0.0f +17.0f, 840.0f));
        vote.setPosition(Vector2f(650.0f + 17.0f, 840.0f));
        compress.setPosition(Vector2f(800.0f + 17.0f, 420.0f));

        test_text.setPosition(Vector2f(0.0f, 350.0f));
        test_hist_text.setPosition(Vector2f(355.0f, 350.0f));
        hist_hist_text.setPosition(Vector2f(355.0f, 770.0f));
        hist_text.setPosition(Vector2f(0.0f, 770.0f));
        pixels_text.setPosition(Vector2f(2.0f, 1190.0f));
        vote_text.setPosition(Vector2f(650.00f, 1190.0f));
        compress_text.setPosition(Vector2f(800.0f, 770.0f));
        run_info_text.setPosition(Vector2f(1000.0f, 840.0f));

        window.draw(test);
        window.draw(test_text);
        window.draw(test_hist_text);
        window.draw(hist_hist_text);
        window.draw(hist);
        window.draw(hist_text);
        window.draw(pixels);
        window.draw(pixels_text);
        window.draw(compress);
        window.draw(compress_text);
        window.draw(vote);
        window.draw(vote_text);
        window.draw(run_info_text);

        test_hist.display(window);
        hist_hist.display(window);
        test_mask.display(window);
        pixel_mask.display(window);
        test_cropped.display(window);
        compress_cropped.display(window);
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
        setTestHistText();
        setHistHistText();
    }
    void setTestHistText() {
        test_hist_text.setFont(font);
        test_hist_text.setString(L"Гистограмма тестового изображения    Расположение пикселей       Сжатое изображение");
        test_hist_text.setCharacterSize(27);
        test_hist_text.setFillColor(Color::Black);
    }
    void setHistHistText() {
        hist_hist_text.setFont(font);
        hist_hist_text.setString(L"   Гистограмма выбранного эталона");
        hist_hist_text.setCharacterSize(27);
        hist_hist_text.setFillColor(Color::Black);
    }
    void setHistText(const wstring& text) {
        hist_text.setFont(font);
        hist_text.setString(text);
        hist_text.setCharacterSize(27);
        hist_text.setFillColor(Color::Black);
    }
    void setRunInfoText(const wstring& text) {
        run_info_text.setFont(font);
        run_info_text.setString(text);
        run_info_text.setCharacterSize(27);
        run_info_text.setFillColor(Color::Black);
        run_info_text.setLineSpacing(1.2);
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
    void setTestHist(long* _hist, int prec){
        test_hist = Histogram(_hist, prec);
        test_hist.set_pos(305.0f, 336.0f);
    }
    void setHistHist(long* _hist, int prec){
        hist_hist = Histogram(_hist, prec);
        hist_hist.set_pos(305.0f, 420.0f + 336.0f);
    }
    void setTestMask(int s, int n, pair<int, int>* _pixels, int num_of_pixels){
        test_mask = Pixel_Mask(s, n, _pixels, num_of_pixels);
        test_mask.set_pos(817.0f, 0.0f);
    }
    void setPixelMask(int s, int n, pair<int, int>* _pixels, int num_of_pixels){
        pixel_mask = Pixel_Mask(s, n, _pixels, num_of_pixels);
        pixel_mask.set_pos(325.0f + 17.0f, 840.0f);
    }
    void setTestCropped(int s, int n, pair<int, int> scale_factor){
        test_cropped = DownSampled(s, n, scale_factor);
        test_cropped.set_pos(1120.0f, 0.0f);
    }
    void setCompressCropped(int s, int n, pair<int, int> scale_factor){
        compress_cropped = DownSampled(s, n, scale_factor);
        compress_cropped.set_pos(1120.0f, 420.0f);
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
