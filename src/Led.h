#ifndef LED_H
#define LED_H

#ifdef __cplusplus
extern "C" {
#endif

void initLeds();
void ledOn(const uint8_t led_index);
void ledOff(const uint8_t led_index);
void ledToggle(const uint8_t led_index);
void startBlinking(const uint8_t led_index);
void stopBlinking(const uint8_t led_index);
uint8_t isBlinking(const uint8_t led_index);

#ifdef __cplusplus
}
#endif

#endif /* led.h */
