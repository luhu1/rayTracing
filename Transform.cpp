// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.
mat3 Transform::rotate(const float degrees, const vec3& axis)
{
  // YOUR CODE FOR HW2 HERE
  // Please implement this.  Likely the same as in HW 1.
  float x = axis.x;
  float y = axis.y;
  float z = axis.z;

  mat3 A = mat3(
      0.f, -z, y,
      z, 0.f, -x,
      -y, x, 0.f
  );
  A = transpose(A);

  mat3 B = mat3(
      x*x, x*y, x*z,
      x*y, y*y, y*z,
      x*z, y*z, z*z
  );
  B = transpose(B);

  mat3 R = std::cos(degrees) * mat3(1.0f) + (1 - std::cos(degrees)) * B + std::sin(degrees) * A;
  return R;
}

void Transform::left(float degrees, vec3& eye, vec3& up)
{
  // YOUR CODE FOR HW2 HERE
  // Likely the same as in HW 1.
  degrees = glm::radians(degrees);
  vec3 axis = normalize(up);

  mat3 R = Transform::rotate(degrees, axis);
  eye = R * eye;
  up = R * up;
}

void Transform::up(float degrees, vec3& eye, vec3& up)
{
  // YOUR CODE FOR HW2 HERE
  // Likely the same as in HW 1.
  degrees = glm::radians(degrees);

  vec3 axis = normalize(cross(eye, up));
  mat3 R = Transform::rotate(degrees, axis);
  up = R * up;
  eye = R * eye;
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up)
{
  // YOUR CODE FOR HW2 HERE
  // Likely the same as in HW 1.
  vec3 w = normalize(eye);
  vec3 u = normalize(cross(up, w));
  vec3 v = cross(w, u);

  mat4 M = mat4(
      u.x, u.y, u.z, -dot(u, eye),
      v.x, v.y, v.z, -dot(v, eye),
      w.x, w.y, w.z, -dot(w, eye),
      0, 0, 0, 1
  );

  // You will change this return call
  return transpose(M);
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
  mat4 ret;
  // YOUR CODE FOR HW2 HERE
  // New, to implement the perspective transform as well.
  fovy = glm::radians(fovy);
  float d = 1 / std::tan(fovy/2);
  float A = - (zFar + zNear) / (zFar - zNear);
  float B = - (2 * zFar * zNear) / (zFar - zNear);

  ret = mat4(
      d/aspect, 0, 0, 0,
      0, d, 0, 0,
      0, 0, A, B,
      0, 0, -1, 0
  );
  return transpose(ret);
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz)
{
  mat4 ret;
  // YOUR CODE FOR HW2 HERE
  // Implement scaling
  ret = mat4(
      sx, 0 , 0, 0,
      0, sy, 0, 0,
      0, 0, sz, 0,
      0, 0, 0, 1
  );
  return transpose(ret);
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz)
{
  mat4 ret;
  // YOUR CODE FOR HW2 HERE
  // Implement translation
  ret = mat4(
      1, 0, 0, tx,
      0, 1, 0, ty,
      0, 0, 1, tz,
      0, 0, 0, 1
  );
  return transpose(ret);
}

// To normalize the up direction and construct a coordinate frame.
// As discussed in the lecture.  May be relevant to create a properly
// orthogonal and normalized up.
// This function is provided as a helper, in case you want to use it.
// Using this function (in readfile.cpp or display.cpp) is optional.

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec)
{
  vec3 x = glm::cross(up,zvec);
  vec3 y = glm::cross(zvec,x);
  vec3 ret = glm::normalize(y);
  return ret;
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
