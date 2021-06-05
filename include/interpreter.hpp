#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <climits>
#include <type_traits>
#include <stdexcept>
#include <algorithm>

/**
 * Takes in a string, and interprets it as bf.
 *
 * @param code Input code
 * @param flags Changes the manner of interpretation, such as verbosity or optimization
 */
void interpret(std::string code, std::vector<std::string> flags);

/**
 * Takes in dirty bf, and parses out comments.
 *
 * @param dirtyCode Input code
 * @return Cleaned code
 */
std::string clean(std::string dirtyCode);
