# -*- coding: utf-8 -*-
"""
Created on Wed Jul 13 19:28:35 2022

@author: Muriel
"""

from serial import Serial

portserie = Serial(port='COM2', baudrate= 9600, timeout=2)

for i in range(1,13):
    donnerArduino = portserie.readline()
    print(donnerArduino)

portserie.close()