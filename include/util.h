#ifndef LIBTCA9534A_UTIL_H_nmdbgfdgmg
#define LIBTCA9534A_UTIL_H_nmdbgfdgmg

/* get _bit of _x */
#define BIT(_x, _bit) (((_x) >> (_bit)) & 1UL)

/* check _bit of _x is _val */
#define BIT_IS(_x, _bit, _val) ((BIT((_x), (_bit))) == (!!(_val)))

/* set _bit of _x to _val */
#define BIT_TO(_x, _bit, _val) \
	(_x) ^= (((-(!!((unsigned long)(_val)))) ^ (_x)) & (1UL << (_bit)))

/* check enum is between 0 and its max (only for no gap continuous enums) */
#define ENUM_OK(_val, _max) ((_val) >= 0 && (_val) < (_max))

#endif /* LIBTCA9534A_UTIL_H_nmdbgfdgmg */
