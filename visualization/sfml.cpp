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
    Sprite sprite;
    sprite.setTexture(textures[s - 1][n - 1]);
    sprite.scale(Vector2f(3.0f, 3.0f));
    return sprite;
}

