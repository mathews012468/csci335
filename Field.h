#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

class Field
{
  public:

Field(const std::vector<std::vector<int>> & arr) {
	initializeSize(arr); //sets height and width to dimensions of arr
	checkArr(arr); //makes sure arr is a rectangle
	int_array = arr;

	if (height > 0 && width > 0) {
		weightVector(); //weights array is computed
	}
}

Field(std::vector<std::vector<int>> && arr) {
	initializeSize(arr); //sets height and width to dimensions of arr
	checkArr(arr); //makes sure arr is a rectangle
	int_array = std::move(arr);

	if (height > 0 && width > 0) {
		weightVector(); //weights array is copied
	}
}

int Weight(int x1, int y1, int x2, int y2) {
	checkCoords(x1,y1,x2,y2); //makes sure coordinates are in array

	int maxX = std::max(x1,x2);
	int minX = std::min(x1,x2);
	int maxY = std::max(y1,y2);
	int minY = std::min(y1,y2);

	int sum = weights[maxY][maxX];
	if (minX != 0 && minY != 0) {
		sum += weights[minY-1][minX-1] - weights[minY-1][maxX] - weights[maxY][minX-1];
	}
	else if (minX != 0) {
		sum -= weights[maxY][minX-1];
	}
	else if (minY != 0) {
		sum -= weights[minY-1][maxX];
	}

	return sum;
}

int PathCost() {
	std::vector<std::vector<int>> cheapest_path = int_array;
	for (int i = 1; i < width; i++) {//takes care of first row
		cheapest_path[0][i] += cheapest_path[0][i-1];
	}
	for (int i = 1; i < height; i++) {//takes care of first column
		cheapest_path[i][0] += cheapest_path[i-1][0];
	}

	for (int i = 1; i < height; i++) {
		for (int j = 1; j < width; j++) {
			cheapest_path[i][j] += std::min(cheapest_path[i-1][j],cheapest_path[i][j-1]);
		}
	}

	return cheapest_path[height-1][width-1];
}

//prints the field, helped me see what I put in the array
void printArr() const {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			std::cout << int_array[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

//prints the weights array
void printWeights() const {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			std::cout << weights[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

  private:

//field
std::vector<std::vector<int>> int_array;

//weights array
//ij-th component is rectangle stretching from 0,0 to i,j
std::vector<std::vector<int>> weights;

//number of rows in int_array and weights
int height;

//number of columns in int_array and weights
int width;

//sets height and width of array
void initializeSize(const std::vector<std::vector<int>> & arr) {
	height = arr.size();
	if (height > 0) {
		width = arr[0].size();
	}
	else {
		width = 0;
	}
}

//computes weights array
void weightVector() {
	weights = int_array;
	for (int i = 1; i < width; i++) {//takes care of first row
		weights[0][i] += weights[0][i-1];
	}
	for (int i = 1; i < height; i++) {//takes care of first column
		weights[i][0] += weights[i-1][0];
	}

	for (int i = 1; i < height; i++) {
		for (int j = 1; j < width; j++) {
			weights[i][j] += weights[i-1][j] + weights[i][j-1] - weights[i-1][j-1];
		}
	}
}

//throws exception if coordinates are outside the range of array
void checkCoords(int x1, int y1, int x2, int y2) {
	if (x1 > width - 1 || x2 > width - 1 || y1 > height - 1 || y2 > height - 1) {
		throw std::out_of_range("coordinates are too large");
	}

	if (x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0) {
		throw std::out_of_range("coordinates cannot be negative");
	}
}

//throws exception if array is not a rectangle
void checkArr(const std::vector<std::vector<int>> arr) {
	for (int i = 1; i < height; i++) {
		if (arr[i].size() != width) {
			throw std::invalid_argument("array is not a rectangle");
		}
	}
}

};