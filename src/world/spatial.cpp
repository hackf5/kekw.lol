#include "spatial.h"

using namespace kekw::world;

spatial::spatial() : transform() {}

spatial* spatial::root() const {
    if (this->parent_) {
        return this->parent_->root();
    }

    return as_non_const(this);
}

inline spatial* spatial::parent() const { return this->parent_; }

inline void spatial::set_parent(spatial* parent) {
    // todo: should ensure no circ refs.
    this->parent_ = parent;
}