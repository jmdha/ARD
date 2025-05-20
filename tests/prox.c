#include <utest.h/utest.h>

#include "prox.h"

UTEST(prox, empty) {
	struct prox* prox = prox_init(3, 3);
	for (int x = 0; x < 3; x++)
		for (int y = 0; y < 3; y++) {
			ASSERT_EQ(prox_active(prox, x, y), 0);
			ASSERT_EQ(prox_get(prox, x, y), 0);
		}
	prox_free(prox);
}

UTEST(prox, single) {
	struct prox* prox = prox_init(1, 1);
	prox_set(prox, 0, 0);
	ASSERT_TRUE(prox_active(prox, 0, 0));
	ASSERT_EQ(prox_get(prox, 0, 0), 0);
	prox_free(prox);
}

UTEST(prox, north_south) {
	struct prox* prox = prox_init(1, 2);
	prox_set(prox, 0, 0);
	prox_set(prox, 0, 1);
	ASSERT_EQ(prox_get(prox, 0, 0), 1);
	ASSERT_EQ(prox_get(prox, 0, 1), 1);
	prox_free(prox);
}

UTEST(prox, south_north) {
	struct prox* prox = prox_init(1, 2);
	prox_set(prox, 0, 1);
	prox_set(prox, 0, 0);
	ASSERT_EQ(prox_get(prox, 0, 0), 1);
	ASSERT_EQ(prox_get(prox, 0, 1), 1);
	prox_free(prox);
}

UTEST(prox, east_west) {
	struct prox* prox = prox_init(2, 1);
	prox_set(prox, 0, 0);
	prox_set(prox, 1, 0);
	ASSERT_EQ(prox_get(prox, 0, 0), 1);
	ASSERT_EQ(prox_get(prox, 1, 0), 1);
	prox_free(prox);
}

UTEST(prox, west_east) {
	struct prox* prox = prox_init(2, 1);
	prox_set(prox, 0, 0);
	prox_set(prox, 1, 0);
	ASSERT_EQ(prox_get(prox, 0, 0), 1);
	ASSERT_EQ(prox_get(prox, 1, 0), 1);
	prox_free(prox);
}

UTEST(prox, hor) {
	struct prox* prox = prox_init(3, 1);
	prox_set(prox, 0, 0);
	prox_set(prox, 2, 0);
	ASSERT_EQ(prox_get(prox, 0, 0), 0);
	ASSERT_EQ(prox_get(prox, 2, 0), 0);
	prox_set(prox, 1, 0);
	ASSERT_EQ(prox_get(prox, 0, 0), 2);
	ASSERT_EQ(prox_get(prox, 1, 0), 2);
	ASSERT_EQ(prox_get(prox, 2, 0), 2);
	prox_free(prox);
}

UTEST(prox, ver) {
	struct prox* prox = prox_init(1, 3);
	prox_set(prox, 0, 0);
	prox_set(prox, 0, 2);
	ASSERT_EQ(prox_get(prox, 0, 0), 0);
	ASSERT_EQ(prox_get(prox, 0, 2), 0);
	prox_set(prox, 0, 1);
	ASSERT_EQ(prox_get(prox, 0, 0), 2);
	ASSERT_EQ(prox_get(prox, 0, 1), 2);
	ASSERT_EQ(prox_get(prox, 0, 2), 2);
	prox_free(prox);
}

UTEST(prox, mix) {
	struct prox* prox = prox_init(3, 3);
	prox_set(prox, 1, 0);
	prox_set(prox, 1, 2);
	prox_set(prox, 0, 1);
	prox_set(prox, 2, 1);
	prox_set(prox, 1, 1);
	ASSERT_EQ(prox_get(prox, 1, 0), 2);
	ASSERT_EQ(prox_get(prox, 1, 2), 2);
	ASSERT_EQ(prox_get(prox, 0, 1), 2);
	ASSERT_EQ(prox_get(prox, 2, 1), 2);
	ASSERT_EQ(prox_get(prox, 1, 1), 4);
	prox_free(prox);
}
