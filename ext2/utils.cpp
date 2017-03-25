#include "utils.h"
#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>

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

const char* decToOctalStr(int num) {
    string str("0");

    while (num) {
        const char* digit = new char[2]{ (num % 8) + '0', 0 };
        str.insert(1, digit);
        num = num / 8;
    }

    return str.c_str();
}

int roundUp(int num, int multiple) {  
    if(!multiple)  
        return num; 

    int remainder = num % multiple;
     
    if (!remainder)
        return num;

    return num + multiple - remainder; 
}

uint16_t _getuid() {
    return getuid();
}

uint16_t _getgid() {
    return getgid();
}