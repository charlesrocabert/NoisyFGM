#!/usr/bin/env python3
# coding: utf-8

#***********************************************************************
# Copyright (C) 2016-2020
# Charles Rocabert, Guillaume Beslon, Carole Knibbe, Samuel Bernard
# Web: https://github.com/charlesrocabert/SigmaFGM/
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#***********************************************************************

import os
import sys
import math
import time
import numpy
import random
import datetime

### Run a simulation from the parameters set ###
def run_simulation( path, seed,
	g, shutoffd, shutoffg,
	nbdim,
	alpha, beta, Q,
	popsize, initmu, initsigma, inittheta,
	mmu, msigma, mtheta, smu, ssigma, stheta, noise ):
	cmdline  = path
	cmdline += " -seed "+str(seed)
	cmdline += " -g "+str(g)
	cmdline += " -shutoffd "+str(shutoffd)
	cmdline += " -shutoffg "+str(shutoffg)
	cmdline += " -nbdim "+str(nbdim)
	cmdline += " -alpha "+str(alpha)
	cmdline += " -beta "+str(beta)
	cmdline += " -Q "+str(Q)
	cmdline += " -popsize "+str(popsize)
	cmdline += " -initmu "+str(initmu)
	cmdline += " -initsigma "+str(initsigma)
	cmdline += " -inittheta "+str(inittheta)
	cmdline += " -mmu "+str(mmu)
	cmdline += " -msigma "+str(msigma)
	cmdline += " -mtheta "+str(mtheta)
	cmdline += " -smu "+str(smu)
	cmdline += " -ssigma "+str(ssigma)
	cmdline += " -stheta "+str(stheta)
	cmdline += " -noise "+str(noise)
	os.system(cmdline)


############
#   MAIN   #
############

################################################################################
# In this simulation, the configuration of Zhang et al. (2009) is exemplified:
# A population of individuals of size 1000 sits 2 units away from the fitness
# optimum and must evolve towards a one-dimensional fitness optimum.
# Phenotypic noise is evolvable, and the non-Gaussian fitness function owns a
# minimum fitness far from the optimum, thereby generating a plateau of log-
# fitness. See Rocabert et al. (2020) for details.
################################################################################
if __name__ == '__main__':
	#---------------------------#
	# 1) Define main parameters #
	#---------------------------#
	SEED               = random.randint(1,100000000)
	GENERATIONS        = 100000
	SHUTOFF_DISTANCE   = 0.4
	SHUTOFF_GENERATION = 100000
	NB_DIMENSIONS      = 1
	ALPHA              = 3.125
	BETA               = 0.1
	Q                  = 2.0
	POPULATION_SIZE    = 1000
	INITIAL_MU         = 2.0
	INITIAL_SIGMA      = 0.6
	INITIAL_THETA      = 0.0
	M_MU               = 1e-4
	M_SIGMA            = 0.0
	M_THETA            = 0.0
	S_MU               = 0.1
	S_SIGMA            = 0.0
	S_THETA            = 0.0
	NOISE              = "ISOTROPIC"
	REPETITIONS        = 200
	PATH               = "../build/bin/SigmaFGM_simulation"
	
	#---------------------------#
	# 2) Run the simulation     #
	#---------------------------#
	run_simulation(PATH, SEED,
		GENERATIONS, SHUTOFF_DISTANCE, SHUTOFF_GENERATION,
		NB_DIMENSIONS,
		ALPHA, BETA, Q,
		POPULATION_SIZE, INITIAL_MU, INITIAL_SIGMA, INITIAL_THETA,
		M_MU, M_SIGMA, M_THETA, S_MU, S_SIGMA, S_THETA, NOISE)
