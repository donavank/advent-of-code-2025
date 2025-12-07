#pragma once
#include <vector>
#include <string>
/// <summary>
/// Day 1 Challenge Part 1.
/// Reads from rotations.txt to calculate the door entry code which is the
/// number of times the dial ends on zero after performing the rotations. Rotations
/// are encoded as L (rotate toward smaller numbers)/R (rotate toward bigger numbers) and a number
/// to indicate how large the rotation is.
/// </summary>
/// <returns>
/// The code for the secret entrance
/// </return>
int secretEntrance1();

/// <summary>
/// Day 1 Challenge Part 2.
/// Reads from rotations.txt to calculate the REAL door entry code which is the number of times the
/// dial passes 0 at all. Rotations are encoded as L (rotate toward smaller numbers)/R 
/// (rotate toward bigger numbers) and a number to indicate how large the rotation is.
/// </summary>
/// <returns>
/// The REAL door entry code
/// </returns>
int secretEntrance2();

/// <summary>
/// Reads product-id-ranges.txt to find invalid ids in the ranges and sum them. The input test is a comma-
/// separated list of ranges which themselves are 2 integers separated by a '-'. An invalid id is a set of digits
/// repeated twice.
/// </summary>
/// <returns>
/// The sum of invalid ids in the ranges provided.
/// </returns>
long long validateIds1();

/// <summary>
/// Reads product-id-ranges.txt to find invalid ids in the ranges and sum them. The input test is a comma-
/// separated list of ranges which themselves are 2 integers separated by a '-'. An invalid id is any number with a set
/// of digits repeated 2 or more times.
/// </summary>
/// <returns>
/// The sum of invalid ids in the ranges provided.
/// </returns>
long long validateIds2();

static bool isValidId1(long long id);
static std::vector<long long> getInvalidIdsInRange(std::string start, std::string end); \
static int isInvalidId2(long long id);

/// <summary>
/// Calculates the maximum joltages by summing the joltages available for a setting of batteries. A battery's joltage is determined by 
/// turning on 2 digits in the battery's line.
/// </summary>
/// <returns>maximized joltage for the given set of batteries</returns>
int maximizeJoltages1();


