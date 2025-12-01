#pragma once

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