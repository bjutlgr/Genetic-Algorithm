#include "Map.h"
/***********************************************************
 * Class map
 * implementation and methods for a map with a solver using 
 * a genetic algorithm
 *
 * encoding: 
 * 	0 - N
 * 	1 - S
 * 	2 - E
 * 	3 - W
 * 	4 - Random direction
 * 	5 - pick up can
 * 	6 - stay put
 ***********************************************************/
Map::Map()
{
	ifstream inputFile("input.dat");
	string line;
	string param1, param2, param3;
	istringstream liness;
	while(getline(inputFile, line))
	{
		liness.clear();
		liness.str(line);
		getline(liness, param1, ' ');
		getline(liness, param2, ' ');
		getline(liness, param3, ' ');
		if(param1 == "MAP"){
			mapSizeX = atoi(param2.c_str());
			mapSizeY = atoi(param3.c_str());
		} else if(param1 == "ACTION"){
			actions = atoi(param2.c_str());
		} else if(param1 == "START"){
			startX = atoi(param2.c_str());
			startY = atoi(param3.c_str());
		} else if(param1 == "CAN"){
			cans.push_back(Can( atoi(param2.c_str()) , atoi(param3.c_str()) ));
		}
	}
	maxCans = cans.size();

	srand(time(NULL)); // used for population initlizing

	//printf("MAP %d %d\n", mapSizeX, mapSizeY);
	//printf("ACTION %d\n", actions);
	//printf("START %d %d\n", startX, startY);
	//for(int i = 0; i < cans.size(); i++){
	//	cout << cans[i] << endl; 
	//}
}	

/**************************************************************
 * buildMap
 **************************************************************/
void Map::buildMap()
{
 	int i, j;
 	array = new int*[mapSizeY];
	for(i = 0; i < mapSizeY; i++){
		array[i] = new int[mapSizeX];
	}

	for(i = 0; i < mapSizeY; i++){
		for(j = 0; j < mapSizeX; j++){
			array[i][j] = 0;
		}
	}
	// mark start
	array[startY][startX] = 1;
	// 2 = can
	for(i = 0; i < cans.size(); i++){
		//printf("(%d,%d)\n", canX[i], canY[i]);
		array[cans[i].y][cans[i].x] = 2;
	}

	// uncomment to show map
//	int counter = 0;
//	for(i = 0; i < mapSizeY; i++){
//		cout << setw(2) << i << ":";
//		for(j = 0; j < mapSizeX; j++){
//			if(array[i][j] == 1){
//				printf("[*]");
//			} else if(array[i][j] == 2){ 
//				printf("[%d]",counter);
//				counter++;
//			} else {
//				printf("[ ]");
//			}
//		}
//		cout << endl;
//	}
}

/*************************************************************
 * deleteMap
 ************************************************************/
void Map::deleteMap()
{
	for(int i = 0; i < mapSizeY; i++){
		delete [] array[i];
	}
	delete [] array;
}

/**************************************************************
 * initIndividual
 * 
 * initializes individual with gene and fitness score
 *************************************************************/
Map::Individual Map::initIndividual()
{
	vector<int> genes(maxCans);
	int temp;
	for(int i = 0; i < maxCans; i++){
		//temp = int(round(i*( double(rand()) / (long(RAND_MAX)+1) )));
		temp = rand() % (i+1);
		genes[i] = genes[temp];
		genes[temp] = i;
	}
	Individual ind(genes);
	fitness(ind);
	return ind;
}

/*************************************************************
 * fitnessScore
 *
 * calculates several different metrics on the input
 ************************************************************/
void Map::fitness(Individual& ind)
{
	ind.moves = 0;
	ind.score = 0;
	ind.actions = 0;
	int canInd, nextCanInd;
	ind.moves += (abs(startX-cans[ind.genes[0]].x) + abs(startY-cans[ind.genes[0]].y));
	for(int i = 0; i < ind.genes.size()-1; i++){
		canInd = ind.genes[i];
		nextCanInd = ind.genes[i+1];
		ind.moves += (abs(cans[canInd].x-cans[nextCanInd].x) + 
				  abs(cans[canInd].y-cans[nextCanInd].y));
	}
	ind.actions = ind.moves + maxCans;
	ind.score = maxCans*10 - ind.actions;
}

/************************************************************
 * initPopulation
 ***********************************************************/
void Map::initPopulation(int n)
{ 
	for(int i = 0; i < n; i++){
		Individual org = initIndividual();
		//cout << org << endl;
		currentGen.push_back(org);
	}
	sort(currentGen.begin(), currentGen.end(), greater<Individual>());
}

/*************************************************************
 * createNextPopulation
 * creates next generation by means of roulette selection
 ************************************************************/
void Map::createNextPopulation(int size)
{
	vector<double> probSum;
	Individual ind = minFit(currentGen);

	double num = 0.0;
	int sum = 0, i, k;
	double temp = 0.0, temp2 = 0.0;
	int p1 = 0, p2 = 0;

	// scale and sum the scores
	for(i = 0; i < currentGen.size(); i++){
		sum += currentGen[i].actions;
	}
	
	// make the "roulette" wheel
	for(i = 0; i < currentGen.size(); i++){
		temp += double(currentGen[i].actions)/sum;
		//cout << temp << endl;
		probSum.push_back(temp);
	}

	//for(i = 0; i < actions; i++){
	for(k = 0; k < int(currentGen.size()/10); k++){
		num = double(rand())/RAND_MAX;	// indicator for p1
		//cout << "num1 = " << num << ". ";
		for(i = 0; i < currentGen.size(); i++){
			if(i==0){
				if(num<=probSum[i]){
					p1 = i;
					break;
				}
			} else {
				if(num>probSum[i-1] && num<=probSum[i]){
					p1 = i;
					break;
				}
			}
		}
		//cout << "parent 1 index: " << p1;

		p2 = p1;
		while(p2==p1){ // runs at least once, and continues until parents are distinct
			num = double(rand())/RAND_MAX;
			//cin >> temp;
			for(i = 0; i < currentGen.size(); i++){
				if(i==0){
					if(num<=probSum[i]){
						p2 =i;
						break;
					}
				} else {
					if(num>probSum[i-1] && num<=probSum[i]){
						p2 = i;
						break;
					}
				}
			}		
		}
		//cout << ". num2 = " << num << ". ";
		//cout << "parent 2 index: " << p2 << endl; 

		nextGen.push_back(createChild(currentGen[p1], currentGen[p2])); // goes with attemp 2, and 3
	}
	
	// Attempt 3: concatenate first N offsprings with top 90 of previous gen
	// mutate();
	currentGen.erase(currentGen.end()-int(currentGen.size()/10), currentGen.end()); // get rid of bottom 10% of current
	currentGen.insert(currentGen.end(), nextGen.begin(), nextGen.end()); // append bottom 10%
	sort(currentGen.begin(), currentGen.end(), greater<Individual>());
	for(i = 0; i < currentGen.size(); i++) fitness(currentGen[i]);
	nextGen.clear();
}

/************************************************************
 * createChild
 ************************************************************/
Map::Individual Map::createChild(Individual& parent1, Individual& parent2)
{
	vector<int> chGenes;
	int point = int(maxCans*( double(rand()) / (long(RAND_MAX)+1) )); // choose crossover point
	int which = rand() % 2; 
		
	int i, j;
	//cout << "Crossover point: " << point << ", which: " << which << endl;
	int lookup, toMatch;

	if(which == 0){ // take the first n = point genes from parent 1
		chGenes = parent2.genes;
		for(i = 0; i < point; i ++){
			toMatch = parent1.genes[i];
			//cout << "Looking for " << toMatch << endl;
			for(j = i; j < maxCans; j++){
				if(parent2.genes[j] == toMatch){
					//cout << "swapping " << chGenes[i] << " with " << chGenes[j] << endl; 
					chGenes[j] = chGenes[i];
					chGenes[i] = toMatch;
					break;
				}
			}
		}	 
	} else if(which == 1){ // take the first n = point genes from parent 2
		chGenes = parent1.genes;
		for(i = 0; i < point; i ++){
			toMatch = parent2.genes[i];
			for(j = i; j < maxCans; j++){
				if(parent1.genes[j] == toMatch){
					chGenes[j] = chGenes[i];
					chGenes[i] = toMatch;
					break;
				}
			}
		}
	}
	Individual child(chGenes);
	fitness(child);
	return child;
}

/*****************************************************************
 * attempt to mutate the next generation
 * simply swaps two genes
 ****************************************************************/
void Map::mutate()
{
	double mutateProb;
	int mutatePoint1, mutatePoint2, temp;
	vector<Individual>::iterator iter;
	for(iter = nextGen.begin(); iter != nextGen.end(); iter++){
		mutateProb = double(rand())/RAND_MAX;
		if(mutateProb < 0.1){
			//cout << "Mutate!\n";
			mutatePoint1 = int(maxCans*( double(rand()) / (long(RAND_MAX)+1) ));
			mutatePoint2 = mutatePoint1;
			while(mutatePoint2 == mutatePoint1){
				mutatePoint2 = int(maxCans*( double(rand()) / (long(RAND_MAX)+1) ));	
			}
			//cout << "Swap index " << mutatePoint1 << " with " << mutatePoint2 << endl;
			temp = (*iter).genes[mutatePoint1];
			(*iter).genes[mutatePoint1] = (*iter).genes[mutatePoint2];
			(*iter).genes[mutatePoint2] = temp;
			fitness((*iter));
		}
	}
}
 
/************************************************************
 * displayPopulations
 ************************************************************/
void Map::dispPopulations()
{
	for(int i = 0; i < currentGen.size(); i++){
		cout << "i: " << i << "- " << currentGen[i] << endl;
	}
	cout << endl;
}

/***********************************************************
 * maxFit
 **********************************************************/
Map::Individual Map::maxFit(vector<Individual>& pop)
{
	return *max_element(pop.begin(), pop.end());
}

/**********************************************************
 * minFit
 *********************************************************/
Map::Individual Map::minFit(vector<Individual>& pop)
{
	return *min_element(pop.begin(), pop.end());
}

/***********************************************************
 * GA - genetic algorithm
 ***********************************************************/
void Map::GA()
{
	int populationSize = 100*maxCans;
	initPopulation(populationSize);
	//dispPopulations();
	int temp;

	int prevBest = (*max_element(currentGen.begin(), currentGen.end())).moves;
	int best;
	int streak = 0;
	int runtime = 0;
	while(streak != 100){
		createNextPopulation(populationSize);
		//dispPopulations();
		//cin >> temp;
		best = (*max_element(currentGen.begin(), currentGen.end())).moves;
		//cout << "streak: " << streak << endl;
		if(best == prevBest){
			streak++;
		} else {
			streak = 0;
		}
		prevBest = best;
	}
	//cout << (*max_element(currentGen.begin(), currentGen.end())) << endl;
	cout << "DONE!\n";
	tracePath((*max_element(currentGen.begin(), currentGen.end()))); 
}
	
/**********************************************************
 * tracePath
 *********************************************************/
void Map::tracePath(Individual& ind)
{
	int curX = startX, curY = startY;
	ofstream outputFile("output.dat");
	outputFile << "(" << startX << "," << startY << ") - start";
	int nextX, nextY;
	for(int i = 0; i < ind.genes.size(); i++){
		nextX = cans[ind.genes[i]].x;
		nextY = cans[ind.genes[i]].y;
		while(!(nextX == curX && nextY == curY)){
			outputFile << endl;
			if(nextX>curX) curX++;
			else if(nextX<curX) curX--;
			else if(nextY>curY) curY++;
			else if(nextY<curY) curY--;
			
			outputFile << "(" << curX << "," << curY << ")";
		}
		outputFile << " - picked up can";
	}
	outputFile << endl;
	outputFile << "Total number of cans picked up: " << maxCans << endl;
	outputFile << "Total actions required (moved + pick ups): " << ind.moves << "+" << maxCans << "=" << ind.actions << endl;
	outputFile << "The score for this run is " << ind.score << endl << endl;
	
	outputFile << "This problem was done by realizing the problem as the traveling salesman problem,\n"; 
	outputFile << "which is known to be NP-hard. Optimal solution not guaranteed.\n";
	if(ind.actions > actions){
		outputFile << "This run has exceeded the total number of actions allowed\n";
		outputFile << "Optimal solution (picking up all the cans using least amount of actions below the allowed) possibly nonexistent.\n";
	} else {
		outputFile << "Actions allowed not exceeded by total actions.\n";
		outputFile << "Possible candidate for optimal solution.\n";
	}
	outputFile.close();
}











