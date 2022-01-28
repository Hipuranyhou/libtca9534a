#ifndef LIBTCA9534A_TCA9534A_H_ijtorewitn
#define LIBTCA9534A_TCA9534A_H_ijtorewitn

#include "i2c.h"

#include <linux/types.h>

#define TCA9534A_ADDR 0x38U

/* registers */
enum tca9534a_reg {
	TCA9534A_REG_IN,
	TCA9534A_REG_OUT,
	TCA9534A_REG_POL,
	TCA9534A_REG_CONF,
	__TCA9534A_REG_CNT
};

/* ports */
enum tca9534a_port {
	TCA9534A_PORT_0,
	TCA9534A_PORT_1,
	TCA9534A_PORT_2,
	TCA9534A_PORT_3,
	TCA9534A_PORT_4,
	TCA9534A_PORT_5,
	TCA9534A_PORT_6,
	TCA9534A_PORT_7,
	__TCA9534A_PORT_CNT
};

/* polarity, only input */
enum tca9534a_port_pol {
	TCA9534A_PORT_POL_ORIG,
	TCA9534A_PORT_POL_INV,
	__TCA9534A_PORT_POL_CNT
};

/* level, only output */
enum tca9534a_port_lvl {
	TCA9534A_PORT_LVL_LOW,
	TCA9534A_PORT_LVL_HIGH,
	__TCA9534A_PORT_LVL_CNT
};

/* direction */
enum tca9534a_port_dir {
	TCA9534A_PORT_DIR_OUT,
	TCA9534A_PORT_DIR_IN,
	__TCA9534A_PORT_DIR_CNT
};

/**
 * Gets register state into val.
 * @param[in]  fd  File descriptor of opened /dev/i2c-<bus>.
 * @param[in]  reg Register to read.
 * @param[out] val Pointer to register state destination.
 * @return int -1 on error, 0 on success.
 */
int tca9534a_reg_get(const int fd, const enum tca9534a_reg reg,
		     __u8 *const val);

/**
 * Sets register state to val. TCA9534A_REG_IN not allowed.
 * @param[in] fd  File descriptor of opened /dev/i2c-<bus>.
 * @param[in] reg Register to set.
 * @param[in] val New state of register.
 * @return int -1 on error, 0 on success.
 */
int tca9534a_reg_set(const int fd, const enum tca9534a_reg reg, const __u8 val);

/**
 * Sets register to datasheet default. TCA9534A_REG_IN not allowed.
 * @param[in] fd  File descriptor of opened /dev/i2c-<bus>.
 * @param[in] reg Register to reset.
 * @return int -1 on error, 0 on success.
 */
int tca9534a_reg_reset(const int fd, const enum tca9534a_reg reg);

/**
 * Gets input port level into lvl.
 * @param[in]  fd  File descriptor of opened /dev/i2c-<bus>.
 * @param[in]  port Port to check.
 * @param[out] lvl  Pointer to input port state destination.
 * @return int -1 on error, 0 on success.
 */
int tca9534a_port_read(const int fd, const enum tca9534a_port port,
		       enum tca9534a_port_lvl *const lvl);

/**
 * Sets output port level to lvl. Preserves other TCA9534A_REG_OUT ports.
 * @param[in] fd   File descriptor of opened /dev/i2c-<bus>.
 * @param[in] port Port to set.
 * @param[in] lvl  New level of output port.
 * @return int -1 on error, 0 on success.
 */
int tca9534a_port_write(const int fd, const enum tca9534a_port port,
			const enum tca9534a_port_lvl lvl);

/**
 * Sets input port polarity to pol. Preserves other TCA9534A_REG_POL ports.
 * @param[in] fd   File descriptor of opened /dev/i2c-<bus>.
 * @param[in] port Port to set.
 * @param[in] pol  New polarity of input port.
 * @return int -1 on error, 0 on success.
 */
int tca9534a_port_pol(const int fd, const enum tca9534a_port port,
		      const enum tca9534a_port_pol pol);

/**
 * Sets port direction to dir. Preserves other TCA9534A_REG_CONF ports.
 * @param[in] fd   File descriptor of opened /dev/i2c-<bus>.
 * @param[in] port Port to set.
 * @param[in] dir  New direction of port.
 * @return int -1 on error, 0 on success.
 */
int tca9534a_port_dir(const int fd, const enum tca9534a_port port,
		      const enum tca9534a_port_dir dir);

#endif /* LIBTCA9534A_TCA9534A_H_ijtorewitn */
