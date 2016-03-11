========================================================================
# OpenCV 3.1 - Object detection and tracking Project Overview
CONSOLE APPLICATION : PSU - CS410 Project 2

Moving Pedestrian and Vehicle Detection and Tracking

Main goal was to:
* Implement moving pedestrain and vehicle detection.
* Implement moving pedestrain and vehicle tracking.

This code implements HAAR recognition for both cars and people. For every frame in a video/camera, when something is detected, we start tracking the object (Media Flow algorithm). 

Problem is that we never stop tracking the object, even if its they are not in the camera anymore. Needs improvement.

Other notes:

* Some test files might be found in 'misc' directory
* Problem is not 100% solved. Might have bugs!
