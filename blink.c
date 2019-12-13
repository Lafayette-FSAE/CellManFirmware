/* The "Hello world!" of microcontrollers. Blink LED on/off */
#include <stdint.h>
#include "stm8.h"


/* Build in LED is in pin B5 (STM8S103 board) or D3 (STM8S003F3 board) */
#ifdef STM8S103
#define LED_PORT    PB
#define LED_PIN     PIN5
#else
#define LED_PORT    PA
#define LED_PIN     PIN3
#endif

/* Simple busy loop delay */
void delay(unsigned long count) {
    while (count--)
        nop();
}

void init_adc() {
    ADC_CSR = 0;
    ADC_CR1 = 0;
    ADC_CR2 = 0;
    ADC_CR3 = 0;

/*
    ADC_DRH = 0;
    ADC_DRL = 0;
    ADC_TDRH = 0;
    ADC_TDRL = 0;
    ADC_HTRH = 0;
    ADC_HTRL = 0;
    ADC_LTRH = 0;
    ADC_LTRL = 0;
    ADC_AWSRH = 0;
    ADC_AWSRL = 0;
    ADC_AWCRH = 0;
    ADC_AWCRL = 0;
*/
    ADC_CSR = 2; // Select channel 2 (AIN2=PC4)

    ADC_CR1 |= ADC_CR1_ADON; // ADON
    ADC_CR2 &= ~ADC_CR2_ALIGN; // Align left

    delay(1000); // Give little time to be ready for first conversion
}

uint16_t analog_read() {
    ADC_CR1 &= ~ADC_CR1_CONT; // Single conversion mode
    ADC_CR1 |= ADC_CR1_ADON; // Start conversion
    do { nop(); } while ((ADC_CSR >> 7) == 0);
    ADC_CSR &= ~ADC_CSR_EOC; // Clear "End of conversion"-flag
    return (ADC_DRH << 2) | (ADC_DRL >> 6);  // Left aligned
}

int main(void)
{
    /* Set clock to full speed (16 Mhz) */
    CLK_CKDIVR = 0;

    /* GPIO setup */
    // Set pin data direction as output
    PORT(LED_PORT, DDR)  |= LED_PIN; // i.e. PB_DDR |= (1 << 5);
    // Set pin as "Push-pull"
    PORT(LED_PORT, CR1)  |= LED_PIN; // i.e. PB_CR1 |= (1 << 5);

    // int value;

	while(1) {
        // value = analog_read();

        // long delay_length = 300000L;

        // LED on
        PORT(LED_PORT, ODR) |= LED_PIN; // PB_ODR |= (1 << 5);
        delay(600000L);
        // LED off
        PORT(LED_PORT, ODR) &= ~LED_PIN; // PB_ODR &= ~(1 << 5);
        delay(300000L);
    }
}