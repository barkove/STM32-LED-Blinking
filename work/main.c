/* Заголовочный файл для нашего семейства микроконтроллеров*/
#include "stm32f3xx.h"

#define F_CPU 	        8000000UL 
#define BlinkFreq       1                               // must be greater than 0
#define SysTicks      	(F_CPU / (2 * BlinkFreq) - 1)   // N is required ticks (N <= 2^24)
                                                        // N - 1 should be written
#define TIM7_Prescaler  (1 << 7)
#define TIM7_Ticks      (F_CPU / TIM7_Prescaler / (2 * BlinkFreq) - 1)

const int LED[] = {0x200, 0x100, 0x400, 0x8000, 0x800, 0x4000, 0x1000, 0x2000};
const int CircleLEDind[] = {0, 1, 3, 5, 7, 6, 4, 2};

/* Инициализация и запуск системного таймера */
void SysTick_init_run(uint32_t ticks) 
{
    SysTick->LOAD = ticks;		    // Загрузка значения
    SysTick->VAL = ticks;		    // Обнуляем таймеры и флаги записью
 
    SysTick->CTRL =	SysTick_CTRL_CLKSOURCE_Msk |    // Используем ТИ процессора
                    SysTick_CTRL_TICKINT_Msk   |    // Бросать прерывание
                    SysTick_CTRL_ENABLE_Msk;        // Запустить таймер
}

/* Инициализация и запуск TIM7 */
void TIM7_init_run(uint32_t ticks)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM7EN; // Включаем тактирование
    TIM7->CR1 |= TIM_CR1_CEN;           // Запускаем таймер
    TIM7->DIER |= TIM_DIER_UIE;         // Разрешить прерывание
    TIM7->CR1 |= TIM_CR1_URS;           // Только переполнение генерит прерывание
    TIM7->PSC = TIM7_Prescaler;         // Устанавливаем делитель частоты
    TIM7->ARR = TIM7_Ticks;             // Устанавливаем границу overflow'a
}

/* Инициализация портов для LED */
void LED_init()
{
    /* Включаем тактирование на порту GPIO */
	RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
	/* Настраиваем режим работы портов связанных с LED в Output*/
	GPIOE->MODER = 0x55550000;
	/* Настраиваем Output type в режим Push-Pull */
	GPIOE->OTYPER = 0;
	/* Настраиваем скорость работы порта в Low */
	GPIOE->OSPEEDR = 0;
}

/* Зажечь диод LED[index] */
void LED_on(short index)
{
    GPIOE->ODR |= LED[index];
}

/* Погасить диод LED[index] */
void LED_off(short index)
{
    GPIOE->ODR &= ~LED[index];
}

/* Инвертировать состояние диода LED[index]*/
void LED_invert(short index)
{
    GPIOE->ODR ^= LED[index];
}

/* Тело основной программы */
int main(void)
{   
    LED_init();

    __enable_irq ();
    NVIC_EnableIRQ(TIM7_IRQn);    
    TIM7_init_run(TIM7_Ticks);
    
    for (int i = 0; i < sizeof(LED) / sizeof(int); ++i) {
        LED_on(i);
    }
    
    while(1) {}
}

/* Попеременно зажигает светодиоды по прерыванию системного таймера */
void SysTick_Handler(void)
{  
    for (int i = 0; i < sizeof(LED) / sizeof(int); ++i) {
        LED_invert(i);
    }
}

/* Попеременно зажигает светодиоды по прерыванию TIM7 */
void TIM7_IRQHandler()
{
    for (int i = 0; i < sizeof(LED) / sizeof(int); ++i) {
        LED_invert(i);
    }
    TIM7->SR &= ~TIM_SR_UIF;            // Сброс флага требования прерывания
}
