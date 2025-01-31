#include "usda-writer.hh"
#include "pprinter.hh"

#include <fstream>
#include <iostream>
#include <sstream>

namespace tinyusdz {
namespace usda {

namespace {

// TODO: Use ryu print for precise floating point output?

#if 0
std::ostream &operator<<(std::ostream &ofs, const Matrix4f &m) {
  ofs << "( ";

  ofs << "(" << m.m[0][0] << ", " << m.m[0][1] << ", " << m.m[0][2] << ", " << m.m[0][3] << "), ";
  ofs << "(" << m.m[1][0] << ", " << m.m[1][1] << ", " << m.m[1][2] << ", " << m.m[1][3] << "), ";
  ofs << "(" << m.m[2][0] << ", " << m.m[2][1] << ", " << m.m[2][2] << ", " << m.m[2][3] << "), ";
  ofs << "(" << m.m[3][0] << ", " << m.m[3][1] << ", " << m.m[3][2] << ", " << m.m[3][3] << ")";

  ofs << " )";

  return ofs;
}

std::ostream &operator<<(std::ostream &ofs, const Matrix4d &m) {
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

std::ostream &operator<<(std::ostream &os, XformOpValueType const &v) {
  switch (v.index()) {
    case 0:
      os << nonstd::get<0>(v);
      break;
    case 1:
      os << nonstd::get<1>(v);
      break;
    case 2:
      os << nonstd::get<2>(v);
      break;
    case 3:
      os << nonstd::get<3>(v);
      break;
    case 4:
      os << nonstd::get<4>(v);
      break;
    case 5:
      os << nonstd::get<5>(v);
      break;
    case 6:
      os << nonstd::get<6>(v);
      break;
    case 7:
      os << nonstd::get<7>(v);
      break;
    default:
      os << "XformOpValueType:???";
  }

  return os;
}
#endif

inline std::string GetTypeName(XformOpValueType const &v) {
  if (auto pval = nonstd::get_if<float>(&v)) {
    (void)pval;
    return "float";
  }

  if (auto pval = nonstd::get_if<double>(&v)) {
    (void)pval;
    return "double";
  }

  if (auto pval = nonstd::get_if<Vec3f>(&v)) {
    (void)pval;
    return "float3";
  }
  if (auto pval = nonstd::get_if<Vec3d>(&v)) {
    (void)pval;
    return "double3";
  }
  if (auto pval = nonstd::get_if<Matrix4d>(&v)) {
    (void)pval;
    return "matrix4d";
  }
  if (auto pval = nonstd::get_if<Quatf>(&v)) {
    (void)pval;
    return "quatf";
  }
  if (auto pval = nonstd::get_if<Quatd>(&v)) {
    (void)pval;
    return "quatd";
  }

  return "TypeName(XformOpValueType) = ???";
}

std::string PrintIntArray(const std::vector<int32_t> &data) {
  std::stringstream ofs;

  ofs << "[";
  for (size_t i = 0; i < data.size(); i++) {
    ofs << data[i];

    if (i != (data.size() - 1)) {
      ofs << ", ";
    }
  }
  ofs << "]";

  return ofs.str();
}

#if 0
std::string PrintVec3fArray(const std::vector<Vec3f> &data)
{
  std::stringstream ofs;

  ofs << "[";
  for (size_t i = 0; i < data.size(); i++) {

    ofs << "(" << data[i][0] << ", " << data[i][1] << ", " << data[i][2] << ")";

    if (i != (data.size() - 1)) {
      ofs << ", ";
    }
  }
  ofs << "]";

  return ofs.str();

}
#endif

std::string PrintVec3fArray(const std::vector<Vec3f> &data) {
  std::stringstream ofs;

  ofs << "[";
  // TODO: Use ryu print?
  for (size_t i = 0; i < data.size(); i++) {
    ofs << "(" << data[i][0] << ", " << data[i][1] << ", "
        << data[i][2] << ")";

    if (i != (data.size() - 1)) {
      ofs << ", ";
    }
  }
  ofs << "]";

  return ofs.str();
}


#if 0
std::string PrintAsVec3fArray(const std::vector<float> &data) {
  std::stringstream ofs;

  if ((data.size() % 3) != 0) {
    throw std::runtime_error("data is not a valid array of float3");
  }

  ofs << "[";
  // TODO: Use ryu print?
  for (size_t i = 0; i < data.size() / 3; i++) {
    ofs << "(" << data[3 * i + 0] << ", " << data[3 * i + 1] << ", "
        << data[3 * i + 2] << ")";

    if (i != ((data.size() / 3) - 1)) {
      ofs << ", ";
    }
  }
  ofs << "]";

  return ofs.str();
}
#endif

class Writer {
 public:
  Writer(const Scene &scene) : _scene(scene) {}

  std::string Indent(size_t level) {
    std::stringstream ss;
    for (size_t i = 0; i < level; i++) {
      ss << "  ";
    }

    return ss.str();
  }

  bool WriteGeomMesh(std::ostream &ofs, const GeomMesh &mesh, size_t level) {
    std::cout << "Writing GeomMesh: " << mesh.name << " ...\n";

    ofs << Indent(level) << "\n";
    ofs << Indent(level) << "def GeomMesh \"" << mesh.name << "\"\n";
    ofs << Indent(level) << "{\n";

    // params
    ofs << Indent(level + 1)
        << "int[] faceVertexCounts = " << PrintIntArray(mesh.faceVertexCounts)
        << "\n";
    ofs << Indent(level + 1)
        << "int[] faceVertexIndices = " << PrintIntArray(mesh.faceVertexIndices)
        << "\n";
    ofs << Indent(level + 1)
        << "point3f[] points = " << PrintVec3fArray(mesh.points) << "\n";

    if (auto p = primvar::as_vector<Vec3f>(&mesh.normals.var)) {
      std::vector<Vec3f> normals = (*p);

      if (normals.size()) {
        ofs << Indent(level + 1)
            << "normal3f[] normals = " << PrintVec3fArray(normals);

        if (mesh.normals.interpolation != InterpolationInvalid) {
          ofs << Indent(level + 2) << "(\n";
          ofs << Indent(level + 3) << "interpolation = \"" << to_string(mesh.normals.interpolation) << "\"\n";
          ofs << Indent(level + 2) << ")\n";
        } else {
          ofs << "\n";
        }
      }
    }

    // primvars

    // uniforms
    // TODO
    ofs << Indent(level + 1) << "uniform token subdivisionScheme = \"none\"\n";

    return true;
  }

  bool WriteXform(std::ostream &ofs, const Xform &xform, size_t level) {
    std::cout << "Writing Xform: " << xform.name << " ...\n";

    ofs << Indent(level) << "\n";
    ofs << Indent(level) << "def Xform \"" << xform.name << "\"\n";
    ofs << Indent(level) << "{\n";

    if (xform.xformOps.size()) {
      // xformOpOrder
      ofs << Indent(level + 1) << "uniform token[] xformOpOrder = [";

      for (size_t i = 0; i < xform.xformOps.size(); i++) {
        ofs << "\"" << XformOp::GetOpTypeName(xform.xformOps[i].op) << "\"";

        if (i != (xform.xformOps.size() - 1)) {
          ofs << ", ";
        }
      }

      ofs << "]\n";

      for (size_t i = 0; i < xform.xformOps.size(); i++) {
        ofs << Indent(level + 1);

        ofs << GetTypeName(xform.xformOps[i].value);

        ofs << " " << XformOp::GetOpTypeName(xform.xformOps[i].op) << " = ";

#if 0 // TODO
        nonstd::visit([&ofs](XformOpValueType &&arg) { ofs << arg; },
                      xform.xformOps[i].value);
#endif
        ofs << "\n";
      }
    }

    return true;
  }

  bool WriteNode(std::ostream &ofs, const Node &node, size_t level) {
    if (node.type == NODE_TYPE_XFORM) {
      if ((node.index < 0) || (size_t(node.index) >= _scene.xforms.size())) {
        // invalid index
        return false;
      }

      if (!WriteXform(ofs, _scene.xforms.at(size_t(node.index)), level)) {
        return false;
      }

    } else if (node.type == NODE_TYPE_GEOM_MESH) {
      if ((node.index < 0) ||
          (size_t(node.index) >= _scene.geom_meshes.size())) {
        // invalid index
        return false;
      }

      if (!WriteGeomMesh(ofs, _scene.geom_meshes.at(size_t(node.index)),
                         level)) {
        return false;
      }

    } else {
      // unsupported node.
      _err += "Unsupported node type.\n";
      return false;
    }

    for (const auto &child : node.children) {
      if (!WriteNode(ofs, child, level + 1)) {
        return false;
      }
    }

    ofs << Indent(level) << "}\n";

    return true;
  }

  const Scene &_scene;

  const std::string &Error() const { return _err; }

 private:
  Writer() = delete;
  Writer(const Writer &) = delete;

  std::string _err;
};

}  // namespace

bool SaveAsUSDA(const std::string &filename, const Scene &scene,
                std::string *warn, std::string *err) {
  (void)warn;

  std::stringstream ss;

  ss << "#usda 1.0\n";
  ss << "(\n";
  ss << "  doc = \"TinyUSDZ v" << tinyusdz::version_major << "."
     << tinyusdz::version_minor << "." << tinyusdz::version_micro << "\"\n";
  ss << "  metersPerUnit = " << scene.metersPerUnit << "\n";
  ss << "  upAxis = \"" << scene.upAxis << "\"\n";
  ss << "  timeCodesPerSecond = \"" << scene.timeCodesPerSecond << "\"\n";
  ss << ")\n";

  // TODO
  Writer writer(scene);

  std::cout << "# of nodes: " << scene.nodes.size() << "\n";

  for (const auto &root : scene.nodes) {
    if (!writer.WriteNode(ss, root, 0)) {
      if (err && writer.Error().size()) {
        (*err) += writer.Error();
      }

      return false;
    }
  }

  std::ofstream ofs(filename);
  if (!ofs) {
    if (err) {
      (*err) += "Failed to open file [" + filename + "] to write.\n";
    }
    return false;
  }

  ofs << ss.str();

  std::cout << "Wrote to [" << filename << "]\n";

  return true;
}

} // namespace usda
}  // namespace tinyusdz
