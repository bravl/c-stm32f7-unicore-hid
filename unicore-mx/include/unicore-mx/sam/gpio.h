/* This provides unification of code over SAM subfamilies */

/*
 * Copyright (C) 2014 Felix Held <felix-libopencm3@felixheld.de>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#if defined(SAM3A)
#       include <unicore-mx/sam/3a/gpio.h>
#elif defined(SAM3N)
#       include <unicore-mx/sam/3n/gpio.h>
#elif defined(SAM3S)
#       include <unicore-mx/sam/3s/gpio.h>
#elif defined(SAM3U)
#       include <unicore-mx/sam/3u/gpio.h>
#elif defined(SAM3X)
#       include <unicore-mx/sam/3x/gpio.h>
#elif defined(SAM4L)
#       include <unicore-mx/sam/4l/gpio.h>
#else
#       error "sam family not defined."
#endif
