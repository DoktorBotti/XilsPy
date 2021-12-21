#include <pybind11/pybind11.h>
#include "Acq.h"
#include "iostream"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

void hello_world() {
    std::cout << "hello world" << std::endl;
}

namespace py = pybind11;

PYBIND11_MODULE(xisl_binding, m) {
    m.doc() = R"pbdoc(
        Pybind11 plugin for XISL
        -----------------------

        .. currentmodule:: xisl

        .. autosummary::
           :toctree: _generate

           hello_world
           Acquisition_EnumSensors
    )pbdoc";

    m.def("hello_world", &hello_world, R"pbdoc(
        prints out "hello world" to stdout

        Here are some other explanations. Most interestingly: the print should hopefully be triggered automatically, even if std::cout was used.
    )pbdoc");

    m.def("Acquisition_EnumSensors",
          [](unsigned int &num_sensors, bool bEnableIRQ, bool bAlwaysOpen) {
              unsigned int sensor;
              auto ret = Acquisition_EnumSensors(&sensor, bEnableIRQ, bAlwaysOpen);
              num_sensors = sensor;
              return ret;
          }, R"pbdoc(
        Calls simple library function

        Some other explanation about the subtract function.
    )pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
