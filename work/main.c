/* Заголовочный файл для нашего семейства микроконтроллеров*/
#include "stm32f3xx.h"

const int LED[] = {0x200, 0x100, 0x400, 0x8000, 0x800, 0x4000, 0x1000, 0x2000};
const int CircleLEDind[] = {0, 1, 3, 5, 7, 6, 4, 2};
/* Тело основной программы */
int main(void)
{
	/* Включаем тактирование на порту GPIO */
	RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
	
	/* Настраиваем режим работы портов PC8 и PC9 в Output*/
	GPIOE->MODER = 0x55550000;
	/* Настраиваем Output type в режим Push-Pull */
	GPIOE->OTYPER = 0;
	
	/* Настраиваем скорость работы порта в Low */
	GPIOE->OSPEEDR = 0;

    short index = 0;
    
	while(1)
	{
		/* Поочередное зажигание диодов */
		GPIOE->ODR = LED[CircleLEDind[index]];
		for (int i = 0; i < 500000; ++i) {}	// Искусственная задержка
        	index = index == (sizeof(LED)/sizeof(int) - 1) ? 0 : index + 1;
	}		
}