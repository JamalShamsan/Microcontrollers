#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Mar 23 17:13:43 2026

@author: rasmus
"""

import serial
import numpy as np
import matplotlib.pyplot as plt

port= "COM26"
baudrate=9600
longitude=[]
latitude=[]
max=200

ser = serial.Serial(port, baudrate)
print(f"Connected to {port} at {baudrate} baud")

for n in range(0, max):
        line = ser.readline().decode('ascii', errors='replace').strip()                
        if line.startswith('$'):
            # Basic parsing
            parts = line.split(',')
            if parts[0].endswith('GGA') and len(parts) > 9:
                print(f"  -> Lat: {parts[2]} {parts[3]}, Lon: {parts[4]} {parts[5]}, Alt: {parts[9]}m")
                # calculate decimal degrees right here before storing !
                # Determine if latitude or longitude based on string length
                degreeslat=int(parts[2][:2])
                minuteslat=float(parts[2][2:])
                degreeslong=int(parts[4][:3])
                minuteslong=float(parts[4][3:])
                
                decimallat = degreeslat + (minuteslat / 60) 
                decimallong = degreeslong + (minuteslong / 60)            
                print(decimallat, decimallong)
            
                #latitude.append(float(parts[2]))
                #longitude.append(float(parts[4]))
                latitude.append(decimallat)
                longitude.append(decimallong)  
                       
            elif parts[0].endswith('RMC') and len(parts) > 7:
                print(f"  -> Speed: {parts[7]} knots, Course: {parts[8]}°")

ser.close()


latitude_mean=np.mean(latitude)
longitude_mean=np.mean(longitude)

print("longitude mean", longitude_mean)
print("longitude sdev", np.std(longitude))

print("latitude mean", latitude_mean)
print("latitude sdev", np.std(latitude))

fig, (ax1, ax2) = plt.subplots(2)

latitude_nooffset=(latitude-latitude_mean)*1e5     
longitude_nooffset=(longitude-longitude_mean)*1e5

a=np.hstack(latitude_nooffset)
b=np.hstack(longitude_nooffset)

ax1.hist(a, bins='auto')
# ax1.ylabel('number of measurements')
# ax1.xlabel('latitude in TBD')
ax2.hist(b, bins='auto')
# ax2.ylabel('number of measurements')
# ax2.xlabel('longitude in TBD')
plt.show()