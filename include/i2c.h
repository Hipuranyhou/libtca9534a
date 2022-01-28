#ifndef LIBTCA9534A_I2C_H_qwreertqwe
#define LIBTCA9534A_I2C_H_qwreertqwe

#include <linux/types.h>

#define I2C_ADDR_MAX 0x7fU

/**
 * Gets one byte register state into val.
 * @param[in]  fd   File descriptor of opened /dev/i2c-<bus>.
 * @param[in]  addr I2C address of slave.
 * @param[in]  reg  Register to read address.
 * @param[out] val  Pointer to register state destination.
 * @return int -1 on error, 0 on success.
 */
int i2c_get_reg(const int fd, const __u8 addr, const __u8 reg, __u8 *const val);

/**
 * Sets one byte register state to val.
 * @param[in] fd   File descriptor of opened /dev/i2c-<bus>.
 * @param[in] addr I2C address of slave.
 * @param[in] reg  Register to read address.
 * @param[in] val  New state of register.
 * @return int -1 on error, 0 on success.
 */
int i2c_set_reg(const int fd, const __u8 addr, const __u8 reg, const __u8 val);

#endif /* LIBTCA9534A_I2C_H_qwreertqwe */
