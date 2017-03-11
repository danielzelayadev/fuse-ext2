#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

using namespace std;

void split(const string& s, char c, vector<string>& v);
bool indexOutOfBounds(int index, int size);

#endif