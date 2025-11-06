# Behavior Follower

A ZMK behavior that triggers the behavior mapped to a specific position on the current layer.

## Overview

> [!CAUTION]
> LLM-assisted README (overseen). 

The behavior follower allows you to trigger a behavior that is mapped to a different position on the same layer. This is useful for creating complex key combinations or for reusing existing key mappings in different contexts.

## How it Works

When the behavior follower is activated, it:
1. Determines the currently active layer
2. Looks up the behavior bound to the specified position on that layer
3. Triggers that behavior (both press and release events)
4. Returns `ZMK_BEHAVIOR_TRANSPARENT` to allow further processing

## Configuration

The behavior follower uses the `zmk,behavior-follower` compatible binding and requires two parameters:

- **param1**: The position index to follow (0-based)
- **param2**: Unused (reserved for future use)

### Usage

```dts
#include <behaviors/follower.dtsi>
```

## Example

In the endgame-trackball configuration, the behavior follower is used with rotary encoders:

```dts
#define DECLARE_ENCODERS sensor-bindings = <&rotenc_follower 10 8 &rotenc_follower 9 11>

behaviors {
    rotenc_follower: rotenc_follower {
        compatible = "zmk,behavior-sensor-rotate-var";
        #sensor-binding-cells = <2>;
        bindings = <&flw>, <&flw>;
    };
};
```

In this example:
- When the rotary encoder turns clockwise, it triggers the behavior at position 10
- When the rotary encoder turns counter-clockwise, it triggers the behavior at position 8
- The second encoder uses positions 9 and 11

## Keymap Integration

In your keymap, you can reference positions that will be followed:

```dts
default_layer {
    bindings = <
        // Position 0    Position 1
        &kp A           &kp B
        // Position 2    Position 3  
        &kp C           &kp D
        // Position 8    Position 9
        &kp E           &kp F
        // Position 10   Position 11
        &kp G           &kp H
    >;
};
```

With the encoder configuration above, turning the first encoder would trigger keys G and H, while the second encoder would trigger keys F and E.
