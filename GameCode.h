#pragma once

#ifndef GAME_CODE
#define GAME_CODE

#include <SFML/Graphics.hpp>
#include<vector>
#include<iostream>
#include<string>
#include<fstream>
#include<locale>
#include<sstream>
#include<iterator>

namespace Game {
	//Drawing Shapes
	class Bonus;

	class Square :public sf::RectangleShape
	{
	private:
		Bonus* _bns;
		float _size;
	public:
		Square(float const size);
		float GetSize() const;
		~Square();
		Bonus* GetBonus();
		void SetBonus(Bonus* bns);
		std::vector<sf::Vector2i> DropBonus(std::vector<std::vector<Square>>& GameField, sf::Vector2i& ToPos, sf::Vector2i& FromPos);
	};

	class Bonus
	{
		friend std::vector<sf::Vector2i> Square::DropBonus(std::vector<std::vector<Square>>& GameField, sf::Vector2i& ToPos, sf::Vector2i& FromPos);
	private:
		virtual std::vector<sf::Vector2i> DropBonus(std::vector<std::vector<Square>>& GameField, sf::Vector2i& ToPos, sf::Vector2i& FromPos) = 0;
	};

	class Bomb : public Bonus
	{
		friend std::vector<sf::Vector2i> Square::DropBonus(std::vector<std::vector<Square>>& GameField, sf::Vector2i& ToPos, sf::Vector2i& FromPos);
	private:
		std::vector<sf::Vector2i> DropBonus(std::vector<std::vector<Square>>& GameField, sf::Vector2i& ToPos, sf::Vector2i& FromPos);
	};

	class ReDrawer : public Bonus
	{
		friend std::vector<sf::Vector2i> Square::DropBonus(std::vector<std::vector<Square>>& GameField, sf::Vector2i& ToPos, sf::Vector2i& FromPos);
	private:
		std::vector<sf::Vector2i> DropBonus(std::vector<std::vector<Square>>& GameField, sf::Vector2i& ToPos, sf::Vector2i& FromPos);
	};
	//Game Functions
	class Square;
	const int GameBoardSize = 8;
	const int TileSize = 50;
	bool isTileClicked(int const click_x, int const click_y); 
	sf::Color GetRandomColor();
	std::vector<std::vector<Square>> GenerateGameBoard();
	void DrawGameBoard(sf::RenderWindow& window, std::vector<std::vector<Square>> const& elements); 
	std::vector<sf::Vector2i> ReplaceAndDeleteTiles(sf::RenderWindow& window, std::vector<std::vector<Square>>& GameBoard, int const FirstTileX, int const FirstTileY); 
	bool areCoordinatesEqual(int const First_x, int const First_y, int const Second_x, int const Second_y);
	bool areCoordinatesAdjacent(int const First_x, int const First_y, int const Second_x, int const Second_y);
	void swapTileColors(Square& s1, Square& s2);
	void clearQueue(std::vector<sf::Vector2i>& q); 
	bool isCoordinateInQueue(std::vector<sf::Vector2i>& q, sf::Vector2i& vec); 
	void FindTilesToDelete(std::vector<sf::Vector2i>& DeletedTiles, std::vector<std::vector<Square>>& GameBoard, int TileX, int TileY);
	void DeleteTiles(std::vector<std::vector<Square>>& GameBoard, std::vector<sf::Vector2i>& TilesToDelete); 
	bool compareByYCoordinate(sf::Vector2i const& a, sf::Vector2i const& b);
	void FillDeletedTiles(std::vector<sf::Vector2i>& vec, std::vector<std::vector<Square>>& GameBoard); 
	void DestroyTile(std::vector<std::vector<Square>>& GameBoard, sf::Vector2i& pos); 
	void SwapTiles(std::vector<std::vector<Square>>& GameBoard, sf::Vector2i& pos1, sf::Vector2i& pos2);
	void swapTileBonuses(Square& s1, Square& s2); 
	std::vector<sf::Vector2i> ReleaseAllBonuses(std::vector<std::vector<Square>>& GameBoard, std::vector<std::pair<sf::Vector2i, sf::Vector2i>>& FromToPairs);
	std::vector<std::pair<sf::Vector2i, sf::Vector2i>> RandomlyGenerateBonus(std::vector<std::vector<Square>>& GameBoard, std::vector<sf::Vector2i>& DeletedTiles);
	bool isCoordinateInVector(std::vector<std::pair<sf::Vector2i, sf::Vector2i>>& vec, sf::Vector2i& v);


} // namespace Game

#endif
