# ASEF eye locator

Xiaochao (yxccumt@gmail.com)

Rev. 2

Just rewrote the code. More updates coming in the next few days.  Quite a few people are interesed in this project, but unfortunately the code in the repo was just a dump from my experimental test for an academic project.  I spend a few days on rewriting it (as a hobby). Please let me know if you have any problem with it. (I tested the code on Mac OS X Moutain Lion, but it should work on different Linux distros) 

## Prerequisite:

* System: Linux/Mac OS X. 
* Having OpenCV installed. 
* You need to check if you have pkg-config setup for opencv by executing "pkg-config opencv --cflags --libs", and make sure that the results make sense. 

## To compile:

	make

## To run 

(make sure you have a webcam attached):

	./asef_camera

## TODOs :

(in next few days)

1. Add command line parsing options. ( ...file names are hard-coded now... )
2. Add a seperate binary for single image demostration.
3. Add byte swapping for endieness. 
4. Add some comments, clean a little bit more.
5. Documents and citing.

