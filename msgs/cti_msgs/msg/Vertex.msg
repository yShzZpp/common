uint32 id                   # Vertex id
bool valid                  
geometry_msgs/Point[] path  
uint32 weight               # the weight of the vertex (e.g. length of the segment)
float32 width               # fee space next to the vertex
uint32[] successors         # edges to successors 
uint32[] predecessors       # edges to predecessor