#!/usr/bin/env python

import os
import sys

import numpy as np
import matplotlib.pyplot as plt


xvals, yvals = np.loadtxt(sys.argv[1], delimiter=",", skiprows=1, unpack=True)
plt.plot(xvals, yvals)
plt.xlabel("XS.width")
plt.ylabel("XS.Qb_cap")
plt.tight_layout()
plt.show()
