#ifndef SIN_H
#define SIN_H

void func_sinc(void);
void reset_sinc(void);
void im_sinc(void);
void deg_sinc(void);
void config_hz_ts(float new_f, float new_ts);
void config_item(int new_f);
void get_printf(void);
void get_printf_2(void);
void get_printf_3(void);
float get_ts(void);
float get_y0(void);

#endif // SIN_H