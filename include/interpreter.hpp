#pragma once
#include <iostream>
#include <vector>
#include <string>

/**
 * Takes in a string, and interprets it as bf.
 *
 * @param code Input code
 */
void interpret(std::string code, bool isVerbose);

/**
 * Takes in dirty bf, and parses out comments.
 *
 * @param dirtyCode Input code
 * @return Cleaned code
 */
std::string clean(std::string dirtyCode);
