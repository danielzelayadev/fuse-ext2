#include "utils.h"
#include <iostream>

void split(const string& s, char c, vector<string>& v) {
   string::size_type i = 0;
   string::size_type j = s.find(c);

   while (j != string::npos) {
      if (i != j)
            v.push_back(s.substr(i, j-i));
      i = ++j;
      j = s.find(c, j);

      if (j == string::npos)
         v.push_back(s.substr(i, s.length()));
   }
}

bool indexOutOfBounds(int index, int size) {
      return index < 0 || index >= size;
}

void turnOnBit(char* b, int pos) {
    *b |= (1 << pos);
}

void turnOffBit(char* b, int pos) {
    *b &= ~(1 << pos);
}

void toggleBit(char* b, int pos) {
    *b ^= (1 << pos);
}

bool bitIsOn(char b, int pos) {
    return (b & (1 << pos));
}

bool bitIsOff(char b, int pos) {
    return !(b & (1 << pos));
}