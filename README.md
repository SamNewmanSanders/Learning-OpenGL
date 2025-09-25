In this project I learnt graphics fundamentals and openGL. My program rendered bouncing balls inside a transparent cube with basic lighting and camera control. imGUI was also used. (Version #1)
My next objective was then to optimize the program. The bouncing ball sim is good for this as it is a simple idea so I can focus on optimization that isn't only applicable to this program.

**Version 1** using naive ball-ball collision and rendering: **1000 balls at 18FPS**. 80% of time spend on the collision update loops - O(N^2)

**Version 2** implements spatial partitioning: **5000 balls at 19.6FPS**. 25x improvement, now 34% spend on update loop

**Version 3** implements instancing. **5000 balls at 36.8FPS**. 43% spent rendering. Expected a greater performance improvement but rendering performance shouldn't be influenced by the particle number much now.
Also a bug where I can see individual triangles of the box but I will ignore for now. Video at https://drive.google.com/file/d/1AadGuN_76DQubrQ6pWYZwHZHRWBp5F61/view?usp=drive_link, had to slow sim speed otherwise video compression would ruin it.

Future optimisation ideas:
  - Avoid vectors of objects which are bad for cache friendliness. Use arrays instead for variables like position, velocity (can go through these much quicker). AOS vs SOA
  - Compute shader if implemented right would massively speed up update loop as its all parrallel computations. Be careful of double counting to maintain energy
  - Improve hash function (use single 64 bit int)
