// wifi_manager.h
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void wifi_init();
void wifi_disconnect();
int wifi_status();

#ifdef __cplusplus
}
#endif