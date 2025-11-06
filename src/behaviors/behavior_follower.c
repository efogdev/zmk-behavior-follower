#define DT_DRV_COMPAT zmk_behavior_follower

#include <zephyr/device.h>
#include <drivers/behavior.h>
#include <zephyr/logging/log.h>
#include <zmk/behavior.h>
#include <zmk/keymap.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static int on_follower_binding_pressed(struct zmk_behavior_binding *binding, struct zmk_behavior_binding_event event) {
    const zmk_keymap_layer_index_t layer_idx = zmk_keymap_highest_layer_active();
    const zmk_keymap_layer_id_t layer_id = zmk_keymap_layer_index_to_id(layer_idx);

    const struct zmk_behavior_binding *to_invoke = zmk_keymap_get_layer_binding_at_idx(layer_id, binding->param1);
    const struct zmk_behavior_binding_event new_event = {
        .layer = layer_id,
        .position = binding->param1,
        .timestamp = k_uptime_get(),
    #if IS_ENABLED(CONFIG_ZMK_SPLIT)
        .source = ZMK_POSITION_STATE_CHANGE_SOURCE_LOCAL,
    #endif
    };

    zmk_behavior_invoke_binding(to_invoke, new_event, true);
    zmk_behavior_invoke_binding(to_invoke, new_event, false);
    return ZMK_BEHAVIOR_TRANSPARENT;
}

static int on_follower_binding_released(struct zmk_behavior_binding *binding, struct zmk_behavior_binding_event event) {
    return ZMK_BEHAVIOR_TRANSPARENT;
}

static int behavior_follower_init(struct device *dev) {
    return 0;
}

static const struct behavior_driver_api behavior_follower_driver_api = {
    .binding_pressed = on_follower_binding_pressed,
    .binding_released = on_follower_binding_released,
};

#define AH_INST(n) BEHAVIOR_DT_INST_DEFINE(n, behavior_follower_init, NULL, NULL, \
       NULL, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &behavior_follower_driver_api);

DT_INST_FOREACH_STATUS_OKAY(AH_INST)
