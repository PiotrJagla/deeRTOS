#include <avr/io.h>
#include <util/delay.h>

#define SDA_PIN PB0
#define SCL_PIN PB2

void setup_pins() {
    // Set SDA and SCL as inputs
    DDRB &= ~((1 << SDA_PIN) | (1 << SCL_PIN));
    // Enable pull-up resistors on SDA and SCL
    PORTB |= (1 << SDA_PIN) | (1 << SCL_PIN);
}

void setup_usi() {
    // Enable Two-Wire mode (USIWM1:0 = 1:1)
    USICR = (1 << USIWM1) | (1 << USIWM0) |
            (1 << USICS1) | (1 << USICS0); // Two-Wire mode with software clock strobe
    // Clear the USI Start Condition Interrupt Flag
    USISR = (1 << USISIF);
}

void twi_init() {
    setup_pins();
    setup_usi();
}

void twi_start() {
    // Generate a start condition on the I2C bus
    PORTB &= ~(1 << SDA_PIN);  // Pull SDA low
    _delay_us(4);              // Short delay
    PORTB &= ~(1 << SCL_PIN);  // Pull SCL low
}

void twi_stop() {
    // Generate a stop condition on the I2C bus
    PORTB &= ~(1 << SDA_PIN);  // Ensure SDA is low
    PORTB |= (1 << SCL_PIN);   // Release SCL
    _delay_us(4);              // Short delay
    PORTB |= (1 << SDA_PIN);   // Release SDA
}

void twi_write_byte(uint8_t data) {
    // Write a byte of data to the I2C bus
    USIDR = data;              // Load data into USI data register
    USISR = (1 << USIOIF);     // Clear the overflow flag

    for (uint8_t i = 0; i < 8; i++) {
        USICR |= (1 << USITC); // Toggle the clock
        while (!(USISR & (1 << USIOIF))); // Wait for the transfer to complete
        USISR = (1 << USIOIF); // Clear the overflow flag
    }
}

uint8_t twi_read_byte() {
    // Read a byte of data from the I2C bus
    uint8_t data = 0;
    USISR = (1 << USIOIF);     // Clear the overflow flag

    for (uint8_t i = 0; i < 8; i++) {
        USICR |= (1 << USITC); // Toggle the clock
        while (!(USISR & (1 << USIOIF))); // Wait for the transfer to complete
        data = (data << 1) | (USIDR & 0x01); // Read the data bit
        USISR = (1 << USIOIF); // Clear the overflow flag
    }

    return data;
}

void twi_write(uint8_t slave_address, uint8_t reg_address, uint8_t data) {
    twi_start();
    twi_write_byte(slave_address << 1); // Write mode
    twi_write_byte(reg_address);
    twi_write_byte(data);
    twi_stop();
}

int main() {
		DDRB |= 1<<DDB3;
    twi_init();

    // Example: write to device at address 0x48, register 0x68

    while (1) {
			twi_write(0x48, 0x68, 0x42);  // Example byte to send to register 0x68
			PORTB ^= (1<<PORTB3);
			_delay_ms(300);
        // Main loop
    }

    return 0;
}
