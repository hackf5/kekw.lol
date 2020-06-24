#include "scene-window-layer.h"

#include <limits>
#include <unordered_map>

using namespace kekw;

class initialize_context_impl : public initialize_context {
   public:
    initialize_context_impl(const kekw::window_context *window_ctx, kekw::scene *scene)
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

class update_context_impl : public update_context {
    friend scene_window_layer;

   public:
    update_context_impl(const kekw::window_context *window_ctx, kekw::scene *scene)
        : update_context(window_ctx, scene),
          hit_distance_(std::numeric_limits<real_t>::max()),
          hit_id_(false) {}

    inline vec3_ret_t get_mouse_ray() const override { return this->mouse_ray_; };

    inline void set_mouse_ray(vec3_param_t ray) override { this->mouse_ray_ = ray; };

    void register_hit(unsigned long id, real_t distance) override {
        if (distance < this->hit_distance_) {
            this->hit_distance_ = distance;
            this->hit_id_ = id;
        }
    }

    unsigned long get_hit_id() const override { return this->hit_id_; }

   private:
    vec3 mouse_ray_;
    real_t hit_distance_;
    unsigned long hit_id_;
};

class render_context_impl : public render_context {
   public:
    render_context_impl(
        const window_context *window_ctx, kekw::scene *scene, update_context *update_ctx)
        : render_context(window_ctx, scene), update_ctx_(update_ctx) {}

    const update_context *update_ctx() override { return this->update_ctx_; };

   private:
    update_context *update_ctx_;
};

void scene_window_layer::initialize(window_context *context) {
    initialize_context_impl ctx(context, this->scene_.get());

    this->scene_->on_initialize(&ctx);

    this->last_update_ctx =
        std::make_unique<update_context_impl>(context, this->scene_.get());
}

void scene_window_layer::update(window_context *context) {
    auto ctx = std::make_unique<update_context_impl>(context, this->scene_.get());
    this->scene_->on_update(ctx.get());
    this->last_update_ctx = std::move(ctx);
}

void scene_window_layer::render(window_context *context) {
    render_context_impl ctx(context, this->scene_.get(), this->last_update_ctx.get());
    this->scene_->on_render(&ctx);
}