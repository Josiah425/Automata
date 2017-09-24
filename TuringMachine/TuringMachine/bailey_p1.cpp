#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

int main(int argc, char* argv[]) {
	std::ifstream fin;
	fin.open(argv[1]);
	std::string machine = argv[2];
	char before[10001];
	char after[10001];
	memset(before, '_', 10001);
	memset(after, '_', 10001);
	int maxTransitions = atoi(argv[3]);
	int start;
	std::string line;

	//Get accept and reject states
	std::vector<int> accepts, rejects;
	std::string state, num, type;
	std::vector<int> q, r;
	std::vector<char> a, b, x;
	char qS, aS, rS, bS, xS;
	int transitionCount = 0;
	while (getline(fin, line)) {
		std::istringstream lineStream(line);
		lineStream >> state >> num >> type;
		if (type == "reject") rejects.push_back(atoi(num.c_str()));
		else if (type == "accept") accepts.push_back(atoi(num.c_str()));
		else if(type == "start") start = atoi(num.c_str());
		else if (state == "transition") {
			std::istringstream lineStream(line);
			lineStream >> state >> qS >> aS >> rS >> bS >> xS;
			q.push_back(qS - '0');
			a.push_back(aS);
			r.push_back(rS - '0');
			b.push_back(bS);
			x.push_back(xS);
			transitionCount++;
		}
	}

	int index = 0;
	int currState = start;
	bool acceptB = false;
	bool rejectB = false;
	std::cout << currState << "->";
	char symbol = machine[index];
	for (int i = 0; i < maxTransitions; i++) {
		for (int j = 0; j < transitionCount; j++) {
			if (q[j] == currState && a[j] == symbol) {
				currState = r[j];
				if(index >= 0 && index < machine.size()) machine[index] = b[j];
				else if (index < 0) before[10001 + index] = b[j];
				else after[index] = b[j];
				if (x[j] == 'R') index++;
				else index--;
				if(index >= 0 && index < machine.size()) symbol = machine[index];
				else if (index < 0) symbol = before[10001 + index];
				else symbol = after[index];
				std::cout << currState << "->";
				break;
			}
		}
		for (int j = 0; j < rejects.size(); j++) {
			if (currState == rejects[j]) {
				std::cout << " reject" << std::endl;
				rejectB = true;
			}
		}
		for (int j = 0; j < accepts.size(); j++) {
			if (currState == accepts[j]) {
				std::cout << " accept" << std::endl;
				acceptB = true;
			}
		}
		if (acceptB || rejectB) break;
	}
	if (acceptB == false && rejectB == false) std::cout << " quit" << std::endl;
	system("pause");
}