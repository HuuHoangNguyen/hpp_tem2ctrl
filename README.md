# hpp_tem2ctrl
This is a tool to control Winway's Thermal Controller


----------------------------------------------------------------------
Help :
  Case 1: Set temperature to thermal controller

       # ./hpp_tem2ctrl ttyUSB<n> <tempeature> [w/W]

  Case 2: Read temperature from thermal controller 

       # ./hpp_tem2ctrl ttyUSB<n>  [r]

  Case 3: Display to converted a/some temperature to hex value

       # ./hpp_tem2ctrl <tempeature> [num-temp]

       ttyUSB<n>     : Serial port, example: ttyUSB0, ttyUSB1, ttyUSB2 ...
       <temperature> : Temperature [-30 C -> 110 C]
       [w]           : Write temperature to serial port
                     : We can not need pressed the argument

       [r]           : Read temperature from serial port
                     : We can not need pressed the argument

       [num-temp]    : Number of temperature need converted to hex value
                     : If not press the argument, then default is 1

----------------------------------------------------------------------
 Author : HuuHoang Nguyen (hoangnh.hpp@gmail.com, hhnguyen@apm.com)
 Version: 2.00
 Date   : Jan 05, 2016
 License: MIT 
 Copyright 2016 HPPTECH 
----------------------------------------------------------------------
