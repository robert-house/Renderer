//--------------------------------------------------------------------------------------
// File: Simulation.h
//
// Core world simulation used to house entities in 3D space and calculate physics for
// obj_phys objects
//
// Concepts:
//     Maintain a 3D field that houses the world space vectors for all objects in the
//     active simulation.
//
// Author: Robert House 2015
//--------------------------------------------------------------------------------------
#pragma once

class Simulation
{
private:
	const float		GRAV_CONSTANT_DEFAULT = 9.8;
	
	// Set up max size for rules of the simulation
	double			_MaxWorldSize;
	unsigned int	_MaxEntities;
	double			_MaxAllowedVelocity;

	// Add parameters for various factories



public:
	Simulation();
	~Simulation();
};

