#pragma once
#include <iostream>

inline void DEB(const char* c) { std::cout << "---DEBUG: " << c << "---" << std::endl; }
inline bool ERR(const char* c) { std::cout << "------ERROR: " << c << "---" << std::endl; return false; }
inline void TEST(const char* c) { std::cout << "------------TEST: " << c << "------------" << std::endl; }
