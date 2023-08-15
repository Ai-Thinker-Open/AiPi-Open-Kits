#define __MODULE_WIRELESS_C_
#include "module_wireless.h"

int8_t module_wireless_send_38k_low_write_start(struct module_wireless_send_type *this, module_wireless_dat_t *module_wireless_send_dat)
{
    uint16_t i = 0;
    if (this == NULL)
    {
        return -1;
    }
    if (module_wireless_send_dat == NULL)
    {
        return -1;
    }

    if (module_wireless_send_dat->direction)
    {
        // this->module_wireless_send_config->module_wireless_send_config_write(1);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(0);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(1);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(0);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(1);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(0);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(1);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(0);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(1);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(0);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(1);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(0);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(1);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(0);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(1);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(0);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(1);
        for (i = 0; i < module_wireless_send_dat->encoding_high; i++)
        {
            this->module_wireless_send_config->module_wireless_send_config_delay();
        }
        this->module_wireless_send_config->module_wireless_send_config_write(0);
        for (i = 0; i < module_wireless_send_dat->encoding_low; i++)
        {
            this->module_wireless_send_config->module_wireless_send_config_write(0);
            this->module_wireless_send_config->module_wireless_send_config_38k_delay();
            this->module_wireless_send_config->module_wireless_send_config_write(1);
            this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        }
        this->module_wireless_send_config->module_wireless_send_config_write(1);
    }
    else
    {
        // this->module_wireless_send_config->module_wireless_send_config_write(0);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(1);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(0);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(1);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(0);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(1);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(0);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(1);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(0);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(1);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(0);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(1);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(0);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(1);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(0);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(1);
        // this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        // this->module_wireless_send_config->module_wireless_send_config_write(0);
        for (i = 0; i < module_wireless_send_dat->encoding_low; i++)
        {
            this->module_wireless_send_config->module_wireless_send_config_write(0);
            this->module_wireless_send_config->module_wireless_send_config_38k_delay();
            this->module_wireless_send_config->module_wireless_send_config_write(1);
            this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        }
        this->module_wireless_send_config->module_wireless_send_config_write(1);
        for (i = 0; i < module_wireless_send_dat->encoding_high; i++)
        {
            this->module_wireless_send_config->module_wireless_send_config_delay();
        }
        this->module_wireless_send_config->module_wireless_send_config_write(0);
    }

    return 0;
}
int8_t module_wireless_send_38k_low_write_end(struct module_wireless_send_type *this, module_wireless_dat_t *module_wireless_send_dat)
{
    uint16_t i = 0;
    if (this == NULL)
    {
        return -1;
    }
    if (module_wireless_send_dat == NULL)
    {
        return -1;
    }

    if (module_wireless_send_dat->direction)
    {
        for (i = 0; i < 50; i++)
        {
            this->module_wireless_send_config->module_wireless_send_config_write(0);
            this->module_wireless_send_config->module_wireless_send_config_38k_delay();
            this->module_wireless_send_config->module_wireless_send_config_write(1);
            this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        }
        this->module_wireless_send_config->module_wireless_send_config_write(1);
    }
    else
    {
        for (i = 0; i < 50; i++)
        {
            this->module_wireless_send_config->module_wireless_send_config_write(0);
            this->module_wireless_send_config->module_wireless_send_config_38k_delay();
            this->module_wireless_send_config->module_wireless_send_config_write(1);
            this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        }
        // this->module_wireless_send_config->module_wireless_send_config_write(1);
        // for (i = 0; i < module_wireless_send_dat->encoding_high; i++)
        // {
        //     this->module_wireless_send_config->module_wireless_send_config_delay();
        // }
        this->module_wireless_send_config->module_wireless_send_config_write(0);
    }


}
int8_t module_wireless_send_38k_low_write(struct module_wireless_send_type *this, module_wireless_dat_t *module_wireless_send_dat)
{
    uint16_t i = 0;
    if (this == NULL)
    {
        return -1;
    }
    if (module_wireless_send_dat == NULL)
    {
        return -1;
    }

    if (module_wireless_send_dat->direction)
    {
        this->module_wireless_send_config->module_wireless_send_config_write(1);
        for (i = 0; i < module_wireless_send_dat->encoding_high; i++)
        {
            this->module_wireless_send_config->module_wireless_send_config_delay();
        }
        this->module_wireless_send_config->module_wireless_send_config_write(0);
        for (i = 0; i < module_wireless_send_dat->encoding_low; i++)
        {
            this->module_wireless_send_config->module_wireless_send_config_write(0);
            this->module_wireless_send_config->module_wireless_send_config_38k_delay();
            this->module_wireless_send_config->module_wireless_send_config_write(1);
            this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        }
        this->module_wireless_send_config->module_wireless_send_config_write(1);
    }
    else
    {
        this->module_wireless_send_config->module_wireless_send_config_write(0);
        for (i = 0; i < module_wireless_send_dat->encoding_low; i++)
        {
            this->module_wireless_send_config->module_wireless_send_config_write(0);
            this->module_wireless_send_config->module_wireless_send_config_38k_delay();
            this->module_wireless_send_config->module_wireless_send_config_write(1);
            this->module_wireless_send_config->module_wireless_send_config_38k_delay();
        }
        this->module_wireless_send_config->module_wireless_send_config_write(1);
        for (i = 0; i < module_wireless_send_dat->encoding_high; i++)
        {
            this->module_wireless_send_config->module_wireless_send_config_delay();
        }
        this->module_wireless_send_config->module_wireless_send_config_write(0);
    }

    return 0;
}
int8_t module_wireless_send_38k_low_write_byte(struct module_wireless_send_type *this, module_wireless_dat_t *module_wireless_send_dat_zero, module_wireless_dat_t *module_wireless_send_dat_one, uint8_t dat)
{
    uint8_t i = 0, tmp = dat;
    if (this == NULL)
    {
        return -1;
    }
    if (module_wireless_send_dat_zero == NULL)
    {
        return -1;
    }
    if (module_wireless_send_dat_one == NULL)
    {
        return -1;
    }
    for (i = 0; i < 8; i++)
    {
        if (tmp & 0x01)
        {
            module_wireless_send_38k_low_write(this, module_wireless_send_dat_one);
        }
        else
        {
            module_wireless_send_38k_low_write(this, module_wireless_send_dat_zero);
        }
        tmp >>= 1;
    }

    return 0;
}

int8_t module_wireless_send_write_start(struct module_wireless_send_type *this, module_wireless_dat_t *module_wireless_send_dat)
{
    uint16_t i = 0;
    if (this == NULL)
    {
        return -1;
    }
    if (module_wireless_send_dat == NULL)
    {
        return -1;
    }

    if (module_wireless_send_dat->direction)
    {
        this->module_wireless_send_config->module_wireless_send_config_write(0);
        this->module_wireless_send_config->module_wireless_send_config_delay();
        this->module_wireless_send_config->module_wireless_send_config_write(1);
        for (i = 0; i < module_wireless_send_dat->encoding_high; i++)
        {
            this->module_wireless_send_config->module_wireless_send_config_delay();
        }
        this->module_wireless_send_config->module_wireless_send_config_write(0);
        for (i = 0; i < module_wireless_send_dat->encoding_low; i++)
        {
            this->module_wireless_send_config->module_wireless_send_config_delay();
        }
        this->module_wireless_send_config->module_wireless_send_config_write(1);
    }
    else
    {
        this->module_wireless_send_config->module_wireless_send_config_write(1);
        this->module_wireless_send_config->module_wireless_send_config_delay();
        this->module_wireless_send_config->module_wireless_send_config_write(0);
        for (i = 0; i < module_wireless_send_dat->encoding_low; i++)
        {
            this->module_wireless_send_config->module_wireless_send_config_delay();
        }
        this->module_wireless_send_config->module_wireless_send_config_write(1);
        for (i = 0; i < module_wireless_send_dat->encoding_high; i++)
        {
            this->module_wireless_send_config->module_wireless_send_config_delay();
        }
        this->module_wireless_send_config->module_wireless_send_config_write(0);
    }

    return 0;
}

int8_t module_wireless_send_write(struct module_wireless_send_type *this, module_wireless_dat_t *module_wireless_send_dat)
{
    uint16_t i = 0;
    if (this == NULL)
    {
        return -1;
    }
    if (module_wireless_send_dat == NULL)
    {
        return -1;
    }

    if (module_wireless_send_dat->direction)
    {
        this->module_wireless_send_config->module_wireless_send_config_write(1);
        for (i = 0; i < module_wireless_send_dat->encoding_high; i++)
        {
            this->module_wireless_send_config->module_wireless_send_config_delay();
        }
        this->module_wireless_send_config->module_wireless_send_config_write(0);
        for (i = 0; i < module_wireless_send_dat->encoding_low; i++)
        {
            this->module_wireless_send_config->module_wireless_send_config_delay();
        }
        this->module_wireless_send_config->module_wireless_send_config_write(1);
    }
    else
    {
        this->module_wireless_send_config->module_wireless_send_config_write(0);
        for (i = 0; i < module_wireless_send_dat->encoding_low; i++)
        {
            this->module_wireless_send_config->module_wireless_send_config_delay();
        }
        this->module_wireless_send_config->module_wireless_send_config_write(1);
        for (i = 0; i < module_wireless_send_dat->encoding_high; i++)
        {
            this->module_wireless_send_config->module_wireless_send_config_delay();
        }
        this->module_wireless_send_config->module_wireless_send_config_write(0);
    }

    return 0;
}

int8_t module_wireless_send_write_byte(struct module_wireless_send_type *this, module_wireless_dat_t *module_wireless_send_dat_zero, module_wireless_dat_t *module_wireless_send_dat_one, uint8_t dat)
{
    uint8_t i = 0, tmp = dat;
    if (this == NULL)
    {
        return -1;
    }
    if (module_wireless_send_dat_zero == NULL)
    {
        return -1;
    }
    if (module_wireless_send_dat_one == NULL)
    {
        return -1;
    }
    for (i = 0; i < 8; i++)
    {
        if (tmp & 0x01)
        {
            module_wireless_send_write(this, module_wireless_send_dat_one);
        }
        else
        {
            module_wireless_send_write(this, module_wireless_send_dat_zero);
        }
        tmp >>= 1;
    }

    return 0;
}

int8_t module_wireless_receive_falling(struct module_wireless_receive_type *this) // 下降沿触发
{
    if (this == NULL)
    {
        return -1;
    }

    if (this->module_wireless_receive_config->module_wireless_receive_start_flag)
    {
        this->module_wireless_receive_config->module_wireless_receive_high_level_time = this->module_wireless_receive_config->module_wireless_receive_config_tick() - this->module_wireless_receive_config->module_wireless_receive_bench_time;
        this->module_wireless_receive_config->module_wireless_receive_waiting_count = this->module_wireless_receive_config->module_wireless_receive_waiting_time;
        if (this->module_wireless_receive_config->module_wireless_receive_start_direction == 0)
        {
            // 下降沿触发,代表一个周期结束，需要求出这个数据为多少
            this->module_wireless_receive_config->module_wireless_receive_buf[this->module_wireless_receive_config->module_wireless_receive_buf_count].encoding_low = this->module_wireless_receive_config->module_wireless_receive_low_level_time;
            this->module_wireless_receive_config->module_wireless_receive_buf[this->module_wireless_receive_config->module_wireless_receive_buf_count].encoding_high = this->module_wireless_receive_config->module_wireless_receive_high_level_time;
            this->module_wireless_receive_config->module_wireless_receive_buf_count++;
        }
        this->module_wireless_receive_config->module_wireless_receive_bench_time = this->module_wireless_receive_config->module_wireless_receive_config_tick();
    }
    else
    {
        if (this->module_wireless_receive_config->module_wireless_receive_start_direction == 0)
        {
            // 下降沿没有启动的话，先启动
            this->module_wireless_receive_config->module_wireless_receive_start_flag = 1;
            this->module_wireless_receive_config->module_wireless_receive_bench_time = this->module_wireless_receive_config->module_wireless_receive_config_tick();
            this->module_wireless_receive_config->module_wireless_receive_waiting_count = this->module_wireless_receive_config->module_wireless_receive_waiting_time;
        }
    }

    return 0;
}

int8_t module_wireless_receive_rising(struct module_wireless_receive_type *this) // 上升沿触发
{
    if (this == NULL)
    {
        return -1;
    }

    if (this->module_wireless_receive_config->module_wireless_receive_start_flag)
    {
        this->module_wireless_receive_config->module_wireless_receive_low_level_time = this->module_wireless_receive_config->module_wireless_receive_config_tick() - this->module_wireless_receive_config->module_wireless_receive_bench_time;
        this->module_wireless_receive_config->module_wireless_receive_waiting_count = this->module_wireless_receive_config->module_wireless_receive_waiting_time;
        if (this->module_wireless_receive_config->module_wireless_receive_start_direction)
        {
            // 上升沿触发,代表一个周期结束，需要求出这个数据为多少
            this->module_wireless_receive_config->module_wireless_receive_buf[this->module_wireless_receive_config->module_wireless_receive_buf_count].encoding_low = this->module_wireless_receive_config->module_wireless_receive_low_level_time;
            this->module_wireless_receive_config->module_wireless_receive_buf[this->module_wireless_receive_config->module_wireless_receive_buf_count].encoding_high = this->module_wireless_receive_config->module_wireless_receive_high_level_time;
            this->module_wireless_receive_config->module_wireless_receive_buf_count++;
        }
        this->module_wireless_receive_config->module_wireless_receive_bench_time = this->module_wireless_receive_config->module_wireless_receive_config_tick();
    }
    else
    {
        if (this->module_wireless_receive_config->module_wireless_receive_start_direction)
        {
            // 上升沿没有启动的话，先启动
            this->module_wireless_receive_config->module_wireless_receive_start_flag = 1;
            this->module_wireless_receive_config->module_wireless_receive_bench_time = this->module_wireless_receive_config->module_wireless_receive_config_tick();
            this->module_wireless_receive_config->module_wireless_receive_waiting_count = this->module_wireless_receive_config->module_wireless_receive_waiting_time;
        }
    }

    return 0;
}

int8_t module_wireless_receive_read(struct module_wireless_receive_type *this, module_wireless_dat_t *module_wireless_receive_dat, uint16_t *lenght)
{
    uint16_t i = 0;
    if (this == NULL)
    {
        return -1;
    }
    if (module_wireless_receive_dat == NULL)
    {
        return -1;
    }
    if (this->module_wireless_receive_config->module_wireless_receive_start_flag)
    {
        if (this->module_wireless_receive_config->module_wireless_receive_waiting_count > 0)
        {
            this->module_wireless_receive_config->module_wireless_receive_waiting_count--;
        }
        else
        {
            this->module_wireless_receive_config->module_wireless_receive_start_flag = 0;
            for (i = 0; i < this->module_wireless_receive_config->module_wireless_receive_buf_count; i++)
            {
                module_wireless_receive_dat[i].encoding_high = this->module_wireless_receive_config->module_wireless_receive_buf[i].encoding_high;
                module_wireless_receive_dat[i].encoding_low = this->module_wireless_receive_config->module_wireless_receive_buf[i].encoding_low;
                module_wireless_receive_dat[i].direction = 4;
            }
            *lenght = this->module_wireless_receive_config->module_wireless_receive_buf_count;
            this->module_wireless_receive_config->module_wireless_receive_buf_count = 0;
            // 代表这一组数据接收完成
            // printf("receive complete\r\n");

            return 1; // 代表接收成功
        }
    }

    return 0;
}

int8_t module_wireless_receive_nec_analysis(struct module_wireless_receive_type *this, module_wireless_nec_dat_t *module_wireless_nec_dat, module_wireless_dat_t *module_wireless_receive_dat, uint16_t lenght, uint8_t *buf)
{
    uint8_t i = 0, temp = 0;
    if (this == NULL)
    {
        return -1;
    }
    if (buf == NULL)
    {
        return -1;
    }
    if (lenght != 33)
    {
        return -1;
    }

    if ((module_wireless_nec_dat->module_wireless_nec_start.encoding_low > module_wireless_receive_dat[0].encoding_low - module_wireless_receive_dat[0].direction) && (module_wireless_nec_dat->module_wireless_nec_start.encoding_low < module_wireless_receive_dat[0].encoding_low + module_wireless_receive_dat[0].direction) && (module_wireless_nec_dat->module_wireless_nec_start.encoding_high > module_wireless_receive_dat[0].encoding_high - module_wireless_receive_dat[0].direction) && (module_wireless_nec_dat->module_wireless_nec_start.encoding_high < module_wireless_receive_dat[0].encoding_high + module_wireless_receive_dat[0].direction))
    {
        for (i = 1; i < lenght; i++)
        {
            temp >>= 1;
            if ((module_wireless_nec_dat->module_wireless_nec_zero.encoding_low > module_wireless_receive_dat[i].encoding_low - module_wireless_receive_dat[i].direction) && (module_wireless_nec_dat->module_wireless_nec_zero.encoding_low < module_wireless_receive_dat[i].encoding_low + module_wireless_receive_dat[i].direction) && (module_wireless_nec_dat->module_wireless_nec_zero.encoding_high > module_wireless_receive_dat[i].encoding_high - module_wireless_receive_dat[i].direction) && (module_wireless_nec_dat->module_wireless_nec_zero.encoding_high < module_wireless_receive_dat[i].encoding_high + module_wireless_receive_dat[i].direction))
            {
                // printf("0\r\n");
                temp &= 0x7F;
            }
            else if ((module_wireless_nec_dat->module_wireless_nec_one.encoding_low > module_wireless_receive_dat[i].encoding_low - module_wireless_receive_dat[i].direction) && (module_wireless_nec_dat->module_wireless_nec_one.encoding_low < module_wireless_receive_dat[i].encoding_low + module_wireless_receive_dat[i].direction) && (module_wireless_nec_dat->module_wireless_nec_one.encoding_high > module_wireless_receive_dat[i].encoding_high - module_wireless_receive_dat[i].direction) && (module_wireless_nec_dat->module_wireless_nec_one.encoding_high < module_wireless_receive_dat[i].encoding_high + module_wireless_receive_dat[i].direction))
            {
                // printf("1\r\n");
                temp |= 0x80;
            }
            else
            {
                return -1;
            }

            if (i % 8 == 0)
            {
                buf[i / 8 - 1] = temp;
                temp = 0;
            }
        }
    }
    else
    {
        return -1;
    }
    return 0;
}

int8_t module_wireless_send_create(module_wireless_send_type_t *this, module_wireless_send_config_t *module_wireless_send_config)
{
    if (this == NULL)
    {
        return -1;
    }
    if (module_wireless_send_config == NULL)
    {
        return -1;
    }

    this->module_wireless_send_config = module_wireless_send_config;

    this->module_wireless_send_write_start = module_wireless_send_write_start;
    this->module_wireless_send_write = module_wireless_send_write;
    this->module_wireless_send_write_byte = module_wireless_send_write_byte;

    this->module_wireless_send_38k_low_write_start = module_wireless_send_38k_low_write_start;
    this->module_wireless_send_38k_low_write = module_wireless_send_38k_low_write;
    this->module_wireless_send_38k_low_write_byte = module_wireless_send_38k_low_write_byte;

    return 0;
}

int8_t module_wireless_receive_create(module_wireless_receive_type_t *this, module_wireless_receive_config_t *module_wireless_receive_config)
{
    if (this == NULL)
    {
        return -1;
    }
    if (module_wireless_receive_config == NULL)
    {
        return -1;
    }

    this->module_wireless_receive_config = module_wireless_receive_config;

    this->module_wireless_receive_falling = module_wireless_receive_falling;
    this->module_wireless_receive_rising = module_wireless_receive_rising;
    this->module_wireless_receive_read = module_wireless_receive_read;
    this->module_wireless_receive_nec_analysis = module_wireless_receive_nec_analysis;

    return 0;
}
