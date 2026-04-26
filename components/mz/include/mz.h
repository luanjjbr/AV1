#ifndef SIN_H
#define SIN_H

void func_mz(void);
void reset_sinc(void);
void im_sinc(void);
void deg_sinc(void);
void config_hz_ts(float a_config, float T_config);
void config_item(int value_mz);

void get_printf(void);
void get_printf_2(void);
void get_printf_3(void);
float get_a(void);
float get_ts(void);
float get_y0(void);
int get_model_item(void);
void ram_sinc(void);

#endif // SIN_H
