#include "scene-window-layer.h"

#include <src/world/plane.h>

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
          hit_map_(),
          drag_plane_({0.f, 0.f, -8.f}, {1.f, 0.f, -8.f}, {0.f, 1.f, -8.f}) {}

    inline vec3_ret_t get_mouse_ray() const override { return this->mouse_ray_; };

    inline void set_mouse_ray(vec3_param_t ray) override { this->mouse_ray_ = ray; };

    void register_hit(
        const std::string &category, entity_id_t id, real_t distance) override {
        auto it = this->hit_map_.find(category);
        if (it == this->hit_map_.end()) {
            this->hit_map_.insert(std::make_pair(category, hit_data(id, distance)));
        } else if (distance < it->second.distance_) {
            it->second.id_ = id;
            it->second.distance_ = distance;
        }
    }

    inline entity_id_t get_hit_id(const std::string &category) const override {
        auto it = this->hit_map_.find(category);
        return it != this->hit_map_.end() ? it->second.id_ : 0;
    }

    inline vec3 get_drag_plane_intercept() const override {
        real_t distance;
        this->drag_plane_.hit_test(
            this->scene()->cam()->position(), this->get_mouse_ray(), distance);
        auto drag_plane_intercept =
            this->scene()->cam()->position() + distance * this->get_mouse_ray();
        return drag_plane_intercept;
    }

   private:
    struct hit_data {
        hit_data(entity_id_t id, real_t distance) : id_(id), distance_(distance) {}
        entity_id_t id_ = 0;
        real_t distance_ = std::numeric_limits<real_t>::max();
    };

    vec3 mouse_ray_;
    std::unordered_map<std::string, hit_data> hit_map_;
    plane drag_plane_;
};

class render_context_impl : public render_context {
   public:
    render_context_impl(
        const window_context *window_ctx, kekw::scene *scene, update_context *update_ctx)
        : render_context(window_ctx, scene), update_ctx_(update_ctx), pass_(0) {}

    inline const update_context *update_ctx() override { return this->update_ctx_; };

    inline void next_pass() { ++this->pass_; }

    inline unsigned int pass() const override { return this->pass_; };

   private:
    update_context *update_ctx_;
    unsigned int pass_;
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
    this->scene_->on_late_update(ctx.get());
    this->last_update_ctx = std::move(ctx);
}

void scene_window_layer::render(window_context *context) {
    render_context_impl ctx(context, this->scene_.get(), this->last_update_ctx.get());

    this->scene_->on_render(&ctx);
    ctx.next_pass();
    this->scene_->on_render(&ctx);
}