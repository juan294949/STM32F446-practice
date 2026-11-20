#include "spi.h"
#include "clock.h"
#include "driver_wait.h"
#include "stm32f446/regs/spi.h"
static bool initialized;
static driver_status_t wait_spi(uint32_t mask, bool set, driver_deadline_t d)
{
    for (;;) {
        const uint32_t sr = SPI1->SR;
        if (sr & (SPI_SR_OVR | SPI_SR_MODF | SPI_SR_FRE)) return DRIVER_IO_ERROR;
        if (((sr & mask) != 0u) == set) return DRIVER_OK;
        if (driver_expired(d)) return DRIVER_TIMEOUT;
    }
}
driver_status_t spi1_init(uint32_t max_hz, unsigned mode)
{
    if (mode > 3u || max_hz < APB2_HZ / 256u) return DRIVER_INVALID_ARGUMENT;
    if (initialized && (SPI1->SR & SPI_SR_BSY)) return DRIVER_BUSY;
    unsigned br = 0u; uint32_t divisor = 2u;
    while (APB2_HZ / divisor > max_hz && br < 7u) { ++br; divisor *= 2u; }
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    (void)RCC->APB2ENR;
    RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST;
    RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST;
    GPIOA->BSRR = 1u << 4;
    GPIOA->OTYPER &= ~(15u << 4);
    GPIOA->MODER = (GPIOA->MODER & ~(255u << 8)) |
                   (1u << 8) | (2u << 10) | (2u << 12) | (2u << 14);
    GPIOA->OSPEEDR = (GPIOA->OSPEEDR & ~(255u << 8)) | (170u << 8);
    GPIOA->PUPDR &= ~(255u << 8);
    GPIOA->AFR[0] = (GPIOA->AFR[0] & ~0xFFF00000u) | 0x55500000u;
    SPI1->CR2 = 0u;
    SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_SSM | SPI_CR1_SSI |
                (br << SPI_CR1_BR_Pos) |
                ((mode & 1u) ? SPI_CR1_CPHA : 0u) |
                ((mode & 2u) ? SPI_CR1_CPOL : 0u);
    SPI1->CR1 |= SPI_CR1_SPE;
    initialized = true;
    return DRIVER_OK;
}
void spi1_select(bool selected)
{
    if (initialized) GPIOA->BSRR = selected ? (1u << 20) : (1u << 4);
}
driver_status_t spi1_transfer(const uint8_t *tx, uint8_t *rx, size_t length, uint32_t timeout_ms)
{
    if (!initialized) return DRIVER_NOT_INITIALIZED;
    if (!length) return DRIVER_OK;
    if (SPI1->SR & SPI_SR_BSY) return DRIVER_BUSY;
    const driver_deadline_t d = driver_deadline(timeout_ms);
    if (SPI1->SR & SPI_SR_RXNE) { (void)SPI1->DR; (void)SPI1->SR; }
    for (size_t i = 0u; i < length; ++i) {
        driver_status_t s = wait_spi(SPI_SR_TXE, true, d);
        if (s != DRIVER_OK) return s;
        *(volatile uint8_t *)&SPI1->DR = tx ? tx[i] : 0xFFu;
        s = wait_spi(SPI_SR_RXNE, true, d);
        if (s != DRIVER_OK) return s;
        const uint8_t value = *(volatile uint8_t *)&SPI1->DR;
        if (rx) rx[i] = value;
    }
    return wait_spi(SPI_SR_BSY, false, d);
}
driver_status_t spi1_transaction(const uint8_t *tx, uint8_t *rx, size_t length, uint32_t timeout_ms)
{
    if (!initialized) return DRIVER_NOT_INITIALIZED;
    spi1_select(true);
    const driver_status_t s = spi1_transfer(tx, rx, length, timeout_ms);
    spi1_select(false);
    return s;
}
void spi1_deinit(void)
{
    if (!initialized) return;
    GPIOA->BSRR = 1u << 4;
    RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST;
    RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1RST;
    RCC->APB2ENR &= ~RCC_APB2ENR_SPI1EN;
    initialized = false;
}
