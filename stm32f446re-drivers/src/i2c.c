#include <stdbool.h>
#include "i2c.h"
#include "clock.h"
#include "driver_wait.h"
#include "stm32f446/regs/i2c.h"
static bool initialized;
static driver_status_t error_status(uint32_t sr)
{
    if (sr & I2C_SR1_ARLO) return DRIVER_ARBITRATION_LOST;
    if (sr & I2C_SR1_AF) return DRIVER_NACK;
    if (sr & (I2C_SR1_BERR | I2C_SR1_OVR | I2C_SR1_TIMEOUT)) return DRIVER_IO_ERROR;
    return DRIVER_OK;
}
static driver_status_t wait_flag(uint32_t mask, driver_deadline_t d)
{
    for (;;) {
        const uint32_t sr = I2C1->SR1;
        const driver_status_t error = error_status(sr);
        if (error != DRIVER_OK) return error;
        if (sr & mask) return DRIVER_OK;
        if (driver_expired(d)) return DRIVER_TIMEOUT;
    }
}
static void clear_address(void) { (void)I2C1->SR1; (void)I2C1->SR2; }
static driver_status_t send_address(uint8_t address, bool read, driver_deadline_t d)
{
    I2C1->CR1 |= I2C_CR1_START;
    driver_status_t s = wait_flag(I2C_SR1_SB, d);
    if (s != DRIVER_OK) return s;
    I2C1->DR = ((uint32_t)address << 1) | (read ? 1u : 0u);
    /* Leave ADDR asserted until ACK/POS have been configured for RX length. */
    return wait_flag(I2C_SR1_ADDR, d);
}
static driver_status_t receive_bytes(uint8_t *data, size_t length, driver_deadline_t d)
{
    driver_status_t s;
    if (length == 1u) {
        I2C1->CR1 &= ~I2C_CR1_ACK;
        const uint32_t primask = __get_PRIMASK();
        __disable_irq();
        clear_address();
        I2C1->CR1 |= I2C_CR1_STOP;
        __set_PRIMASK(primask);
        s = wait_flag(I2C_SR1_RXNE, d);
        if (s != DRIVER_OK) return s;
        data[0] = (uint8_t)I2C1->DR;
        return DRIVER_OK;
    }
    if (length == 2u) {
        /* POS was set before START. */
        I2C1->CR1 &= ~I2C_CR1_ACK;
        clear_address();
        s = wait_flag(I2C_SR1_BTF, d);
        if (s != DRIVER_OK) return s;
        const uint32_t primask = __get_PRIMASK();
        __disable_irq();
        I2C1->CR1 |= I2C_CR1_STOP;
        data[0] = (uint8_t)I2C1->DR;
        data[1] = (uint8_t)I2C1->DR;
        __set_PRIMASK(primask);
        return DRIVER_OK;
    }
    I2C1->CR1 |= I2C_CR1_ACK;
    clear_address();
    while (length > 3u) {
        s = wait_flag(I2C_SR1_RXNE, d);
        if (s != DRIVER_OK) return s;
        *data++ = (uint8_t)I2C1->DR;
        --length;
    }
    /* Final three bytes: BTF holds SCL, clear ACK, consume, then STOP. */
    s = wait_flag(I2C_SR1_BTF, d);
    if (s != DRIVER_OK) return s;
    I2C1->CR1 &= ~I2C_CR1_ACK;
    *data++ = (uint8_t)I2C1->DR;
    s = wait_flag(I2C_SR1_BTF, d);
    if (s != DRIVER_OK) return s;
    const uint32_t primask = __get_PRIMASK();
    __disable_irq();
    I2C1->CR1 |= I2C_CR1_STOP;
    *data++ = (uint8_t)I2C1->DR;
    *data = (uint8_t)I2C1->DR;
    __set_PRIMASK(primask);
    return DRIVER_OK;
}
void i2c1_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    (void)RCC->APB1ENR;
    GPIOB->BSRR = (1u << 8) | (1u << 9);
    GPIOB->OTYPER |= (1u << 8) | (1u << 9);
    GPIOB->OSPEEDR = (GPIOB->OSPEEDR & ~(15u << 16)) | (10u << 16);
    GPIOB->PUPDR &= ~(15u << 16);
    GPIOB->AFR[1] = (GPIOB->AFR[1] & ~255u) | 0x44u;
    GPIOB->MODER = (GPIOB->MODER & ~(15u << 16)) | (10u << 16);
    RCC->APB1RSTR |= RCC_APB1RSTR_I2C1RST;
    RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;
    I2C1->CR1 = 0u;
    I2C1->CR2 = APB1_HZ / 1000000u;
    I2C1->OAR1 = 1u << 14;
    I2C1->OAR2 = 0u;
    I2C1->CCR = APB1_HZ / (2u * 100000u);
    I2C1->TRISE = APB1_HZ / 1000000u + 1u;
    I2C1->CR1 = I2C_CR1_PE;
    initialized = true;
}
static driver_status_t transfer(uint8_t address, const uint8_t *tx, size_t tx_length,
                                uint8_t *rx, size_t rx_length, uint32_t timeout_ms)
{
    if (address < 0x08u || address > 0x77u || (tx_length && !tx) ||
        (rx_length && !rx) || (!tx_length && !rx_length)) return DRIVER_INVALID_ARGUMENT;
    if (!initialized) return DRIVER_NOT_INITIALIZED;
    const driver_deadline_t d = driver_deadline(timeout_ms);
    driver_status_t s = DRIVER_OK;
    while (I2C1->SR2 & I2C_SR2_BUSY) {
        if (driver_expired(d)) { initialized = false; return DRIVER_TIMEOUT; }
    }
    I2C1->CR1 &= ~(I2C_CR1_ACK | I2C_CR1_POS);
    if (tx_length) {
        s = send_address(address, false, d);
        if (s != DRIVER_OK) goto fail;
        clear_address();
        for (size_t i = 0u; i < tx_length; ++i) {
            s = wait_flag(I2C_SR1_TXE, d);
            if (s != DRIVER_OK) goto fail;
            I2C1->DR = tx[i];
        }
        s = wait_flag(I2C_SR1_BTF, d);
        if (s != DRIVER_OK) goto fail;
    }
    if (rx_length) {
        I2C1->CR1 |= I2C_CR1_ACK;
        if (rx_length == 2u) I2C1->CR1 |= I2C_CR1_POS;
        s = send_address(address, true, d);
        if (s != DRIVER_OK) goto fail;
        s = receive_bytes(rx, rx_length, d);
        if (s != DRIVER_OK) goto fail;
    } else I2C1->CR1 |= I2C_CR1_STOP;
    while ((I2C1->CR1 & I2C_CR1_STOP) || (I2C1->SR2 & I2C_SR2_BUSY)) {
        s = error_status(I2C1->SR1);
        if (s != DRIVER_OK) goto fail;
        if (driver_expired(d)) { s = DRIVER_TIMEOUT; goto fail; }
    }
    I2C1->CR1 &= ~(I2C_CR1_ACK | I2C_CR1_POS);
    return DRIVER_OK;
fail:
    if (s != DRIVER_ARBITRATION_LOST && (I2C1->SR2 & I2C_SR2_MSL)) I2C1->CR1 |= I2C_CR1_STOP;
    I2C1->SR1 &= ~(I2C_SR1_AF | I2C_SR1_ARLO | I2C_SR1_BERR | I2C_SR1_OVR | I2C_SR1_TIMEOUT);
    I2C1->CR1 &= ~(I2C_CR1_ACK | I2C_CR1_POS);
    initialized = false;
    return s;
}
driver_status_t i2c1_write(uint8_t address, const uint8_t *data, size_t length, uint32_t timeout_ms)
{
    if (!data || !length) return DRIVER_INVALID_ARGUMENT;
    return transfer(address, data, length, NULL, 0u, timeout_ms);
}
driver_status_t i2c1_read(uint8_t address, uint8_t *data, size_t length, uint32_t timeout_ms)
{
    if (!data || !length) return DRIVER_INVALID_ARGUMENT;
    return transfer(address, NULL, 0u, data, length, timeout_ms);
}
driver_status_t i2c1_write_read(uint8_t address, const uint8_t *tx, size_t tx_length,
                               uint8_t *rx, size_t rx_length, uint32_t timeout_ms)
{
    if (!tx || !rx || !tx_length || !rx_length) return DRIVER_INVALID_ARGUMENT;
    return transfer(address, tx, tx_length, rx, rx_length, timeout_ms);
}
