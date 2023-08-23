#define __MODULE_BUTTON_C_
#include "module_button.h"

static int8_t module_button_get_buttonvalue(struct module_button_type *this, uint16_t *value)
{
    // 判断状态是否有变化
    static uint8_t reservestatus = 0;
    uint16_t timercountvalue = 0;

    if (this == null)
    {
        return -1;
    }

    if (value == null)
    {
        return -1;
    }

    this->module_button_config->module_button_current_status = this->module_button_config->module_button_statusin();
    // printf("status value is %d\r\n", this->module_button_config->module_button_current_status);

    if (this->module_button_config->module_button_last_status != this->module_button_config->module_button_current_status)
    {
        this->module_button_config->module_button_last_status = this->module_button_config->module_button_current_status;
        this->module_button_config->module_button_longtimer_count = 0;
        return 0;
    }
    // 去抖动
    if (this->module_button_config->module_button_longtimer_count < 2)
    {
        this->module_button_config->module_button_longtimer_count++;
        return 0;
    }
    if (this->module_button_config->module_button_current_status) // 判断当前状态
    {
        reservestatus = this->module_button_config->module_button_last_status;
        if (this->module_button_config->module_button_longtimer_value != null)
        {
            timercountvalue = this->module_button_config->module_button_longtimer_value[reservestatus - 1];
        }
        else
        {
            timercountvalue = 10000;
        }
        if (this->module_button_config->module_button_longtimer_count < timercountvalue)
        {
            this->module_button_config->module_button_longtimer_count++;
        }

        if (this->module_button_config->module_button_longtimer_count >= timercountvalue)
        {
            // 返回长按按键
            if (!this->module_button_config->module_button_process_flag)
            {
                this->module_button_config->module_button_pres_flag = 0;
                this->module_button_config->module_button_process_flag = 1;
                *value = (2 * this->module_button_config->module_button_number + reservestatus);
                return 1;
            }
        }
        else
        {
            if (this->module_button_config->module_button_pres_flag)
            {
                // 设置双击按键标志
                if (this->module_button_config->module_button_contimer_value != null)
                {
                    timercountvalue = this->module_button_config->module_button_contimer_value[reservestatus - 1];
                }
                else
                {
                    timercountvalue = 0;
                }
                if ((this->module_button_config->module_button_contimer_count != 0) &&
                    (this->module_button_config->module_button_contimer_count < timercountvalue))
                {
                    if (!this->module_button_config->module_button_process_flag)
                    {
                        this->module_button_config->module_button_process_flag = 1;
                        *value = (1 * this->module_button_config->module_button_number + reservestatus);
                        return 1;
                    }
                }
            }
            else
            {
                this->module_button_config->module_button_pres_flag = 1;
                this->module_button_config->module_button_contimer_count = 0;
            }
        }
    }
    else
    {
        // 等待双击按键是否执行
        this->module_button_config->module_button_longtimer_count = 0;
        if (this->module_button_config->module_button_contimer_value != null)
        {
            if (reservestatus != 0)
            {
                timercountvalue = this->module_button_config->module_button_contimer_value[reservestatus - 1];
            }
        }
        else
        {
            timercountvalue = 0;
        }
        if (this->module_button_config->module_button_contimer_count < timercountvalue)
        {
            this->module_button_config->module_button_contimer_count++;
            return 0;
        }
        // 返回单击按键并清除设置状态
        if ((this->module_button_config->module_button_pres_flag == 1) && (this->module_button_config->module_button_process_flag == 0))
        {
            this->module_button_config->module_button_pres_flag = 0;
            this->module_button_config->module_button_process_flag = 1;
            *value = reservestatus;
            return 1;
        }
        else
        {
            this->module_button_config->module_button_pres_flag = 0;
            this->module_button_config->module_button_process_flag = 0;
        }
    }

    return 0;
}

int8_t module_button_create(module_button_type_t *this, module_button_config_t *module_button_config)
{
    if (this == null)
    {
        return -1;
    }
    if (module_button_config == null)
    {
        return -1;
    }

    this->module_button_config = module_button_config;

    this->module_button_config->module_button_current_status = 0;
    this->module_button_config->module_button_last_status = 0;
    this->module_button_config->module_button_process_flag = 0;
    this->module_button_config->module_button_pres_flag = 0;
    this->module_button_config->module_button_contimer_count = 0;
    this->module_button_config->module_button_longtimer_count = 0;

    this->module_button_get_buttonvalue = module_button_get_buttonvalue;

    return 0;
}
