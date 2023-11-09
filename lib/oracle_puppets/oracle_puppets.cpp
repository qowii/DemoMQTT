#include <Arduino.h>
#include <Deneyap_Servo.h>

#include <oracle_puppets.h>
#include <oracle_utils.h>

static oracle_puppets_context_t oracle_puppets_context;

static const int ORACLE_PUPPETS_PUPPET_INIT_ANGLE[ORACLE_PUPPETS_NUM_PUPPETS] = {
    ORACLE_PUPPETS_PUPPET1_INIT_ANGLE,
    ORACLE_PUPPETS_PUPPET2_INIT_ANGLE,
    ORACLE_PUPPETS_PUPPET3_INIT_ANGLE,
    ORACLE_PUPPETS_PUPPET4_INIT_ANGLE,
    ORACLE_PUPPETS_PUPPET5_INIT_ANGLE,
    ORACLE_PUPPETS_PUPPET6_INIT_ANGLE,
    ORACLE_PUPPETS_PUPPET7_INIT_ANGLE,
    ORACLE_PUPPETS_PUPPET8_INIT_ANGLE
};

static const int ORACLE_PUPPETS_PUPPET_MIN_ANGLE[ORACLE_PUPPETS_NUM_PUPPETS] = {
    ORACLE_PUPPETS_PUPPET1_MIN_ANGLE,
    ORACLE_PUPPETS_PUPPET2_MIN_ANGLE,
    ORACLE_PUPPETS_PUPPET3_MIN_ANGLE,
    ORACLE_PUPPETS_PUPPET4_MIN_ANGLE,
    ORACLE_PUPPETS_PUPPET5_MIN_ANGLE,
    ORACLE_PUPPETS_PUPPET6_MIN_ANGLE,
    ORACLE_PUPPETS_PUPPET7_MIN_ANGLE,
    ORACLE_PUPPETS_PUPPET8_MIN_ANGLE
};

static const int ORACLE_PUPPETS_PUPPET_MAX_ANGLE[ORACLE_PUPPETS_NUM_PUPPETS] = {
    ORACLE_PUPPETS_PUPPET1_MAX_ANGLE,
    ORACLE_PUPPETS_PUPPET2_MAX_ANGLE,
    ORACLE_PUPPETS_PUPPET3_MAX_ANGLE,
    ORACLE_PUPPETS_PUPPET4_MAX_ANGLE,
    ORACLE_PUPPETS_PUPPET5_MAX_ANGLE,
    ORACLE_PUPPETS_PUPPET6_MAX_ANGLE,
    ORACLE_PUPPETS_PUPPET7_MAX_ANGLE,
    ORACLE_PUPPETS_PUPPET8_MAX_ANGLE
};

static const int ORACLE_PUPPETS_PUPPET_GPIO_ANGLE[ORACLE_PUPPETS_NUM_PUPPETS] = {
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
    oracle_puppets_puppet_context_t *puppet = &(ctx->puppets[index]);

    if (UINT8_MAX == angle) {
        return false;
    }

    puppet->servos.motor.write(angle);
    return true;
}

void oracle_puppets_update_servo(oracle_puppets_puppet_context_t *puppet)
{
    uint8_t angle, next_batch, running_index;

    if (!puppet->servos.enable) {
        return;
    }

    running_index = puppet->batchs.running_index;
    if (!puppet->batchs.frames[running_index].ready) {
        return;
    }

    angle = oracle_utils_read_uint16(puppet->batchs.next_angle);
    puppet->batchs.frame_index += 2;

    if (oracle_puppets_move_servo(puppet->index, angle) &&
        ORACLE_PUPPETS_NUM_FRAMES > puppet->batchs.frame_index) {
            puppet->batchs.next_angle += 2;
        return;
    }

    /* Clean current batch */
    puppet->batchs.frames[running_index].ready = false;

    /* Prepare next batch */
    next_batch = (running_index + 1) % ORACLE_PUPPETS_NUM_BATCHS;
    puppet->batchs.running_index = next_batch;
    puppet->batchs.frame_index = 0;
    puppet->batchs.next_angle = puppet->batchs.frames[next_batch].buffer;
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