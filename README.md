# ASEF eye locator

[Xiaochao Yang](http://www.xiaochaoyang.com)

Rev. 2

Quite a few people are interesed in this project, but unfortunately the code in the repo was just a dump from an experimental project for research. I spent a few days rewriting it. Please let me know if you have any problem with it. The code is tested on Mac OS X and Ubuntu, should also work on other Linux distros. 


## About ASEF
ASEF stands for Average of Synthetic Exact Filters. With trained filters (./data/EyeLocatorASEF128x128.fel), ASEF can quickly locate both eyes in an image with a face present. I ported the python implementation from [D.S. Bolme's FaceL library](http://sourceforge.net/apps/mediawiki/pyvision/index.php?title=FaceL:_Facile_Face_Labeling "FaceL"). Please refer to his paper on the details of the algorithm:

[Average of Synthetic Exact Filters](http://www.cs.colostate.edu/~bolme/publications/Bolme2009Asef.pdf)

Here is the video demo of the same code running on an iPhone 4:

[http://www.youtube.com/watch?v=cQA_Do1mZ9E](http://www.youtube.com/watch?v=cQA_Do1mZ9E). 
(You need to make many changes to have it running on iOS, read the suggestions below)

For those who are only interested in the implementation of ASEF, please look at 

```c
	void asef_locate_eyes(AsefEyeLocator *asef)
```

in asef.c

__Caveat__: ASEF operates on the results of Haar face detection. ASEF is fast, but face detection is slow. Also face detection is not good at handling rotation (both in-plane and out-of-plane), so the ASEF has the same problem. 

## Suggestions to iOS/Mac/Android developers

I'm pretty sure Apple provides face detection, eye and mouth tracking functions (bought from Pola Rose) to desktop and mobile developers. Please refer to the SDK manual. I think you should use them instead of this as they're speed and accuracy optimized for the system, without the dependency to OpenCV. (Loading OpenCV static libraries on a iPhone can take 5-10 seconds!!)

Android provides face detection function on the video live feeds, but not eye and mouth detection. If you look up in the manual, their APIs actually have eye and mouth detections, but they're not implemented on the most of the syetems I've seen: Nexus S, Galaxy Nexus, Galaxy S3, at Android version 4.2. Let's hope Google can add them soon. 


## Prerequisites:

* System: Linux/Mac OS X. 
* Having gcc and related command line tools installed.
* Having OpenCV installed. 
* You need to check if you have pkg-config setup for opencv by executing "pkg-config opencv --cflags --libs", and make sure that the results make sense. 
* __Optional but strongly recommended__: Enable the TBB support in OpenCV. The face detection function in OpenCV is multi-threaded with TBB. It will take advantage of your multi-core processor. (Mac homebrew user, install with "brew instal opencv --with-tbb")

## To compile:
In a terminal, cd to the source code folder and run:

```bash
	make
```

## To run 

Make sure you have a webcam attached. In a terminal, cd to the source code folder and run:

```bash
	./camera_main -f ./data/haarcascade_frontalface_alt2.xml -e ./data/EyeLocatorASEF128x128.fel
```


## TODOs :

(in next few days)

* Add a seperate binary for single image demostration.
* Add byte swapping for endieness. 
* Add some comments