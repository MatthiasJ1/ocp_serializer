# Available at setup time due to pyproject.toml
from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup
import os

__version__ = "1.0.0"
description="Serialize OCCT objects for OCP"

# The main interface is through Pybind11Extension.
# * You can add cxx_std=11/14/17, and then build_ext can be removed.
# * You can set include_pybind11=false to add the include directory yourself,
#   say from a submodule.
#
# Note:
#   Sort input source files if you glob sources to ensure bit-for-bit
#   reproducible builds (https://github.com/pybind/python_example/pull/53)

ext_modules = [
    Pybind11Extension(
        "ocp_serializer",
        ["src/main.cpp"],
        define_macros=[
            ("VERSION_INFO", __version__),
            ("DESCRIPTION", description),
        ],
        include_dirs = [os.path.expandvars("$CONDA_PREFIX/include/opencascade")],
        libraries = ["TKBRep"],
    ),
]

setup(
    name="ocp_serializer",
    version=__version__,
    author="Matthias J",
    author_email="",
    url="https://github.com/MatthiasJ1/ocp_serializer",
    description=description,
    long_description="",
    ext_modules=ext_modules,
    extras_require={"test": "pytest"},
    # Currently, build_ext only provides an optional "highest supported C++
    # level" feature, but in the future it may provide more features.
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.7",
)
