#include "header.h"

int main() {
    /// Создание окна и фона
    RenderWindow window(VideoMode(1010, 880), "Face recognition");
    RectangleShape background(Vector2f(1010.0f,900.0f));
    background.setFillColor(sf::Color::White);
    window.setFramerateLimit(60);
    Event event{};


    const int people = 40;  /// Общие параметры
    vector<int> exemplars = {3,6,5,9}; /// Вектор из номеров эталонов
    int num_of_exemplars = int(exemplars.size());
    int s = 1; /// Здесь и далее s - порядковый номер субъекта в базе
    int n = 0;/// Здесь и далее n - номер изображения конкретного субъекта
    int count = 0;

    pictures pictures_data = pictures(people); /// Структура хранящая изображения

    /// Параметры каждого метода
    auto base_hist = make_multibase_hist(people, 256 / 64, pictures_data, exemplars);
    int mistakes_hist = 0;

    auto pixels = choose_pixels(550);
    auto base_pixels = make_multibase_pixels(people, pixels, 550, pictures_data, exemplars);
    int mistakes_pixels = 0;


    auto base_compress = make_multibase_compress(people, pictures_data, exemplars);
    int mistakes_compress = 0;

    set(1, 1, 1); /// веса для голосования

    int mistakes_vote = 0;

    /// Вспомогательные параметры и объекты

    Screen screen = Screen();
    fill_textures();

    int ans_hist, ans_hist_s, ans_hist_n;
    int ans_pixels, ans_pixels_s, ans_pixels_n;
    int ans_compress, ans_compress_s, ans_compress_n;
    int ans_vote_s, ans_vote_n;
    long *vector_hist, *vector_pixels, *vector_compress;
    while (window.isOpen()) /// Основой цикл
    {
        { /// Обновление параметров
            count++;
            n++;
            if (n == 11) {
                s++;
                n = 1;
            }
            if (s == 1 + people) {
                count--;
                break;
            }
        }

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
            }
        }

        if (find(exemplars.begin(), exemplars.end(), n) != exemplars.end()) {
            count--;
            continue;
        }

        screen.setTest(get_sprite(s, n));
        screen.setTestText(
                L"   Тестовое изображение\n  Субъект " + to_wstring(s) + L" изображение " + to_wstring(n));
        //cout << "TEST: s = " << s << " n = " << n << "*" << endl;
        { /// Histogram
            vector_hist = get_result_hist(base_hist, n, s, num_of_exemplars * people, 256 / 64,
                                          pictures_data);
            ans_hist = check_ans_int(vector_hist, num_of_exemplars * people);
            ans_hist_s = 1 + ans_hist % people;
            ans_hist_n = exemplars[ans_hist / people];
            screen.setHist(get_sprite(ans_hist_s, ans_hist_n));
            if (ans_hist_s != s)
                mistakes_hist++;
            //cout << "Hist: s = " << ans_hist_s << " n = " << ans_hist_n << endl;
            screen.setHistText(
                    L"      Метод гистограмм\n Текущая точность " + get_acc_w(count, mistakes_hist) + L"%");
        }
        { /// Pixels
            vector_pixels = get_result_pixels(base_pixels, pixels, num_of_exemplars * people, 550, s, n,
                                              pictures_data);
            ans_pixels = check_ans_int(vector_pixels, num_of_exemplars * people);
            ans_pixels_s = 1 + ans_pixels % people;
            ans_pixels_n = exemplars[ans_pixels / people];
            screen.setPixels(get_sprite(ans_pixels_s, ans_pixels_n));
            if (ans_pixels_s != s)
                mistakes_pixels++;
            //cout << "Pixels: s = " << ans_pixels_s << " n = " << ans_pixels_n << endl;
            screen.setPixelsText(
                    L"     Случайные пиксели\nТекущая точность " + get_acc_w(count, mistakes_pixels) +
                    L"%");
        }
        { /// Compress
            vector_compress = get_result_compress(base_compress, n, s, num_of_exemplars * people,
                                                  pictures_data);
            ans_compress = check_ans_int(vector_compress, num_of_exemplars * people);
            ans_compress_s = 1 + ans_compress % people;
            ans_compress_n = exemplars[ans_compress / people];
            screen.setCompress(get_sprite(ans_compress_s, ans_compress_n));
            if (ans_compress_s != s)
                mistakes_compress++;
            //cout << "Compress: s = " << ans_compress_s << " n = " << ans_compress_n << endl;
            screen.setCompressText(
                    L"  Сжатие изображения\nТекущая точность " + get_acc_w(count, mistakes_compress) +
                    L"%");
        }
//        { /// Vote
//            if (ans_pixels_s == ans_compress_s) {
//                ans_vote_s = ans_pixels_s;
//                ans_vote_n = ans_pixels_n;
//            } else {
//                ans_vote_s = ans_hist_s;
//                ans_vote_n = ans_hist_n;
//            }
//            screen.setVote(get_sprite(ans_vote_s, ans_vote_n));
//            if (ans_vote_s != s)
//                mistakes_vote++;
//            screen.setVoteText(
//                    L"          Голосование\nТекущая точность " + get_acc_w(count, mistakes_vote) + L"%");
//        }
        {/// Vote 2
            //set(13, 3, 3);

            auto vector_vote = combine_results(vector_hist, vector_pixels, vector_compress,
                                               num_of_exemplars * people);
            auto ans_vote = check_ans_int(vector_vote, num_of_exemplars * people);
            ans_vote_s = 1 + ans_vote % people;
            ans_vote_n = exemplars[ans_vote / people];
            screen.setVote(get_sprite(ans_vote_s, ans_vote_n));
            if (ans_vote_s != s)
                mistakes_vote++;
            screen.setVoteText(
                    L"          Голосование\nТекущая точность " + get_acc_w(count, mistakes_vote) + L"%");
        }
        window.clear();
        window.draw(background);
        screen.update_window(window);
        window.display();
        //usleep(300000); /// Задержка (мкс)
    }
//    cout << get_acc(mistakes_hist, count) << " " << get_acc(mistakes_pixels, count) << " "
//    << get_acc(mistakes_compress, count) << " " << get_acc(mistakes_vote, count) << endl;
    cout << "Финальная точность: " << endl<<
         "Гистограммы " << get_acc(mistakes_hist, count) << "%" << endl <<
         "Пиксели " << get_acc(mistakes_pixels, count) << "%" << endl <<
         "Сжатие " << get_acc(mistakes_compress, count) << "%" << endl <<
         "Голосование " << get_acc(mistakes_vote, count) << "%" << endl;
    { /// Memory clean
        for (int i = 0; i < num_of_exemplars * people; i++) {
            delete[] base_hist[i];
            delete[] base_pixels[i];
            for (int j = 0; j < 28; j++) {
                delete[] base_compress[i][j];
            }
            delete[] base_compress[i];
        }
        delete[] pixels;
        delete[] base_hist;
        delete[] base_pixels;
        delete[] base_compress;
    }

    return 0;
}