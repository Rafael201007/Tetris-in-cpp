#include <iostream>
#include <SFML/Graphics.hpp>
#include <memory>

class Object {
public:
    std::shared_ptr<sf::Texture> spriteTexture;
    std::shared_ptr<sf::Sprite> sprite;

    Object(std::string imgfile, float startXpos, float startYpos, int originX = 0, int originY = 0, float scaleX = 1, float scaleY = 1) {
        spriteTexture = std::make_shared<sf::Texture>();

        if (!spriteTexture->loadFromFile(imgfile)) {
            std::cerr << "N�o foi poss�vel carregar a imagem: " << imgfile << std::endl;
        }

        sprite = std::make_shared<sf::Sprite>(*spriteTexture);

        sprite->setPosition({ startXpos, startYpos });


        if (originX && originY) {
            sprite->setOrigin(sf::Vector2f(static_cast<float>(originX), static_cast<float>(originY)));
        }

        sprite->setScale({ scaleX, scaleY });
    }
};

class Tile : public Object {
public:
    int xGridPos;
    int yGridPos;

    Tile(int xGrid, int yGrid)
        : Object("Sprites/tile1.png", xGrid * 8.0f, yGrid * 8.0f,4,4) { 
        xGridPos = xGrid; yGridPos = yGrid;
    }
};

class Texture {
public:
    std::shared_ptr<sf::Texture> texture;

    Texture(std::string imgfile) {
        texture = std::make_shared<sf::Texture>();

        if (!texture->loadFromFile(imgfile)) {
            std::cerr << "N�o foi poss�vel carregar a imagem: " << imgfile << std::endl;
        }
    }
};

int main()
{
    const int width = 256;
    const int height = 240;
    
    int colums = width / 8;
    int rows = height / 8;

    std::vector<std::vector<Tile>> tileMap;

    for (int i = 0; i < rows + 1; i++) {
        std::vector<Tile> row;
        for (int a = 0; a < colums + 1; a++) {
            Tile tile(a,i);
            row.emplace_back(tile);
        }
        tileMap.push_back(row);
    }
  
    Texture empty_tile("Sprites/empty_tile.png");

    //loop pelos tiles antes de abrir a janela
    for (const auto& row : tileMap) {
        for (auto& tile : row) {
            // fazendo os tiles das bordas ficarem vazio
            if (tile.xGridPos == 0 or tile.xGridPos == 32 or tile.yGridPos == 0 or tile.yGridPos == 30) {
                tile.sprite->setTexture(*empty_tile.texture);
            }
        }
    }

    std::unique_ptr window = std::make_unique<sf::RenderWindow> (sf::VideoMode({ width,height }), "Tetris");

    window->setFramerateLimit(60);

    while (window->isOpen()) {
        // os tiles do x 0 e 32, e y 0 e 30 s�o as bordas, sempre devem ficar vazios
        // os tiles usaveis s�o de x 1 a 31 e y 1 a 29

        while (const std::optional event = window->pollEvent()) {

            if (event->is<sf::Event::Closed>()) {
                window->close();
            }

        }

        window->clear();

        // loop pelos tiles depois de abrir a janela
        for (const auto& row : tileMap) {
            for (auto& tile : row) {
                window->draw(*tile.sprite);
            }
        }

        window->display();
    }

    return 0;
}

