#include "spatial.h"

using namespace kekw::world;

spatial::spatial() : transform(), parent_(nullptr) {}

spatial* spatial::root() const {
    if (this->parent_) {
        return this->parent_->root();
    }

    return as_non_const(this);
}