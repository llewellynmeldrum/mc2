#include "profiler.hpp"
#include "util/cppslop.hpp"
#include "util/logger.hpp"
#include "stable_type_name.hpp"

Profiler::Profiler(){
    LOG_THIS_CTOR();
}
Profiler::~Profiler(){
    LOG_THIS_DTOR();
}
