#include "spatial.h"

using namespace kekw::world;

spatial* spatial::root() {
    if (this->parent_) {
        return this->parent_->root();
    }

    return this;
}

inline spatial* spatial::parent() { return this->parent_; }