#include "GameCode.h"

int main() {
    std::srand(static_cast<unsigned int>(time(0)));

    bool gameStateChanged = true;
    std::vector<std::vector<Game::Square>> gameBoard = Game::GenerateGameBoard();
    std::vector<std::pair<sf::Vector2i, sf::Vector2i>> bonusMoves;
    std::vector<sf::Vector2i> deletedTiles;

    sf::RenderWindow window(sf::VideoMode(800, 800), "Gems", sf::Style::Close);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (Game::isTileClicked(event.mouseButton.x, event.mouseButton.y)) {
                    int tileX = event.mouseButton.x / 80;
                    int tileY = event.mouseButton.y / 80;
                    if (gameBoard[tileX][tileY].getFillColor() != sf::Color::Black) {
                        deletedTiles = ReplaceAndDeleteTiles(window, gameBoard, tileX, tileY);

                        std::sort(deletedTiles.begin(), deletedTiles.end(), Game::compareByYCoordinate);

                        bonusMoves = RandomlyGenerateBonus(gameBoard, deletedTiles);

                        gameStateChanged = true;
                    }
                }
            }
        }
\
        if (gameStateChanged) {
            window.clear(sf::Color::Black);

            Game::DrawGameBoard(window, gameBoard);

            window.display();

            sf::sleep(sf::seconds(1));

            gameStateChanged = false;

            if (!deletedTiles.empty()) {
                FillDeletedTiles(deletedTiles, gameBoard);
                gameStateChanged = true;
            }
            else if (!bonusMoves.empty()) {
                deletedTiles = ReleaseAllBonuses(gameBoard, bonusMoves);
                bonusMoves.clear();
                gameStateChanged = true;
            }
            else {
                gameStateChanged = false;
            }
        }
    }

    return 0;
}