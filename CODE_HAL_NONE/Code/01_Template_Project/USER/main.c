#include "sys.h"
#include "delay.h"
#include "usart.h"

/*********************************************************************************
              ___   _     _____  _____  _   _  _____  _____  _   __
             / _ \ | |   |_   _||  ___|| \ | ||_   _||  ___|| | / /
            / /_\ \| |     | |  | |__  |  \| |  | |  | |__  | |/ /
            |  _  || |     | |  |  __| | . ` |  | |  |  __| |    \
            | | | || |_____| |_ | |___ | |\  |  | |  | |___ | |\  \
            \_| |_/\_____/\___/ \____/ \_| \_/  \_/  \____/ \_| \_/

 *	******************************************************************************
 *	正点原子 Pandora STM32L475 IoT开发板	实验0-1
 *	Template工程模板-新建工程章节使用
 *	技术支持：www.openedv.com
 *	淘宝店铺：http://openedv.taobao.com
 *	关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 *	广州市星翼电子科技有限公司
 *	作者：正点原子 @ALIENTEK
 *	******************************************************************************/


void Delay(__IO uint32_t nCount);
void Delay(__IO uint32_t nCount)
{
    while(nCount--) {};
}

int main(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    HAL_Init();                         //初始化HAL库
    SystemClock_Config();               //初始化系统时钟为80M
    
    __HAL_RCC_GPIOE_CLK_ENABLE();       //开启GPIOE时钟

    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;     //推挽输出
    GPIO_InitStruct.Pull = GPIO_PULLUP;             //上拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;   //高速
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    while(1)
    {
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);     //PE7 置1
        Delay(0x7FFFFF);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);   //PE7 置0
        Delay(0x7FFFFF);
    }
}


