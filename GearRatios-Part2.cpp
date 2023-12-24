#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_set>

using namespace std;

bool HasNearbyNumber(const int& Line0Index, const string& bopttomLine, const string& line1, const string& line2, const unordered_set<char>& numbersToFind);
void FindNearbyNumbers(vector<int>& numberVector, const string& bottomLine, const string& middleLine, const string& topLine, const int& symbolIndex, const unordered_set<char>& numbersToFind);
int ReturnTotalNumber(const string& line, int index, const unordered_set<char>& numbersToFind);
int SumAndReturn(const vector<int>& numbers);

int main() {
	fstream myFile;
	string topLine, middleLine, bottomLine;

	unordered_set<char> ignoreCharsToFindSymbol = { '.', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };
	unordered_set<char> numbersToFind = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	vector<int> numbers;

	myFile.open("../../../cool.txt", ios::in);

	if (myFile.is_open()) {
		bool numbersExistAroundSymbol = false;
		bool endLoop = false;
		bool nextLineFinish = false;
		//get the first 3 lines
		//write line 0
		while (!endLoop) {

			if (!getline(myFile, bottomLine)) {
				bottomLine.clear();
			}

			if (bottomLine.empty()) {
				nextLineFinish = true;
			}

			//cout << "--------------------- line start -------------------" << endl;
			int currentIndex = 0;
			vector<int> symbolIndexesToCheck;

			//if middle line is empty, skip logic, add this current line to middle line
			if (middleLine.empty()) {
				topLine = middleLine; // 1 -> 2
				middleLine = bottomLine; // 0 -> 1
				continue;
			}

			/*cout << "top line: " << topLine << endl;
			cout << "middle line: " << middleLine << endl;
			cout << "bottom line: " << bottomLine << endl;*/

			//check for symbols that are not numbers or fulls tops
			int indexMax = middleLine.length();
			while (currentIndex < indexMax) {

				//add to list if its not a full stop or any number
				if (ignoreCharsToFindSymbol.find(middleLine[currentIndex]) == ignoreCharsToFindSymbol.end()) {
					symbolIndexesToCheck.push_back(currentIndex);
				}

				//continue seaching
				currentIndex++;
			}

			//Does this symbol have numbers?
			for (int symbolIndex : symbolIndexesToCheck) {

				numbersExistAroundSymbol = HasNearbyNumber(symbolIndex, bottomLine, middleLine, topLine, numbersToFind);

				if (numbersExistAroundSymbol) {
					//cout << "has nearby number" << endl; 
				}

				//if has symbol check surrounding for numbers
				if (numbersExistAroundSymbol) {
					vector<int> nearByNumbers;
					FindNearbyNumbers(nearByNumbers, bottomLine, middleLine, topLine, symbolIndex, numbersToFind);

					for (auto ele : nearByNumbers) {
						numbers.push_back(ele);
						cout << ele << endl;
					}
				}

				////reset for next itt
				numbersExistAroundSymbol = false;
			}

			topLine = middleLine; // 1 -> 2
			middleLine = bottomLine; // 0 -> 1

			if (nextLineFinish) {
				endLoop = true;
			}
		}
	}
	else {
		cout << "Failed to open file." << endl;
	}
	cout << "-----------------------------TOTAL----------------------------" << endl;
	cout << "TOTAL: " << SumAndReturn(numbers) << endl;

	myFile.close();
}

bool HasNearbyNumber(const int& index, const string& bottomLine, const string& middleLine, const string& topLine, const unordered_set<char>& numbersToFind) {

	//check if bottom or top line is NULL
	bool ignoreTopLine = false, ignoreBottomLine = false;
	if (topLine.empty()) ignoreTopLine = true;
	if (bottomLine.empty()) ignoreBottomLine = true;

	//check bounds of line
	int indexMin = max(index - 1, 0);
	int indexMax = min(index + 2, static_cast<int>(middleLine.size() - 1));

	for (int i = indexMin; i < indexMax; i++) {
		//if there is a number in this index position

		if (!ignoreBottomLine && numbersToFind.find(bottomLine[i]) != numbersToFind.end()) return true;
		if (numbersToFind.find(middleLine[i]) != numbersToFind.end()) return true;
		if (!ignoreTopLine && numbersToFind.find(topLine[i]) != numbersToFind.end()) return true;
	}

	return false;
}

void FindNearbyNumbers(vector<int>& numberVector, const string& bottomLine, const string& middleLine, const string& topLine, const int& symbolIndex, const unordered_set<char>& numbersToFind) {

	//check if bottom or top line is NULL
	bool ignoreTopLine = false, ignoreBottomLine = false;
	if (topLine.empty()) ignoreTopLine = true;
	if (bottomLine.empty()) ignoreBottomLine = true;

	//check bounds of line
	int indexMin = max(symbolIndex - 1, 0);
	int indexMax = min(symbolIndex + 2, static_cast<int>(middleLine.size()));

	bool isSingleNumberBottom = false, isSingleNumberTop = false;

	//check if bottom or top rows have a single number // e.g 333 so it doesn't count it as 3 numbers -> 333, 333, 333
	if (!ignoreBottomLine && numbersToFind.find(bottomLine[symbolIndex]) != numbersToFind.end()) {
		isSingleNumberBottom = true;
		//cout << "BOTTOM ROW: giant number detected" << endl;
	}

	if (!ignoreTopLine && numbersToFind.find(topLine[symbolIndex]) != numbersToFind.end()) {
		isSingleNumberTop = true;
		//cout << "TOP ROW: giant number detected" << endl;
	}

	//if there is a number in this index position
	//find this entire number and push back to the vector

	//BOTTOM ROW
	for (int i = indexMin; i < indexMax; i++) {

		if (!ignoreBottomLine && numbersToFind.find(bottomLine[i]) != numbersToFind.end()) {

			//check if its 1 giant number so we get no repeats
			if (isSingleNumberBottom) {
				numberVector.push_back(ReturnTotalNumber(bottomLine, symbolIndex, numbersToFind));
				isSingleNumberBottom = false;
				break;
			}
			numberVector.push_back(ReturnTotalNumber(bottomLine, i, numbersToFind));
		}
	}

	//MIDDLE ROW
	for (int i = indexMin; i < indexMax; i++) {
		if (numbersToFind.find(middleLine[i]) != numbersToFind.end()) {
			numberVector.push_back(ReturnTotalNumber(middleLine, i, numbersToFind));
		}
	}

	//TOP ROW
	for (int i = indexMin; i < indexMax; i++) {
		if (!ignoreTopLine && numbersToFind.find(topLine[i]) != numbersToFind.end()) {

			//check if its 1 giant number so we get no repeats
			if (isSingleNumberTop) {
				numberVector.push_back(ReturnTotalNumber(topLine, symbolIndex, numbersToFind));
				isSingleNumberTop = false;
				break;
			}
			numberVector.push_back(ReturnTotalNumber(topLine, i, numbersToFind));
		}
	}

	if (numberVector.size() != 2) {
		numberVector.clear();
	}
	else {
		int temp = numberVector[0] * numberVector[1];
		numberVector.clear();
		numberVector.push_back(temp);
	}
}

int ReturnTotalNumber(const string& line, int index, const unordered_set<char>& numbersToFind) {
	string finishedNumberAsString = "";
	int finishedNumberAsInt = 0;

	int currentIndex = index;

	finishedNumberAsString += line[index];

	currentIndex++;
	//search all indexes to the right of the number until not a number
	while (currentIndex < line.length() && numbersToFind.find(line[currentIndex]) != numbersToFind.end()) {
		//add to back of string
		finishedNumberAsString += line[currentIndex];
		currentIndex++;
	}

	currentIndex = index;
	currentIndex--;
	//search all indexes to the left of the number until not a number
	while (currentIndex >= 0 && numbersToFind.find(line[currentIndex]) != numbersToFind.end()) {
		//add to front of string
		finishedNumberAsString.insert(0, 1, line[currentIndex]);
		currentIndex--;
	}

	finishedNumberAsInt = stoi(finishedNumberAsString);

	return finishedNumberAsInt;
}

int SumAndReturn(const vector<int>& numbers) {
	int total = 0;

	for (int num : numbers)
	{
		total += num;
	}

	return total;
}