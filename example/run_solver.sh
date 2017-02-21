
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

../build/bin/run_solver\
-stabt        0\
-t            1000\
-seed         0\
-nbdim        2\
-nbparticles  1000\
-initmu       2.5\
-initsigma    1e-15\
-inittheta    0.0\
-dmu          0.0\
-dsigma       0.0\
-dtheta       0.0\
-statistics\
-oneaxis\
-nonoise\
-norotation\
#-weightfitness\
