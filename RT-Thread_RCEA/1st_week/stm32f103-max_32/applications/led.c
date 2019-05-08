/**
 * 
 * 使用信号量,同步 LED 状态, 500ms 亮, 500ms 灭
 * 
 * 
 * 
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#define THREAD_PRORITY      25
#define THREAD_TIMESLICE    5

/* defined the LED0 pin: PE5 */
#define LED0_PIN_NUM    GET_PIN(E, 5)
//#define LED0_PIN_NUM 69

int led_toggle(void)
{
    /* 调用 API 读出当前电平 然后输出相反电平 */
    rt_pin_write(LED0_PIN_NUM, !rt_pin_read(LED0_PIN_NUM));
    return rt_pin_read(LED0_PIN_NUM);
}

static rt_sem_t dynamic_sem = RT_NULL;

ALIGN(RT_ALIGN_SIZE)
static char thread1_stack[1024];
static struct rt_thread thread1;

static void rt_thread1_entry(void *parameter)
{
    while(1)
    {
        rt_thread_delay(500);
        //释放信号量
        rt_sem_release(dynamic_sem);
        rt_kprintf("release semaphore\n");
    }
}

ALIGN(RT_ALIGN_SIZE)
static char thread2_stack[1024];
static struct rt_thread thread2;
static void rt_thread2_entry(void *parameter)
{
    static rt_err_t result;
    int led_status = PIN_LOW;
    rt_pin_mode(LED0_PIN_NUM, PIN_MODE_OUTPUT);
    while (1)
    {
        
        rt_kprintf("wait semaphore\n");
        result = rt_sem_take(dynamic_sem, RT_WAITING_FOREVER);
        if (result != RT_EOK)
        {
            //删除信号量
            rt_sem_delete(dynamic_sem);
        }
        led_status = rt_pin_read(LED0_PIN_NUM);/* 调用 API 读出当前电平 然后输出相反电平 */
        rt_kprintf("LED status: %s\n", led_status?"ON":"OFF");
        rt_pin_write(LED0_PIN_NUM, !led_status);
    }
}

int led_semaphore_sapmle()
{
    /**/
    dynamic_sem = rt_sem_create("led_sem", 0, RT_IPC_FLAG_FIFO);
    if (dynamic_sem == RT_NULL)
    {
        rt_kprintf("create dynamic semaphore failed.\n");
        return -1;
    }
    else
    {
        rt_kprintf("Create done. \n");
    }
    
    rt_thread_init( &thread1,
                    "thread1",
                    rt_thread1_entry,
                    RT_NULL,
                    &thread1_stack[0],
                    sizeof(thread1_stack),
                    THREAD_PRORITY, THREAD_TIMESLICE);
    rt_thread_startup(&thread1);

    rt_thread_init( &thread2,
                    "thread2",
                    rt_thread2_entry,
                    RT_NULL,
                    &thread2_stack[0],
                    sizeof(thread2_stack),
                    THREAD_PRORITY-1, THREAD_TIMESLICE);
    rt_thread_startup(&thread2);

    return 0;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(led_semaphore_sapmle, led_semaphore sapmle);
