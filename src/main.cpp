#include <pybind11/pybind11.h>
#include <sstream>
#include <vector>

#include <TopoDS_Shape.hxx>
#include <TopLoc_Location.hxx>

#include <BinTools.hxx>
#include <BinTools_OStream.hxx>
#include <BinTools_IStream.hxx>
#include <BinTools_ShapeReader.hxx>
#include <BinTools_ShapeWriter.hxx>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

py::bytes serialize_shape(const TopoDS_Shape &shape) {
    std::ostringstream buf;
    BinTools::Write(shape, buf);
    return py::bytes(std::move(buf.str()));
}

TopoDS_Shape deserialize_shape(const py::bytes &buf) {
    std::istringstream stream(buf);
    TopoDS_Shape shape;
    BinTools::Read(shape, stream);
    return shape;
}

py::bytes serialize_location(const TopLoc_Location &location) {
    std::ostringstream buf;
    BinTools_OStream occtStream(buf);
    BinTools_ShapeWriter().WriteLocation(occtStream, location);
    return py::bytes(std::move(buf.str()));
}

TopLoc_Location deserialize_location(const py::bytes &buf) {
    std::istringstream stream(buf);
    BinTools_IStream occtStream(stream);
    // This is not a memory leak and can only be copied due to weird occt impl
    return *BinTools_ShapeReader().ReadLocation(occtStream);
}

std::string _test() {
    return "Ok";
}

std::string _testOCCT() {
    TopoDS_Shape shape;
    return "Ok";
}

PYBIND11_MODULE(ocp_serializer, m) {

    m.def("serialize_shape", &serialize_shape);
    m.def("deserialize_shape", &deserialize_shape);
    m.def("serialize_location", &serialize_location);
    m.def("deserialize_location", &deserialize_location);

    m.def("_test", &_test);
    m.def("_testOCCT", &_testOCCT);

    m.doc() = R"pbdoc(
        OCP Serializer
        -----------------------
        )pbdoc" MACRO_STRINGIFY(DESCRIPTION);
#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
