#include "Map.h"

using namespace std;

int main()
{
	Map map = Map();
	map.buildMap();
	map.GA();
	map.deleteMap();
	return 0;
}
