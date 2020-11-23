#pragma once
#include <vector>

class Intersection{
	public:
		int getXPos();
		int getYPos();
		std::vector<Intersection> getAdj();
	private:
		int xPos;
		int yPos;
		std::vector<Intersection> adj;
};

