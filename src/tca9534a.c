#include "tca9534a.h"

#include "i2c.h"
#include "util.h"

/* TCA9534A_REG_IN does not have default */
static const __u8 tca9534a_reg_def[__TCA9534A_REG_CNT] = {
	[TCA9534A_REG_IN] = 0x00,
	[TCA9534A_REG_OUT] = 0xff,
	[TCA9534A_REG_POL] = 0x00,
	[TCA9534A_REG_CONF] = 0xff
};

int tca9534a_reg_get(const int fd, const enum tca9534a_reg reg, __u8 *const val)
{
	/* sanity check */
	if (fd < 0 || !ENUM_OK(reg, __TCA9534A_REG_CNT) || !val)
		return -1;

	return i2c_get_reg(fd, TCA9534A_ADDR, reg, val);
}

int tca9534a_reg_set(const int fd, const enum tca9534a_reg reg, const __u8 val)
{
	/* sanity check */
	if (fd < 0 || !ENUM_OK(reg, __TCA9534A_REG_CNT) ||
	    reg == TCA9534A_REG_IN)
		return -1;

	return i2c_set_reg(fd, TCA9534A_ADDR, reg, val);
}

int tca9534a_reg_reset(const int fd, const enum tca9534a_reg reg)
{
	/* sanity check */
	if (fd < 0 || !ENUM_OK(reg, __TCA9534A_REG_CNT) ||
	    reg == TCA9534A_REG_IN)
		return -1;

	return tca9534a_reg_set(fd, reg, tca9534a_reg_def[reg]);
}

static int tca9534a_reg_port_set(const int fd, const int reg, const int port,
				 const int val)
{
	__u8 state;

	/* only public functions check sanity */

	/* load old state */
	if (tca9534a_reg_get(fd, reg, &state))
		return -1;

	BIT_TO(state, port, val);

	/* save new state */
	return tca9534a_reg_set(fd, reg, state);
}

static int tca9534a_reg_port_get(const int fd, const int reg, const int port,
				 int *const val)
{
	__u8 state;

	/* only public functions check sanity */

	/* load old state */
	if (tca9534a_reg_get(fd, reg, &state))
		return -1;

	*val = BIT(state, port);

	return 0;
}

int tca9534a_port_read(const int fd, const enum tca9534a_port port,
		       enum tca9534a_port_lvl *const lvl)
{
	/* sanity check */
	if (fd < 0 || !ENUM_OK(port, __TCA9534A_PORT_CNT) || !lvl)
		return -1;

	return tca9534a_reg_port_get(fd, TCA9534A_REG_IN, port, lvl);
}

int tca9534a_port_write(const int fd, const enum tca9534a_port port,
			const enum tca9534a_port_lvl lvl)
{
	/* sanity check */
	if (fd < 0 || !ENUM_OK(port, __TCA9534A_PORT_CNT) ||
	    !ENUM_OK(lvl, __TCA9534A_PORT_LVL_CNT))
		return -1;

	return tca9534a_reg_port_set(fd, TCA9534A_REG_CONF, port, lvl);
}

int tca9534a_port_pol(const int fd, const enum tca9534a_port port,
		      const enum tca9534a_port_pol pol)
{
	/* sanity check */
	if (fd < 0 || !ENUM_OK(port, __TCA9534A_PORT_CNT) ||
	    !ENUM_OK(pol, __TCA9534A_PORT_POL_CNT))
		return -1;

	return tca9534a_reg_port_set(fd, TCA9534A_REG_CONF, port, pol);
}

int tca9534a_port_dir(const int fd, const enum tca9534a_port port,
		      const enum tca9534a_port_dir dir)
{
	/* sanity check */
	if (fd < 0 || !ENUM_OK(port, __TCA9534A_PORT_CNT) ||
	    !ENUM_OK(dir, __TCA9534A_PORT_DIR_CNT))
		return -1;

	return tca9534a_reg_port_set(fd, TCA9534A_REG_CONF, port, dir);
}
