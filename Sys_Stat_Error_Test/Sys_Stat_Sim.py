#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on November 07 10:09 PM 2019
Created in PyCharm
Created as QGP_Fluctuations/Sys_Stat_Sim

@author: Dylan Neff, Dyn04
"""

import matplotlib.pyplot as plt
import numpy as np


def main():
    trials = 10000
    num = 200
    mean = 5
    stat = 2
    sys = 3*0
    cover_stat = 0.0
    cover_sys = 0.0
    avgs = []
    for i in range(trials):
        # x = np.arange(0, num, 1)
        ystat = np.random.normal(mean, stat, num)
        ysys = np.random.normal(mean, sys, num)
        y = ysys + ystat - mean
        avg = np.average(y)
        avgs.append(avg)
        davg = np.sqrt(num*stat**2)/num
        sigavg = np.std(y)/num**0.5
        # print(f'average: {avg}+-{davg}')
        if avg-davg < mean < avg+davg:
            cover_stat += 1
        if avg-sigavg < mean < avg+sigavg:
            cover_sys += 1
        # print(f'std: {sigavg}, davg: {davg}')
        # plt.hist(y)
        # plt.show()
    print(f'{cover_stat / trials * 100}% cover_stat')
    print(f'{cover_sys / trials * 100}% cover_sys')

    sig = np.std(avgs)
    cover_sig = 0
    for avg in avgs:
        if avg-sig < mean < avg+sig:
            cover_sig += 1
    print(f'{cover_sig / trials * 100}% cover_sig')

    plt.hist(avgs)
    # plt.errorbar(x, y, fmt='ro', yerr=stat)
    plt.show()

    print('donzo')


if __name__ == "__main__":
    main()
