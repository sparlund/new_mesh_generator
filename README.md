### new_mesh_generator

Given a point cloud, this program construct a Delaunay triangulation mesh between them. I.e., a bunch of triangles connecting the points.


## Example with 100 points
<img src="output.gif"/>

# TODO:
- [ ] Enable constrained edges between points, such that the shape could have holes in it
- [ ] Read a text file containing points


```
--------------------------------------------------
Language           Number of files   Lines of code
--------------------------------------------------
C++                              5             400
C++ Header                       5             115
CMake                            3              17
--------------------------------------------------
SUM:                            13             532
--------------------------------------------------
```

