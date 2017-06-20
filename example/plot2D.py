
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
import time
import math
from matplotlib.pyplot import *
from matplotlib.patches import Ellipse
	
### Load mean state ###
def load_mean_state():
	step_vec = []
	t_vec = []
	mu1_vec = []
	mu2_vec = []
	sigma1_vec = []
	sigma2_vec = []
	theta_vec = []
	f = open("2Dstatistics.txt", "r")
	l = f.readline()
	l = f.readline()
	while l:
		l = l.strip("\n")
		l = l.split(" ")
		step_vec.append(float(l[0]))
		t_vec.append(float(l[1]))
		mu1_vec.append(float(l[2]))
		mu2_vec.append(float(l[3]))
		sigma1_vec.append(float(l[4]))
		sigma2_vec.append(float(l[5]))
		theta_vec.append(float(l[6]))
		l = f.readline()
	f.close()
	return t_vec, mu1_vec, mu2_vec, sigma1_vec, sigma2_vec, theta_vec

##################
#      MAIN      #
##################

os.system("rm -rf tmp")
os.mkdir("tmp")
ion()

t_vec, mu1_vec, mu2_vec, sigma1_vec, sigma2_vec, theta_vec = load_mean_state()

for i in range(len(t_vec)):
	if i%1000 == 0:
		print i
		clf()
		ellipse = Ellipse(xy=[mu1_vec[i], mu2_vec[i]], width=2*sigma1_vec[i], height=2*sigma2_vec[i], angle=math.degrees(theta_vec[i]))
		fig     = figure(0)
		ax      = fig.add_subplot(111, aspect='equal')
		plot(mu1_vec[0:i], mu2_vec[0:i], "g")
		ax.add_artist(ellipse)
		ellipse.set_clip_box(ax.bbox)
		ellipse.set_alpha(0.7)
		ellipse.set_facecolor("b")
		plot([0,0], [-4, 4], "r")
		plot([-4, 4], [0,0], "r")
		ax.set_xlim(-4, 4)
		ax.set_ylim(-4, 4)
		draw()
		fname = 'tmp/_tmp%03d.png' % i
		savefig(fname)
		time.sleep(0.01)


