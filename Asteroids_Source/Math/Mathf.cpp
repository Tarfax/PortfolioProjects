#include "Mathf.h"

std::mt19937 Mathf::randomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Mathf::distribution;
bool Mathf::randomInit = false;