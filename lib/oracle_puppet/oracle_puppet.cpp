
#include <oracle_puppet.h>

static const int oracle_puppet_servo_init_angle[ORACLE_PUPPET_MAX_NUM_DEFAULT] = {
    ORACLE_PUPPET_SERVO1_INIT_ANGLE,
    ORACLE_PUPPET_SERVO2_INIT_ANGLE,
    ORACLE_PUPPET_SERVO3_INIT_ANGLE,
    ORACLE_PUPPET_SERVO4_INIT_ANGLE,
    ORACLE_PUPPET_SERVO5_INIT_ANGLE,
    ORACLE_PUPPET_SERVO6_INIT_ANGLE,
    ORACLE_PUPPET_SERVO7_INIT_ANGLE,
    ORACLE_PUPPET_SERVO8_INIT_ANGLE
};

static const int oracle_puppet_servo_min_angle[ORACLE_PUPPET_MAX_NUM_DEFAULT] = {
    ORACLE_PUPPET_SERVO1_MIN_ANGLE,
    ORACLE_PUPPET_SERVO2_MIN_ANGLE,
    ORACLE_PUPPET_SERVO3_MIN_ANGLE,
    ORACLE_PUPPET_SERVO4_MIN_ANGLE,
    ORACLE_PUPPET_SERVO5_MIN_ANGLE,
    ORACLE_PUPPET_SERVO6_MIN_ANGLE,
    ORACLE_PUPPET_SERVO7_MIN_ANGLE,
    ORACLE_PUPPET_SERVO8_MIN_ANGLE
};

static const int oracle_puppet_servo_max_angle[ORACLE_PUPPET_MAX_NUM_DEFAULT] = {
    ORACLE_PUPPET_SERVO1_MAX_ANGLE,
    ORACLE_PUPPET_SERVO2_MAX_ANGLE,
    ORACLE_PUPPET_SERVO3_MAX_ANGLE,
    ORACLE_PUPPET_SERVO4_MAX_ANGLE,
    ORACLE_PUPPET_SERVO5_MAX_ANGLE,
    ORACLE_PUPPET_SERVO6_MAX_ANGLE,
    ORACLE_PUPPET_SERVO7_MAX_ANGLE,
    ORACLE_PUPPET_SERVO8_MAX_ANGLE
};

static const int oracle_puppet_servo_gpio_pin[ORACLE_PUPPET_MAX_NUM_DEFAULT] = {
    ORACLE_PUPPET_SERVO1_GPIO_PIN,
    ORACLE_PUPPET_SERVO2_GPIO_PIN,
    ORACLE_PUPPET_SERVO3_GPIO_PIN,
    ORACLE_PUPPET_SERVO4_GPIO_PIN,
    ORACLE_PUPPET_SERVO5_GPIO_PIN,
    ORACLE_PUPPET_SERVO6_GPIO_PIN,
    ORACLE_PUPPET_SERVO7_GPIO_PIN,
    ORACLE_PUPPET_SERVO8_GPIO_PIN
};

oracle_puppet_frames_t *oracle_puppet_frames_allocate(void)
{
    oracle_puppet_frames_t *frames;

    const int num_batchs = ORACLE_PUPPET_NUM_BATCHS;
    const int num_frames = ORACLE_PUPPET_NUM_FRAMES;
    
    frames = (oracle_puppet_frames_t *)malloc(sizeof(oracle_puppet_frames_t));
    frames->num_batchs = num_batchs;
    frames->batch_index = 0;
    frames->num_frames = num_frames;
    frames->frame_index = 0;
    frames->frame_positions = (uint32_t **) malloc(sizeof(uint32_t *) * num_batchs);
    frames->frame_delay = (uint32_t *) malloc(sizeof(uint32_t) * num_batchs);
    frames->frame_status = (bool *) malloc(sizeof(bool) * num_batchs);
    for (int i = 0; i < num_batchs; i++) {
        frames->frame_positions[i] = (uint32_t *) malloc(sizeof(uint32_t) * num_frames);
        memset(frames->frame_positions[i], 0, sizeof(uint32_t) * num_frames);
        frames->frame_delay[i] = 0;
        frames->frame_status[i] = false;
    }

    return frames;
}

oracle_puppet_context_t *oracle_puppet_context_allocate(const int index)
{
    oracle_puppet_context_t *ctx;
    ctx = (oracle_puppet_context_t *) malloc(sizeof(oracle_puppet_context_t));
    ctx->frames = oracle_puppet_frames_allocate();

    ctx->index = index;
    ctx->angle_min = oracle_puppet_servo_min_angle[index];
    ctx->angle_ini = oracle_puppet_servo_init_angle[index];
    ctx->angle_max = oracle_puppet_servo_max_angle[index];
    ctx->gpio_pin = oracle_puppet_servo_gpio_pin[index];
    
    return ctx;
}