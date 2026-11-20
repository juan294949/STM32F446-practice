#ifndef TEST_CONFIG_H
#define TEST_CONFIG_H
/* Enable only after connecting the named fixture. */
#ifndef TEST_ENABLE_SPI_LOOPBACK
#define TEST_ENABLE_SPI_LOOPBACK 0 /* PA7 MOSI wired to PA6 MISO. */
#endif
#ifndef TEST_ENABLE_I2C_SENSOR
#define TEST_ENABLE_I2C_SENSOR 0 /* MPU6050 example, external PB8/9 pull-ups. */
#endif
#ifndef TEST_ENABLE_ADC_GROUND
#define TEST_ENABLE_ADC_GROUND 0 /* PA0 wired to GND. */
#endif
#ifndef TEST_I2C_ADDRESS
#define TEST_I2C_ADDRESS 0x68u
#endif
#ifndef TEST_I2C_REGISTER
#define TEST_I2C_REGISTER 0x75u
#endif
#ifndef TEST_I2C_EXPECTED
#define TEST_I2C_EXPECTED 0x68u
#endif
#endif
