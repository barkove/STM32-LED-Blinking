/* Заголовочный файл для нашего семейства микроконтроллеров*/
#include "stm32f3xx.h"

#define F_CPU 		    8000000UL 
#define BlinkFreq       1
#define TimerTick  	    (F_CPU / (2 * BlinkFreq) - 1)    // N is required ticks (N <= 2^24)
                                                        // N - 1 should be written

const int LED[] = {0x200, 0x100, 0x400, 0x8000, 0x800, 0x4000, 0x1000, 0x2000};
int LEDs = 0;

/* Инициализация и запуск таймера */
void SysTick_init_run(uint32_t ticks) 
{
    SysTick->LOAD = ticks;		    // Загрузка значения
    SysTick->VAL = ticks;		    // Обнуляем таймеры и флаги записью
 
    SysTick->CTRL =	SysTick_CTRL_CLKSOURCE_Msk |    // Используем ТИ процессора
                    SysTick_CTRL_TICKINT_Msk   |    // Бросать прерывание
                    SysTick_CTRL_ENABLE_Msk;        // Запустить таймер
}

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
 
    SysTick_init_run(TimerTick);
    
    for (int i = 0; i < sizeof(LED) / sizeof(int); ++i) {
        LEDs |= LED[i];
    }
 
    GPIOE->ODR = 0;
    
    while(1) {}
}

/* Попеременно зажигает светодиоды по прерыванию */
void SysTick_Handler(void)
{  
    GPIOE->ODR ^= LEDs;
}
