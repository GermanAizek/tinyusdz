#pragma once

//
// pretty-print routine(using iostream) in non-intrusive way.
// Some build configuration may not want I/O module(e.g. mobile/embedded device), so provide print routines in separated file.
//
// TODO: Move `print` related code in other files to here.
//

#include <string>
#include <ostream>
#include <sstream>

#include "prim-types.hh"

namespace std {

inline std::ostream &operator<<(std::ostream &os, const tinyusdz::Vec2f &v) {
  os << "(" << v[0] << ", " << v[1] << ")";
  return os;
}

inline std::ostream &operator<<(std::ostream &os, const tinyusdz::Vec3f &v) {
  os << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")";
  return os;
}

inline std::ostream &operator<<(std::ostream &os, const tinyusdz::Vec4f &v) {
  os << "(" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ")";
  return os;
}

inline std::ostream &operator<<(std::ostream &os, const tinyusdz::Vec2d &v) {
  os << "(" << v[0] << ", " << v[1] << ")";
  return os;
}

inline std::ostream &operator<<(std::ostream &os, const tinyusdz::Vec3d &v) {
  os << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")";
  return os;
}

inline std::ostream &operator<<(std::ostream &os, const tinyusdz::Vec4d &v) {
  os << "(" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ")";
  return os;
}

inline std::ostream &operator<<(std::ostream &ofs, const tinyusdz::Matrix4f &m) {
  ofs << "( ";

  ofs << "(" << m.m[0][0] << ", " << m.m[0][1] << ", " << m.m[0][2] << ", " << m.m[0][3] << "), ";
  ofs << "(" << m.m[1][0] << ", " << m.m[1][1] << ", " << m.m[1][2] << ", " << m.m[1][3] << "), ";
  ofs << "(" << m.m[2][0] << ", " << m.m[2][1] << ", " << m.m[2][2] << ", " << m.m[2][3] << "), ";
  ofs << "(" << m.m[3][0] << ", " << m.m[3][1] << ", " << m.m[3][2] << ", " << m.m[3][3] << ")";

  ofs << " )";

  return ofs;
}

inline std::ostream &operator<<(std::ostream &ofs, const tinyusdz::Matrix4d &m) {
  ofs << "( ";

  ofs << "(" << m.m[0][0] << ", " << m.m[0][1] << ", " << m.m[0][2] << ", "
      << m.m[0][3] << "), ";
  ofs << "(" << m.m[1][0] << ", " << m.m[1][1] << ", " << m.m[1][2] << ", "
      << m.m[1][3] << "), ";
  ofs << "(" << m.m[2][0] << ", " << m.m[2][1] << ", " << m.m[2][2] << ", "
      << m.m[2][3] << "), ";
  ofs << "(" << m.m[3][0] << ", " << m.m[3][1] << ", " << m.m[3][2] << ", "
      << m.m[3][3] << ")";

  ofs << " )";

  return ofs;
}

template<typename T>
inline std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) {
  os << "[";
  for (size_t i = 0; i < v.size(); i++) {
    os << v[i];
    if (i != (v.size() -1)) {
      os << ", ";
    }
  }
  os << "]";
  return os;
}


} // namespace std

namespace tinyusdz {

std::string to_string(tinyusdz::Visibility v);
std::string to_string(tinyusdz::Orientation o);
std::string to_string(tinyusdz::Extent e);
std::string to_string(tinyusdz::Interpolation interp);

//

std::string to_string(const Klass &klass, uint32_t indent = 0);
std::string to_string(const GPrim &gprim, const uint32_t indent = 0);
std::string to_string(const Xform &xform, const uint32_t indent = 0);
std::string to_string(const GeomSphere &sphere, const uint32_t indent = 0);
std::string to_string(const GeomMesh &mesh, const uint32_t indent = 0);
std::string to_string(const GeomPoints &pts, const uint32_t indent = 0);
std::string to_string(const GeomBasisCurves &curves, const uint32_t indent = 0);
std::string to_string(const GeomCapsule &geom, const uint32_t indent = 0);
std::string to_string(const GeomCone &geom, const uint32_t indent = 0);
std::string to_string(const GeomCylinder &geom, const uint32_t indent = 0);
std::string to_string(const GeomCube &geom, const uint32_t indent = 0);

std::string to_string(const tinyusdz::AnimatableVisibility &v, const uint32_t );

} // namespace tinyusdz
