#ifndef MAP_H
#define MAP_H

#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <string>
#include <functional>
using namespace std;
class Map
{
	public:	
		struct Can {
			int x, y;
			
			Can(int ix, int iy) : x(ix), y(iy) {}
			
			friend ostream& operator<<(ostream& out, const Can& can){
				out << "(" << can.x << "," << can.y << ")";
				return out;
			}
		};
		
		struct Individual {
			vector<int> genes;
			int moves;
			int score;
			int actions;

			Individual(vector<int> g) : genes(g), actions(0), score(0) {}
			
			bool operator<(const Individual& other) const {
				return actions > other.actions;
			}

			bool operator>(const Individual& other) const {
				return actions < other.actions;
			}

			friend ostream& operator<<(ostream& out, const Individual& ind){
				for(int i = 0; i < ind.genes.size(); i++){
					out << ind.genes[i];
				}
				out << "| moves: " << ind.moves
					<< "| actions: " << ind.actions
					<< "| score: " << ind.score;
				return out;
			}
		};

	private:
		int mapSizeX, mapSizeY;
		int actions;
		int startX, startY;
		vector<Can> cans; 
		int maxCans;
	 	int** array;

		vector<Individual> currentGen;
		vector<Individual> nextGen;	
	public:
		Map();

		// getters and setters
		int getMapSizeX() const { return mapSizeX; }
		int getMapSizeY() const { return mapSizeY; }
		int getActions() const { return actions; }
		int getStartX() const { return startX; }
		int getStartY() const { return startY; }

		void buildMap();
		void deleteMap();

		char getRandAction();
				
		Individual initIndividual();
		void initPopulation(int);
		Individual createChild(Individual&, Individual&);
		void createNextPopulation(int);
		void mutate();

		void fitness(Individual&);
		Individual maxFit(vector<Individual>&);
		Individual minFit(vector<Individual>&);

		int scoreIndividual(vector<int>);
		
		void GA();
		
		// debugging functions
		void dispPopulations(); 
		void tracePath(Individual&);
};

#endif
