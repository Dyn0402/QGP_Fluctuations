#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on November 21 2:57 PM 2019
Created in PyCharm
Created as QGP_Fluctuations/rotate_angles_alg_test

@author: Dylan Neff, Dyn04
"""


import numpy as np


def main():
    angles = np.asarray([0.0, 2.0, 3.0, 4.0, 5.0, 6.0]) * np.pi
    print(angles/np.pi)
    print(np.asarray(rotate_angles(angles, -np.pi/2))/np.pi)


def rotate_angle(angle, rotate):
    angle += rotate
    while angle > 2*np.pi:
        angle -= 2*np.pi
    while angle < 0:
        angle += 2*np.pi

    return angle


def rotate_angles(angles, rotate):
    new_angles = []
    for angle in angles:
        new_angles.append(rotate_angle(angle, rotate))

    return new_angles


if __name__ == '__main__':
    main()
