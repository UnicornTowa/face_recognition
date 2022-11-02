#include "header.h"

array<array<Texture, 10>, 40> textures;

void fill_textures(){
    for(int s = 1; s <= 40; s++){
        for(int n = 1; n <= 10; n++){
            string name = "database/s" + to_string(s) + "/" + to_string(n) + ".pgm";
            Texture texture;
            if (!texture.loadFromFile(name, IntRect(0, 0, 92, 112)))
                cerr << "Cant load texture" << endl;
            textures[s - 1][n - 1] = texture;
        }
    }
}


Sprite get_sprite(int s, int n){
    //string name = "database/s" + to_string(s) + "/" + to_string(n) + ".pgm";
    //auto texture = new Texture;
    //if (!texture->loadFromFile(name, IntRect(0, 0, 92, 112)))
    //    cerr << "Cant load texture" << endl;
    Texture* texture = &textures[s - 1][n - 1];
    Sprite sprite;
    sprite.setTexture(*texture);
    sprite.scale(Vector2f(3.0f, 3.0f));
    return sprite;
}

