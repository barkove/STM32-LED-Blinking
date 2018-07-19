/* Заголовочный файл для нашего семейства микроконтроллеров*/
#include "stm32f3xx.h"

#define F_CPU 		    12000000UL
#define BlinkFreq       10
#define TimerTick  	    (F_CPU / (2 * BlinkFreq) - 1)    // N - requiers ticks (N <= 2^24)
                                                        // N - 1 - should be written

const int LED[] = {0x200, 0x100, 0x400, 0x8000, 0x800, 0x4000, 0x1000, 0x2000};
int LEDs = 0;

/* Тело основной программы */
int main(void)
{
	/* Включаем тактирование на порту GPIO */
	RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
	/* Настраиваем режим работы портов LED в Output*/
	GPIOE->MODER = 0x55550000;
	/* Настраиваем Output type в режим Push-Pull */
	GPIOE->OTYPER = 0;
	/* Настраиваем скорость работы порта в Low */
	GPIOE->OSPEEDR = 0;
 
    SysTick->LOAD = TimerTick;		    // Загрузка значения
    SysTick->VAL = TimerTick;		    // Обнуляем таймеры и флаги
 
    SysTick->CTRL =	SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk   |
                    SysTick_CTRL_ENABLE_Msk;
    
    for (int i = 0; i < sizeof(LED) / sizeof(int); ++i) {
        LEDs |= LED[i];
    }
    
    GPIOE->ODR = LEDs;
}

/* Попеременно зажигает светодиоды по прерыванию */
void SysTick_Handler(void)
{  
    GPIOE->ODR ^= LEDs;
}
