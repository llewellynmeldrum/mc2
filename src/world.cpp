#include "world.hpp"
#include "util/cppslop.hpp"
#include "util/logger.hpp"
#include "stable_type_name.hpp"

World::World(){
    LOG_THIS_CTOR();
}
World::~World(){
    LOG_THIS_DTOR();
}
