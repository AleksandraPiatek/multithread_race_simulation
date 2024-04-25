# Race simulation

## General info


Race simulation is a multithread project. There are two tracks, horizontal and vertical. On the vertical track there are three vehicles, spawned in random position and moving with the same speed. In the begginig on horizontal track there are also three vehicles, but there are new vehicles entering the track after random delay. Each one has different random speed. Horizontal vehicles are leaving the track after 3 laps. <br />
Each vehicle is a separate thread. Horizontal vehicles threads are ending, after they leave the track. New threads are dynamically created by another, managing thread.

## Technologies
Project is created with:
* C++
* OpenGL
	
## Setup
