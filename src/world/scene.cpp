#include "scene.h"

using namespace kekw;

scene::scene(std::unique_ptr<camera> cam, std::unique_ptr<entity> root)
    : cam_(std::move(cam)), root_(std::move(root)) {}

scene::~scene() {}