#include "i2c.h"

#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>

int i2c_get_reg(const int fd, const __u8 addr, const __u8 reg, __u8 *const val)
{
	struct i2c_rdwr_ioctl_data pkt = { 0 };
	struct i2c_msg msg[2] = { 0 };

	/* sanity check */
	if (fd < 0 || addr > I2C_ADDR_MAX || !val)
		return -1;

	pkt.msgs = msg;
	pkt.nmsgs = 2;

	msg[0].addr = addr;
	msg[0].len = sizeof(reg);
	msg[0].buf = (__u8 *)&reg;

	msg[1].addr = addr;
	msg[1].flags = I2C_M_RD;
	msg[1].len = sizeof(*val);
	msg[1].buf = val;

	return ioctl(fd, I2C_RDWR, &pkt) < 0 ? -1 : 0;
}

int i2c_set_reg(const int fd, const __u8 addr, const __u8 reg, const __u8 val)
{
	__u8 data[2] = { 0 };
	struct i2c_rdwr_ioctl_data pkt = { 0 };
	struct i2c_msg msg[1] = { 0 };

	/* sanity check */
	if (fd < 0 || addr > I2C_ADDR_MAX)
		return -1;

	pkt.msgs = msg;
	pkt.nmsgs = 1;

	data[0] = reg;
	data[1] = val;

	msg[0].addr = addr;
	msg[0].len = sizeof(data);
	msg[0].buf = data;

	return ioctl(fd, I2C_RDWR, &pkt) < 0 ? -1 : 0;
}
