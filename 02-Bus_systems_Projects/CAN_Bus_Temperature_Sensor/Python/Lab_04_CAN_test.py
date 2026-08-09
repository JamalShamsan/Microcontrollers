# -*- coding: utf-8 -*-

# Created on Thu Mar 30 18:15:07 2023

#@author: Rasmus Rettig
# sample code for bus systems lab
# pip install canalystii

import canalystii
import time 

# Connect to the Canalyst-II device
# Passing a bitrate to the constructor causes both channels to be initialized and started.
dev = canalystii.CanalystDevice(bitrate=500000)

# Receive all pending messages on channel 0 / clear buffer
for msg in dev.receive(1):
     print(msg)

# sample code for continuous listening:
#while True:
#    msg=dev.receive(0)
#    if msg: print(msg)

# The canalystii.Message class is a ctypes Structure, to minimize overhead

while True:
   pl=(1 , 2, 3, 4) #load is created here 
   new_message = canalystii.Message(can_id=0XFE,
                                 remote=False,
                                 extended=False,
                                 data_len=4,
                                 data=pl)
     # Send one copy to channel 1
   dev.send(1, new_message)
   #time.sleep(0.1)

   for msg in dev.receive(0):
     print(msg)

# Stop both channels (need to call start() again to resume capturing or send any messages)
dev.stop(0)
dev.stop(1)

# delete dev at the end to free the interface
del dev

