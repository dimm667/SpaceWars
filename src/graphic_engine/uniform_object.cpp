#include "uniform_object.h"

int UniformObjectMonitor::current_binding_point_ = 0;

UniformObjectMonitor::UniformObjectMonitor()
{
    own_binding_point_ = current_binding_point_;
    current_binding_point_++;
}

UniformObjectMonitor::~UniformObjectMonitor()
{
}
