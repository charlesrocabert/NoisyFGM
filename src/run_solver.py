
#!/usr/bin/env python
# coding: utf-8

#**********************************************************************
# Copyright (C) 2016-2017 Charles Rocabert, Samuel Bernard
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
#**********************************************************************

import os
import sys
import math

def run_solver( stabt, t, shutofffitness, shutofftime, seed, nbdim,
	fitness_shape, fitness_parameter,
	nbpart, initmu, initsigma, inittheta, dmu, dsigma, dtheta,
	stats, stats2D,
	oneaxis, nonoise, isotropic, norotation, qagi ):
	cmdline = "../build/bin/run_solver"
	cmdline += " -stabt "+str(stabt)
	cmdline += " -t "+str(t)
	cmdline += " -shutofffitness "+str(shutofffitness)
	cmdline += " -shutofftime "+str(shutofftime)
	cmdline += " -seed "+str(seed)
	cmdline += " -nbdim "+str(nbdim)
	cmdline += " -fitnessshape "+str(fitness_shape)
	cmdline += " -fitnessparameter "+str(fitness_parameter)
	cmdline += " -nbparticles "+str(nbpart)
	cmdline += " -initmu "+str(initmu)
	cmdline += " -initsigma "+str(initsigma)
	cmdline += " -inittheta "+str(inittheta)
	cmdline += " -dmu "+str(dmu)
	cmdline += " -dsigma "+str(dsigma)
	cmdline += " -dtheta "+str(dtheta)
	if stats:
		cmdline += " -statistics"
	if stats2D:
		cmdline += " -2Dstatistics"
	if oneaxis:
		cmdline += " -oneaxis"
	if nonoise:
		cmdline += " -nonoise"
	if isotropic:
		cmdline += " -isotropicnoise"
	if norotation:
		cmdline += " -norotation"
	if qagi:
		cmdline += " -qagi"
	print cmdline
	os.system(cmdline)
	os.system("Rscript plot_data.R /Users/charlesrocabert/git/SigmaFGM/example/output.png")


############
#   MAIN   #
############

if __name__ == '__main__':
	STABILIZING_TIME  = 0
	SIMULATION_TIME   = 0
	SHUTOFF_FITNESS   = 0.9
	SHUTOFF_TIME      = 20000
	SEED              = 87693
	NB_DIMENSIONS     = 10
	FITNESS_SHAPE     = "exponential"
	FITNESS_PARAMETER = 2.0
	NB_PARTICLES      = 10000
	INITIAL_MU        = 4.0/math.sqrt(float(NB_DIMENSIONS))
	INITIAL_SIGMA     = 1e-15
	INITIAL_THETA     = 0.0
	D_MU              = 0.01/math.sqrt(float(NB_DIMENSIONS))
	D_SIGMA           = 0.1/math.sqrt(float(NB_DIMENSIONS))
	D_THETA           = 0.1/math.sqrt(float(NB_DIMENSIONS))
	STATISTICS        = True
	STATISTICS_2D     = False
	ONE_AXIS          = False
	NO_NOISE          = False
	ISOTROPIC_NOISE   = False
	NO_ROTATION       = False
	QAGI              = False

	run_solver(STABILIZING_TIME, SIMULATION_TIME, SHUTOFF_FITNESS, SHUTOFF_TIME,
		SEED, NB_DIMENSIONS,
		FITNESS_SHAPE, FITNESS_PARAMETER,
		NB_PARTICLES, INITIAL_MU, INITIAL_SIGMA, INITIAL_THETA, D_MU, D_SIGMA, D_THETA,
		STATISTICS, STATISTICS_2D,
		ONE_AXIS, NO_NOISE, ISOTROPIC_NOISE, NO_ROTATION, QAGI)
