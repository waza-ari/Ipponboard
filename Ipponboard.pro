# Copyright 2018 Florian Muecke. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE.txt file.

TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += \
	base \
	GamepadDemo \
	test
#base.depends = core gamepad
