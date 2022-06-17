add_executable(new_mesh_generator
               main.cpp
               Edge.cpp
               Mesh.cpp
               Point.cpp
               Triangle.cpp
)
target_compile_options(new_mesh_generator PRIVATE -g)
