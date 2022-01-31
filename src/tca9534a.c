/**
 * Copyright 2022 Jakub Šatoplet
 *
 * Use of this source code is governed by an MIT-style license that can be found
 * in the LICENSE file.
 */

#include "tca9534a.h"

#include <errno.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>

/**
 * Default values for TCA9534A registers from datasheet.
 */
static const __u8 tca9534a_reg_def[__TCA9534A_REG_CNT] = {
	[TCA9534A_REG_IN] = 0x00, /**< Does not have a default. */
	[TCA9534A_REG_OUT] = 0xff,
	[TCA9534A_REG_POL] = 0x00,
	[TCA9534A_REG_CONF] = 0xff
};

/**
 * Checks if val is in [0, end). Works only for continuous enums starting at 0.
 * @param[in] val Enum value to be checked.
 * @param[in] end Special enum end value or enum max + 1.
 * @return 0 not valid, 1 valid.
 */
static inline int tca9534a_enum_valid(const int val, const int end)
{
	return val >= 0 && val < end;
}

/**
 * Gets one byte register state into val.
 * @param[in]  dev Pointer to filled in struct tca9534a.
 * @param[in]  reg Register to read address.
 * @param[out] val Pointer to register state destination.
 * @return -1 on error (and sets errno), 0 on success.
 * @exception <ioctl> Any errno listed by ioctl().
 */
static int tca9534a_i2c_reg_get(const struct tca9534a *const dev,
				const __u8 reg, __u8 *const val)
{
	struct i2c_rdwr_ioctl_data pkt = { 0 };
	struct i2c_msg msg[2] = { 0 };

	/* only public functions check sanity */

	pkt.msgs = msg;
	pkt.nmsgs = 2;

	msg[0].addr = dev->addr;
	msg[0].len = sizeof(reg);
	msg[0].buf = (__u8 *)&reg;

	msg[1].flags = I2C_M_RD;
	msg[1].addr = dev->addr;
	msg[1].len = sizeof(*val);
	msg[1].buf = val;

	return ioctl(dev->fd, I2C_RDWR, &pkt) < 0 ? -1 : 0;
}

/**
 * Sets one byte register state to val.
 * @param[in] dev Pointer to filled in struct tca9534a.
 * @param[in] reg Register to read address.
 * @param[in] val New state of register.
 * @return -1 on error (and sets errno), 0 on success.
 * @exception <ioctl> Any errno listed by ioctl().
 */
static int tca9534a_i2c_reg_set(const struct tca9534a *const dev,
				const __u8 reg, const __u8 val)
{
	__u8 data[2] = { 0 };
	struct i2c_rdwr_ioctl_data pkt = { 0 };
	struct i2c_msg msg[1] = { 0 };

	/* only public functions check sanity */

	pkt.msgs = msg;
	pkt.nmsgs = 1;

	data[0] = reg;
	data[1] = val;

	msg[0].addr = dev->addr;
	msg[0].len = sizeof(data);
	msg[0].buf = data;

	return ioctl(dev->fd, I2C_RDWR, &pkt) < 0 ? -1 : 0;
}

/**
 * Gets port state bit from given register into val.
 * @param[in]  dev  Pointer to filled in struct tca9534a.
 * @param[in]  reg  Register to read.
 * @param[in]  port Port to check.
 * @param[out] val  Pointer to register state destination.
 * @return -1 on error (and sets errno), 0 on success.
 * @exception <ioctl> Any errno listed by ioctl().
 */
static int tca9534a_reg_port_get(const struct tca9534a *const dev,
				 const int reg, const int port, int *const val)
{
	__u8 state;

	/* only public functions check sanity */

	/* load old state */
	if (tca9534a_reg_get(dev, reg, &state))
		return -1;

	*val = (state >> port) & 1ULL;

	return 0;
}

/**
 * Gets port state bit from given register into val.
 * @param[in] dev  Pointer to filled in struct tca9534a.
 * @param[in] reg  Register to set.
 * @param[in] port Port to set.
 * @param[in] val  New state of port in register.
 * @return -1 on error (and sets errno), 0 on success.
 * @exception <ioctl> Any errno listed by ioctl().
 */
static int tca9534a_reg_port_set(const struct tca9534a *const dev,
				 const int reg, const int port, const int val)
{
	__u8 state;

	/* only public functions check sanity */

	/* load old state */
	if (tca9534a_reg_get(dev, reg, &state))
		return -1;

	state ^= ((-(unsigned)val) ^ state) & (1ULL << port);

	/* save new state */
	return tca9534a_reg_set(dev, reg, state);
}

int tca9534a_valid(const struct tca9534a *const dev)
{
	if (!dev) {
		errno = EINVAL;
		return 0;
	}

	return dev->fd >= 0 && dev->addr >= 0x38 && dev->addr <= 0x3f;
}

int tca9534a_reset(const struct tca9534a *const dev)
{
	if (!tca9534a_valid(dev)) {
		errno = EINVAL;
		return -1;
	}

	/* Return on first fail. */
	if (tca9534a_reg_reset(dev, TCA9534A_REG_OUT) ||
	    tca9534a_reg_reset(dev, TCA9534A_REG_POL) ||
	    tca9534a_reg_reset(dev, TCA9534A_REG_CONF))
		return -1;

	return 0;
}

int tca9534a_reg_get(const struct tca9534a *const dev,
		     const enum tca9534a_reg reg, __u8 *const val)
{
	/* sanity check */
	if (!tca9534a_valid(dev) ||
	    !tca9534a_enum_valid(reg, __TCA9534A_REG_CNT) || !val) {
		errno = EINVAL;
		return -1;
	}

	return tca9534a_i2c_reg_get(dev, reg, val);
}

int tca9534a_reg_set(const struct tca9534a *const dev,
		     const enum tca9534a_reg reg, const __u8 val)
{
	/* sanity check */
	if (!tca9534a_valid(dev) ||
	    !tca9534a_enum_valid(reg, __TCA9534A_REG_CNT) ||
	    reg == TCA9534A_REG_IN) {
		errno = EINVAL;
		return -1;
	}

	return tca9534a_i2c_reg_set(dev, reg, val);
}

int tca9534a_reg_reset(const struct tca9534a *const dev,
		       const enum tca9534a_reg reg)
{
	/* sanity check */
	if (!tca9534a_valid(dev) ||
	    !tca9534a_enum_valid(reg, __TCA9534A_REG_CNT) ||
	    reg == TCA9534A_REG_IN) {
		errno = EINVAL;
		return -1;
	}

	return tca9534a_reg_set(dev, reg, tca9534a_reg_def[reg]);
}

int tca9534a_port_read(const struct tca9534a *const dev,
		       const enum tca9534a_port port,
		       enum tca9534a_port_lvl *const lvl)
{
	/* sanity check */
	if (!tca9534a_valid(dev) ||
	    !tca9534a_enum_valid(port, __TCA9534A_PORT_CNT) || !lvl) {
		errno = EINVAL;
		return -1;
	}

	return tca9534a_reg_port_get(dev, TCA9534A_REG_IN, port, (int *)lvl);
}

int tca9534a_port_write(const struct tca9534a *const dev,
			const enum tca9534a_port port,
			const enum tca9534a_port_lvl lvl)
{
	/* sanity check */
	if (!tca9534a_valid(dev) ||
	    !tca9534a_enum_valid(port, __TCA9534A_PORT_CNT) ||
	    !tca9534a_enum_valid(lvl, __TCA9534A_PORT_LVL_CNT)) {
		errno = EINVAL;
		return -1;
	}

	return tca9534a_reg_port_set(dev, TCA9534A_REG_OUT, port, lvl);
}

int tca9534a_port_pol(const struct tca9534a *const dev,
		      const enum tca9534a_port port,
		      const enum tca9534a_port_pol pol)
{
	/* sanity check */
	if (!tca9534a_valid(dev) ||
	    !tca9534a_enum_valid(port, __TCA9534A_PORT_CNT) ||
	    !tca9534a_enum_valid(pol, __TCA9534A_PORT_POL_CNT)) {
		errno = EINVAL;
		return -1;
	}

	return tca9534a_reg_port_set(dev, TCA9534A_REG_POL, port, pol);
}

int tca9534a_port_dir(const struct tca9534a *const dev,
		      const enum tca9534a_port port,
		      const enum tca9534a_port_dir dir)
{
	/* sanity check */
	if (!tca9534a_valid(dev) ||
	    !tca9534a_enum_valid(port, __TCA9534A_PORT_CNT) ||
	    !tca9534a_enum_valid(dir, __TCA9534A_PORT_DIR_CNT)) {
		errno = EINVAL;
		return -1;
	}

	return tca9534a_reg_port_set(dev, TCA9534A_REG_CONF, port, dir);
}
