#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Apr 23 11:13:12 2026

@author: rasmus
"""

import serial

port = "COM26"   # Change this to your port
baudrate = 9600
filename ='GNSS.txt'
    
try:
    ser = serial.Serial(port, baudrate)
except:         
    print("something is wrong)")

try:
    with open(filename, "a") as file: 
        while True:
                line = ser.readline().decode('ascii', errors='replace').strip()
                
                if line.startswith('$'):
                    print(f"{line}")
                    file.write(f"{line}\n")
                    file.flush()
                    
                    # Basic parsing
                    # parts = line.split(',')
                    # if parts[0].endswith('GGA') and len(parts) > 9:
                    #    print(f"  -> Lat: {parts[2]} {parts[3]}, Lon: {parts[4]} {parts[5]}, Alt: {parts[9]}m")
                    # elif parts[0].endswith('RMC') and len(parts) > 7:
                    #     print(f"  -> Speed: {parts[7]} knots, Course: {parts[8]}°")
        
except KeyboardInterrupt:
    print("\n\nStopped by user")
finally:
    ser.close()
    

    
    
    
    
    
    
    
    
    
