#include "profiler.hpp"
#include "cppslop.hpp"
#include "logger.hpp"
#include "stable_type_name.hpp"

Profiler::Profiler(){
    LOG_THIS_CTOR();
}
Profiler::~Profiler(){
    LOG_THIS_DTOR();
}
