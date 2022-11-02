#include "header.h"




int main() {

    RenderWindow window(VideoMode(1010, 900), "Face recognition");
    RectangleShape background(Vector2f(1010.0f,900.0f));
    background.setFillColor(sf::Color::White);
    window.setFramerateLimit(60);
    Event event{};



    const int people = 40;
    const int first_exemplar = 3;
    const int second_exemplar = 8;
    int s = 1;
    int n = 1;
    int count = 0;

    auto base_hist = make_multibase_hist(people, 256/64, first_exemplar, second_exemplar);
    int mistakes_hist = 0;

    auto pixels = choose_pixels(550);
    auto base_pixels = make_multibase_pixels(people, pixels, 550, first_exemplar, second_exemplar);
    int mistakes_pixels = 0;

    auto base_compress = make_multibase_compress(people, first_exemplar, second_exemplar);
    int mistakes_compress = 0;

    Screen screen = Screen();
    fill_textures();

    int ans_hist;
    int ans_pixels;
    int ans_compress;
    int ans_vote;

    int mistakes_vote = 0;

    while (window.isOpen()){

        count++;
        n++;
        if (n == 11) {
            s++;
            n = 1;
        }
        if (s == 1 + people) {
            break;
        }

        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
            }
        }
        if (n == first_exemplar or n == second_exemplar)
            continue;

        screen.setTest(get_sprite(s, n));
        screen.setTestText(L"   Тестовое изображение\n  Субъект " + to_wstring(s) + L" изображение " + to_wstring(n));

        { /// Histogram
            ans_hist = check_ans_int(get_result_hist(base_hist, n, s, 2 * people, 256 / 64), 2 * people);
            if (ans_hist / people == 0)
                screen.setHist(get_sprite(ans_hist, first_exemplar));
            else
                screen.setHist(get_sprite(ans_hist - people, second_exemplar));
            if (ans_hist != s && ans_hist - people != s)
                mistakes_hist++;
            screen.setHistText(L"      Метод гистограмм\n Текущая точность " + get_acc(count, mistakes_hist) + L"%");
        }
        { /// Pixels
            ans_pixels = check_ans_int(get_result_pixels(base_pixels, pixels, 2 * people, 550, s, n), 2 * people);
            if (ans_pixels / people == 0)
                screen.setPixels(get_sprite(ans_pixels, first_exemplar));
            else
                screen.setPixels(get_sprite(ans_pixels - people, second_exemplar));
            if (ans_pixels != s && ans_pixels - people != s)
                mistakes_pixels++;
            screen.setPixelsText(L"     Случайные пиксели\nТекущая точность " + get_acc(count, mistakes_pixels) + L"%");
        }
        { /// Compress
            ans_compress = check_ans_int(get_result_compress(base_compress, n, s, 2 * people), 2 * people);
            if (ans_compress / people == 0)
                screen.setCompress(get_sprite(ans_compress, first_exemplar));
            else
                screen.setCompress(get_sprite(ans_compress - people, second_exemplar));
            if (ans_compress != s && ans_compress - people != s)
                mistakes_compress++;
            screen.setCompressText(L"  Сжатие изображения\nТекущая точность " + get_acc(count, mistakes_compress) + L"%");
        }
        { /// Vote
            if (ans_pixels / people == 0){
                if (ans_pixels == ans_compress or ans_pixels == ans_compress - people) {
                    ans_vote = ans_pixels;
                    screen.setVote(get_sprite(ans_vote, first_exemplar));
                }
                else
                    ans_vote = ans_hist;
            }
            else if (ans_pixels - people == ans_compress or ans_pixels - people == ans_compress - people) {
                ans_vote = ans_pixels;
                screen.setVote(get_sprite(ans_vote - people, second_exemplar));
            }
            else ans_vote = ans_hist;
            if (ans_vote == ans_hist){
                if (ans_vote / 40 == 0){
                    screen.setVote(get_sprite(ans_vote, first_exemplar));
                }
                else
                    screen.setVote(get_sprite(ans_vote - 40, second_exemplar));
            }
            if (ans_vote != s && ans_vote - people != s)
                mistakes_vote++;
            screen.setVoteText(L"          Голосование\nТекущая точность " + get_acc(count, mistakes_vote) + L"%");
        }
        window.clear();
        window.draw(background);
        screen.update_window(window);
        window.display();
        usleep(400000);
    }
    cout << "Финальная точность: \nГистограммы " << get_acc(mistakes_hist, people, 2) << "%" << endl <<
    "Пиксели " << get_acc(mistakes_pixels, people, 2) << "%" << endl <<
    "Сжатие " << get_acc(mistakes_compress, people, 2) << "%" << endl <<
    "Голосование " << get_acc(mistakes_vote, people, 2) << "%" << endl;
    { /// Memory clean
        for (int i = 0; i < 2 * people; i++) {
            delete[] base_hist[i];
            delete[] base_pixels[i];
            for (int j = 0; j < 28; j++) {
                delete[] base_compress[i][j];
            }
            delete[] base_compress[i];
        }
        delete[] base_hist;
        delete[] base_pixels;
        delete[] base_compress;
    }

    return 0;
}