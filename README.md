Two project objectives: **Learn basic graphics** and **gain optimization experience**

So firstly I learnt openGL and made my program capable of rendering bouncing spheres inside a transparent box using basic lighting. (Also integrated imGUI). See version #1

The second objective is to optimize the program. I intentionally used naive phsysics (sphere-sphere collisions) and naive rendering (no instancing) at first so I could see the effect. I also chose a simple ball collision program so I can focus on technique rather than scenario specific optimisation.

**Version #1** (naive version). 1000 balls at average 18 FPS. Predictably 80% of time spent updating collisions.

**Version #2** (spatial partitioning). 5000 balls at average 19.6 FPS. Ovbiously improved the updating speed, now only 34% of total.
Still loads of room for improvement with SOA's, maybe compute shaders but would like to learn instancing first. Slo-mo'd the sim and physics still works 

