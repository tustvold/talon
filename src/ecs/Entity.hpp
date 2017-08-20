#pragma once
#include "Talon.hpp"
#include <boost/hana.hpp>

using namespace boost::hana::literals;

TALON_NS_BEGIN

using EntityID = size_t;
static const EntityID MaxEntityID = 1024;

TALON_NS_END