#ifndef __Primitives_H__
#define __Primitives_H__

// Renders an upgright (+z-axis) unit height cone with the given interior angle.
void cone(float angle);

// Renders an upright (+z-axis) cone with the base of the cone on the 
// xy plane, and it's rotational symmetry about the z-axis.
void cone(float radius, float height);

// Renders an OpenGL cube having a corner at the origin, and another corner at 
// (x, y, z). If centered = true, then that volume is translated so that its
// center is at the origin.
void cube(float x, float y, float z, bool centered);

#endif
