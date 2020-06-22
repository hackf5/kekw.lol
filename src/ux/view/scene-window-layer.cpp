#include "scene-window-layer.h"

#include <unordered_map>

using namespace kekw::ux::view;

class initialize_context_impl : public kekw::initialize_context {
   public:
    initialize_context_impl(
        const kekw::window_context *window_ctx, kekw::world::scene *scene)
        : initialize_context(window_ctx, scene) {}

    virtual void register_service(
        const std::string &name, std::shared_ptr<void> service) override {
        this->_services.insert(std::make_pair(name, service));
    }

    std::shared_ptr<void> locate_service(const std::string &name) override {
        return (*this->_services.find(name)).second;
    }

   private:
    std::unordered_map<std::string, std::shared_ptr<void>> _services;
};

class update_context_impl : public kekw::update_context {
    friend scene_window_layer;

   public:
    update_context_impl(const kekw::window_context *window_ctx, kekw::world::scene *scene)
        : update_context(window_ctx, scene) {}

    kekw::world::vec3_ret_t get_mouse_ray() const override { return this->mouse_ray_; };

    void set_mouse_ray(kekw::world::vec3_param_t ray) override {
        this->mouse_ray_ = ray;
    };

    inline const update_context *previous_context() { return this->previous_context_; }

   private:
    update_context_impl *previous_context_;
    kekw::world::vec3 mouse_ray_;
};

void scene_window_layer::initialize(window_context *info) {
    initialize_context_impl ctx(info, this->scene_.get());

    this->scene_->on_initialize(&ctx);

    this->previous_context_ =
        std::make_unique<update_context_impl>(info, this->scene_.get());
}

void scene_window_layer::update(window_context *info) {
    auto ctx = std::make_unique<update_context_impl>(info, this->scene_.get());
    ctx->previous_context_ =
        dynamic_cast<update_context_impl *>(this->previous_context_.get());

    this->scene_->on_update(ctx.get());

    // prevent an infinite chain of contexts.
    ctx->previous_context_ = nullptr;
    this->previous_context_ = std::move(ctx);
}

void scene_window_layer::render(window_context *info) {
    render_context ctx(info, this->scene_.get());
    this->scene_->on_render(&ctx);
}