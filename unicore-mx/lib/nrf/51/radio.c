/** @addtogroup radio_defines
 *
 * @brief <b>Access functions for the NRF51 2.4 GHz Radio </b>
 * @ingroup NRF51_defines
 * LGPL License Terms @ref lgpl_license
 * @author @htmlonly &copy; @endhtmlonly 2016
 * Maxim Sloyko <maxims@google.com>
 *
 */

/*
 * This file is part of the unicore-mx project.
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

#include <unicore-mx/nrf/ficr.h>
#include <unicore-mx/nrf/radio.h>


/** @brief Set radio mode.
 *
 * @details The function also performs all required overrides for BLE and NRF mode.
 *
 * @param[in] fmode enum radio_mode the new mode.
 * */
void radio_set_mode(enum radio_mode mode)
{
    volatile uint32_t* override_pos = 0;
    if ((RADIO_MODE_BLE_1MBIT == mode)
            && (FICR_OVERRIDEEN & ~FICR_OVERRIDEEN_BLE_1MBIT)) {
        /* Need to use Override */
        override_pos = &FICR_BLE_1MBIT0;
    } else if ((RADIO_MODE_NRF_1MBIT == mode)
            && (FICR_OVERRIDEEN & ~FICR_OVERRIDEEN_NRF_1MBIT)) {
        override_pos = &FICR_NRF_1MBIT0;
    }

    if (override_pos) {
        uint8_t i;
        for (i = 0; i <= 4; ++i, ++override_pos) {
            RADIO_OVERRIDE(i) = *override_pos;
        }

        RADIO_OVERRIDE4 |= RADIO_OVERRIDE4_ENABLE;
    } else {
        RADIO_OVERRIDE4 &= ~RADIO_OVERRIDE4_ENABLE;
    }

    RADIO_MODE = mode;
}

/** @brief Set radio transmission power.
 *
 * @details Note, not all supported power levels are BLE compliant.
 *
 * @param[in] txpower enum radio_txpower
 * */
void radio_set_txpower(enum radio_txpower txpower)
{
    RADIO_TXPOWER = txpower;
}

/** @brief Set bit transmission order to LSB first. */
void radio_set_lsbfirst(void)
{
    RADIO_PCNF1 &= ~RADIO_PCNF1_ENDIAN_BIG;
}

/** @brief Set bit transmission order to MSB first. */
void radio_set_msbfirst(void)
{
    RADIO_PCNF1 |= RADIO_PCNF1_ENDIAN_BIG;
}

/** @brief Enable on the air data whitening
 *
 * @details the in-memory data will remain unwhitened.
 * */
void radio_enable_whitening(void)
{
    RADIO_PCNF1 |= RADIO_PCNF1_WHITEEN;
}

/** @brief Disable on the air data whitening. */
void radio_disable_whitening(void)
{
    RADIO_PCNF1 &= ~RADIO_PCNF1_WHITEEN;
}

/** @brief Set CRC length in number of bytes.
 *
 * @param[in] crc_len uint8_t CRC length in number of bytes (1-3), 0 = CRC disabled.
 */
void radio_set_crclen(uint8_t crc_len)
{
    uint32_t reg_crc = RADIO_CRCCNF;
    reg_crc &= ~RADIO_CRCCNF_LEN_MASK;
    RADIO_CRCCNF = reg_crc | RADIO_CRCCNF_LEN_MASKED(crc_len);
}

/** @brief Disable CRC calculation. */
void radio_disable_crc(void)
{
    RADIO_CRCCNF &= ~RADIO_CRCCNF_LEN_MASK;
}

/** @brief Enable the peripheral. */
void radio_enable(void)
{
    RADIO_POWER = RADIO_POWER_ENABLED;
}

/** @brief Disable the peripheral. */
void radio_disable(void)
{
    RADIO_POWER = RADIO_POWER_DISABLED;
}


/** @brief Set Base Address length.
 *
 * @param[in] ba_len uint8_t Base Address length in number of bytes (2-4).
 */
void radio_set_balen(uint8_t ba_len)
{
    uint32_t reg_pcnf1 = RADIO_PCNF1;
    reg_pcnf1 &= ~RADIO_PCNF1_BALEN_MASK;
    RADIO_PCNF1 = reg_pcnf1 | RADIO_PCNF1_BALEN_MASKED(ba_len);
}

/** @brief Set maximum transmission length in number of bytes.
 *
 * @param[in] maxlen uint8_t maximum transmission length.
 */
void radio_set_maxlen(uint8_t maxlen)
{
    uint32_t reg_pcnf1 = RADIO_PCNF1;
    reg_pcnf1 &= ~RADIO_PCNF1_MAXLEN_MASK;
    RADIO_PCNF1 = reg_pcnf1 | RADIO_PCNF1_MAXLEN_MASKED(maxlen);
}

/** @brief Exclude access address from CRC calculation.
 *
 * @param[in] is_skip_addr bool If true, CRC will be calculated over PDU only,
 *  if false, it will also include the Access Address.
 */
void radio_set_crc_skipaddr(bool is_skip_addr)
{
    if (is_skip_addr) {
        RADIO_CRCCNF |= RADIO_CRCCNF_SKIPADDR;
    } else {
        RADIO_CRCCNF &= ~RADIO_CRCCNF_SKIPADDR;
    }
}

/** @brief Configure the radio to be used in BLE mode.
 *
 * @details This needs to be called before the radio can be used in BLE mode.
 *  It will set som BLE standard parameters, like Inter-Frame Spacing time,
 *  LSB first, enable whitening, properly configure CRC (for advertising) and address length.
 */
void radio_configure_ble(void)
{
    radio_set_mode(RADIO_MODE_BLE_1MBIT);
    RADIO_TIFS = RADIO_BLE_TIFS;
    radio_set_lsbfirst();
    radio_enable_whitening();

    radio_set_crclen(RADIO_BLE_CRCLEN);
    RADIO_CRCPOLY = RADIO_BLE_CRCPOLY;
    RADIO_CRCINIT = RADIO_BLE_CRCINIT;
    radio_set_crc_skipaddr(true);
    radio_set_balen(3);
}

/** @brief Configure the packet.
 *
 * @details See the data sheet for details.
 */
void radio_configure_packet(uint8_t lf_len_bits, uint8_t s0_len_bytes, uint8_t s1_len_bits)
{
    RADIO_PCNF0 = RADIO_PCNF0_LFLEN_MASKED(lf_len_bits)
        | RADIO_PCNF0_S0LEN_MASKED(s0_len_bytes)
        | RADIO_PCNF0_S1LEN_MASKED(s1_len_bits);
}

/** @brief Set radio frequency.
 *
 * @param[in] freq uint8_t Frequency offset from 2.4GHz in MHz, for example "29" will
 *  tune the radio to 2429MHz
 */
void radio_set_frequency(uint8_t freq)
{
    RADIO_FREQUENCY = freq;
}

/** @brief Set Data Whitening Initialization Vector.
 *
 * @param[in] iv uint8_t Initialization Vector. For BLE, this is channel index.
 */
void radio_set_datawhiteiv(uint8_t iv)
{
    RADIO_DATAWHITEIV = iv;
}

/* @brief Set Address (base and prefix)
 *
 * @details Note that bases are shared between addresses 1-7,
 * so changing one of them will change others too.
 *
 * @param[in] addr_index uint8_t address index (0-7)
 * @param[in] base uint32_t base part of the address. If balen < 4, appropriate number
 *  of LSBs will be thrown away.
 * @param[in] prefix uint8_t Address prefix.
 */
void radio_set_addr(uint8_t addr_index, uint32_t base, uint8_t prefix)
{
    if (addr_index == 0) {
        RADIO_BASE0 = base;
    } else {
        RADIO_BASE1 = base;
    }

    uint32_t reg_prefix = RADIO_PREFIX_AP(addr_index);
    reg_prefix &= ~RADIO_PREFIX_AP_MASK(addr_index);
    RADIO_PREFIX_AP_SET(addr_index, reg_prefix | RADIO_PREFIX_AP_MASKED(addr_index, prefix));
}

/* @brief Set TX address index
 *
 * @details The address needs to be previously configured with radio_set_addr()
 *
 * @param[in] address_index uint8_t address index (0-7)
 */
void radio_set_tx_address(uint8_t addr_index)
{
    RADIO_TXADDRESS = addr_index;
}

/* @brief Set pointer for RX/TX data
 *
 * @param[in] packet_ptr uint8_t* packet buffer address.
 */
void radio_set_packet_ptr(uint8_t* packet_ptr)
{
    RADIO_PACKETPTR = (uint32_t)packet_ptr;
}

/* @brief Enable shortcuts
 *
 * @param[in] shorts uint16_t shortcuts to enable.
 */
void radio_enable_shorts(uint16_t shorts)
{
    periph_enable_shorts(RADIO_BASE, shorts);
}

/* @brief Disable shortcuts
 *
 * @param[in] shorts uint16_t shortcuts to disable.
 */
void radio_disable_shorts(uint16_t shorts)
{
    periph_disable_shorts(RADIO_BASE, shorts);
}

/* @brief Clear all shortcuts */
void radio_clear_shorts(void)
{
    periph_clear_shorts(RADIO_BASE);
}

/* @brief Enable radio Transmitter */
void radio_enable_tx(void)
{
    periph_trigger_task(RADIO_TASK_TXEN);
}

/* @brief Enable radio Receiver */
void radio_enable_rx(void)
{
    periph_trigger_task(RADIO_TASK_RXEN);
}

/* @brief Enable interrupts by mask */
void radio_enable_interrupts(uint32_t interrupts)
{
    periph_enable_interrupts(RADIO_BASE, interrupts);
}

/* @brief Disable interrupts by mask */
void radio_disable_interrupts(uint32_t interrupts)
{
    periph_disable_interrupts(RADIO_BASE, interrupts);
}