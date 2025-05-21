#include "GameCode.h"
namespace Game
{
    std::vector<sf::Vector2i> Bomb::DropBonus(std::vector<std::vector<Square>>& GameField, sf::Vector2i& ToPos, sf::Vector2i& FromPos)
    {
        std::vector<sf::Vector2i> DeletedSquares;
        DestroyTile(GameField, ToPos);
        DeletedSquares.push_back(ToPos);
        for (size_t i = 0; i < 4; i++)
        {
            sf::Vector2i toBeDeletedPos;
            do {
                toBeDeletedPos.x = rand() % 8;
                toBeDeletedPos.y = rand() % 8;
            } while (GameField[toBeDeletedPos.x][toBeDeletedPos.y].getFillColor() == sf::Color::Black);

            DestroyTile(GameField, toBeDeletedPos);
            DeletedSquares.push_back(toBeDeletedPos);
        }
        delete GameField[ToPos.x][ToPos.y].GetBonus();
        return DeletedSquares;
    }

    std::vector<sf::Vector2i> Bonus::DropBonus(std::vector<std::vector<Square>>& GameField, sf::Vector2i& ToPos, sf::Vector2i& FromPos)
    {
        return*new std::vector<sf::Vector2i>;
    }

    std::vector<sf::Vector2i> ReDrawer::DropBonus(std::vector<std::vector<Square>>& GameField, sf::Vector2i& ToPos, sf::Vector2i& FromPos)
    {

        sf::Color lastColor = GameField[FromPos.x][FromPos.y].getFillColor();
        if (lastColor == sf::Color::Black)
            return *new std::vector<sf::Vector2i>;

        for (size_t i = 0; i < 2; i++)
        {
            sf::Vector2i toBeReDrawedPos(rand() % 2 + 2, rand() % 2 + 2);
            bool canBeGenerated = false;
            int tempx = ToPos.x, tempy = ToPos.y;
            while (!canBeGenerated)
            {
                tempx = ToPos.x;
                tempy = ToPos.y;

                int var = rand() % 4;
                switch (var)
                {
                case 0:
                    tempx += toBeReDrawedPos.x;
                    tempy += toBeReDrawedPos.y;
                    canBeGenerated = (tempx < GameBoardSize && tempy < GameBoardSize);
                    break;
                case 1:
                    tempx += toBeReDrawedPos.x;
                    tempy -= toBeReDrawedPos.y;
                    canBeGenerated = (tempx < GameBoardSize && tempy >= 0);
                    break;
                case 2:
                    tempx -= toBeReDrawedPos.x;
                    tempy += toBeReDrawedPos.y;
                    canBeGenerated = (tempx >= 0 && tempy < GameBoardSize);
                    break;
                case 3:
                    tempx -= toBeReDrawedPos.x;
                    tempy -= toBeReDrawedPos.y;
                    canBeGenerated = (tempx >= 0 && tempy >= 0);
                    break;

                    if (canBeGenerated == true && (GameField[tempx][tempy].getFillColor() == sf::Color::Black))
                        canBeGenerated = false;
                }


            }

            GameField[tempx][tempy].setFillColor(lastColor);

        }
        delete GameField[ToPos.x][ToPos.y].GetBonus();
        return *new std::vector<sf::Vector2i>;
    }

    Square::Square(float const size) : RectangleShape(*new sf::Vector2f(size, size)), _size(size), _bns(NULL)
    {};

    Square::~Square()
    {
        delete _bns;
    }

    float Square::GetSize() const
    {
        return _size;
    };

    void Square::SetBonus(Bonus* bns)
    {
        _bns = bns;
    }

    std::vector<sf::Vector2i> Square::DropBonus(std::vector<std::vector<Square>>& GameField, sf::Vector2i& ToPos, sf::Vector2i& FromPos)
    {
        return (*_bns).DropBonus(GameField, ToPos, FromPos);
    };

    Bonus* Square::GetBonus()
    {
        return _bns;
    };
    class Square;

    std::vector<std::vector<Square>> GenerateGameBoard()
    {
        std::vector<std::vector<Square>> elements(8);

        for (size_t i = 0; i < GameBoardSize; i++)
        {
            for (size_t j = 0; j < GameBoardSize; j++)
            {
                elements[i].push_back(*new Square(TileSize));
                elements[i][j].setFillColor(GetRandomColor());
                elements[i][j].setPosition(i * 80, j * 80);
            }
        }
        return elements;
    }

    sf::Color GetRandomColor()
    {
        int num = rand() % 5;
        switch (num)
        {
        case 1:
            return sf::Color::Cyan;
            break;
        case 2:
            return sf::Color::Red;
            break;
        case 3:
            return sf::Color::Blue;
            break;
        case 4:
            return sf::Color::White;
            break;
        default:
            return sf::Color::Green;
            break;
        }
    }

    void DrawGameBoard(sf::RenderWindow& window, std::vector<std::vector<Square>> const& elements)
    {
        for (size_t i = 0; i < GameBoardSize; i++)
            for (size_t j = 0; j < GameBoardSize; j++)
                window.draw(elements[i][j]);
    }

    bool isTileClicked(int const click_x, int const click_y)
    {
        int integer_x = click_x / 80;
        int integer_y = click_y / 80;

        int delta_x = click_x - integer_x * 80;
        int delta_y = click_y - integer_y * 80;
        if (delta_x <= TileSize && delta_y <= TileSize)
            return true;
        else
            return false;
    }

    void swap_colors(Square& s1, Square& s2)
    {
        sf::Color color = s2.getFillColor();
        s2.setFillColor(s1.getFillColor());
        s1.setFillColor(color);
    }

    void swapTileBonuses(Square& s1, Square& s2)
    {
        Bonus* temp = s1.GetBonus();
        s1.SetBonus(s2.GetBonus());
        s2.SetBonus(temp);
    }

    bool areCoordinatesAdjacent(int const First_x, int const First_y, int const Second_x, int const Second_y)
    {
        int delta_x = First_x - Second_x;
        int delta_y = First_y - Second_y;
        if ((fabs(delta_x) == 1 || fabs(delta_y) == 1) && ((fabs(delta_x) + fabs(delta_y)) == 1))
            return true;
        else
            return false;
    }

    bool areCoordinatesEqual(int const First_x, int const First_y, int const Second_x, int const Second_y)
    {
        return (First_x == Second_x) && (First_y == Second_y) ? true : false;
    }

    std::vector<sf::Vector2i> DeleteCompletely(std::vector<std::vector<Square>>& GameBoard, int const FirstSquareX, int const FirstSquareY)
    {
        std::vector<sf::Vector2i> vec;
        int size = 0;
        do
        {
            size = vec.size();
            FindTilesToDelete(vec, GameBoard, FirstSquareX, FirstSquareY);
        } while (vec.size() != size);

        DeleteTiles(GameBoard, vec);
        return vec;
    }

    std::vector<sf::Vector2i> ReplaceAndDeleteTiles(sf::RenderWindow& window, std::vector<std::vector<Square>>& GameBoard, int const FirstSquareX, int const FirstSquareY)
    {
        std::vector<sf::Vector2i> vec1;
        bool wasClicked = true;
        while (wasClicked)
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }
                else if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (isTileClicked(event.mouseButton.x, event.mouseButton.y))
                    {

                        int SecondSquareX = event.mouseButton.x / 80;
                        int SecondSquareY = event.mouseButton.y / 80;

                        if (GameBoard[SecondSquareX][SecondSquareY].getFillColor() == sf::Color::Black)
                            return vec1;

                        if (areCoordinatesAdjacent(FirstSquareX, FirstSquareY, SecondSquareX, SecondSquareY))
                        {
                            sf::Vector2i pos1(FirstSquareX, FirstSquareY), pos2(SecondSquareX, SecondSquareY);
                            SwapTiles(GameBoard, pos1, pos2);
                            std::vector<sf::Vector2i>  vec2;
                            if (GameBoard[FirstSquareX][FirstSquareY].getFillColor() != sf::Color::Black)
                                vec1 = DeleteCompletely(GameBoard, FirstSquareX, FirstSquareY);
                            if (GameBoard[SecondSquareX][SecondSquareY].getFillColor() != sf::Color::Black)
                                vec2 = DeleteCompletely(GameBoard, SecondSquareX, SecondSquareY);

                            vec1.insert(vec1.end(), vec2.begin(), vec2.end());
                            wasClicked = false;

                            return vec1;
                        }
                        else if (areCoordinatesEqual(FirstSquareX, FirstSquareY, SecondSquareX, SecondSquareY))
                        {
                            wasClicked = false;
                            return vec1;
                        }
                        else
                            wasClicked = true;
                    }

                }
            }
        }
    }

    void clearQueue(std::vector<sf::Vector2i>& q)
    {
        std::vector<sf::Vector2i> empty;
        std::swap(q, empty);
    }

    bool isCoordinateInQueue(std::vector<sf::Vector2i>& q, sf::Vector2i& vec)
    {
        std::vector<sf::Vector2i> tempvec(q);
        int size = q.size();
        for (size_t i = 0; i < size; i++)
        {
            if (tempvec.front() == vec)
                return true;
            tempvec.erase(tempvec.begin());
        }
        return false;
    }

    void FindTilesToDelete(std::vector<sf::Vector2i>& DeletedSquares, std::vector<std::vector<Square>>& GameBoard, int SquareX, int SquareY)
    {
        int CurSize = DeletedSquares.size();

        std::vector<sf::Vector2i> tempq(DeletedSquares);

        for (size_t j = 0; CurSize == 0 ? j <= CurSize : j < CurSize; j++)
        {
            if (CurSize == 0)
                ;
            else
            {
                sf::Vector2i vec = tempq.front();
                tempq.erase(tempq.begin());
                SquareX = vec.x;
                SquareY = vec.y;
            }

            for (int i = SquareY; i < GameBoardSize; i++)
            {
                sf::Vector2i* indexes = new sf::Vector2i(SquareX, i);
                if (GameBoard[SquareX][i].getFillColor() == GameBoard[SquareX][SquareY].getFillColor() &&
                    !isCoordinateInQueue(DeletedSquares, *indexes))
                    DeletedSquares.push_back(*indexes);
                else if (GameBoard[SquareX][i].getFillColor() == GameBoard[SquareX][SquareY].getFillColor())
                    ;
                else
                    break;
            }

            for (int i = SquareY; i >= 0; i--)
            {
                sf::Vector2i* indexes = new sf::Vector2i(SquareX, i);
                if (GameBoard[SquareX][i].getFillColor() == GameBoard[SquareX][SquareY].getFillColor()
                    && !isCoordinateInQueue(DeletedSquares, *indexes))
                    DeletedSquares.push_back(*indexes);
                else if (GameBoard[SquareX][i].getFillColor() == GameBoard[SquareX][SquareY].getFillColor())
                    ;
                else
                    break;
            }

            int RowDeletedElemCount = DeletedSquares.size() - CurSize;
            if (RowDeletedElemCount < 3 && CurSize == 0)
                clearQueue(DeletedSquares), RowDeletedElemCount = 0;
            else
                RowDeletedElemCount = DeletedSquares.size();

            for (int i = SquareX; i < GameBoardSize; i++)
            {
                sf::Vector2i* indexes = new sf::Vector2i(i, SquareY);
                if (GameBoard[i][SquareY].getFillColor() == GameBoard[SquareX][SquareY].getFillColor() &&
                    !isCoordinateInQueue(DeletedSquares, *indexes))
                    DeletedSquares.push_back(*indexes);
                else if (GameBoard[i][SquareY].getFillColor() == GameBoard[SquareX][SquareY].getFillColor())
                    ;
                else
                    break;
            }

            for (int i = SquareX; i >= 0; i--)
            {
                sf::Vector2i* indexes = new sf::Vector2i(i, SquareY);
                if (GameBoard[i][SquareY].getFillColor() == GameBoard[SquareX][SquareY].getFillColor() &&
                    !isCoordinateInQueue(DeletedSquares, *indexes))
                    DeletedSquares.push_back(*indexes);
                else if (GameBoard[i][SquareY].getFillColor() == GameBoard[SquareX][SquareY].getFillColor())
                    ;
                else
                    break;
            }
            int ColDeletedElemCount = DeletedSquares.size() - RowDeletedElemCount;
            if (ColDeletedElemCount < 3 && CurSize == 0)
            {
                for (size_t i = 0; i < ColDeletedElemCount; i++)
                {
                    DeletedSquares.pop_back();
                }
            }
        }
    }

    void DeleteTiles(std::vector<std::vector<Square>>& GameBoard, std::vector<sf::Vector2i>& SqToDelete)
    {
        int size = SqToDelete.size();
        std::vector<sf::Vector2i> tempq(SqToDelete);
        for (size_t i = 0; i < size; i++)
        {
            sf::Vector2i vec = tempq.front();
            tempq.erase(tempq.begin());
            DestroyTile(GameBoard, vec);
        }

    }


    bool compareByYCoordinate(sf::Vector2i const& a, sf::Vector2i const& b) {
        return a.y > b.y;
    }


    void FillDeletedTiles(std::vector<sf::Vector2i>& vec, std::vector<std::vector<Square>>& GameBoard)
    {
        for (auto it = vec.begin(); it != vec.end();)
        {
            auto& el = *it;

            for (int i = el.y; i >= 0; i--)
            {
                if (i < 0)
                    break;
                else if (GameBoard[el.x][i].getFillColor() != sf::Color::Black)
                {


                    sf::Vector2i CurPoint(el.x, el.y);
                    sf::Vector2i NewPoint(el.x, i);
                    SwapTiles(GameBoard, CurPoint, NewPoint);
                    vec.insert(std::lower_bound(vec.begin(), vec.end(), NewPoint, compareByYCoordinate), NewPoint);
                    break;
                }

            }
            it = vec.erase(vec.begin());
        }
    }


    void DestroyTile(std::vector<std::vector<Square>>& GameBoard, sf::Vector2i& pos)
    {
        GameBoard[pos.x][pos.y].setFillColor(sf::Color::Black);
        GameBoard[pos.x][pos.y].SetBonus(NULL);
    }

    void SwapTiles(std::vector<std::vector<Square>>& GameBoard, sf::Vector2i& pos1, sf::Vector2i& pos2)
    {
        swap_colors(GameBoard[pos1.x][pos1.y], GameBoard[pos2.x][pos2.y]);
        swapTileBonuses(GameBoard[pos1.x][pos1.y], GameBoard[pos2.x][pos2.y]);
    }

    bool isCoordinateInQueue(std::vector<std::pair<sf::Vector2i, sf::Vector2i>>& vec, sf::Vector2i& v)
    {
        for (auto& el : vec)
        {
            if (el.second == v)
                return true;
        }
        return false;
    }

    std::vector<std::pair<sf::Vector2i, sf::Vector2i>> RandomlyGenerateBonus(std::vector<std::vector<Square>>& GameBoard, std::vector<sf::Vector2i>& DeletedSquares)
    {
        std::vector<std::pair<sf::Vector2i, sf::Vector2i>> res;
        for (auto& vec : DeletedSquares)
        {
            int chance = rand() % 100;
            if (chance <= 10)
            {
                sf::Vector2i toBePlaced(rand() % 2 + 2, rand() % 2 + 2);
                bool canBeGenerated = false;
                int tempx = vec.x, tempy = vec.y;
                sf::Vector2i ToPos;
                while (!canBeGenerated)
                {
                    tempx = vec.x;
                    tempy = vec.y;

                    int var = rand() % 4;
                    switch (var)
                    {
                    case 0:
                        tempx += toBePlaced.x;
                        tempy += toBePlaced.y;
                        canBeGenerated = (tempx < GameBoardSize && tempy < GameBoardSize);
                        break;
                    case 1:
                        tempx += toBePlaced.x;
                        tempy -= toBePlaced.y;
                        canBeGenerated = (tempx < GameBoardSize && tempy >= 0);
                        break;
                    case 2:
                        tempx -= toBePlaced.x;
                        tempy += toBePlaced.y;
                        canBeGenerated = (tempx >= 0 && tempy < GameBoardSize);
                        break;
                    case 3:
                        tempx -= toBePlaced.x;
                        tempy -= toBePlaced.y;
                        canBeGenerated = (tempx >= 0 && tempy >= 0);
                        break;
                    }
                    ToPos.x = tempx;
                    ToPos.y = tempy;
                    if (isCoordinateInQueue(res, ToPos))
                        canBeGenerated = false;
                }

                res.push_back(std::make_pair(vec, ToPos));

            }

        }

        for (auto& vec : res)
        {
            int BonusNumber = rand() % 2;

            if (BonusNumber == 0)
                GameBoard[vec.second.x][vec.second.y].SetBonus(new ReDrawer());
            else
                GameBoard[vec.second.x][vec.second.y].SetBonus(new Bomb());
        }

        return res;
    }

    std::vector<sf::Vector2i> ReleaseAllBonuses(std::vector<std::vector<Square>>& GameBoard, std::vector<std::pair<sf::Vector2i, sf::Vector2i>>& FromToPairs)
    {
        std::vector<sf::Vector2i> DeletedSquares;
        for (auto& el : FromToPairs)
        {
            if (GameBoard[el.second.x][el.second.y].getFillColor() == sf::Color::Black && GameBoard[el.second.x][el.second.y].GetBonus() == NULL)
                continue;
            std::vector<sf::Vector2i> Deleted = GameBoard[el.second.x][el.second.y].DropBonus(GameBoard, el.second, el.first);
            DeletedSquares.insert(DeletedSquares.end(), Deleted.begin(), Deleted.end());
        }
        std::sort(DeletedSquares.begin(), DeletedSquares.end(), compareByYCoordinate);
        return DeletedSquares;
    }
}