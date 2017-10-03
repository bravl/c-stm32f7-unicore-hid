/** @defgroup rtc_defines RTC Defines

@brief <b>Defined Constants and Types for the STM32F4xx RTC</b>

@ingroup STM32F4xx_defines

@version 1.0.0

@date 5 December 2012

LGPL License Terms @ref lgpl_license
 */

/*
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

#ifndef UNICOREMX_RTC_H
#define UNICOREMX_RTC_H

#include <unicore-mx/stm32/common/rtc_common_l1f024.h>

BEGIN_DECLS

void rtc_enable_wakeup_timer(void);
void rtc_disable_wakeup_timer(void);
void rtc_enable_wakeup_timer_interrupt(void);
void rtc_disable_wakeup_timer_interrupt(void);

END_DECLS

#endif
