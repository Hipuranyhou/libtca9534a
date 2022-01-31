#ifndef LIBTCA9534A_TCA9534A_H_ijtorewitn
#define LIBTCA9534A_TCA9534A_H_ijtorewitn

#include <linux/types.h>

/**
 * Struct describing TCA9534A device.
 */
struct tca9534a {
	int fd;	   /**< Opened /dev/i2c-<bus>. */
	__u8 addr; /**< I2C address between 0x38 and 0x3f. */
};

/**
 * Enum describing TCA9534A registers.
 */
enum tca9534a_reg {
	TCA9534A_REG_IN,   /**< Input register. */
	TCA9534A_REG_OUT,  /**< Output register. */
	TCA9534A_REG_POL,  /**< Input polarity inversion register. */
	TCA9534A_REG_CONF, /**< Configuration (direction) register. */
	__TCA9534A_REG_CNT
};

/**
 * Enum describing TCA9534A ports.
 */
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

/**
 * Enum describing possible port logic level in the input/output register.
 * 
 * Setting this makes sense only for output ports.
 */
enum tca9534a_port_lvl {
	TCA9534A_PORT_LVL_LOW,	/**< Logic 0. */
	TCA9534A_PORT_LVL_HIGH, /**< Logic 1. */
	__TCA9534A_PORT_LVL_CNT
};

/**
 * Enum describing possible port polarity inversion in the polarity register.
 *
 * Setting this makes sense only for input ports.
 */
enum tca9534a_port_pol {
	TCA9534A_PORT_POL_ORIG, /**< Original polarity. */
	TCA9534A_PORT_POL_INV,	/**< Inverted polarity. */
	__TCA9534A_PORT_POL_CNT
};

/**
 * Enum describing possible port direction in the configuration register.
 */
enum tca9534a_port_dir {
	TCA9534A_PORT_DIR_OUT, /**< Output port. */
	TCA9534A_PORT_DIR_IN,  /**< Input port. */
	__TCA9534A_PORT_DIR_CNT
};

/**
 * Checks if dev->fd >= 0 and dev->addr is between 0x38 and 0x3f.
 * @param[in] dev Pointer to filled in struct tca9534a.
 * @return 0 not valid (and sets errno), 1 valid.
 * @exception EINVAL Invalid argument (dev is NULL).
 */
int tca9534a_valid(const struct tca9534a *const dev);

/**
 * Gets register state into val.
 * @param[in]  dev Pointer to filled in struct tca9534a.
 * @param[in]  reg Register to read.
 * @param[out] val Pointer to register state destination.
 * @return -1 on error (and sets errno), 0 on success.
 * @exception EINVAL  Invalid argument.
 * @exception <ioctl> Any errno listed by ioctl().
 */
int tca9534a_reg_get(const struct tca9534a *const dev,
		     const enum tca9534a_reg reg, __u8 *const val);

/**
 * Sets register state to val. TCA9534A_REG_IN not allowed.
 * @param[in] dev Pointer to filled in struct tca9534a.
 * @param[in] reg Register to set.
 * @param[in] val New state of register.
 * @return -1 on error (and sets errno), 0 on success.
 * @exception EINVAL  Invalid argument.
 * @exception <ioctl> Any errno listed by ioctl().
 */
int tca9534a_reg_set(const struct tca9534a *const dev,
		     const enum tca9534a_reg reg, const __u8 val);

/**
 * Sets register to datasheet default. TCA9534A_REG_IN not allowed.
 * @param[in] dev Pointer to filled in struct tca9534a.
 * @param[in] reg Register to reset.
 * @return -1 on error (and sets errno), 0 on success.
 * @exception EINVAL  Invalid argument.
 * @exception <ioctl> Any errno listed by ioctl().
 */
int tca9534a_reg_reset(const struct tca9534a *const dev,
		       const enum tca9534a_reg reg);

/**
 * Gets input port level into lvl.
 * @param[in]  dev Pointer to filled in struct tca9534a.
 * @param[in]  port Port to check.
 * @param[out] lvl  Pointer to input port state destination.
 * @return -1 on error (and sets errno), 0 on success.
 * @exception EINVAL  Invalid argument.
 * @exception <ioctl> Any errno listed by ioctl().
 */
int tca9534a_port_read(const struct tca9534a *const dev,
		       const enum tca9534a_port port,
		       enum tca9534a_port_lvl *const lvl);

/**
 * Sets output port level to lvl. Preserves other TCA9534A_REG_OUT ports.
 * @param[in] dev  Pointer to filled in struct tca9534a.
 * @param[in] port Port to set.
 * @param[in] lvl  New level of output port.
 * @return -1 on error (and sets errno), 0 on success.
 * @exception EINVAL  Invalid argument.
 * @exception <ioctl> Any errno listed by ioctl().
 */
int tca9534a_port_write(const struct tca9534a *const dev,
			const enum tca9534a_port port,
			const enum tca9534a_port_lvl lvl);

/**
 * Sets input port polarity to pol. Preserves other TCA9534A_REG_POL ports.
 * @param[in] dev  Pointer to filled in struct tca9534a.
 * @param[in] port Port to set.
 * @param[in] pol  New polarity of input port.
 * @return -1 on error (and sets errno), 0 on success.
 * @exception EINVAL  Invalid argument.
 * @exception <ioctl> Any errno listed by ioctl().
 */
int tca9534a_port_pol(const struct tca9534a *const dev,
		      const enum tca9534a_port port,
		      const enum tca9534a_port_pol pol);

/**
 * Sets port direction to dir. Preserves other TCA9534A_REG_CONF ports.
 * @param[in] dev  Pointer to filled in struct tca9534a.
 * @param[in] port Port to set.
 * @param[in] dir  New direction of port.
 * @return -1 on error (and sets errno), 0 on success.
 * @exception EINVAL  Invalid argument.
 * @exception <ioctl> Any errno listed by ioctl().
 */
int tca9534a_port_dir(const struct tca9534a *const dev,
		      const enum tca9534a_port port,
		      const enum tca9534a_port_dir dir);

#endif /* LIBTCA9534A_TCA9534A_H_ijtorewitn */
