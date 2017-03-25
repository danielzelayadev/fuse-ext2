#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

using namespace std;

void split(const string& s, char c, vector<string>& v);
bool indexOutOfBounds(int index, int size);
void turnOnBit(char* b, int pos);
void turnOffBit(char* b, int pos);
void toggleBit(char* b, int pos);
bool bitIsOn(char b, int pos);
bool bitIsOff(char b, int pos);
const char* decToOctalStr(int num);
int roundUp(int num, int multiple);
uint16_t _getuid();
uint16_t _getgid();

#endif