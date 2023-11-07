#include <Arduino.h>
#include <Deneyap_Servo.h>

#include <oracle_puppets.h>
#include <oracle_utils.h>

static oracle_puppets_context_t oracle_puppets_context;

static const int ORACLE_PUPPETS_PUPPET_init_angle[ORACLE_PUPPETS_NUM_PUPPETS] = {
    ORACLE_PUPPETS_PUPPET1_INIT_ANGLE,
    ORACLE_PUPPETS_PUPPET2_INIT_ANGLE,
    ORACLE_PUPPETS_PUPPET3_INIT_ANGLE,
    ORACLE_PUPPETS_PUPPET4_INIT_ANGLE,
    ORACLE_PUPPETS_PUPPET5_INIT_ANGLE,
    ORACLE_PUPPETS_PUPPET6_INIT_ANGLE,
    ORACLE_PUPPETS_PUPPET7_INIT_ANGLE,
    ORACLE_PUPPETS_PUPPET8_INIT_ANGLE
};

static const int ORACLE_PUPPETS_PUPPET_min_angle[ORACLE_PUPPETS_NUM_PUPPETS] = {
    ORACLE_PUPPETS_PUPPET1_MIN_ANGLE,
    ORACLE_PUPPETS_PUPPET2_MIN_ANGLE,
    ORACLE_PUPPETS_PUPPET3_MIN_ANGLE,
    ORACLE_PUPPETS_PUPPET4_MIN_ANGLE,
    ORACLE_PUPPETS_PUPPET5_MIN_ANGLE,
    ORACLE_PUPPETS_PUPPET6_MIN_ANGLE,
    ORACLE_PUPPETS_PUPPET7_MIN_ANGLE,
    ORACLE_PUPPETS_PUPPET8_MIN_ANGLE
};

static const int ORACLE_PUPPETS_PUPPET_max_angle[ORACLE_PUPPETS_NUM_PUPPETS] = {
    ORACLE_PUPPETS_PUPPET1_MAX_ANGLE,
    ORACLE_PUPPETS_PUPPET2_MAX_ANGLE,
    ORACLE_PUPPETS_PUPPET3_MAX_ANGLE,
    ORACLE_PUPPETS_PUPPET4_MAX_ANGLE,
    ORACLE_PUPPETS_PUPPET5_MAX_ANGLE,
    ORACLE_PUPPETS_PUPPET6_MAX_ANGLE,
    ORACLE_PUPPETS_PUPPET7_MAX_ANGLE,
    ORACLE_PUPPETS_PUPPET8_MAX_ANGLE
};

static const int ORACLE_PUPPETS_PUPPET_gpio_pin[ORACLE_PUPPETS_NUM_PUPPETS] = {
    ORACLE_PUPPETS_PUPPET1_GPIO_PIN,
    ORACLE_PUPPETS_PUPPET2_GPIO_PIN,
    ORACLE_PUPPETS_PUPPET3_GPIO_PIN,
    ORACLE_PUPPETS_PUPPET4_GPIO_PIN,
    ORACLE_PUPPETS_PUPPET5_GPIO_PIN,
    ORACLE_PUPPETS_PUPPET6_GPIO_PIN,
    ORACLE_PUPPETS_PUPPET7_GPIO_PIN,
    ORACLE_PUPPETS_PUPPET8_GPIO_PIN
};

bool oracle_puppets_move_servo(const uint8_t index, const uint8_t angle)
{
    oracle_puppets_context_t *ctx = &oracle_puppets_context;
    oracle_puppets_puppet_context_t *puppet = &(ctx->puppets[i]);

    if (UINT8_MAX == angle) {
        return false;
    }

    puppet->servos.motor.write(angle);
    return true;
}

void oracle_puppets_update_servo(oracle_puppets_puppet_context_t *puppet)
{
    char *ptr;
    uint16_t frame_index;
    uint8_t angle, next_batch, running_index;

    if (!puppet->servos.enable) {
        return;
    }

    running_index = puppet->batchs.running_index;
    if (!puppet->batchs.frames[running_index].ready) {
        continue;
    }

    ptr = puppet->batchs.frames.next_angle;
    angle = oracle_utils_read_uint16(ptr);
    frame_index = puppet->batchs.frame_index + 2;

    if (oracle_puppets_move_servo(puppet->index, angle) &&
        ORACLE_PUPPETS_NUM_FRAMES > frame_index) {
        return;
    }

    next_batch = (running_index + 1) % ORACLE_PUPPETS_NUM_BATCHS;
    puppet->batchs.frames[running_index].ready = false;
    ptr = puppet->batchs.frames[next_batch].buffer;
    puppet->batchs.frames.next_angle = ptr;
}


void oracle_puppets_loop(void)
{
    oracle_puppets_context_t *ctx = &oracle_puppets_context;

    if (!ctx->puppets->enable) {
        return;
    }

    for (int i = 0; i < ctx->num_puppets; i++ ) {
        oracle_puppets_update_servo(&(ctx->puppets[i]));
    }
}

void oracle_puppets_setup(void)
{
    return;
}